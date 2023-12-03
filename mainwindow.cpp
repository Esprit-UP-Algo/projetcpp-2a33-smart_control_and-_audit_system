#include "mainwindow.h"
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "Connexion.h"
#include "finance.h"
#include <QtCharts/QChart>
#include <QPainter>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QDesktopServices>
#include <QPdfWriter>
#include <QWidget>
//
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rapport.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QImage>
#include <QFont>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDesktopServices>
#include <QUrl>
//pour le statique
#include <QtCharts/QBarSeries>
#include <QtCharts> // Inclure les en-têtes de la bibliothèque de graphiques de Qt
#include <QtSql> // Inclure les en-têtes de la bibliothèque de base de données de Qt
#include <QChartView> // Inclure l'en-tête pour la vue du graphique
#include <QPieSeries> // Inclure l'en-tête pour la série de graphique en camembert
#include <QPieSlice> // Inclure l'en-tête pour la tranche de graphique en camembert
//#include <QtXlsx>
#include <QApplication>
//#include <QRadioButton>
#include <QCheckBox>
#include <QtWidgets/QWidget>
#include <QSqlQuery>
#include <QtDebug>
#include <QVariant>
#include "qrcode.h"
#include "arduino.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using namespace qrcodegen;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   //ui->lineEditIdfacture->setValidator(new QIntValidator(0,99999999,this));
    ui->comboBoxfacture->setModel(Fimp.afficher());
    ui->comboBoxsupression->setModel(Fimp.afficher());
    ui->comboBoxRembourse->setModel(Fimp.afficher());
    ui->tableViewFinance->setModel(Fimp.afficher());

   model = new QSqlQueryModel();
   model->setQuery("SELECT * FROM Finance");
   ui->tableViewFinance->setModel(model);

   ui->comboBoxTri->addItem("Par defaut");
   ui->comboBoxTri->addItem("Croissant");
   ui->comboBoxTri->addItem("Decroissant");

   connect(ui->comboBoxTri, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_comboBoxTri_currentIndexChanged);
   int ret=A.connect_arduino(); // lancer la connexion à arduino
       switch(ret){
       case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
           break;
       case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
          break;
       case(-1):qDebug() << "arduino is not available";
       }
//RAPPORT
       ui->tab_rapport->setModel(R.afficher());
       ui->comboBox_supprimer->setModel(R.afficher());//*
       ui->comboBox_modifier_2->setModel(R.afficher());//*
       ui->comboBox_remplissage->setModel(R.afficher());//*
        //tri
       model = new QSqlQueryModel();
       model->setQuery("SELECT * FROM RAPPORT");
       ui->tab_rapport->setModel(model);
       ui->comboBoxTri_3->addItem("par defaut");
       ui->comboBoxTri_3->addItem("Decroissant");
       ui->comboBoxTri_3->addItem("Croissant");




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_ajouter_clicked()

{
    QString str_FACTURE_ID = ui->lineEditIdfacture->text();
      int facture_id = str_FACTURE_ID.toInt();

  QDate date_de_facture= ui->DateEditfacture->date();
  QDate date_de_paiment= ui->DateEditpaiment->date();
  QString  description= ui->lineEditdescription->text();
  float TVA = ui->lineEditTVA->text().toFloat();
  float montant = ui->lineEditmontant->text().toFloat();
  QString str_EXPERTISE_ID = ui->lineEditexpertise_id->text();
      int expertise_id =str_EXPERTISE_ID.toInt();

      ui->lblErreurId->setText("");
      ui->lblErreurEXpersiteID->setText("");

  bool isValid = true;

     if (str_FACTURE_ID.isEmpty()) {
         ui->lblErreurId->setText("<font color='black'>ID de FACTURE ne peut pas être vide !</font>");
         isValid = false;
     }

  QSqlQuery checkQuery;
    checkQuery.prepare("SELECT EXPERTISE_ID FROM FINANCE WHERE EXPERTISE_ID = :EXPERTISE_ID");
    checkQuery.bindValue(":EXPERTISE_ID", expertise_id);
    if (checkQuery.exec() && checkQuery.next()) {
        ui->lblErreurEXpersiteID->setText("<font color='black'>ID d'expertise existe déjà !</font>");
        isValid = false;
    }
    checkQuery.prepare("SELECT FACTURE_ID FROM FINANCE WHERE FACTURE_ID = :FACTURE_ID");
        checkQuery.bindValue(":FACTURE_ID", facture_id);
        if (checkQuery.exec() && checkQuery.next()) {
            ui->lblErreurId->setText("<font color='black'>FACTURE ID existe déjà !</font>");
            isValid = false;
        }


        if (isValid) {

  Finance F(facture_id, date_de_facture, date_de_paiment,TVA, description, montant, expertise_id);
  bool test= F.ajouter();
  if (test)
  {
      QMessageBox::information(nullptr, QObject::tr("OK"),
                  QObject::tr("Ajout effectue.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);

      ui->tableViewFinance->setModel(Fimp.afficher());

       ui->comboBoxfacture->addItem(QString::number(facture_id));

}
  else {
      QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                  QObject::tr("Ajout non effectue.\n"
                              "Click Cancel to exit."), QMessageBox::Cancel);

     }
}
}

void MainWindow::on_pb_supprimer_clicked()
{

     int facture_id = ui->comboBoxsupression->currentText().toInt();
        bool test = Fimp.supprimer(facture_id);

        if (test) {
             ui->tableViewFinance->setModel(Fimp.afficher());
             ui->comboBoxsupression->setModel(Fimp.afficher());

            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Deletion successful.\n"
                                                 "Click Cancel to exit."), QMessageBox::Cancel);
        } else {
            qDebug() << "Deletion unsuccessful.";
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Deletion unsuccessful.\n"
                                              "Click Cancel to exit."), QMessageBox::Cancel);
        }

}



void MainWindow::on_pb_modifier_clicked()
{
    int facture_id = ui->lineEditIdfacture->text().toInt();
    QDate date_de_facture= ui->DateEditfacture->date();
    QDate date_de_paiment= ui->DateEditpaiment->date();
    QString description = ui->lineEditdescription->text();
    float TVA = ui->lineEditTVA->text().toFloat();
    float montant = ui->lineEditmontant->text().toFloat();

    int expertise_id = ui->lineEditexpertise_id->text().toInt();

    if (description.isEmpty() || TVA <= 0 || montant <= 0) {
          QMessageBox::critical(nullptr, QObject::tr("Erreur de modification"),
              QObject::tr("Veuillez vérifier les champs de saisie."), QMessageBox::Ok);
          return; // Arrêtez la modification si la validation échoue
      }

      Finance F(facture_id, date_de_facture, date_de_paiment, TVA, description, montant, expertise_id);

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



void MainWindow::on_comboBoxTri_currentIndexChanged(int index)
{
    if (index == 0) // Reset
    {
       model->setQuery("SELECT * FROM Finance");
    }
    else if (index == 1) // Croissant
    {
        model->setQuery("SELECT * FROM Finance ORDER BY DATE_DE_FACTURE ASC");
    }
    else if (index == 2) // Décroissant
       {
           model->setQuery("SELECT * FROM Finance ORDER BY DATE_DE_FACTURE DESC");
       }
}

void MainWindow::on_on_pb_rembourser_clicked_clicked()
{

    // int facture_id = ui->lineEditRembourser->text().toInt();
             int facture_id = ui->comboBoxRembourse->currentText().toInt();

        if (facture_id <= 0) {
            QMessageBox::critical(nullptr, QObject::tr("Erreur de saisie"),
                QObject::tr("Veuillez saisir un ID de facture valide."), QMessageBox::Ok);
            return; // Arrêtez le remboursement si la validation échoue
        }

        bool remboursementReussi = Fimp.rembourser(facture_id);
        if (remboursementReussi) {
            ui->tableViewFinance->setModel(Fimp.afficher());
            QMessageBox::information(nullptr, QObject::tr("Remboursement réussi"),
                QObject::tr("Remboursement effectué avec succès."), QMessageBox::Ok);
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Remboursement échoué"),
                QObject::tr("Le remboursement a échoué. Veuillez vérifier les données saisies."), QMessageBox::Ok);
        }

}





void MainWindow::on_stati_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT Description, COUNT(*) AS PaymentCount FROM FINANCE GROUP BY Description");
        int chequeCount = 0;
        int virementCount = 0;
        int especeCount = 0;
        for (int i = 0; i < model->rowCount(); ++i) {
            QString paymentType = model->index(i, 0).data().toString().toLower();
            int paymentCount = model->index(i, 1).data().toInt();
            if (paymentType == "cheque") {
                chequeCount = paymentCount;
            } else if (paymentType == "virement") {
                virementCount = paymentCount;
            } else if (paymentType == "espece") {
                especeCount = paymentCount;
            }
        }

        int totalPayments = chequeCount + virementCount + especeCount;
        QPieSeries *series = new QPieSeries();
        if (chequeCount != 0) {
            QString chequePercentage = QString("Cheque: %1%").arg((chequeCount * 100.0) / totalPayments, 0, 'f', 2);
            QPieSlice *chequeSlice = series->append("Cheque", chequeCount);
            chequeSlice->setLabel(chequePercentage);
            chequeSlice->setLabelVisible();
            chequeSlice->setPen(QPen());
        }

        if (virementCount != 0) {
            QString virementPercentage = QString("Virement: %1%").arg((virementCount * 100.0) / totalPayments, 0, 'f', 2);
            QPieSlice *virementSlice = series->append("Virement", virementCount);
            virementSlice->setLabel(virementPercentage);
            virementSlice->setLabelVisible();
            virementSlice->setPen(QPen());
        }

        if (especeCount != 0) {
            QString especePercentage = QString("Espece: %1%").arg((especeCount * 100.0) / totalPayments, 0, 'f', 2);
            QPieSlice *especeSlice = series->append("Espece", especeCount);
            especeSlice->setLabel(especePercentage);
            especeSlice->setLabelVisible();
            especeSlice->setPen(QPen());
        }
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Pourcentage par Type de Paiement");
        chart->legend()->show();
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(1000, 500);
        chartView->show();

}

void MainWindow::on_lineEdit_recherche_textChanged(const QString &arg1)
{
    Finance F;

          if (arg1!="")
                  ui->tableViewFinance->setModel(F.rechercher(arg1));
              else {
                  ui->tableViewFinance->setModel(F.rechercher(arg1));

              }

}

void MainWindow::on_pushButton_clicked()
{
    {
        QPdfWriter pdf("C:/loujain/facture.pdf");
            QPainter painter(&pdf);

                                     int i = 4000;

                                     painter.setFont(QFont("Bodoni MT", 18));
                                     painter.drawRect(0,3000,6300,2600);
                                     painter.setPen(Qt::black);
                                     painter.drawText(200,1900,"LISTE DES FACTURES");

                                     painter.drawRect(0,3000,9600,500);
                                     painter.setPen(Qt::black);
                                     painter.setFont(QFont("Arial", 10));

                                     painter.drawText(100,3000,"FACTURE_ID");
                                     painter.drawText(1500,3000,"DATE_DE_FACTURE");
                                     painter.drawText(3500,3000,"DATE_DE_PAIMENT");
                                     painter.drawText(6000,3000,"TVA");
                                     painter.drawText(7000,3000,"DESCRIPTION");
                                     painter.drawText(8500,3000,"MONTANT");

                                     QString  value=ui->lineEdit_recherche->text();
                                     QSqlQuery query;
                                     query.prepare("select * from FINANCE where (FACTURE_ID LIKE '%"+value+"%')");
                                     query.exec();
                                     while (query.next())
                                     {
                                         painter.drawText(100,i,query.value(0).toString());
                                         painter.drawText(1500,i,query.value(1).toString());
                                         painter.drawText(3500,i,query.value(2).toString());
                                         painter.drawText(6000,i,query.value(3).toString());
                                         painter.drawText(7000,i,query.value(4).toString());
                                         painter.drawText(8500,i,query.value(5).toString());
                                         i = i +1000;
                                     }

                                     int reponse = QMessageBox::question(this, "PDF généré", "Afficher le PDF ?", QMessageBox::Yes |  QMessageBox::No);
                                     if (reponse == QMessageBox::Yes)
                                     {
                                         QDesktopServices::openUrl(QUrl::fromLocalFile("C:/FINANCE/facture.pdf"));
                                         painter.end();
                                     }
                                     if (reponse == QMessageBox::No)
                                     {
                                         painter.end();
                                     }
    }
}

void MainWindow::on_stati_2_clicked()
{
   /* // Construire la requête SQL pour obtenir le nombre de factures pour chaque mois
           QString queryStr = "SELECT EXTRACT(MONTH FROM DATE_DE_FACTURE) AS Mois, COUNT(DISTINCT FACTURE_ID) AS NombreDeFactures FROM FINANCE GROUP BY EXTRACT(MONTH FROM DATE_DE_FACTURE)";
           QSqlQueryModel *model = new QSqlQueryModel();
           model->setQuery(queryStr);

           // Afficher les données de la requête pour débogage
           for (int i = 0; i < model->rowCount(); ++i) {
               int mois = model->index(i, 0).data().toInt();
               int nombreDeFactures = model->index(i, 1).data().toInt();
               qDebug() << "Mois : " << mois << ", Nombre de factures : " << nombreDeFactures;
           }

           // Si le modèle ne contient pas de lignes, vérifions s'il y a des erreurs
           if (model->rowCount() == 0) {
               qDebug() << "Aucune donnée dans le modèle. Erreur : " << model->lastError().text();
           }

           // Créer le diagramme à barres
           QBarSeries *barSeries = new QBarSeries();

           // Remplir l'ensemble de barres avec le nombre de factures pour chaque mois
           for (int i = 0; i < model->rowCount(); ++i) {
               int mois = model->index(i, 0).data().toInt();
               int nombreDeFactures = model->index(i, 1).data().toInt();

               // Ajouter une barre au diagramme avec la couleur bleue
               QBarSet *barSet = new QBarSet(QString::number(mois));
               barSet->append(nombreDeFactures);

               // Définir la couleur de la barre à bleu
               barSet->setColor(Qt::blue);

               barSeries->append(barSet);
           }

           // Si le nombre de séries est de zéro, vérifions s'il y a des erreurs
           if (barSeries->barSets().size() == 0) {
               qDebug() << "Aucune série de barres dans le diagramme. Vérifiez les données ou les erreurs.";
           }

           // Créer le diagramme
           QChart *barChart = new QChart();

           // Ajouter la série au diagramme
           barChart->addSeries(barSeries);

           // Définir l'axe des abscisses pour le diagramme
           QBarCategoryAxis *axisX = new QBarCategoryAxis();

           // Specify the order of the months explicitly
           axisX->setCategories(QStringList() << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec");

           barChart->setAxisX(axisX, barSeries);

           // Définir l'axe des ordonnées pour le diagramme
           QValueAxis *axisY = new QValueAxis();
           barChart->setAxisY(axisY, barSeries);

           // Configurer le titre du diagramme
           barChart->setTitle("Nombre de Factures par Mois");
           barChart->legend()->hide();

           // Utilisé pour afficher le diagramme
           QChartView *barChartView = new QChartView(barChart);
           barChartView->setRenderHint(QPainter::Antialiasing);
           barChartView->resize(1000, 500);
           barChartView->show();*/
}

void MainWindow::on_CoutTotal_clicked()
{
    int facture_id = ui->comboBoxfacture->currentText().toInt();
        int expertise_id = ui->lineEditexper->text().toInt();

        Fimp.Montanttotal(facture_id, expertise_id);
          ui->tableViewFinance->setModel(Fimp.afficher());

        float Montanttotal = Fimp.Montanttotal(facture_id, expertise_id);

        if (Montanttotal != -1) {
            // Utilisez la valeur du Montanttotal comme nécessaire
            qDebug() << "Valeur envoyée à Arduino : " << Montanttotal;





            // Convertissez le Montanttotal en chaîne de caractères
            QString montantString = QString::number(Montanttotal);

            // Envoyez la chaîne de caractères à Arduino
         A.write_to_arduino(montantString.toUtf8().constData()); // la communication entre votre programme Qt et Arduino se fait souvent via des chaînes de caractères

            qDebug() << "Après l'appel à A.write_to_arduino";
        } else {
            // Gestion de l'échec de récupération du montant total
            qDebug() << "Erreur lors de la récupération du Montant Total.";

            // Ajoutez des messages de débogage pour afficher le modèle après la mise à jour
            qDebug() << "Affichage du modèle après la mise à jour :";
           // ui->tableViewFinance->setModel(Fimp.afficher());

            QMessageBox::information(nullptr, QObject::tr("Calcul du montant total"),
                                     QObject::tr("Calcul et mise à jour du montant total effectués avec succès."), QMessageBox::Ok);
        }
}




void MainWindow::on_pushButton_7_clicked()
{
    QString str_RAPPORT_ID = ui->lineEdit_id->text();
    int RAPPORT_ID = str_RAPPORT_ID.toInt();


    QString RESULTAT_DE_RAPPORT = ui->lineEdit_resultat_rapport->currentText();
     QDate DATE_VISITE = ui->dateEdit_date_rapport->date();

    QString str_EXPERTISE_ID = ui->lineEdit_EXPERTISE_ID->text();
    int EXPERTISE_ID =str_EXPERTISE_ID.toInt();


    ui->lblErreurId->setText("");
    ui->lblErreurEXpersiteID->setText("");
    ui->lblErreurResultat->setText("");

    bool isValid = true;
    if (str_RAPPORT_ID.isEmpty()) {
        ui->lblErreurId->setText("<font color='black'>ID de RAPPORT ne peut pas être vide !</font>");
        isValid = false;
    }


    if (RESULTAT_DE_RAPPORT.isEmpty()) {
         ui->lblErreurResultat->setText("<font color='black'>RESULTAT DE RAPPORT ne peut pas être vide !</font>");
         isValid = false;
     }

    /*if (str_EXPERTISE_ID.isEmpty()) {
        ui->lblErreurEXpersiteID->setText("<font color='black'>ID de EXPERTISE ne peut pas être vide ! !</font>");
        isValid = false;
    }*/
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT EXPERTISE_ID FROM RAPPORT WHERE EXPERTISE_ID = :EXPERTISE_ID");
    checkQuery.bindValue(":EXPERTISE_ID", EXPERTISE_ID);
    if (checkQuery.exec() && checkQuery.next()) {
        ui->lblErreurEXpersiteID->setText("<font color='black'>ID d'expertise existe déjà !</font>");
        isValid = false;
    }


    checkQuery.prepare("SELECT RAPPORT_ID FROM RAPPORT WHERE RAPPORT_ID = :RAPPORT_ID");
    checkQuery.bindValue(":RAPPORT_ID", RAPPORT_ID);
    if (checkQuery.exec() && checkQuery.next()) {
        ui->lblErreurId->setText("<font color='black'>RAPPORT ID existe déjà !</font>");
        isValid = false;
    }


    if (isValid) {
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

void MainWindow::on_pb_supprimer_3_clicked()
{
    int RAPPORT_ID=ui->comboBox_supprimer->currentText().toInt();//*

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

void MainWindow::on_lineEdit_recherche_3_textChanged(const QString &arg1)
{
    if (arg1!="")
        ui->tab_rapport->setModel(R.rechercher(arg1));
    else {
        ui->tab_rapport->setModel(R.rechercher(arg1));

    }
}

void MainWindow::on_comboBoxTri_3_currentIndexChanged(int index)
{
    if (index == 0)//sans tri(par defaut)
        {
        model->setQuery("SELECT * FROM RAPPORT");
        }
        else if (index == 1)//décroissant
        {
        model->setQuery("SELECT * FROM RAPPORT ORDER BY DATE_VISITE DESC");
        ui->tab_rapport->setModel(model);

        }
    else if (index == 2)//croissant
           {
        model->setQuery("SELECT * FROM RAPPORT ORDER BY DATE_VISITE ASC");
        ui->tab_rapport->setModel(model);//(refraichement)

           }
}

void MainWindow::on_pdf_clicked()
{
    QPdfWriter pdf("C:/loujain/liste_rapport.pdf");
        QPainter painter(&pdf);

        QImage image("C:/loujain/logo.png");
        if (image.isNull()) {
            QMessageBox::warning(this, "Error", "Failed to load image file!");
            return;
        }

        int imageX = 4500;
        int imageY = 500;
        painter.drawImage(imageX, imageY, image);

       int i = 4000;

        painter.setFont(QFont("Arial", 18));
        painter.drawRect(0,3000,6300,2600);
        painter.setPen(Qt::black);
        painter.drawText(250,1900,"LISTE DES Rapports d'IndusCheck ");

       painter.drawRect(0,3000,9600,500);
        painter.setPen(Qt::black);
         painter.setFont(QFont("Arial", 10));

        painter.drawText(100,3000,"RAPPORT_ID");
         painter.drawText(1500,3000,"RESULTAT_DE_RAPPORT");
         painter.drawText(4250,3000,"DATE_VISITE");
        painter.drawText(6000,3000,"EXPERTISE_ID");

   QString  value=ui->lineEdit_recherche->text();
   QSqlQuery query;
   query.prepare("select * from RAPPORT where (RAPPORT_ID LIKE '%"+value+"%')");
   query.exec();
                                 while (query.next())
                                 {
                                     painter.drawText(100,i,query.value(0).toString());
                                     painter.drawText(2000,i,query.value(1).toString());
                                     painter.drawText(4000,i,query.value(2).toString());
                                     painter.drawText(6500,i,query.value(3).toString());
                                     i = i +1000;
                                 }

    int reponse = QMessageBox::question(this, "PDF généré", "Afficher le PDF des Rapports realisès ?", QMessageBox::Yes |  QMessageBox::No);
     if (reponse == QMessageBox::Yes)
                                 {
                                     QDesktopServices::openUrl(QUrl::fromLocalFile("C:/loujain/liste_rapport.pdf"));
                                     painter.end();

                                 }
                                 if (reponse == QMessageBox::No)
                                 {
                                     painter.end();
                                 }
}

void MainWindow::on_stati_5_clicked()
{
    QSqlQueryModel * model= new QSqlQueryModel();

        model->setQuery("select * from RAPPORT where RESULTAT_DE_RAPPORT='confirme' ");
        int number1=model->rowCount();
        model->setQuery("select * from RAPPORT where RESULTAT_DE_RAPPORT ='non confirme' ");
        int number2=model->rowCount();
        model->setQuery("select * from RAPPORT where RESULTAT_DE_RAPPORT ='controle' ");
        int number3=model->rowCount();

        int total=number1+number2+number3;

        QString a = QString("confirme"+QString::number((number1*100)/total,'f',2)+"%" );
        QString b = QString("non confirme"+QString::number((number2*100)/total,'f',2)+"%" );
        QString c = QString("controle"+QString::number((number3*100)/total,'f',2)+"%" );

        QPieSeries *series = new QPieSeries();

        series->append(a,number1);
        series->append(b,number2);
        series->append(c,number3);

            if (number1 != 0)
        if (number1!= 0)
        {
            QPieSlice *slice = series->slices().at(0);
            slice->setLabelVisible();
            slice->setPen(QPen());
        }
        if (number2!=0)
        {

                 QPieSlice *slice = series->slices().at(1);
                 slice->setLabelVisible();
        }
        if (number3!=0)
        {

                 QPieSlice *slice = series->slices().at(2);
                 slice->setLabelVisible();
        }

       QChart *chart = new QChart();

                chart->addSeries(series);
                chart->setTitle("Pourcentage par Resultat de Rapports");
                chart->legend()->hide();

                QChartView *chartView = new QChartView(chart);
                chartView->setRenderHint(QPainter::Antialiasing);
                chartView->resize(1000,500);
                chartView->show();

}

void MainWindow::on_pushButton_arduino_clicked()
{
    QString id = ui->lineEdit_arduino->text();
  // QByteArray idData = id.toUtf8();
  //  A.write_to_arduino(idData);
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT RESULTAT_DE_RAPPORT FROM RAPPORT WHERE RAPPORT_ID = :id");
    checkQuery.bindValue(":id", id);

    if (checkQuery.exec() && checkQuery.next()) {
      QString resultat = checkQuery.value(0).toString();
       if (resultat == "confirme") {
            A.write_to_arduino("1");
        } else if (resultat == "non_confirme") {
            A.write_to_arduino("2");
        } else if (resultat == "controle") {
            A.write_to_arduino("3");
        } else {
            QMessageBox::warning(this, "Erreur", "Valeur de RESULTAT_DE_RAPPORT non reconnue.");
        }
    }
    else
    {
      //  QMessageBox::warning(this, "Erreur", "ID non trouvé dans la base de données.");
         A.write_to_arduino("4");
    }

}

void MainWindow::on_pushButton_6_clicked()
{
    ui->stackedWidget_4->setCurrentIndex(0);
}

void MainWindow::on_qrcodegen_2_clicked()
{
    {
           using namespace qrcodegen;


                       QTableView tab_rapport;
                       QSqlQueryModel * Mod=new  QSqlQueryModel();
                       QString value=ui->QRCODE->text();


                            QSqlQuery qry;




                            qry.prepare("select * from RAPPORT where RAPPORT_ID='"+value+"'");
                            qry.exec();
                            Mod->setQuery(qry);
                            tab_rapport.setModel(Mod);

/*QString test;
     int RAPPORT_ID;
     QString RESULTAT_DE_RAPPORT;
     QDate DATE_VISITE;
     int EXPERTISE_ID;*/

                           QString RAPPORT_ID =tab_rapport.model()->data(tab_rapport.model()->index(0, 0)).toString().simplified();
                           QString RESULTAT_DE_RAPPORT= tab_rapport.model()->data(tab_rapport.model()->index(0, 1)).toString().simplified();
                           QString DATE_VISITE= tab_rapport.model()->data(tab_rapport.model()->index(0, 2)).toString().simplified();
                           QString  EXPERTISE_ID= tab_rapport.model()->data(tab_rapport.model()->index(0, 3)).toString().simplified();


                       QString text = "Le Resultat de Rapport d'ID " +RAPPORT_ID +" est "+RESULTAT_DE_RAPPORT+" le "+ DATE_VISITE ;
                         // Create the QR Code object
                         QrCode qr = QrCode::encodeText( text.toUtf8().data(), QrCode::Ecc::MEDIUM );

                         qint32 sz = qr.getSize();
                         QImage im(sz,sz, QImage::Format_RGB32);
                           QRgb black = qRgb( 9,13,12 );
                           QRgb white = qRgb(255,255,255);
                         for (int y = 0; y < sz; y++)
                           for (int x = 0; x < sz; x++)
                             im.setPixel(x,y,qr.getModule(x, y) ? black : white );//setpixelmap tafichilek qr code
                         ui->qrcodecommande_2->setPixmap( QPixmap::fromImage(im.scaled(200,200,Qt::KeepAspectRatio,Qt::FastTransformation),Qt::MonoOnly) );
       }
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget_4->setCurrentIndex(1);
}

int MainWindow::compterCasesCocheesDansOnglet() {
    int nombreCasesCochees = 0;
  QList<QCheckBox *> listeCases = ui->tab_4->findChildren<QCheckBox *>();
    for (QCheckBox *checkBox : listeCases) {
        if (checkBox->isChecked()) {
            nombreCasesCochees++;
        }
    }

    return nombreCasesCochees;
}

void MainWindow::on_pushButton_enregistrer_clicked()
{
    int nombreCasesCochees = compterCasesCocheesDansOnglet();
    int RAPPORT_ID = ui->comboBox_remplissage->currentText().toInt();
    QString resultatRapport;
     QByteArray data;
    if (nombreCasesCochees == 10) {
        resultatRapport = "confirme";
        A.write_to_arduino("1");

    } else if (nombreCasesCochees >= 2 && nombreCasesCochees <= 9) {
        resultatRapport = "controle";
         A.write_to_arduino("3");

    } else {
        resultatRapport = "non_confirme";
         A.write_to_arduino("2");
    }

    QSqlQuery query;
    query.prepare("UPDATE RAPPORT SET RESULTAT_DE_RAPPORT = :RESULTAT_DE_RAPPORT WHERE RAPPORT_ID = :RAPPORT_ID");
    query.bindValue(":RESULTAT_DE_RAPPORT", resultatRapport);
    query.bindValue(":RAPPORT_ID", RAPPORT_ID);
    if (query.exec()) {
        if (resultatRapport == "confirme") {
            A.write_to_arduino("1");
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE EXPERTISE SET note_resultat = 'valide' WHERE EXPERTISE_ID = (SELECT EXPERTISE_ID FROM RAPPORT WHERE RAPPORT_ID = :RAPPORT_ID)");
            updateQuery.bindValue(":RAPPORT_ID", RAPPORT_ID);
            if (updateQuery.exec()) {
                ui->tab_rapport->setModel(R.afficher());
                ui->comboBox_remplissage->setModel(R.afficher());
                QMessageBox::information(nullptr, QObject::tr("Résultat de Rapport "),
                    QObject::tr("Résultat de Rapport enregistré.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            } else {

                QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                   QObject::tr("Erreur lors de la mise à jour du résultat de rapport dans EXPERTISE.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            }
        }
        else {

            ui->tab_rapport->setModel(R.afficher());
            ui->comboBox_remplissage->setModel(R.afficher());
        }
    } else {

        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
            QObject::tr("Erreur lors de l'enregistrement du résultat de rapport.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pb_modifier_4_clicked()
{
    int RAPPORT_ID=ui->comboBox_modifier_2->currentText().toInt();//*
    QString RESULTAT_DE_RAPPORT=ui->lineEdit_resultat_rapport_3->currentText();
    QDate DATE_VISITE = QDate::fromString(ui->dateEdit_date_rapport_3->text(), "dd/MM/yyyy");
    int EXPERTISE_ID=ui->lineEdit_EXPERTISE_ID_3->text().toInt();
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

void MainWindow::on_valider1_2_clicked()
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

}
}

void MainWindow::on_reset_2_clicked()
{
    ui->lineEdit_resultat_rapport_2->clear();
    ui->lineEdit_EXPERTISE_ID_2->clear();

}

void MainWindow::on_pushButton_rapport_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_finance_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::on_pushButton_4_clicked()
{
       ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_11_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_12_clicked()
{
      ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_pushButton_13_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_pushButton_17_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_pushButton_18_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_pushButton_19_clicked()
{
     ui->stackedWidget_2->setCurrentIndex(0);
}
