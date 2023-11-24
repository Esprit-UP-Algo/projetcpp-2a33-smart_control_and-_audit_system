#include "employe.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QSqlQueryModel>
#include <QtGui>
#include <QtWidgets>


employe::employe()
{
   id_emp=0;nom_emp="";prenom_emp="";email="";titre_de_poste="";numero_de_telephone=0;etat=0;passe=0;
}
employe::employe(int id_emp,QString nom_emp, QString prenom_emp, QString email, QString titre_de_poste, int numero_de_telephone, int etat, int passe){
    this->id_emp=id_emp;
    this->nom_emp=nom_emp;
    this->prenom_emp=prenom_emp;
    this->email=email;
    this->titre_de_poste=titre_de_poste;
    this->numero_de_telephone=numero_de_telephone;
    this->etat=etat;
    this->passe=passe;


}
int employe::getID()
{
    return id_emp;
}
QString employe:: getNom()
{
    return nom_emp;
}
QString employe:: getPrenom()
{
    return prenom_emp;
}
QString employe:: getemail()
{
    return email;
}
QString employe::getTitre()
{
    return email;
}

int employe::getnumero()
{
    return numero_de_telephone;
}
int employe::getEtat(){
    return etat;

}
int employe::getPasse(){
    return passe;

}
void employe::setId(int id_emp)
{
    this->id_emp=id_emp;
}
void employe:: setNom(QString nom_emp)
{
    this->nom_emp=nom_emp;
}
void employe::  setPrenom(QString prenom_emp)
{
   this->prenom_emp=prenom_emp;
}
void employe::setemail(QString email)
{
    this->email=email;
}
void employe:: setTitre(QString titre_de_poste)
{
    this->titre_de_poste=titre_de_poste;
}

void employe:: setNumero(int numero_de_telephone)
{
    this->numero_de_telephone=numero_de_telephone;
}
void employe::setEtat(int etat)
{
    this->etat=etat;
}
void employe::setPasse(int passe)
{
    this->passe=passe;
}
bool employe:: ajouter ()
{
    QSqlQuery query;
   // QString id_emp_string= QString::number(id_emp);

    query.prepare("INSERT INTO EMPLOYE (ID_EMP,NOM_EMP, PRENOM_EMP, EMAIL, TITRE_DE_POSTE,NUMERO_DE_TELEPHONE,ETAT,PASSE) "
                             "VALUES ( :id_emp,:nom_emp, :prenom_emp, :email, :titre_de_poste, :numero_de_telephone, :etat, :passe)");
    query.bindValue(":id_emp", id_emp);
             query.bindValue(":nom_emp", nom_emp);
             query.bindValue(":prenom_emp", prenom_emp);
             query.bindValue(":email", email);
             query.bindValue(":titre_de_poste", titre_de_poste);
             query.bindValue(":numero_de_telephone", numero_de_telephone);
             query.bindValue(":etat", etat);
             query.bindValue(":passe", passe);


             query.exec();
             return query.exec();

}
bool employe :: supprimer(int id_emp)
{
    QSqlQuery query;
              query.prepare("Delete from EMPLOYE where id_emp=:id_emp");
              query.bindValue(":id_emp", id_emp);
              return query.exec();
}
QSqlQueryModel* employe::afficher()
{
    QSqlQueryModel* model=new QSqlQueryModel();


    model->setQuery("SELECT * FROM EMPLOYE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("id_emp"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom_emp"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom_emp"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("titre_de_poste"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("numero_de_telephone"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("etat"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("passe"));



    return model;



}
bool employe::modifier()
{


    QSqlQuery query;
         query.prepare("UPDATE EMPLOYE SET  NOM_EMP = :nom_emp, PRENOM_EMP = :prenom_emp , EMAIL = :email, TITRE_DE_POSTE = :titre_de_poste, ETAT= :etat, PASSE=:passe where  ID_EMP = :id_emp");
         query.bindValue(":id_emp", id_emp);
         query.bindValue(":nom_emp", nom_emp);
         query.bindValue(":prenom_emp", prenom_emp);
         query.bindValue(":email", email);
         query.bindValue(":titre de poste", titre_de_poste);
         query.bindValue(":numero de telephone", numero_de_telephone);
         query.bindValue(":etat", etat);
         query.bindValue(":passe", passe);


          bool success = query.exec();
         return success;
}
QSqlQuery employe::rechercher_id(int id_emp,QString nom_emp , QString prenom_emp){
    QSqlQuery query;
        query.prepare("SELECT * from EMPLOYE  where ID_EMP = :id_emp OR NOM_EMP= :nom_emp OR PRENOM_EMP= :prenom_emp ");
        query.bindValue(":id_emp", id_emp);
        query.bindValue(":nom_emp", nom_emp);
        query.bindValue(":prenom_emp", prenom_emp);
query.exec();

        return query;
}
QSqlQueryModel * employe::tri_titre_de_poste(){
    QSqlQuery *q = new QSqlQuery();
             QSqlQueryModel *model = new QSqlQueryModel();
             q->prepare("SELECT * FROM EMPLOYE ORDER BY TITRE_DE_POSTE ");
             q->exec();
             model->setQuery(*q);

             return model;

}

int employe::compteretat1()
{
    int etat1;
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM EMPLOYE WHERE etat = 1");
       query.next();
      etat1 = query.value(0).toInt();
        return etat1;
}
int employe::compteretat0()
{
    int etat0;
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM EMPLOYE WHERE etat = 0");
     query.next();
     etat0 = query.value(0).toInt();
        return etat0;

}


void employe::affecterEmployeSelonDisponibilite(int id_emp) {
    QSqlQuery query;
    query.prepare("UPDATE employe SET etat = :nouvelEtat WHERE id_emp = :id");
    query.bindValue(":id", id_emp);

    if (compteretat0() > 0) {
        query.bindValue(":nouvelEtat", 1);  // 1 pour disponible
    } else {
        query.bindValue(":nouvelEtat", 0);  // 0 pour non disponible
    }

    if (query.exec()) {
        qDebug() << "Employé affecté selon la disponibilité avec succès.";
        QMessageBox::information(nullptr,"Disponible","employe affecte avec succee!");
    } else {
        qDebug() << "Erreur lors de l'affectation de l'employé selon la disponibilité : " << query.lastError();
    }
 }



