#ifndef INCIDENCE_H
#define INCIDENCE_H

#include <QObject>
#include <kcalcore/incidence.h>
#include <kcalcore/todo.h>
#include <QDate>

class Incidence : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString summary READ summary WRITE setSummary NOTIFY summaryChanged)
    Q_PROPERTY(int priority READ priority WRITE setPriority NOTIFY priorityChanged)
    Q_PROPERTY(QDate creationDate READ creationDate)

public:
    Q_INVOKABLE QString escribirLongaIncidence();

    Incidence(QObject *parent);

    ~Incidence();

    QString description();
    void setDescription(QString &description);

    QString summary();
    void setSummary(QString &summary);

    int priority();
    void setPriority(int priority);

    QDate creationDate();

Q_SIGNALS:
    void descriptionChanged();
    void summaryChanged();
    void priorityChanged();

    virtual KCalCore::Incidence* get_object() = 0;

protected:
    QString* longa;

private:
    KCalCore::Incidence* _object;
};


#endif // INCIDENCE_H
