#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QRegExp>
#include <QPainter>
#include <QtCharts>
#include<QChartView>
#include<QBarSet>
#include<QBarSeries>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   ui->stackedWidget->setCurrentIndex(0);
    ui->tab_employe->setModel(S.afficher());
    QBarSet *set0 = new QBarSet("Employe disponible");

        QBarSet *set1 = new QBarSet("Employe non disponible ");

        int countEtat0 = S.compteretat0();
            int countEtat1 = S.compteretat1();
            * set0 <<countEtat1<<10 ;

            * set1 << countEtat0<<10  ;

        QBarSeries *series = new QBarSeries();

        series->append(set0);

        series->append(set1);

        QChart *chart = new QChart();

        chart->addSeries(series);

        chart->setTitle("Employe");

        chart->setAnimationOptions(QChart:: SeriesAnimations);

        chart->resize(450,300);
         QStringList categories;

        categories << " Etat" ;

        QBarCategoryAxis *axis = new QBarCategoryAxis();

        axis->append(categories);

        chart->createDefaultAxes();

        chart->setAxisX(axis,series);



        QChartView *chartView = new QChartView(chart);

        chartView->setParent(ui->label_15);
        //arduino
        int ret=A.connect_arduino(); // lancer la connexion à arduino
                switch(ret){
                case(0):qDebug()<< "arduino is available and connected to : "<< A.getarduino_port_name();
                    break;
                case(1):qDebug() << "arduino is available but not connected to :" <<A.getarduino_port_name();
                   break;
                case(-1):qDebug() << "arduino is not available";
                }
                 QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
                 //le slot update_label suite à la reception du signal readyRead (reception des données).


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_ajouter_clicked()
{
    int id_emp=ui->l_id->text().toInt();
     QString nom_emp=ui->l_nom->text();
     QString prenom_emp=ui->l_prenom->text();
     QString email=ui->l_email->text();
     QString titre_de_poste=ui->l_titre->text();
     int numero_de_telephone=ui->l_num->text().toInt();
     int etat=ui->l_etat->text().toInt();
     int passe=ui->l_passe2->text().toInt();
     employe S(id_emp,nom_emp,prenom_emp,email,titre_de_poste,numero_de_telephone,etat, passe);
     bool test=S.ajouter();
    QMessageBox msgBox;
     if(test )
     {
         msgBox.setText("Ajout avec succes.");
         ui->tab_employe->setModel(S.afficher());
     }
     else
         msgBox.setText("Echec d'ajout.");
         msgBox.exec();

}
void MainWindow::on_pb_supp_clicked()
{
    employe S1;
    S1.setId(ui->l_id_supp->text().toInt());
    bool test=S1.supprimer(S1.getID());
   QMessageBox msgBox;
    if(test)
    {
        msgBox.setText("Suppression avec succes.");
        ui->tab_employe->setModel(S.afficher());
    }
    else
        msgBox.setText("Echec de supprimer.");
        msgBox.exec();
}
void MainWindow::on_pb_ajouter_2_clicked()
{
            int id_emp=ui->l_id->text().toInt();
            ui->l_id->setReadOnly(true);
            QString nom_emp=ui->l_nom->text();
            QString prenom_emp=ui->l_prenom->text();
            QString email=ui->l_email->text();
            QString titre_de_poste=ui->l_titre->text();
            int numero_de_telephone=ui->l_num->text().toInt();
            int etat=ui->l_etat->text().toInt();
            int passe=ui->l_passe2->text().toInt();

            employe S(id_emp,nom_emp,prenom_emp,email,titre_de_poste,numero_de_telephone,etat,passe);
            bool test=S.modifier();
            QMessageBox msgBox;
            if(test )
               {
                   msgBox.setText("Modification avec succes.");
                   ui->tab_employe->setModel(S.afficher());
               }
               else
                   msgBox.setText("Echec de Modification.");
                   msgBox.exec();
}
void MainWindow::on_le_recherche_clicked()
{
        int id_emp = ui->le_id_rech->text().toInt();
       QString nom_emp = ui->le_nom_rech->text();
         QString prenom_emp = ui->le_prenom_rech->text();


employe employe;
QSqlQuery query = employe.rechercher_id( id_emp,nom_emp,prenom_emp );

        if (query.exec() && query.next()) {


            ui->tab_employe_rech->setColumnCount(query.record().count());
            for (int i = 0; i < query.record().count(); i++) {
                QTableWidgetItem *headerItem = new QTableWidgetItem(query.record().fieldName(i));
                ui->tab_employe_rech->setHorizontalHeaderItem(i, headerItem);

            }

            // Remplir les données trouvées dans le QTableWidget
            ui->tab_employe_rech->setRowCount(0);
            do {
                int row = ui->tab_employe_rech->rowCount();
                ui->tab_employe_rech->insertRow(row);
                for (int col = 0; col < query.record().count(); col++) {
                    QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());

                    ui->tab_employe_rech->setItem(row, col, item);
                }
            } while (query.next());
        }

        employe.affecterEmployeSelonDisponibilite(id_emp);

}
void MainWindow::on_le_annuler_clicked()
{
    // Supprimer le contenu du QTableWidget
    ui->tab_employe_rech->clearContents();
    ui->tab_employe_rech->setRowCount(0);
    ui->tab_employe_rech->clear();
}

void MainWindow::on_pb_tri_clicked()
{
    employe employe;
        QSqlQueryModel *model = employe.tri_titre_de_poste();

        ui->tab_employe->setModel(model);
}
void MainWindow::on_l_pdf_clicked()
{

    QPdfWriter pdf("C:/projet_pdf/pdf");
    int i = 4000;
    QPainter painter(&pdf);
    QString strStream;

    QTextStream out(&strStream);
    painter.setPen(Qt::red);
    painter.setFont(QFont("Time New Roman", 25));
    painter.drawText(3000,1400,"Liste Des Employés");
    painter.setPen(Qt::black);
    painter.setFont(QFont("Time New Roman", 7));
    painter.setPen(Qt::darkCyan);
    painter.drawRect(100,100,9400,2500);
    painter.drawRect(100,3000,9400,500);
    painter.setPen(Qt::black);
    painter.drawText(300,3300," id_emp");
    painter.drawText(1300,3300,"nom");
    painter.drawText(2300,3300,"prenom");
    painter.drawText(3300,3300,"email");
    painter.drawText(4300,3300,"titre_de_poste");
    painter.drawText(5300,3300,"numero_de_telephone");
    painter.drawText(6300,3300,"etat");
    painter.drawText(7300,3300,"passe");
    painter.setPen(Qt::darkCyan);
    painter.drawRect(100,3000,9400,9000);
    painter.setPen(Qt::black);
    QSqlQuery query;
    query.prepare("select * from EMPLOYE");
    query.exec();
    while (query.next())
    {
        painter.drawText(300,i,query.value(0).toString());
        painter.drawText(1300,i,query.value(1).toString());
        painter.drawText(2300,i,query.value(2).toString());
        painter.drawText(3300,i,query.value(3).toString());
        painter.drawText(4300,i,query.value(4).toString());
        painter.drawText(5300,i,query.value(5).toString());
        painter.drawText(6300,i,query.value(6).toString());
        painter.drawText(7300,i,query.value(6).toString());
        i +=350;
    }
        QMessageBox::information(this, QObject::tr("PDF Enregistré!"),
        QObject::tr("PDF Enregistré!.\n" "Click Cancel to exit."), QMessageBox::Cancel);

}
void MainWindow::on_pb_confirmer_clicked()
{

    QString UserName = ui -> l_titreLogin->text();
    QString Password= ui->l_pass -> text();

    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE TITRE_DE_POSTE = :titre_de_poste AND PASSE = :passe");
    query.bindValue(":titre_de_poste", UserName);
    query.bindValue(":passe", Password);

    if (query.exec() && query.next()) {
        QMessageBox::information(this,"CONNECTED","Connecté");
        ui->stackedWidget->setCurrentIndex(1);

    }
    else {
        QMessageBox::warning(this,"INVALID","veuillez entrer un nom d'utilisateur ou un mot de passe valide");
    }


}



void MainWindow::on_pb_annuler_clicked()
{
    QMessageBox::StandardButton reply;

    reply= QMessageBox::question(this,"close?","are you sure to close the app?");
    if(reply==QMessageBox::Yes)
    {
QApplication::quit();
    }
}





