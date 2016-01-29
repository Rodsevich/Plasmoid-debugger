#ifndef FILECALENDARPLUGIN_H
#define FILECALENDARPLUGIN_H

#include <QQmlExtensionPlugin>
//#include "prueba.h"
#include "incidence.h"
#include "calendartodo.h"
#include "calendarevent.h"
#include "filecalendar.h"

class FileCalendarPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    virtual void registerTypes(const char *uri);
};

#endif // FILECALENDARPLUGIN_H
