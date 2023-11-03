#include "finance.h"
#include "Connexion.h"

Finance::Finance()
{
    facture_id=0;
    date_de_facture = QDate::fromString( "dd/MM/yyyy");
    date_de_paiment = QDate::fromString("dd/MM/yyyy");
    description="";
    TVA=0.0;
    montant=0.0;
}

Finance::Finance (int facture_id  , QDate date_de_facture, QDate date_de_paiment,float TVA,QString description, float montant)
{
    this->facture_id = facture_id;
    this->date_de_facture= date_de_facture;
    this->date_de_paiment = date_de_paiment;
    this->description = description;
    this->TVA = TVA;
    this->montant = montant;

}


bool Finance::ajouter()
{


    QSqlQuery query;
    QString tva_string =QString:: number(TVA);
    QString montant_string =QString:: number(montant);

    query.prepare("INSERT INTO FINANCE (FACTURE_ID, DATE_DE_FACTURE, DATE_DE_PAIMENT, TVA, DESCRIPTION,MONTANT)"
                  "VALUES(:FACTURE_ID, :DATE_DE_FACTURE, :DATE_DE_PAIMENT, :TVA ,:DESCRIPTION,  :MONTANT)");

    query.bindValue(":FACTURE_ID",facture_id);
    query.bindValue(":DATE_DE_FACTURE", date_de_facture);
    query.bindValue(":DATE_DE_PAIMENT", date_de_paiment);
    query.bindValue(":TVA", tva_string);
    query.bindValue(":DESCRIPTION", description);
    query.bindValue(":MONTANT", montant_string);


    return query.exec();
}


QSqlQueryModel *Finance::afficher()
{
QSqlQueryModel *model=new QSqlQueryModel;
model ->setQuery("select * from Finance");
model ->setHeaderData(0, Qt::Horizontal, QObject::tr("facture_id"));
model ->setHeaderData(1, Qt::Horizontal, QObject::tr("date_de_facture"));
model ->setHeaderData(2, Qt::Horizontal, QObject::tr("date_de_paiment"));
model ->setHeaderData(3, Qt::Horizontal, QObject::tr("TVA"));
model ->setHeaderData(4, Qt::Horizontal, QObject::tr("description"));
model ->setHeaderData(5, Qt::Horizontal, QObject::tr("montant"));

return model;

}

bool Finance::supprimer(int facture_id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FINANCE WHERE FACTURE_ID = :FACTURE_ID");
    query.bindValue(":FACTURE_ID", facture_id);

    return query.exec();
}


bool Finance::modifier()
{
    QSqlQuery query;
    QString tva_string =QString:: number(TVA);
    QString montant_string =QString:: number(montant);
    query.prepare("UPDATE FINANCE SET DATE_DE_FACTURE = :DATE_DE_FACTURE, DATE_DE_PAIMENT = :DATE_DE_PAIMENT, TVA = :TVA, DESCRIPTION = :DESCRIPTION, MONTANT = :MONTANT WHERE FACTURE_ID = :FACTURE_ID");

    query.bindValue(":FACTURE_ID", facture_id);
    query.bindValue(":DATE_DE_FACTURE", date_de_facture);
    query.bindValue(":DATE_DE_PAIMENT", date_de_paiment);
    query.bindValue(":TVA", tva_string);
    query.bindValue(":DESCRIPTION", description);
    query.bindValue(":MONTANT", montant_string);

    return query.exec();
}
