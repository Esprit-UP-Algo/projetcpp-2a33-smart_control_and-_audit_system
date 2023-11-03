#ifndef FINANCE_H
#define FINANCE_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QDate>




class Finance
{
public:


    Finance();
    Finance (int, QDate,QDate, float,QString, float);

    //GETTERS
    int get_idf(){return facture_id;}
    QDate get_datefacture(){return date_de_facture;}
    QDate get_datepaiment(){return date_de_paiment;}
    QString get_description(){return description;}
    int get_TVA(){return TVA;}
    int get_montant(){return montant;}
    //SETTERS
    void setidf(int facture_id){this->facture_id = facture_id;}
    void setdatefacture(QDate date_de_facture){this->date_de_facture= date_de_facture;}
    void setdatepaiment(QDate date_de_paiment){this->date_de_paiment = date_de_paiment;}
    void setdescription(QString description){this->description = description;}
    void setTVA(int TVA){this->TVA = TVA;}
    void setmontant(int montant){this->montant = montant;}

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer (int facture_id);
    bool modifier();

private:
       int facture_id;
       QDate date_de_facture, date_de_paiment;
       float TVA, montant;
       QString description;




};

#endif // FINANCE_H
