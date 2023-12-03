#include "rapport.h"
#include "mainwindow.h"
#include <QDate>
#include <QSqlQuery>
#include <QString>
#include <QDebug>
#include <QCheckBox>
#include <QtWidgets>
#include <QtNetwork>
//
#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <utility>
//
Rapport::Rapport()
{
    RAPPORT_ID=0;
    RESULTAT_DE_RAPPORT="";
    DATE_VISITE=QDate();
    EXPERTISE_ID=0;

}


 Rapport::Rapport(int RAPPORT_ID,QString RESULTAT_DE_RAPPORT,QDate DATE_VISITE,int EXPERTISE_ID)
 {
 this->RAPPORT_ID=RAPPORT_ID;
 this->RESULTAT_DE_RAPPORT=RESULTAT_DE_RAPPORT;
 this->DATE_VISITE=DATE_VISITE;
 this->EXPERTISE_ID=EXPERTISE_ID;
 }


bool  Rapport::ajouter()
{
 QSqlQuery query;
QString res= QString::number(RAPPORT_ID);
query.prepare("insert into RAPPORT (RAPPORT_ID,RESULTAT_DE_RAPPORT,DATE_VISITE,EXPERTISE_ID)" "VALUES(:RAPPORT_ID,:RESULTAT_DE_RAPPORT,:DATE_VISITE,:EXPERTISE_ID)");
query.bindValue(":RAPPORT_ID",res);
query.bindValue(":RESULTAT_DE_RAPPORT",RESULTAT_DE_RAPPORT);
query.bindValue(":DATE_VISITE",DATE_VISITE);
query.bindValue(":EXPERTISE_ID",EXPERTISE_ID);
return query.exec();
}

bool Rapport::ajouter_resultat()
{
    QSqlQuery query;
        query.prepare("insert into RAPPORT (RESULTAT_DE_RAPPORT) VALUES(:RESULTAT_DE_RAPPORT)");
        query.bindValue(":RESULTAT_DE_RAPPORT", RESULTAT_DE_RAPPORT);
        return query.exec();
}

QSqlQueryModel * Rapport::afficher()
{
    QSqlQueryModel * model= new QSqlQueryModel();

    model->setQuery("SELECT * FROM RAPPORT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("RAPPORT_ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("RESULTAT_DE_RAPPORT"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATE_VISITE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("EXPERTISE_ID"));
    return model;
    }

QSqlQueryModel * Rapport::rechercher(QString test)
{
    QSqlQueryModel* model=new QSqlQueryModel();
  QSqlQuery query;

  model->setQuery("SELECT * from RAPPORT WHERE RAPPORT_ID LIKE ('%"+test+"%') OR UPPER(RESULTAT_DE_RAPPORT) LIKE UPPER('%"+test+"%') OR UPPER(DATE_VISITE) LIKE UPPER('%"+test+"%')OR UPPER(EXPERTISE_ID) LIKE UPPER('%"+test+"%')");

    // model->setQuery("SELECT * FROM RAPPORT WHERE (RAPPORT_ID LIKE '%"+test+"%')");

   model->setHeaderData(0, Qt::Horizontal, QObject::tr("RAPPORT_ID"));
   model->setHeaderData(1, Qt::Horizontal, QObject::tr("RESULTAT_DE_RAPPORT"));
   model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATE_VISITE"));
   model->setHeaderData(3, Qt::Horizontal, QObject::tr("EXPERTISE_ID"));
   query.exec();
     return  model;//contenant les résultats de la recherche
}


bool Rapport::supprimer(int RAPPORT_ID)
{
    QSqlQuery query ;
    query.prepare("DELETE from RAPPORT where RAPPORT_ID =:RAPPORT_ID") ;
    query.bindValue(":RAPPORT_ID",RAPPORT_ID);
    return query.exec();
}
bool Rapport::modifier()

 {      QSqlQuery query;
        query.prepare("update RAPPORT set RAPPORT_ID=:RAPPORT_ID,RESULTAT_DE_RAPPORT=:RESULTAT_DE_RAPPORT,DATE_VISITE=:DATE_VISITE ,EXPERTISE_ID=:EXPERTISE_ID where RAPPORT_ID=:RAPPORT_ID");
        query.bindValue(":RAPPORT_ID",RAPPORT_ID);
        query.bindValue(":RESULTAT_DE_RAPPORT",RESULTAT_DE_RAPPORT);
        query.bindValue(":DATE_VISITE", DATE_VISITE);
        query.bindValue(":EXPERTISE_ID", EXPERTISE_ID);


            return query.exec();
}


QSqlQueryModel * Rapport::rechercher1(QString text)
{
  QSqlQueryModel* model=new QSqlQueryModel();
  QSqlQuery query;
  model->setQuery("SELECT * from RAPPORT WHERE RAPPORT_ID LIKE ('%"+text+"%') ");
   model->setHeaderData(1, Qt::Horizontal, QObject::tr("RESULTAT_DE_RAPPORT"));
   query.exec();
     return  model;//contenant les résultats de la recherche
}

