#include "finance.h"
#include "Connexion.h"
#include <QPdfWriter>
#include <QSerialPort>
#include <QSerialPortInfo>

Finance::Finance()
{
    facture_id=0;
    date_de_facture = QDate::fromString( "dd/MM/yyyy");
    date_de_paiment = QDate::fromString("dd/MM/yyyy");
    description="";
    TVA=0.0;
    montant=0.0;
    expertise_id=0;
}

Finance::Finance (int facture_id  , QDate date_de_facture, QDate date_de_paiment,float TVA,QString description, float montant, int expertise_id)
{
    this->facture_id = facture_id;
    this->date_de_facture= date_de_facture;
    this->date_de_paiment = date_de_paiment;
    this->description = description;
    this->TVA = TVA;
    this->montant = montant;
    this->expertise_id=expertise_id;

}


bool Finance::ajouter()
{


    QSqlQuery query;
    QString tva_string =QString:: number(TVA);
    QString montant_string =QString:: number(montant);

    query.prepare("INSERT INTO FINANCE (FACTURE_ID, DATE_DE_FACTURE, DATE_DE_PAIMENT, TVA, DESCRIPTION,MONTANT, EXPERTISE_ID)"
                  "VALUES(:FACTURE_ID, :DATE_DE_FACTURE, :DATE_DE_PAIMENT, :TVA ,:DESCRIPTION,  :MONTANT, :EXPERTISE_ID)");

    query.bindValue(":FACTURE_ID",facture_id);
    query.bindValue(":DATE_DE_FACTURE", date_de_facture);
    query.bindValue(":DATE_DE_PAIMENT", date_de_paiment);
    query.bindValue(":TVA", tva_string);
    query.bindValue(":DESCRIPTION", description);
    query.bindValue(":MONTANT", montant_string);
    query.bindValue(":EXPERTISE_ID",expertise_id);


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
model ->setHeaderData(6, Qt::Horizontal, QObject::tr("expertise_id"));

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
    query.bindValue(":EXPERTISE_ID",expertise_id);


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

    QPdfWriter pdf(QString("C:/loujain/Remboursement_Facture_%1.pdf").arg(facture_id));
    QPainter painter(&pdf);
    int i = 4000;

    // police et taille
       painter.setFont(QFont("Bodoni MT", 18));

       //couleur du texte
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


          QSqlQueryModel* model=new QSqlQueryModel();
        QSqlQuery query;

        model->setQuery("SELECT * from FINANCE WHERE FACTURE_ID LIKE ('%"+test+"%') OR UPPER(MONTANT) LIKE UPPER('%"+test+"%') OR UPPER(DESCRIPTION) LIKE UPPER('%"+test+"%')");

          // model->setQuery("SELECT * FROM RAPPORT WHERE (RAPPORT_ID LIKE '%"+test+"%')");

        model->setHeaderData(0, Qt::Horizontal, QObject::tr("facture_id"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("date_de_facture"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("date_de_paiment"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("TVA"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("description"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("montant"));
         query.exec();
           return  model;//contenant les résultats de la recherche
}

float Finance::Montanttotal(int facture_id, int expertise_id)
{

    QSqlQuery query;
        query.prepare("SELECT NOMBRE_UNITAIRE, NOMBRE_VISITE, COUT_MATERIEL FROM EXPERTISE WHERE EXPERTISE_ID = :EXPERTISE_ID");
        query.bindValue(":EXPERTISE_ID", expertise_id);


        if (query.exec() && query.next())
        {
            float coutUnitaire = query.value(1).toFloat();
            int nombreDeVisites = query.value(2).toInt();
            float coutMateriel = query.value(3).toFloat();

            qDebug() << "Valeurs récupérées depuis EXPERTISE - NOMBRE_UNITAIRE:" << coutUnitaire << ", NOMBRE_VISITE:" << nombreDeVisites << ", COUT_MATERIEL:" << coutMateriel;

            // Calculer le montant total avec une TVA de 20%
            float tva = 0.2;  // Remplacez cela par la TVA réelle si nécessaire
                    float montantTotal = (nombreDeVisites * coutUnitaire + coutMateriel) * (1.0 + tva / 100.0);

                    qDebug() << "Montant total calculé avant la mise à jour dans FINANCE :" << montantTotal;

                    // Mettre à jour la table FINANCE avec le montant total
                    QSqlQuery updateQuery;
                    QString montant_string = QString::number(montantTotal);
                    updateQuery.prepare("UPDATE FINANCE SET MONTANT = :MONTANT_TOTAL WHERE FACTURE_ID = :FACTURE_ID");
                    updateQuery.bindValue(":FACTURE_ID", facture_id);
                    updateQuery.bindValue(":MONTANT_TOTAL", montant_string);

                    qDebug() << "Requête SQL d'UPDATE : " << updateQuery.lastQuery();
                    if (updateQuery.exec())
                            {
                                qDebug() << "Montant mis à jour avec succès.";

                                // Appel de la fonction genererPDF pour créer le PDF avec le montant total
                                generePDF(facture_id, montantTotal, coutUnitaire, nombreDeVisites, coutMateriel);

                                                               return montantTotal;
                            }
                            else
                            {
                                qDebug() << "Erreur lors de la mise à jour du montant:" << updateQuery.lastError().text();
                            }
                        }
                        else
                        {
                            qDebug() << "Erreur lors de la récupération des informations depuis la table Expertise:" << query.lastError().text();

                        }
        return -1;
                    }

void Finance::generePDF(int facture_id, float montantTotal, float coutUnitaire, int nombreDeVisites, float coutMateriel )
{
    QPdfWriter pdf(QString("C:/loujain/Facture_%1.pdf").arg(facture_id));
    QPainter painter(&pdf);
    int i = 4000;

    // police et taille
       painter.setFont(QFont("Bodoni MT", 18));

       //couleur du texte
       painter.setPen(Qt::black);

       painter.drawRect(0, 3000, 6300, 2600);
       painter.drawText(100, 100, QString(" Facture ID %1").arg(facture_id));
       painter.drawText(100, 5500, QString("Nombre Unitaire: %1").arg(coutUnitaire));
       painter.drawText(100, i + 500, QString("Nombre de visite: %1").arg(nombreDeVisites));
       painter.drawText(100, i + 1000, QString("Cout materiel: %1").arg(coutMateriel));
        painter.drawText(800, i + 2000, QString("Montant Total : %1").arg(montantTotal));
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

