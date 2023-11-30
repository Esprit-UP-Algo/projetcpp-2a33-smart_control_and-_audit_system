#ifndef FINANCE_H
#define FINANCE_H
#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QDate>
#include<QMap>
#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>





class Finance
{
public:


    Finance();
    Finance (int, QDate,QDate, float,QString, float,int);

    //GETTERS
    int get_idf(){return facture_id;}
    QDate get_datefacture(){return date_de_facture;}
    QDate get_datepaiment(){return date_de_paiment;}
    QString get_description(){return description;}
    int get_TVA(){return TVA;}
    int get_montant(){return montant;}
    float getMontantTotal(int facture_id, int expertise_id);
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
    bool rembourser(int facture_id);
    QSqlQueryModel * rechercher(QString);
    void genererPDF(int facture_id, float montantRembourse);
    void generePDF(int facture_id, float montantTotal, float coutUnitaire, int nombreDeVisites, float coutMateriel );
    float Montanttotal(int facture_id, int expertise_id);





private:
       int facture_id;
       QDate date_de_facture, date_de_paiment;
       float TVA, montant;
       QString description;
       QString test;
       int expertise_id;




};

#endif // FINANCE_H
