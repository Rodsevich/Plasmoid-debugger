#ifndef FILECALENDARPLUGIN_H
#define PRUEBALUGIN_H

#include <QQmlExtensionPlugin>
#include "prueba.h"
#include "filecalendar.h"
#include "incidence.h"
#include "calendartodo.h"
#include "calendarevent.h"

class FileCalendarPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    virtual void registerTypes(const char *uri);
};

#endif // FILECALENDARPLUGIN_H
