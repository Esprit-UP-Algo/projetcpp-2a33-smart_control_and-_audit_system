#ifndef RAPPORT_H
#define RAPPORT_H
#include <QDate>
#include <QString>
#include <QDebug>
#include "QSqlQueryModel"
class Rapport
{
public:
    Rapport();
    Rapport(int,QString,QDate,int);
    int getRAPPORT_ID(){return RAPPORT_ID ;}
    QString getRESULTAT_DE_RAPPORT(){return  RESULTAT_DE_RAPPORT;}
    QDate getDATE_VISITE(){return DATE_VISITE ;}
    int getEXPERTISE_ID(){return EXPERTISE_ID;}
    void setRAPPORT_ID(int RAPPORT_ID){this->RAPPORT_ID=RAPPORT_ID;}
    void setDATE_VISITE(QDate DATE_VISITE){this->DATE_VISITE=DATE_VISITE;}
    void setRESULTAT_DE_RAPPORT(QString RESULTAT_DE_RAPPORT){this->RESULTAT_DE_RAPPORT=RESULTAT_DE_RAPPORT;}
    void setEXPERTISE_ID(int EXPERTISE_ID){this->EXPERTISE_ID=EXPERTISE_ID;}


    bool ajouter();

    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier();
private:
     int RAPPORT_ID;
     QString RESULTAT_DE_RAPPORT;
     QDate DATE_VISITE;
     int EXPERTISE_ID;
};

#endif // RAPPORT_H
