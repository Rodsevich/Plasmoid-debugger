#include "incidence.h"

Incidence::Incidence(QObject *parent)
    : QObject(parent){
}

Incidence::~Incidence(){
}

QString Incidence::escribirLongaIncidence(){
    return *this->longa;
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
