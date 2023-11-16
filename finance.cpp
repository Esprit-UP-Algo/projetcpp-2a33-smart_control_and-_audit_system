#include "finance.h"
#include "Connexion.h"
#include <QPdfWriter>

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

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Error during deletion:" << query.lastError().text();
        return false;
    }
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



bool Finance::rembourser(int facture_id)
{
    QSqlQuery rechercheQuery;
    rechercheQuery.prepare("SELECT MONTANT FROM FINANCE WHERE FACTURE_ID = :FACTURE_ID");
    rechercheQuery.bindValue(":FACTURE_ID", facture_id);

    if (rechercheQuery.exec() && rechercheQuery.next()) {
        float montant = rechercheQuery.value(0).toFloat();

        float pourcentage = 0.2; // le pourcentage est de 20%
        double montantRembourse = montant * pourcentage;

        genererPDF(facture_id, montantRembourse);

        return true; // Remboursement réussi
    } else {
        qDebug() << "Erreur de recherche de la facture:" << rechercheQuery.lastError().text();
        return false; // Facture introuvable
    }
}
void Finance::genererPDF(int facture_id, float montantRembourse)
{
    // Créer le fichier PDF
    QPdfWriter pdf(QString("C:/FINANCE/Remboursement_Facture_%1.pdf").arg(facture_id));
    QPainter painter(&pdf);
    int i = 4000;

    // Définir la police et la taille
       painter.setFont(QFont("Bodoni MT", 18));

       // Définir la couleur du texte
       painter.setPen(Qt::black);

       painter.drawRect(0, 3000, 6300, 2600);
       painter.drawText(100, 100, QString("Remboursement de la facture ID %1").arg(facture_id));
       painter.drawText(800, 1500, QString("Montant remboursé : %1").arg(montantRembourse));

    QSqlQuery rechercheQuery;
    rechercheQuery.prepare("SELECT * FROM FINANCE WHERE FACTURE_ID = :FACTURE_ID");
    rechercheQuery.bindValue(":FACTURE_ID", facture_id);

    if (rechercheQuery.exec() && rechercheQuery.next()) {
        QSqlRecord record = rechercheQuery.record();


        painter.drawRect(0,3000,9600,500);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 10));

        painter.drawText(100,3000,"FACTURE_ID");
        painter.drawText(1500,3000,"DATE_DE_FACTURE");
        painter.drawText(3500,3000,"DATE_DE_PAIMENT");
        painter.drawText(6000,3000,"TVA");
        painter.drawText(7000,3000,"DESCRIPTION");
        painter.drawText(8500,3000,"MONTANT");
        painter.drawText(100,i,rechercheQuery.value(0).toString());
        painter.drawText(1500,i,rechercheQuery.value(1).toString());
        painter.drawText(3500,i,rechercheQuery.value(2).toString());
        painter.drawText(6000,i,rechercheQuery.value(3).toString());
        painter.drawText(7000,i,rechercheQuery.value(4).toString());
        painter.drawText(8500,i,rechercheQuery.value(5).toString());
        i = i +1000;
    }

    painter.end();
}


QSqlQueryModel *Finance::rechercher(QString test)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM FINANCE WHERE FACTURE_ID LIKE :test");
    query.bindValue(":test", "%" + test + "%");
    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("facture_id"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("date_de_facture"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("date_de_paiment"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("TVA"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("description"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("montant"));
    } else {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
    }
      return model;
}





