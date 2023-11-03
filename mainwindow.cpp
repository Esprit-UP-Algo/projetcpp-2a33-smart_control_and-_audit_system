#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "Connexion.h"
#include "finance.h"
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   //ui->lineEditIdfacture->setValidator(new QIntValidator(0,99999999,this));
   ui->tableViewFinance->setModel(Fimp.afficher());

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pb_ajouter_clicked()

{
  int facture_id = ui->lineEditIdfacture->text().toInt();
  QDate date_de_facture= ui->DateEditfacture->date();
  QDate date_de_paiment= ui->DateEditpaiment->date();
  QString  description= ui->lineEditdescription->text();
  float TVA = ui->lineEditTVA->text().toFloat();
  float montant = ui->lineEditmontant->text().toFloat();

  Finance F(facture_id, date_de_facture, date_de_paiment,TVA, description, montant);

  bool test= F.ajouter();
  if (test)
  {
      ui->tableViewFinance->setModel(Fimp.afficher());
      QMessageBox::information(nullptr, QObject::tr("OK"),
                  QObject::tr("Ajout effectue.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);

}
  else
      QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                  QObject::tr("Ajout non effectue.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);



}

void MainWindow::on_pb_supprimer_clicked()
{
    int facture_id = ui->lineEditIdfacture->text().toInt();
    bool test=Fimp.supprimer(facture_id);
    if (test)
    {
        ui->tableViewFinance->setModel(Fimp.afficher()); //rafraîchir l'affichage des données après la suppression.
        QMessageBox::information(nullptr, QObject::tr("OK"),
                    QObject::tr("suppresion effectue.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

  }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                    QObject::tr("suppresion non effectue.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);



}



void MainWindow::on_pb_modifier_clicked()
{
    int facture_id = ui->lineEditIdfacture->text().toInt();
    QDate date_de_facture= ui->DateEditfacture->date();
    QDate date_de_paiment= ui->DateEditpaiment->date();
    QString description = ui->lineEditdescription->text();
    float TVA = ui->lineEditTVA->text().toFloat();
    float montant = ui->lineEditmontant->text().toFloat();

      Finance F(facture_id, date_de_facture, date_de_paiment, TVA, description, montant);

      bool test = F.modifier();
      if (test)
      {
          ui->tableViewFinance->setModel(Fimp.afficher()); //rafraîchir l'affichage des données après la modification.
          QMessageBox::information(nullptr, QObject::tr("OK"),
              QObject::tr("Modification effectuée.\nClick Cancel to exit."), QMessageBox::Cancel);
      }
      else
      {
          QMessageBox::critical(nullptr, QObject::tr("Not OK"),
              QObject::tr("Modification non effectuée.\nClick Cancel to exit."), QMessageBox::Cancel);
      }
}
