#ifndef INCIDENCE_H
#define INCIDENCE_H

#include <QObject>
#include <kcalcore/incidence.h>
#include <kcalcore/todo.h>
#include <QDate>

class FileCalendar;
class Incidence : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uid READ uid)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString summary READ summary WRITE setSummary NOTIFY summaryChanged)
    Q_PROPERTY(int priority READ priority WRITE setPriority NOTIFY priorityChanged)
    Q_PROPERTY(QDate creationDate READ creationDate)
    Q_PROPERTY(QString parentUid READ parentUid WRITE setParentUid NOTIFY parentUidChanged)
    Q_PROPERTY(QString siblingUid READ siblingUid WRITE setSiblingUid NOTIFY siblingUidChanged)
    Q_PROPERTY(QString childUid READ childUid WRITE setChildUid NOTIFY childUidChanged)

public:

    Incidence(QObject *parent);
    Incidence(QObject *parent, FileCalendar *calendar);
    ~Incidence();

    QString uid();

    QString description();
    void setDescription(QString &description);

    QString summary();
    void setSummary(QString &summary);

    int priority();
    void setPriority(int priority);

    QDate creationDate();

    QString parentUid();
    QString siblingUid();
    QString childUid();

    void setParentUid(QString uid);
    void setSiblingUid(QString uid);
    void setChildUid(QString uid);

    virtual KCalCore::Incidence* get_object() = 0;

Q_SIGNALS:
    void descriptionChanged();
    void priorityChanged();
    void summaryChanged();
    void siblingUidChanged();
    void parentUidChanged();
    void childUidChanged();

protected:
    FileCalendar* _calendar;
private:
    KCalCore::Incidence* _object;
};


#endif // INCIDENCE_H
