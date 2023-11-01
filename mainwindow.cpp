#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rapport.h"
#include <QMessageBox>
#include <QSqlQuery>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_rapport->setModel(R.afficher());
    ui->comboBox_supprimer->setModel(R.afficher());
    ui->comboBox_modifier->setModel(R.afficher());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    // Réinitialisez les étiquettes d'erreur à vide
    ui->lblErreurId->setText("");
    ui->lblErreurEXpersiteID->setText("");
    ui->lblErreurResultat->setText("");

    // Récupération des valeurs des champs d'entrée
    QString str_RAPPORT_ID = ui->lineEdit_id->text();
    int RAPPORT_ID = str_RAPPORT_ID.toInt();


    QString RESULTAT_DE_RAPPORT = ui->lineEdit_resultat_rapport->currentText();
    QDate DATE_VISITE = ui->dateEdit_date_rapport->date();

    QString str_EXPERTISE_ID = ui->lineEdit_EXPERTISE_ID->text();
    int EXPERTISE_ID =str_EXPERTISE_ID.toInt();

    // Validez les champs d'entrée
    bool isValid = true;
    if (str_RAPPORT_ID.isEmpty()) {
        ui->lblErreurId->setText("<font color='red'>ID ne peut pas être vide !</font>");
        isValid = false;
    }

    // Validation pour le résultat de rapport, assurez-vous qu'il n'est pas vide
    if (RESULTAT_DE_RAPPORT.isEmpty()) {
        ui->lblErreurResultat->setText("<font color='red'>RESULTAT DE RAPPORT ne peut pas être vide !</font>");
        isValid = false;
    }

    if (str_EXPERTISE_ID.isEmpty()) {
        ui->lblErreurEXpersiteID->setText("<font color='red'>Type ne peut pas être négatif !</font>");
        isValid = false;
    }

    // Ajouter une vérification de l'existence de l'ID dans la base de données
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT RAPPORT_ID FROM RAPPORT WHERE RAPPORT_ID = :RAPPORT_ID");
    checkQuery.bindValue(":RAPPORT_ID", RAPPORT_ID);
    if (checkQuery.exec() && checkQuery.next()) {
        ui->lblErreurId->setText("<font color='red'>RAPPORT ID existe déjà !</font>");
        isValid = false;
    }

    // Continuez avec les autres validations d'entrée...

    if (isValid) {
        // Si l'entrée est valide et que l'ID n'existe pas, procédez à l'ajout de l'activité

        Rapport R(RAPPORT_ID, RESULTAT_DE_RAPPORT, DATE_VISITE, EXPERTISE_ID);
        bool test = R.ajouter();
        if (test) {
            QMessageBox::information(nullptr, QObject::tr("OK"),
                QObject::tr("Ajout effectué.\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            ui->tab_rapport->setModel(R.afficher());
            ui->comboBox_supprimer->setModel(R.afficher());
            ui->comboBox_modifier->setModel(R.afficher());
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                QObject::tr("Ajout non effectué.\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }
    }
}


void MainWindow::on_pb_supprimer_clicked()
{
    int RAPPORT_ID=ui->comboBox_supprimer->currentText().toInt();

            bool test=R.supprimer(RAPPORT_ID);
            if(test)
            {
                ui->tab_rapport->setModel(R.afficher());
                ui->comboBox_supprimer->setModel(R.afficher());
                ui->comboBox_modifier->setModel(R.afficher());
                QMessageBox::information(nullptr, QObject::tr("Supprimer Rapport"),
                            QObject::tr("Rapport supprimé.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);

            }
            else
                QMessageBox::critical(nullptr, QObject::tr("Supprimer Rapport"),
                            QObject::tr("Erreur !.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
        }

void MainWindow::on_pb_modifier_clicked()
{


int RAPPORT_ID=ui->comboBox_modifier->currentText().toInt();
QString RESULTAT_DE_RAPPORT=ui->lineEdit_resultat_rapport_2->currentText();
QDate DATE_VISITE = QDate::fromString(ui->dateEdit_date_rapport_2->text(), "dd/MM/yyyy");
int EXPERTISE_ID=ui->lineEdit_EXPERTISE_ID_2->text().toInt();
Rapport R(RAPPORT_ID,RESULTAT_DE_RAPPORT,DATE_VISITE,EXPERTISE_ID);

            QSqlQuery query;





            bool test=R.modifier();
            if(test)
            {

                ui->tab_rapport->setModel(R.afficher());
                ui->comboBox_supprimer->setModel(R.afficher());
                ui->comboBox_modifier->setModel(R.afficher());



                QMessageBox::information(nullptr, QObject::tr("Modifier Rapport"),
                            QObject::tr("Rapport Modifier.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);

            }
            else
            {
                QMessageBox::critical(nullptr, QObject::tr("Modifier Rapport"),
                            QObject::tr("Erreur !.\n"
                                        "Click Cancel to exit."), QMessageBox::Cancel);
            }



}


void MainWindow::on_valider1_clicked()
{
    {
        QString RAPPORT_ID=ui->comboBox_modifier->currentText();
                  QSqlQuery query;
                  query.prepare("select * from RAPPORT where RAPPORT_ID='"+RAPPORT_ID+"'");
                  if (query.exec())
                  {
                   while (query.next())
                   {


                       ui->lineEdit_resultat_rapport_2->setCurrentText(query.value(1).toString());
                       ui->dateEdit_date_rapport_2->setDate(query.value(2).toDate());
                       ui->lineEdit_EXPERTISE_ID_2->setText(query.value(3).toString());





                   }
                  }


}}





void MainWindow::on_reset_clicked()
{
    ui->lineEdit_resultat_rapport_2->clear();
    ui->lineEdit_EXPERTISE_ID_2->clear();


}
