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







MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   //ui->lineEditIdfacture->setValidator(new QIntValidator(0,99999999,this));
    ui->comboBoxfacture->setModel(Fimp.afficher());
    ui->comboBoxsupression->setModel(Fimp.afficher());

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

     int facture_id = ui->lineEditRembourser->text().toInt();

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
        QPdfWriter pdf("C:/FINANCE/facture.pdf");
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
    // Construire la requête SQL pour obtenir le nombre de factures pour chaque mois
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
           barChartView->show();
}

void MainWindow::on_CoutTotal_clicked()
{
    int facture_id = ui->comboBoxfacture->currentText().toInt();
     int expertise_id = ui->lineEditexper->text().toInt();


       Fimp.Montanttotal(facture_id, expertise_id);

           // Ajoutez des messages de débogage pour afficher le modèle après la mise à jour
           qDebug() << "Affichage du modèle après la mise à jour :";
           ui->tableViewFinance->setModel(Fimp.afficher());

           QMessageBox::information(nullptr, QObject::tr("Calcul du montant total"),
               QObject::tr("Calcul et mise à jour du montant total effectués avec succès."), QMessageBox::Ok);
       }



