#include "filecalendarplugin.h"

#include <QtQml>

void FileCalendarPlugin::registerTypes (const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.private.filecalendarplugin"));
    qmlRegisterUncreatableType<Incidence>(uri, 0, 1, "Incidence", "");
//    qmlRegisterUncreatableType<CalendarTypes>(uri, 0, 1, "Incidence", "This can't be created");
//    qmlRegisterType<Prueba>(uri, 0, 1, "Prueba");
    qmlRegisterType<FileCalendar>(uri, 0, 1, "FileCalendar");
    qmlRegisterType<CalendarEvent>(uri, 0, 1, "CalendarEvent");
    qmlRegisterType<CalendarToDo>(uri, 0, 1, "CalendarToDo");
    qmlRegisterTypeNotAvailable(uri, 0, 1, "CalendarJournal", "Journals are not implemented, yet...");
}
