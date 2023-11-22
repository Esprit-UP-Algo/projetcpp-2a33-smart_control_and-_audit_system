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
#include "QrCode.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
//#include <QMailMessage>

using qrcodegen::QrCode;
using qrcodegen::QrSegment;


using namespace qrcodegen;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_rapport->setModel(R.afficher());
    ui->comboBox_supprimer->setModel(R.afficher());//*
    ui->comboBox_modifier->setModel(R.afficher());//*
    ui->comboBox_remplissage->setModel(R.afficher());//*
    //QString value=ui->label_28->text();


     //tri
    model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM RAPPORT");
    ui->tab_rapport->setModel(model);
    ui->comboBoxTri->addItem("par defaut");
    ui->comboBoxTri->addItem("Decroissant");
    ui->comboBoxTri->addItem("Croissant");
    connect(ui->comboBoxTri, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_comboBoxTri_currentIndexChanged);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
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

  //  QSqlQuery checkQuery;
    checkQuery.prepare("SELECT RAPPORT_ID FROM RAPPORT WHERE RAPPORT_ID = :RAPPORT_ID");
    checkQuery.bindValue(":RAPPORT_ID", RAPPORT_ID);
    if (checkQuery.exec() && checkQuery.next()) {
        ui->lblErreurId->setText("<font color='black'>RAPPORT ID existe déjà !</font>");
        isValid = false;
    }


    if (isValid) {
        Rapport R(RAPPORT_ID, RESULTAT_DE_RAPPORT, DATE_VISITE, EXPERTISE_ID);
      //  Rapport R(RAPPORT_ID, DATE_VISITE, EXPERTISE_ID);
        bool test = R.ajouter();
        if (test) {
            QMessageBox::information(nullptr, QObject::tr("OK"),
                QObject::tr("Ajout effectué.\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            ui->tab_rapport->setModel(R.afficher());
            ui->comboBox_supprimer->setModel(R.afficher());
            ui->comboBox_modifier->setModel(R.afficher());
          //  R.envoyerNotification();
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                QObject::tr("Ajout non effectué.\n"
                            "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }
    }
}


void MainWindow::on_pb_supprimer_clicked()
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

void MainWindow::on_lineEdit_recherche_textChanged(const QString &arg1)
{
    if (arg1!="")
        ui->tab_rapport->setModel(R.rechercher(arg1));
    else {
        ui->tab_rapport->setModel(R.rechercher(arg1));

    }

}


void MainWindow::on_pb_modifier_clicked()
{

int RAPPORT_ID=ui->comboBox_modifier->currentText().toInt();//*
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



void MainWindow::on_comboBoxTri_currentIndexChanged(int index)
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

                                // painter.setPen(Qt::red);
                                // painter.drawRect(0, 3000, 6300, 6200);


                               /*  // Draw horizontal lines
                                    painter.drawLine(0, 2800, 6300, 3100);
                                    painter.drawLine(0, 4200, 6300, 4200);
                                    painter.drawLine(0, 5600, 6300, 5600);*/

                                  /*  // Draw vertical lines
                                    painter.drawLine(1000, 3000, 1000, 3500);
                                    painter.drawLine(2500, 3000, 2500, 3500);
                                    painter.drawLine(4500, 3000, 4500, 3500);
                                    painter.drawLine(6000, 3000, 6000, 3500);*/

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


void MainWindow::on_stati_clicked()
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



void MainWindow::on_pushButton_enregistrer_clicked() {
    int nombreCasesCochees = compterCasesCocheesDansOnglet();
    QString RAPPORT_ID = ui->comboBox_remplissage->currentText();
    QString resultatRapport;

    if (nombreCasesCochees == 10) {
        resultatRapport = "confirme";
    } else if (nombreCasesCochees >= 2 && nombreCasesCochees <= 9) {
        resultatRapport = "controle";
    } else {
        resultatRapport = "non_confirme";
    }


    QSqlQuery query;
    query.prepare("UPDATE RAPPORT SET RESULTAT_DE_RAPPORT = :RESULTAT_DE_RAPPORT WHERE RAPPORT_ID = :RAPPORT_ID");
    query.bindValue(":RESULTAT_DE_RAPPORT", resultatRapport);
    query.bindValue(":RAPPORT_ID", RAPPORT_ID);

    if (query.exec()) {
        if (resultatRapport == "confirme") {
              // envoyerRapportParEmail;()
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
        } else {

            ui->tab_rapport->setModel(R.afficher());
            ui->comboBox_remplissage->setModel(R.afficher());
        }
    } else {

        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
            QObject::tr("Erreur lors de l'enregistrement du résultat de rapport.\nCliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}




void MainWindow::on_pushButton_2_clicked()
{

 ui->stackedWidget_3->setCurrentIndex(0);
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

void MainWindow::on_pushButton_3_clicked()
{
  ui->stackedWidget_3->setCurrentIndex(1);
}
