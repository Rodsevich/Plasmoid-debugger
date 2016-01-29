#include "incidence.h"

Incidence::Incidence(QObject *parent)
    : QObject(parent)
{
}

Incidence::Incidence(QObject *parent, FileCalendar *calendar)
    : Incidence(parent)
{
    _calendar = calendar;
}

Incidence::~Incidence(){
}

QString Incidence::uid()
{
    return get_object()->uid();
}

QString Incidence::description(){
    return get_object()->description();
}

void Incidence::setDescription(QString &description){
    get_object()->setDescription(description, true); //true = isRich
}

QString Incidence::summary(){
    return get_object()->summary();
}

void Incidence::setSummary(QString &summary){
    get_object()->setSummary(summary, true); //true = isRich
}

int Incidence::priority(){
    return get_object()->priority();
}

void Incidence::setPriority(int priority){
    get_object()->setPriority(priority);
}

QDate Incidence::creationDate(){
    KDateTime createdDT = get_object()->created();
    return createdDT.date();
}

QString Incidence::parentUid()
{
    return get_object()->relatedTo(KCalCore::Incidence::RelTypeParent);
}

QString Incidence::siblingUid()
{//As the only relation supported atm is parent, I've to use a customProperty
    return get_object()->customProperty("qmlCalendar","sibling");
}

QString Incidence::childUid()
{//As the only relation supported atm is parent, I've to use a customProperty
    return get_object()->customProperty("qmlCalendar","child");
}

void Incidence::setParentUid(QString uid)
{
    get_object()->setRelatedTo(uid, KCalCore::Incidence::RelTypeParent);
    emit parentUidChanged();
}

void Incidence::setSiblingUid(QString uid)
{
    get_object()->setCustomProperty("qmlCalendar","sibling",uid);
    emit siblingUidChanged();
}

void Incidence::setChildUid(QString uid)
{
    get_object()->setCustomProperty("qmlCalendar","child",uid);
    emit childUidChanged();
}

//namespace Incidence{
//    QString description(){
//	return get_object()->description();
//    }

//    void setDescription(QString &description){
//	get_object()->setDescription(description, true); //true = isRich
//    }

//    QString summary(){
//	return get_object()->summary();
//    }

//    void setSummary(QString &summary){
//	get_object()->setSummary(summary, true); //true = isRich
//    }

//    int priority(){
//	return get_object()->priority()
//    }

//    void setPriority(int priority){
//	get_object()->setPriority(priority);
//    }

//    QDate creationDate(){
//	KDateTime createdDT = get_object()->created();
//	return createdDT->date();
//    }
//}
