#ifndef PRUEBA_H
#define PRUEBA_H

#include <QObject>
#include <QQmlListProperty>
#include <QFileSystemWatcher>
#include "calendartodo.h"
#include "calendarevent.h"
#include <kcalcore/calendar.h>
#include <kcalcore/memorycalendar.h>
#include <kcalcore/filestorage.h>

class Prueba : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString longa READ longa WRITE setLonga NOTIFY longaChanged)
    Q_PROPERTY(QQmlListProperty<CalendarToDo> todos READ todos NOTIFY todosChanged())
    Q_PROPERTY(QQmlListProperty<CalendarEvent> events READ events NOTIFY eventsChanged())
    Q_PROPERTY(QVariantMap arreglo READ arreglo WRITE setArreglo NOTIFY arregloChanged)
    Q_PROPERTY(QVariantMap customColorScheme READ customColorScheme WRITE setCustomColorScheme NOTIFY customColorSchemeChanged)

public:
    Q_INVOKABLE QString leer();
    Q_INVOKABLE void addToDo(CalendarToDo* todo);
    Q_INVOKABLE void addEvent(CalendarEvent* event);
    Q_INVOKABLE bool saveCalendar();
    Q_INVOKABLE QString retTrue();
    Q_INVOKABLE QVariantMap agregarArreglo(QString&, QVariant&);

    QString uri();
    void setUri(QString &uri);

    QString longa();
    void setLonga(QString &longa);

    QQmlListProperty<CalendarToDo> todos();
    QQmlListProperty<CalendarEvent> events();

    explicit Prueba(QObject* parent = 0);
    ~Prueba();
//    QString leer();

    //TODO: estas funciones no son reconocidas, comprobarlas
    Q_INVOKABLE bool loadCalendar();
    QVariantMap arreglo() const
    {
        return m_arreglo;
    }

    void setArreglo(QVariantMap &arreglo)
    {
        if (m_arreglo == arreglo)
            return;

        m_arreglo = arreglo;
        emit arregloChanged(arreglo);
    }

    QVariantMap customColorScheme() const
    {
        return m_customColorScheme;
    }

    Q_INVOKABLE bool customColorRule(QString, QString);

public slots:
    void fileChangedSlot(QString file);

    void setCustomColorScheme(QVariantMap &customColorScheme)
    {
        if (m_customColorScheme == customColorScheme)
            return;

        m_customColorScheme = customColorScheme;
        emit customColorSchemeChanged();
    }

Q_SIGNALS:
    void fileChanged();
    void uriChanged();
    void longaChanged();
    void todosChanged();
    void eventsChanged();

    void arregloChanged(QVariantMap arreglo);

    void customColorSchemeChanged();

private:
    QString _longa; //Debug output variable

    QString _uri;
    KCalCore::MemoryCalendar::Ptr _calendar;
    KCalCore::FileStorage* _storage = 0;
    QFileSystemWatcher _watcher;
    QList<CalendarToDo *> listToDos;
    QList<CalendarEvent *> listEvents;

    static void append_todo(QQmlListProperty<CalendarToDo> *list, CalendarToDo *todo);
    static CalendarToDo *at_todo(QQmlListProperty<CalendarToDo> *list, int index);
    static int count_todo(QQmlListProperty<CalendarToDo> *list);
    static void clear_todo(QQmlListProperty<CalendarToDo> *list);

    static void append_event(QQmlListProperty<CalendarEvent> *list, CalendarEvent *event);
    static CalendarEvent *at_event(QQmlListProperty<CalendarEvent> *list, int index);
    static int count_event(QQmlListProperty<CalendarEvent> *list);
    static void clear_event(QQmlListProperty<CalendarEvent> *list);
    QVariantMap m_arreglo;
    QVariantMap m_customColorScheme;
};

#endif // PRUEBA_H
