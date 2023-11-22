#ifndef RAPPORT_H
#define RAPPORT_H
#include <QDate>
#include <QString>
#include <QDebug>
#include "QSqlQueryModel"
//
#include <QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
//
#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
//
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
    bool ajouter_resultat();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel *rechercher(QString);
   QSqlQueryModel* afficher_historique();
   // int somme_cases_cochees();
   // void envoyerNotification();
   //void envoyerNotification(int);

private:
     QString test;
     int RAPPORT_ID;
     QString RESULTAT_DE_RAPPORT;
     QDate DATE_VISITE;
     int EXPERTISE_ID;

};

#endif // RAPPORT_H
