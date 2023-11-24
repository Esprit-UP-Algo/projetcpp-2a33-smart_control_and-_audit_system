#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clients.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QPainter>
#include <QPdfWriter>
#include <QSqlQuery>
#include <QDebug>
#include <QString>
#include <QTableView>
#include <QSqlDatabase>
#include <QObject>
#include <QSqlError>
#include <QValidator>
#include <QApplication>
#include <QFileDialog>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStandardItemModel>
#include <QStandardItem>






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    clients c;
    ui->setupUi(this);
    //ui->c ->setValidator(new QIntValidator(1000000,99999999,this)); // ONLY INPUT INT
    /*ui->lineEdit_matmodif->setValidator(new QIntValidator(0,999999,this));
    ui->lineEdit_type->setValidator(new QRegularExpressionValidator(condition,this));
    ui->lineEdit_typemodif->setValidator(new QRegularExpressionValidator(condition,this));
    ui->lineEdit_nombre->setValidator(new QIntValidator(0,4,this));
    ui->lineEdit_nombremodif->setValidator(new QIntValidator(0,4,this));*/

    //ui->setupUi(this);
    ui->tableView->setModel(c.afficher());
    ui->table_historique->setModel(c.afficher_historique());
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::on_ajouter_clicked()
{


    int telephone_c = ui->t->text().toInt();
    int cin = ui->c->text().toInt();
    QString nom_c = ui->n->text();
    QString prenom_c = ui->p->text();
    QString adresse_c = ui->a->text();
    QString client_id = ui->i->text();

    clients c ( nom_c, prenom_c, adresse_c, telephone_c , cin, client_id);

    if(c.ajouter_clients())
    {

        ui->tableView->setModel(c.afficher());
        ui->table_historique->setModel(c.afficher_historique());
        QMessageBox::information(nullptr, QObject::tr("Ajout Client"),
                    QObject::tr("Client ajouté.\n"
                                "Continuer."), QMessageBox::Cancel);


    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Ajout Client"),
                    QObject::tr("Client non ajouté.\n"
                                "Cliquer quitter."), QMessageBox::Cancel);
    }
}


void MainWindow::on_afficher_clicked()
{
    clients c;
    ui->tableView->setModel(c.afficher());
}

void MainWindow::on_modifier_clicked()
{
    int telephone_c = ui->t_2->text().toInt();
    int cin = ui->c_2->text().toInt();
    QString nom_c = ui->n_2->text();
    QString prenom_c = ui->p_2->text();
    QString adresse_c = ui->a_2->text();
    QString client_id = ui->i_2->text();


     clients c ( nom_c, prenom_c, adresse_c, telephone_c , cin, client_id);

     c.modifier();
     if(c.modifier())
     {
         QMessageBox::information(nullptr, QObject::tr("Modif client"),
                     QObject::tr("client modifié.\n"
                                 "Continuer."), QMessageBox::Cancel);
         ui->tableView->setModel(c.afficher());
     }
     else {
         QMessageBox::critical(nullptr, QObject::tr("Modif client"),
                     QObject::tr("client non modifié.\n"
                                 "Cliquer quitter."), QMessageBox::Cancel);
     }
    ui->t_2->setText("");
    ui->c_2->setText("");
    ui->n_2->setText("");
    ui->p_2->setText("");
    ui->a_2->setText("");
    ui->i_2->setText("");
}

void MainWindow::on_supp_clicked()
{
    clients c;
    c.set_client_id(ui->si->text());
    if(c.supprimer(c.get_client_id()))
    {
        QMessageBox::information(nullptr, QObject::tr("Supprime"),
                    QObject::tr("Supprimé.\n"
                                "Cliquer quitter."), QMessageBox::Cancel);
        ui->si->setText("");
        ui->tableView->setModel(c.afficher());
        ui->table_historique->setModel(c.afficher_historique());
    }
    else {
        QMessageBox::critical(nullptr, QObject::tr("Supprime"),
                    QObject::tr("Client pas supprimé.\n"
                                "Cliquer quitter."), QMessageBox::Cancel);
    }

}



void MainWindow::on_PDF_clicked()
{
    QString strStream;
                    QTextStream out(&strStream);

                    const int rowCount = ui->tableView->model()->rowCount();
                    const int columnCount = ui->tableView->model()->columnCount();

                    out <<  "<html>\n"
                        "<head>\n"
                        "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                        <<  QString("<title>%1</title>\n").arg("strTitle")
                        <<  "</head>\n"
                        "<body bgcolor=#ffffff link=#5000A0>\n"

                       //     "<align='right'> " << datefich << "</align>"
                        "<center> <H1>Liste des clients </H1></br></br><table border=1 cellspacing=0 cellpadding=2>\n";

                    // headers
                    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
                    for (int column = 0; column < columnCount; column++)
                        if (!ui->tableView->isColumnHidden(column))
                            out << QString("<th>%1</th>").arg(ui->tableView->model()->headerData(column, Qt::Horizontal).toString());
                    out << "</tr></thead>\n";

                    // data table
                    for (int row = 0; row < rowCount; row++) {
                        out << "<tr> <td bkcolor=0>" << row+1 <<"</td>";
                        for (int column = 0; column < columnCount; column++) {
                            if (!ui->tableView->isColumnHidden(column)) {
                                QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, column)).toString().simplified();
                                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                            }
                        }
                        out << "</tr>\n";
                    }
                    out <<  "</table> </center>\n"
                        "</body>\n"
                        "</html>\n";

              QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Sauvegarder en PDF", QString(), "*.pdf");
                if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

               QPrinter printer (QPrinter::PrinterResolution);
                printer.setOutputFormat(QPrinter::PdfFormat);
               printer.setPaperSize(QPrinter::A4);
              printer.setOutputFileName(fileName);

               QTextDocument doc;
                doc.setHtml(strStream);
                doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
                doc.print(&printer);
}







void MainWindow::on_recherche_b_clicked()
{
    clients cli;
    QString column= ui->comboBox_rech->currentText();
    QString input = ui->recherche_l->text();

    ui->tableView->setModel(cli.afficherRecherche(column,input));
}

void MainWindow::on_tri_b_clicked()
{
    clients cli;
        if (ui->comboBox_tri->currentText()=="CIN")
        {
            ui->tableView->setModel(cli.tri_CIN());
        }
        else if (ui->comboBox_tri->currentText()=="NOM")
        {
            ui->tableView->setModel(cli.tri_nom());
        }
        else
        {
            ui->tableView->setModel(cli.tri_prenom());
        }
}

void MainWindow::on_stat_b_clicked()
{
    clients cl;
    // Assuming you have a QTableView named tableView for displaying statistics
       QMap<QString, int> statistics = cl.getAdresseStatistics();

       QStandardItemModel *model = new QStandardItemModel(this);
       model->setColumnCount(1);
       model->setHorizontalHeaderLabels(QStringList() << "Stat par adresse");

       int row = 0;
       for (const QString &etat : statistics.keys()) {
           QStandardItem *item = new QStandardItem(QString("%1: %2").arg(etat, QString::number(statistics.value(etat))));
           model->setItem(row, 0, item);
           row++;
       }

       ui->tableView_stat->setModel(model);
       ui->tableView_stat->horizontalHeader()->setStyleSheet("color: gold");

       // Hide row numbers
       ui->tableView_stat->verticalHeader()->setVisible(false);
   }


void MainWindow::on_avis_b_clicked()
{

}
