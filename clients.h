#ifndef CLIENTS_H
#define CLIENTS_H
#include <QString>
#include <QSqlQueryModel>



class clients
{
public:
        clients();
        clients(QString ,QString ,QString ,int  ,int ,QString );
        ~clients();



        int get_telephone_c();
        int get_cin();
        QString get_nom_c();
        QString get_prenom_c();
        QString get_adresse_c();
        QString get_client_id();
        void set_telephone_c(int);
        void set_cin(int);
        void set_nom_c(QString);
        void set_prenom_c(QString);
        void set_adresse_c(QString);
        void set_client_id(QString);

        bool ajouter_clients();
        QSqlQueryModel *afficher();
        bool supprimer(QString);
        bool modifier();
        bool ajouter_avis();



        QSqlQueryModel* afficherRecherche(QString column, QString input);


        QSqlQueryModel* tri_CIN();
        QSqlQueryModel* tri_nom();
        QSqlQueryModel* tri_prenom();

        QSqlQueryModel* afficher_historique();

        //QMap<QString, int> getTypeStatistics();
        //QMap<QString, int> getEtatStatistics();
        QMap<QString, int> getAdresseStatistics();

    private:
        int telephone_c,cin;
        QString nom_c,prenom_c,adresse_c,client_id;
};

#endif // CLIENTS_H
