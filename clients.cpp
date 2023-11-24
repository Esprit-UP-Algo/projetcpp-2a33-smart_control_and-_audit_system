#include "clients.h"
#include <QString>
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include "mainwindow.h"
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

clients::~clients()
{

}

clients::clients()
{
    telephone_c=0;
    cin=0;
    nom_c="";
    prenom_c="";
    adresse_c="";
    client_id="";

}

clients::clients(QString nom_c,QString prenom_c,QString adresse_c,int telephone_c ,int cin,QString client_id)
{

    this->nom_c=nom_c;
    this->prenom_c=prenom_c;
    this->adresse_c=adresse_c;
    this->telephone_c=telephone_c;
    this->cin=cin;
    this->client_id=client_id;
}

int clients::get_cin()
{
    return cin;
}
int clients::get_telephone_c()
{
    return telephone_c;
}
QString clients::get_nom_c()
{
    return nom_c;
}
QString clients::get_prenom_c()
{
    return prenom_c;
}
QString clients::get_adresse_c()
{
    return adresse_c;
}
QString clients::get_client_id()
{
    return client_id;
}

void clients::set_cin(int c)
{
    this->cin=c;
}

void clients::set_telephone_c(int c)
{
    this->telephone_c=c;
}
void clients::set_nom_c(QString s)
{
    this->nom_c=s;
}
void clients::set_prenom_c(QString s)
{
    this->prenom_c=s;
}
void clients::set_adresse_c(QString s)
{
    this->adresse_c=s;
}
void clients::set_client_id(QString s)
{
    this->client_id=s;
}

bool clients::ajouter_clients()
{
    QSqlQuery q;
    QSqlQuery q2;//tab 2

    //QString id = QString::number(client_id);
    QString datee = QDate::currentDate().toString(); // date de type date vers type string

    q.prepare("INSERT INTO CLIENTS (NOM_C,PRENOM_C,ADRESSE_C,TELEPHONE_C,CIN,CLIENT_ID)" "VALUES (:n,:p,:a,:t,:c,:i)");
    q2.prepare("INSERT INTO HISTORIQUECLIENT (CIN,REQUETTE,DATEE)" "VALUES (:i, :requette, :datee)"); //insertion f tableau 2

    q.bindValue(":n",nom_c);
    q.bindValue(":p",prenom_c);
    q.bindValue(":a",adresse_c);
    q.bindValue(":t",telephone_c);
    q.bindValue(":c",cin);
    q.bindValue(":i",client_id);

    q2.bindValue(":i",cin);
    q2.bindValue(":requette","AJOUTER");
    q2.bindValue(":datee",datee);

   // q.exec();
   // q2.exec();

      if (q.exec()){ // condition: l'ajout tab1=ajout tab 2
            q2.exec();
            return true;
        }
        return false;
}
QSqlQueryModel *clients::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM clients");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CLIENT_ID"));


    return model;
}
bool clients::supprimer(QString i)
{
    QSqlQuery q;
    QSqlQuery q2;
    QString datee = QDate::currentDate().toString();

    q2.prepare("INSERT INTO historiqueclient (CIN,REQUETTE,DATEE)" "VALUES (:i, :requette, :datee)");

    q.prepare("DELETE FROM clients WHERE CLIENT_ID = :i");
    q.bindValue(":i",i);
    //q2.prepare("INSERT INTO historiqueclient (CIN,REQUETTE,DATEE)" "VALUES (:i, :requette, :datee)");


    q2.bindValue(":i",i);
    q2.bindValue(":requette","SUPPRIMER");
    q2.bindValue(":datee",datee);

    if (q.exec()){
        q2.exec();
        return true;
    }
    return false;
}

bool clients::modifier()
{

         QSqlQuery update_query ;
          update_query.prepare("UPDATE clients SET NOM_C= :n,PRENOM_C= :p,ADRESSE_C= :a,TELEPHONE_C= :t,CIN= :c WHERE CLIENT_ID= :i");

                    update_query.bindValue(":i", client_id);
                    update_query.bindValue(":n", nom_c);
                    update_query.bindValue(":p", prenom_c);
                    update_query.bindValue(":a",adresse_c);
                    update_query.bindValue(":t", telephone_c);
                    update_query.bindValue(":c", cin);

                    if (update_query.exec()) {
                        qDebug() << "patient with ID =" << this->client_id << "was updated from the database.";
                        return true;
                    } else {
                        qDebug() << "Error updating patient:" << update_query.lastError().text();
                        return false ;
                    }

         return false ;
     }



QSqlQueryModel* clients::tri_CIN(){ //requete base
    QSqlQueryModel* model=new QSqlQueryModel();

            model->setQuery("SELECT * FROM CLIENTS ORDER BY CIN ASC");

        return model;
}
QSqlQueryModel* clients::tri_nom(){
    QSqlQueryModel* model=new QSqlQueryModel();

            model->setQuery("SELECT * FROM CLIENTS ORDER BY NOM_C ASC");

        return model;
}
QSqlQueryModel* clients::tri_prenom(){
    QSqlQueryModel* model=new QSqlQueryModel();

            model->setQuery("SELECT * FROM CLIENTS ORDER BY PRENOM_C ASC");

        return model;
}




// recherche bech tebda mena


QSqlQueryModel* clients::afficherRecherche(QString column, QString input){
    qDebug() << column;
    qDebug() << input;
    QSqlQueryModel* model= new QSqlQueryModel();

    model->setQuery("SELECT * FROM CLIENTS WHERE "+column+" = '"+input+"'");

            model->setHeaderData(0,Qt::Horizontal,QObject::tr("NOM"));
            model->setHeaderData(1,Qt::Horizontal,QObject::tr("PRENOM"));
            model->setHeaderData(2,Qt::Horizontal,QObject::tr("ADRESSE"));
            model->setHeaderData(3,Qt::Horizontal,QObject::tr("TELEPHONE"));
            model->setHeaderData(4,Qt::Horizontal,QObject::tr("CIN"));
            model->setHeaderData(5,Qt::Horizontal,QObject::tr("CLIENTS_ID"));
        qDebug() << model;
        return model;
}


QSqlQueryModel* clients::afficher_historique(){
    QSqlQueryModel * model=new QSqlQueryModel();

        model->setQuery("select * from historiqueclient");
        model->setHeaderData(0,Qt::Horizontal,QObject::tr("CIN"));
        model->setHeaderData(1,Qt::Horizontal,QObject::tr("REQUETE"));
        model->setHeaderData(2,Qt::Horizontal,QObject::tr("DATE"));

        return model;
}


QMap<QString, int> clients::getAdresseStatistics() {
    QMap<QString, int> statistics;

    QSqlQuery query;
    query.prepare("SELECT ADRESSE_C, COUNT(*) FROM CLIENTS GROUP BY ADRESSE_C");
    if (query.exec()) {
        while (query.next()) {
            QString adresse = query.value(0).toString(); //transformer en string
            int count = query.value(1).toInt();
            statistics[adresse] = count;
        }
    }

    return statistics;
}
/*QMap<QString, int> clients::getEtatStatistics() {
    QMap<QString, int> statistics;

    QSqlQuery query;
    query.prepare("SELECT ETAT, COUNT(*) FROM CLIENTS GROUP BY ETAT");
    if (query.exec()) {
        while (query.next()) {
            QString etat = query.value(0).toString();
            int count = query.value(1).toInt();
            statistics[etat] = count;
        }
    }

    return statistics;
}*/



/*bool clients::ajouter_avis(){
    QSqlQuery q;

     q.prepare("INSERT INTO CLIENTS (COMMENTAIRE, AVIS)" "VALUES (:commentaire, :avis)");

     q.bindValue(":commentaire",commentaire);
     q.bindValue(":avis",avis);


         q.exec();
}*/
