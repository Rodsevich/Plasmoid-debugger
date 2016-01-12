/*
 *   Copyright 2006-2007 Aaron Seigo <aseigo@kde.org>
 *   Copyright 2008-2010 Marco Martin <notmart@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "svg.h"
#include "private/svg_p.h"
#include "private/theme_p.h"

#include <cmath>

#include <QCoreApplication>
#include <QDir>
#include <QDomDocument>
#include <QMatrix>
#include <QPainter>
#include <QStringBuilder>

#include <kcolorscheme.h>
#include <kconfiggroup.h>
#include <QDebug>
#include <kfilterdev.h>
#include <kiconeffect.h>

#include <plasma/applet.h>
#include <plasma/package.h>
#include <plasma/theme.h>
//#include "debug_p.h"

namespace Plasma
{


SharedSvgRenderer::SharedSvgRenderer(QObject *parent)
    : QSvgRenderer(parent)
{
}

SharedSvgRenderer::SharedSvgRenderer(
    const QString &filename,
    const QString &styleSheet,
    QHash<QString, QRectF> &interestingElements,
    QObject *parent)
    : QSvgRenderer(parent)
{
    KCompressionDevice file(filename, KCompressionDevice::GZip);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    load(file.readAll(), styleSheet, interestingElements);
}

SharedSvgRenderer::SharedSvgRenderer(
    const QByteArray &contents,
    const QString &styleSheet,
    QHash<QString, QRectF> &interestingElements,
    QObject *parent)
    : QSvgRenderer(parent)
{
    load(contents, styleSheet, interestingElements);
}

void SharedSvgRenderer::applyCustomColorScheme(QVariantMap scheme)
{
    QString content, skel = QStringLiteral("%1{color:%2;}");
    for(QVariantMap::const_iterator iter = scheme.begin(); iter != scheme.end(); ++iter){
	QString key = iter.key();
	QString value = iter.value().toString();
	content += skel.arg(key, value);
    }
    _customColorElement.setNodeValue(content);
    emit repaintNeeded();
}

bool SharedSvgRenderer::load(
    const QByteArray &contents,
    const QString &styleSheet,
    QHash<QString, QRectF> &interestingElements)
{
    QDomDocument svg;
    if (!svg.setContent(contents)) {
	return false;
    }
    const QString STYLE = QStringLiteral("style");

    //Gets the reference to the customizable-color-scheme style tag if it exists, else creates it
    //TODO: populate svg's _customColorScheme with existent values
    _customColorElement = svg.elementById(QStringLiteral("customizable-color-scheme"));
    if(_customColorElement.isNull()){
	_customColorElement = svg.createElement(STYLE);
	_customColorElement.setAttribute(QLatin1String("type"), QLatin1String("text/css"));
	_customColorElement.setAttribute(QLatin1String("id"), QLatin1String("customizable-color-scheme"));
	//Gotta insert in the most prevalent position:
	svg.insertBefore(_customColorElement, svg.firstChild());
//	  svg.appendChild(_customColorElement);
    }

    // Apply the style sheet.
    if (!styleSheet.isEmpty() && contents.contains("current-color-scheme")){

	QDomNode defs = svg.elementsByTagName(QStringLiteral("defs")).item(0);

	for (QDomElement style = defs.firstChildElement(STYLE); !style.isNull();
		style = style.nextSiblingElement(STYLE)) {
	    if (style.attribute(QStringLiteral("id")) == QLatin1String("current-color-scheme")) {
		QDomElement colorScheme = svg.createElement(STYLE);
		colorScheme.setAttribute(QLatin1String("type"), QLatin1String("text/css"));
		colorScheme.setAttribute(QLatin1String("id"), QLatin1String("current-color-scheme"));
		defs.replaceChild(colorScheme, style);
		colorScheme.appendChild(svg.createCDATASection(styleSheet));

		interestingElements.insert(QLatin1String("current-color-scheme"), QRect(0, 0, 1, 1));

		break;
	    }
	}
	if (!QSvgRenderer::load(svg.toByteArray(-1))) {
	    return false;
	}
    } else if (!QSvgRenderer::load(contents)) {
	return false;
    }

    // Search the SVG to find and store all ids that contain size hints.
    const QString contentsAsString(QString::fromLatin1(contents));
    QRegExp idExpr(QLatin1String("id\\s*=\\s*(['\"])(\\d+-\\d+-.*)\\1"));
    idExpr.setMinimal(true);

    int pos = 0;
    while ((pos = idExpr.indexIn(contentsAsString, pos)) != -1) {
        QString elementId = idExpr.cap(2);

        QRectF elementRect = boundsOnElement(elementId);
        if (elementRect.isValid()) {
            interestingElements.insert(elementId, elementRect);
        }

        pos += idExpr.matchedLength();
    }

    return true;
}

#define QLSEP QLatin1Char('_')
#define CACHE_ID_WITH_SIZE(size, id, devicePixelRatio) QString::number(int(size.width())) % QLSEP % QString::number(int(size.height())) % QLSEP % id % QLSEP % QLSEP % QString::number(int(devicePixelRatio))
#define CACHE_ID_NATURAL_SIZE(id, devicePixelRatio) QLatin1String("Natural") % QLSEP % id % QLSEP % QLSEP % QString::number(int(devicePixelRatio))

SvgPrivate::SvgPrivate(CustomSvg *svg)
    : q(svg),
      renderer(0),
      styleCrc(0),
      colorGroup(Plasma::Theme::NormalColorGroup),
      lastModified(0),
      devicePixelRatio(1.0),
      scaleFactor(1.0),
      multipleImages(false),
      themed(false),
      useSystemColors(false),
      fromCurrentTheme(false),
      applyColors(false),
      usesColors(false),
      cacheRendering(true),
      themeFailed(false)
{
}

SvgPrivate::~SvgPrivate()
{
    eraseRenderer();
}

//This function is meant for the rects cache
QString SvgPrivate::cacheId(const QString &elementId)
{
    if (size.isValid() && size != naturalSize) {
        return CACHE_ID_WITH_SIZE(size, elementId, devicePixelRatio);
    } else {
        return CACHE_ID_NATURAL_SIZE(elementId, devicePixelRatio);
    }
}

//This function is meant for the pixmap cache
QString SvgPrivate::cachePath(const QString &path, const QSize &size)
{
    return CACHE_ID_WITH_SIZE(size, path, devicePixelRatio) % QLSEP % QString::number(colorGroup);
}

bool SvgPrivate::setImagePath(const QString &imagePath)
{
    QString actualPath = imagePath;
    if (imagePath.startsWith(QLatin1String("file://"))) {
        //length of file://
        actualPath = actualPath.mid(7);
    }
    const bool isThemed = !QDir::isAbsolutePath(actualPath);

    // lets check to see if we're already set to this file
    if (isThemed == themed &&
            ((themed && themePath == actualPath) ||
             (!themed && path == actualPath))) {
        return false;
    }

    eraseRenderer();

    // if we don't have any path right now and are going to set one,
    // then lets not schedule a repaint because we are just initializing!
    bool updateNeeded = true; //!path.isEmpty() || !themePath.isEmpty();

    QObject::disconnect(actualTheme(), SIGNAL(themeChanged()), q, SLOT(themeChanged()));
    if (isThemed && !themed && s_systemColorsCache) {
        // catch the case where we weren't themed, but now we are, and the colors cache was set up
        // ensure we are not connected to that theme previously
        QObject::disconnect(s_systemColorsCache.data(), 0, q, 0);
    }

    themed = isThemed;
    path.clear();
    themePath.clear();
    localRectCache.clear();
    elementsWithSizeHints.clear();
    bool oldFromCurrentTheme = fromCurrentTheme;
    fromCurrentTheme = actualTheme()->currentThemeHasImage(imagePath);

    if (fromCurrentTheme != oldFromCurrentTheme) {
        emit q->fromCurrentThemeChanged(fromCurrentTheme);
    }

    if (themed) {
        themePath = actualPath;
        path = actualTheme()->imagePath(themePath);
        themeFailed = path.isEmpty();
        QObject::connect(actualTheme(), SIGNAL(themeChanged()), q, SLOT(themeChanged()));
    } else if (QFile::exists(actualPath)) {
        QObject::connect(cacheAndColorsTheme(), SIGNAL(themeChanged()), q, SLOT(themeChanged()), Qt::UniqueConnection);
        path = actualPath;
    } else {
#ifndef NDEBUG
        // qCDebug(LOG_PLASMA) << "file '" << path << "' does not exist!";
#endif
    }

    // check if svg wants colorscheme applied
    checkColorHints();

    // also images with absolute path needs to have a natural size initialized,
    // even if looks a bit weird using Theme to store non-themed stuff
    if ((themed && QFile::exists(path)) || QFile::exists(actualPath)) {
        QRectF rect;

        if (cacheAndColorsTheme()->findInRectsCache(path, QStringLiteral("_Natural_%1").arg(scaleFactor), rect)) {
            naturalSize = rect.size();
        } else {
            createRenderer();
            naturalSize = renderer->defaultSize() * scaleFactor;
            //qCDebug(LOG_PLASMA) << "natural size for" << path << "from renderer is" << naturalSize;
            cacheAndColorsTheme()->insertIntoRectsCache(path, QStringLiteral("_Natural_%1").arg(scaleFactor), QRectF(QPointF(0, 0), naturalSize));
            //qCDebug(LOG_PLASMA) << "natural size for" << path << "from cache is" << naturalSize;
        }
    }

    if (!themed) {
        QFile f(actualPath);
        QFileInfo info(f);
        lastModified = info.lastModified().toTime_t();
    }

    q->resize();
    emit q->imagePathChanged();

    return updateNeeded;
}

Theme *SvgPrivate::actualTheme()
{
    if (!theme) {
        theme = new Plasma::Theme(q);
    }

    return theme.data();
}

Theme *SvgPrivate::cacheAndColorsTheme()
{
    if (themed || !useSystemColors) {
        return actualTheme();
    } else {
        // use a separate cache source for unthemed svg's
        if (!s_systemColorsCache) {
            //FIXME: reference count this, so that it is deleted when no longer in use
            s_systemColorsCache = new Plasma::Theme(QLatin1String("internal-system-colors"));
        }

        return s_systemColorsCache.data();
    }
}

QPixmap SvgPrivate::findInCache(const QString &elementId, qreal ratio, const QSizeF &s)
{
    QSize size;
    QString actualElementId;

    if (elementsWithSizeHints.isEmpty()) {
        // Fetch all size hinted element ids from the theme's rect cache
        // and store them locally.
        QRegExp sizeHintedKeyExpr(CACHE_ID_NATURAL_SIZE("(\\d+)-(\\d+)-(.+)", ratio));

        foreach (const QString &key, cacheAndColorsTheme()->listCachedRectKeys(path)) {
            if (sizeHintedKeyExpr.exactMatch(key)) {
                QString baseElementId = sizeHintedKeyExpr.cap(3);
                QSize sizeHint(sizeHintedKeyExpr.cap(1).toInt(),
                               sizeHintedKeyExpr.cap(2).toInt());

                if (sizeHint.isValid()) {
                    elementsWithSizeHints.insertMulti(baseElementId, sizeHint);
                }
            }
        }

        if (elementsWithSizeHints.isEmpty()) {
            // Make sure we won't query the theme unnecessarily.
            elementsWithSizeHints.insert(QString(), QSize());
        }
    }

    // Look at the size hinted elements and try to find the smallest one with an
    // identical aspect ratio.
    if (s.isValid() && !elementId.isEmpty()) {
        QList<QSize> elementSizeHints = elementsWithSizeHints.values(elementId);

        if (!elementSizeHints.isEmpty()) {
            QSize bestFit(-1, -1);

            Q_FOREACH (QSize hint, elementSizeHints) {

                if (hint.width() >= s.width() * ratio && hint.height() >= s.height() * ratio &&
                        (!bestFit.isValid() ||
                         (bestFit.width() * bestFit.height()) > (hint.width() * hint.height()))) {
                    bestFit = hint;
                }
            }

            if (bestFit.isValid()) {
                actualElementId = QString::number(bestFit.width()) % '-' %
                                  QString::number(bestFit.height()) % '-' % elementId;
            }
        }
    }

    if (elementId.isEmpty() || !q->hasElement(actualElementId)) {
        actualElementId = elementId;
    }

    if (elementId.isEmpty() || (multipleImages && s.isValid())) {
        size = s.toSize() * ratio;
    } else {
        size = elementRect(actualElementId).size().toSize() * ratio;
    }

    if (size.isEmpty()) {
        return QPixmap();
    }

    QString id = cachePath(path, size);

    if (!actualElementId.isEmpty()) {
        id.append(actualElementId);
    }

    //qCDebug(LOG_PLASMA) << "id is " << id;

    QPixmap p;
    if (cacheRendering && cacheAndColorsTheme()->findInCache(id, p, lastModified)) {
        p.setDevicePixelRatio(ratio);
        //qCDebug(LOG_PLASMA) << "found cached version of " << id << p.size();
        return p;
    }

    //qCDebug(LOG_PLASMA) << "didn't find cached version of " << id << ", so re-rendering";

    //qCDebug(LOG_PLASMA) << "size for " << actualElementId << " is " << s;
    // we have to re-render this puppy

    createRenderer();

    QRectF finalRect = makeUniform(renderer->boundsOnElement(actualElementId), QRect(QPoint(0, 0), size));

    //don't alter the pixmap size or it won't match up properly to, e.g., FrameSvg elements
    //makeUniform should never change the size so much that it gains or loses a whole pixel
    p = QPixmap(size);

    p.fill(Qt::transparent);
    QPainter renderPainter(&p);

    if (actualElementId.isEmpty()) {
        renderer->render(&renderPainter, finalRect);
    } else {
        renderer->render(&renderPainter, actualElementId, finalRect);
    }

    renderPainter.end();
    p.setDevicePixelRatio(ratio);

    // Apply current color scheme if the svg asks for it
    if (applyColors) {
        QImage itmp = p.toImage();
        KIconEffect::colorize(itmp, cacheAndColorsTheme()->color(Theme::BackgroundColor), 1.0);
        p = p.fromImage(itmp);
    }

    if (cacheRendering) {
        cacheAndColorsTheme()->insertIntoCache(id, p, QString::number((qint64)q, 16) % QLSEP % actualElementId);
    }

    return p;
}

void SvgPrivate::createRenderer()
{
    if (renderer) {
        return;
    }

    //qCDebug(LOG_PLASMA) << kBacktrace();
    if (themed && path.isEmpty() && !themeFailed) {
        Applet *applet = qobject_cast<Applet *>(q->parent());
        //FIXME: this maybe could be more efficient if we knew if the package was empty, e.g. for
        //C++; however, I'm not sure this has any real world runtime impact. something to measure
        //for.
        if (applet && applet->kPackage().isValid()) {
            const KPackage::Package package = applet->kPackage();
            path = package.filePath("images", themePath + QLatin1String(".svg"));

            if (path.isEmpty()) {
                path = package.filePath("images", themePath + QLatin1String(".svgz"));
            }
        }

        if (path.isEmpty()) {
            path = actualTheme()->imagePath(themePath);
            themeFailed = path.isEmpty();
//            if (themeFailed) {
//                qCWarning(LOG_PLASMA) << "No image path found for" << themePath;
//            }
        }
    }

    //qCDebug(LOG_PLASMA) << "********************************";
    //qCDebug(LOG_PLASMA) << "FAIL! **************************";
    //qCDebug(LOG_PLASMA) << path << "**";

    QString styleSheet = "";//cacheAndColorsTheme()->d->svgStyleSheet(colorGroup);
    styleCrc = qChecksum(styleSheet.toUtf8(), styleSheet.size());

    QHash<QString, SharedSvgRenderer::Ptr>::const_iterator it = s_renderers.constFind(styleCrc + path);

    if (it != s_renderers.constEnd()) {
        //qCDebug(LOG_PLASMA) << "gots us an existing one!";
        renderer = it.value();
    } else {
        if (path.isEmpty()) {
            renderer = new SharedSvgRenderer();
        } else {
            QHash<QString, QRectF> interestingElements;
            renderer = new SharedSvgRenderer(path, styleSheet, interestingElements);

            // Add interesting elements to the theme's rect cache.
            QHashIterator<QString, QRectF> i(interestingElements);

            while (i.hasNext()) {
                i.next();
                const QString &elementId = i.key();
                const QRectF &elementRect = i.value();

                const QString cacheId = CACHE_ID_NATURAL_SIZE(elementId, devicePixelRatio);
                localRectCache.insert(cacheId, elementRect);
                cacheAndColorsTheme()->insertIntoRectsCache(path, cacheId, elementRect);
            }
        }

        s_renderers[styleCrc + path] = renderer;
    }

    if (size == QSizeF()) {
        size = renderer->defaultSize();
    }
}

void SvgPrivate::eraseRenderer()
{
    if (renderer && renderer->ref.load() == 2) {
        // this and the cache reference it
        s_renderers.erase(s_renderers.find(styleCrc + path));

        if (theme) {
            theme.data()->releaseRectsCache(path);
        }
    }

    renderer = 0;
    styleCrc = 0;
    localRectCache.clear();
    elementsWithSizeHints.clear();
}

QRectF SvgPrivate::elementRect(const QString &elementId)
{
    if (themed && path.isEmpty()) {
        if (themeFailed) {
            return QRectF();
        }

        path = actualTheme()->imagePath(themePath);
        themeFailed = path.isEmpty();

        if (themeFailed) {
            return QRectF();
        }
    }

    if (path.isEmpty()) {
        return QRectF();
    }

    QString id = cacheId(elementId);

    if (localRectCache.contains(id)) {
        return localRectCache.value(id);
    }

    QRectF rect;
    bool found = cacheAndColorsTheme()->findInRectsCache(path, id, rect);

    //This is a corner case where we are *sure* the element is not valid
    if (found && rect == QRectF()) {
        return rect;
    } else if (found) {
        localRectCache.insert(id, rect);
    } else {
        rect = findAndCacheElementRect(elementId);
    }

    return rect;
}

QRectF SvgPrivate::findAndCacheElementRect(const QString &elementId)
{
    createRenderer();

    // createRenderer() can insert some interesting rects in the cache, so check it
    const QString id = cacheId(elementId);
    if (localRectCache.contains(id)) {
        return localRectCache.value(id);
    }

    QRectF elementRect = renderer->elementExists(elementId) ?
                         renderer->matrixForElement(elementId).map(renderer->boundsOnElement(elementId)).boundingRect() :
                         QRectF();
    naturalSize = renderer->defaultSize() * scaleFactor;

    qreal dx = size.width() / renderer->defaultSize().width();
    qreal dy = size.height() / renderer->defaultSize().height();

    elementRect = QRectF(elementRect.x() * dx, elementRect.y() * dy,
                         elementRect.width() * dx, elementRect.height() * dy);

    cacheAndColorsTheme()->insertIntoRectsCache(path, id, elementRect);

    return elementRect;
}

QMatrix SvgPrivate::matrixForElement(const QString &elementId)
{
    createRenderer();
    return renderer->matrixForElement(elementId);
}

void SvgPrivate::checkColorHints()
{
    if (elementRect(QStringLiteral("hint-apply-color-scheme")).isValid()) {
        applyColors = true;
        usesColors = true;
    } else if (elementRect(QStringLiteral("current-color-scheme")).isValid()) {
        applyColors = false;
        usesColors = true;
    } else {
        applyColors = false;
        usesColors = false;
    }

    // check to see if we are using colors, but the theme isn't being used or isn't providing
    // a colorscheme
    if (qGuiApp) {
        if (usesColors && (!themed || !actualTheme()->colorScheme())) {
            QObject::connect(actualTheme()->d, SIGNAL(applicationPaletteChange()), q, SLOT(colorsChanged()));
        } else {
            QObject::disconnect(actualTheme()->d, SIGNAL(applicationPaletteChange()), q, SLOT(colorsChanged()));
        }
    }
}

bool CustomSvg::eventFilter(QObject *watched, QEvent *event)
{
    return QObject::eventFilter(watched, event);
}

//Following two are utility functions to snap rendered elements to the pixel grid
//to and from are always 0 <= val <= 1
qreal SvgPrivate::closestDistance(qreal to, qreal from)
{
    qreal a = to - from;
    if (qFuzzyCompare(to, from)) {
        return 0;
    } else if (to > from) {
        qreal b = to - from - 1;
        return (qAbs(a) > qAbs(b)) ?  b : a;
    } else {
        qreal b = 1 + to - from;
        return (qAbs(a) > qAbs(b)) ? b : a;
    }
}

QRectF SvgPrivate::makeUniform(const QRectF &orig, const QRectF &dst)
{
    if (qFuzzyIsNull(orig.x()) || qFuzzyIsNull(orig.y())) {
        return dst;
    }

    QRectF res(dst);
    qreal div_w = dst.width() / orig.width();
    qreal div_h = dst.height() / orig.height();

    qreal div_x = dst.x() / orig.x();
    qreal div_y = dst.y() / orig.y();

    //horizontal snap
    if (!qFuzzyIsNull(div_x) && !qFuzzyCompare(div_w, div_x)) {
        qreal rem_orig = orig.x() - (floor(orig.x()));
        qreal rem_dst = dst.x() - (floor(dst.x()));
        qreal offset = closestDistance(rem_dst, rem_orig);
        res.translate(offset + offset * div_w, 0);
        res.setWidth(res.width() + offset);
    }
    //vertical snap
    if (!qFuzzyIsNull(div_y) && !qFuzzyCompare(div_h, div_y)) {
        qreal rem_orig = orig.y() - (floor(orig.y()));
        qreal rem_dst = dst.y() - (floor(dst.y()));
        qreal offset = closestDistance(rem_dst, rem_orig);
        res.translate(0, offset + offset * div_h);
        res.setHeight(res.height() + offset);
    }

    //qCDebug(LOG_PLASMA)<<"Aligning Rects, origin:"<<orig<<"destination:"<<dst<<"result:"<<res;
    return res;
}

void SvgPrivate::themeChanged()
{
    if (q->imagePath().isEmpty()) {
        return;
    }

    if (themed) {
        // check if new theme svg wants colorscheme applied
        checkColorHints();
    }

    QString currentPath = themed ? themePath : path;
    themePath.clear();
    eraseRenderer();
    setImagePath(currentPath);
    q->resize();

    //qCDebug(LOG_PLASMA) << themePath << ">>>>>>>>>>>>>>>>>> theme changed";
    emit q->repaintNeeded();
}

void SvgPrivate::colorsChanged()
{
    if (!usesColors) {
        return;
    }

    eraseRenderer();
    //qCDebug(LOG_PLASMA) << "repaint needed from colorsChanged";

    // in the case the theme follows the desktop settings, refetch the colorschemes
    // and discard the svg pixmap cache
    if (!theme.data()->d->colors) {
        KSharedConfig::openConfig()->reparseConfiguration();
        theme.data()->d->colorScheme = KColorScheme(QPalette::Active, KColorScheme::Window);
        theme.data()->d->buttonColorScheme = KColorScheme(QPalette::Active, KColorScheme::Button);
        theme.data()->d->viewColorScheme = KColorScheme(QPalette::Active, KColorScheme::View);
//        theme.data()->d->discardCache(PixmapCache | SvgElementsCache);
    }

    emit q->repaintNeeded();
}

QHash<QString, SharedSvgRenderer::Ptr> SvgPrivate::s_renderers;
QWeakPointer<Theme> SvgPrivate::s_systemColorsCache;

CustomSvg::CustomSvg(QObject *parent)
    : QObject(parent),
      d(new SvgPrivate(this))
{
}

CustomSvg::~CustomSvg()
{
    delete d;
}

void CustomSvg::setDevicePixelRatio(qreal ratio)
{
    //be completely integer for now
    //devicepixelratio is always set integer in the svg, so needs at least 192dpi to double up.
    //(it needs to be integer to have lines contained inside a svg piece to keep being pixel aligned)
    if (floor(d->devicePixelRatio) == floor(ratio)) {
        return;
    }

//    if (FrameSvg *f = qobject_cast<FrameSvg *>(this)) {
//        f->clearCache();
//    }

    d->devicePixelRatio = floor(ratio);

    emit repaintNeeded();
}

qreal CustomSvg::devicePixelRatio()
{
    return d->devicePixelRatio;
}


void CustomSvg::setScaleFactor(qreal ratio)
{
    //be completely integer for now
    //devicepixelratio is always set integer in the svg, so needs at least 192dpi to double up.
    //(it needs to be integer to have lines contained inside a svg piece to keep being pixel aligned)
    if (floor(d->scaleFactor) == floor(ratio)) {
        return;
    }

    d->scaleFactor = floor(ratio);
    //not resize() because we want to do it unconditionally
    QRectF rect;

    if (d->cacheAndColorsTheme()->findInRectsCache(d->path, QString("_Natural_%1").arg(d->scaleFactor), rect)) {
        d->naturalSize = rect.size();
    } else {
        d->createRenderer();
        d->naturalSize = d->renderer->defaultSize() * d->scaleFactor;
    }

    d->size = d->naturalSize;

    emit repaintNeeded();
    emit sizeChanged();
}

qreal CustomSvg::scaleFactor() const
{
    return d->scaleFactor;
}

void CustomSvg::setColorGroup(Plasma::Theme::ColorGroup group)
{
    if (d->colorGroup == group) {
        return;
    }

    d->colorGroup = group;
    d->renderer = 0;
    emit colorGroupChanged();
    emit repaintNeeded();
}

Plasma::Theme::ColorGroup CustomSvg::colorGroup() const
{
    return d->colorGroup;
}

QPixmap CustomSvg::pixmap(const QString &elementID)
{
    if (elementID.isNull() || d->multipleImages) {
        return d->findInCache(elementID, d->devicePixelRatio, size());
    } else {
        return d->findInCache(elementID, d->devicePixelRatio);
    }
}

QImage CustomSvg::image(const QSize &size, const QString &elementID)
{
    QPixmap pix(d->findInCache(elementID, d->devicePixelRatio, size));
    return pix.toImage();
}

void CustomSvg::paint(QPainter *painter, const QPointF &point, const QString &elementID)
{
    Q_ASSERT(painter->device());
    const int ratio = painter->device()->devicePixelRatio();
    QPixmap pix((elementID.isNull() || d->multipleImages) ? d->findInCache(elementID, ratio, size()) :
                d->findInCache(elementID, ratio));

    if (pix.isNull()) {
        return;
    }

    painter->drawPixmap(QRectF(point, size()), pix, QRectF(QPointF(0, 0), pix.size()));
}

void CustomSvg::paint(QPainter *painter, int x, int y, const QString &elementID)
{
    paint(painter, QPointF(x, y), elementID);
}

void CustomSvg::paint(QPainter *painter, const QRectF &rect, const QString &elementID)
{
    Q_ASSERT(painter->device());
    const int ratio = painter->device()->devicePixelRatio();
    QPixmap pix(d->findInCache(elementID, ratio, rect.size()));

    painter->drawPixmap(QRectF(rect.topLeft(), rect.size()), pix, QRectF(QPointF(0, 0), pix.size()));
}

void CustomSvg::paint(QPainter *painter, int x, int y, int width, int height, const QString &elementID)
{
    Q_ASSERT(painter->device());
    const int ratio = painter->device()->devicePixelRatio();
    QPixmap pix(d->findInCache(elementID, ratio, QSizeF(width, height)));
    painter->drawPixmap(x, y, pix, 0, 0, pix.size().width(), pix.size().height());
}

QSize CustomSvg::size() const
{
    if (d->size.isEmpty()) {
        d->size = d->naturalSize;
    }

    return d->size.toSize();
}

void CustomSvg::resize(qreal width, qreal height)
{
    resize(QSize(width, height));
}

void CustomSvg::resize(const QSizeF &size)
{
    if (qFuzzyCompare(size.width(), d->size.width()) &&
            qFuzzyCompare(size.height(), d->size.height())) {
        return;
    }

    d->size = size;
    d->localRectCache.clear();
    emit sizeChanged();
}

void CustomSvg::resize()
{
    if (qFuzzyCompare(d->naturalSize.width(), d->size.width()) &&
            qFuzzyCompare(d->naturalSize.height(), d->size.height())) {
        return;
    }

    d->size = d->naturalSize;
    d->localRectCache.clear();
    emit sizeChanged();
}

QSize CustomSvg::elementSize(const QString &elementId) const
{
    return d->elementRect(elementId).size().toSize();
}

QRectF CustomSvg::elementRect(const QString &elementId) const
{
    return d->elementRect(elementId);
}

bool CustomSvg::hasElement(const QString &elementId) const
{
    if (d->path.isNull() && d->themePath.isNull()) {
        return false;
    }

    return d->elementRect(elementId).isValid();
}

bool CustomSvg::isValid() const
{
    if (d->path.isNull() && d->themePath.isNull()) {
        return false;
    }

    //try very hard to avoid creation of a parser
    QRectF rect;
    if (d->cacheAndColorsTheme()->findInRectsCache(d->path, QStringLiteral("_Natural_%1").arg(d->scaleFactor), rect)) {
        return true;
    }

    if (!QFile::exists(d->path)) {
        return false;
    }

    d->createRenderer();
    return d->renderer->isValid();
}

void CustomSvg::setContainsMultipleImages(bool multiple)
{
    d->multipleImages = multiple;
}

bool CustomSvg::containsMultipleImages() const
{
    return d->multipleImages;
}

void CustomSvg::setImagePath(const QString &svgFilePath)
{
    if (d->setImagePath(svgFilePath)) {
        //qCDebug(LOG_PLASMA) << "repaintNeeded";
        emit repaintNeeded();
    }
}

QString CustomSvg::imagePath() const
{
    return d->themed ? d->themePath : d->path;
}

void CustomSvg::setUsingRenderingCache(bool useCache)
{
    d->cacheRendering = useCache;
}

bool CustomSvg::isUsingRenderingCache() const
{
    return d->cacheRendering;
}

bool CustomSvg::fromCurrentTheme() const
{
    return d->fromCurrentTheme;
}

void CustomSvg::setUseSystemColors(bool system)
{
    if (d->useSystemColors == system) {
        return;
    }

    d->useSystemColors = system;
    emit repaintNeeded();
}

bool CustomSvg::useSystemColors() const
{
    return d->useSystemColors;
}

void CustomSvg::setTheme(Plasma::Theme *theme)
{
    if (!theme || theme == d->theme.data()) {
        return;
    }

    if (d->theme) {
        disconnect(d->theme.data(), 0, this, 0);
    }

    d->theme = theme;
    connect(theme, SIGNAL(themeChanged()), this, SLOT(themeChanged()));
    d->themeChanged();
}

Theme *CustomSvg::theme() const
{
    return d->actualTheme();
}

QVariantMap CustomSvg::customColorScheme() const
{
    return m_customColorScheme;
}

void CustomSvg::setCustomColorScheme(QVariantMap customColorScheme)
{
    if (m_customColorScheme == customColorScheme)
	return;

    m_customColorScheme = customColorScheme;
    d->renderer->applyCustomColorScheme(m_customColorScheme);
    emit customColorSchemeChanged(customColorScheme);
}

} // Plasma namespace

#include "private/moc_svg_p.cpp"
#include "moc_svg.cpp"
