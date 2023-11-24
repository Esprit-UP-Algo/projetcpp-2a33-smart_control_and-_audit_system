#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtSql>
#include <QPdfWriter>
#include <QPainter>

class employe
{

public:
    employe();
    employe(int ,QString  , QString , QString  , QString  ,  int ,int, int );
    int getID();
    QString getNom();
    QString getPrenom();
    QString getemail();
    QString getTitre();
    int getnumero();
    int getEtat();
    int getPasse();

    void setId(int id_emp);
    void setNom(QString nom_emp);
    void setPrenom(QString prenom_emp);
    void setemail(QString email);
    void setTitre(QString titre_de_poste);
    void setNumero(int numero_de_telephone);
    void setEtat(int etat);
    void setPasse(int passe);


    bool ajouter ();
    QSqlQueryModel * afficher ();
    bool supprimer (int);
    bool modifier();
 //   bool authentification(QString nom_emp, QString prenom_emp,QString titre_de_poste);
    int compteretat1();
    int compteretat0();
    void affecterEmployeSelonDisponibilite(int id_emp);

    QSqlQuery rechercher_id(int id_emp,QString nom_emp , QString prenom_emp);
        QSqlQueryModel * tri_titre_de_poste();

private:
    int id_emp;
    QString nom_emp, prenom_emp, email, titre_de_poste;
    int numero_de_telephone;
    int etat;
    int passe;




};

#endif // EMPLOYE_H
