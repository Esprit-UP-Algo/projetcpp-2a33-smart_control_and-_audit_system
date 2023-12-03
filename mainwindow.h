#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QGraphicsView>
#include <QtCharts/QChart>

#include <QtCharts/QBarSeries>

#include <QtCharts/QBarSet>

#include <QtCharts/QBarCategoryAxis>

#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>


#include "finance.h"
#include "arduino.h"
#include <QMainWindow>
#include <QGroupBox>

#include "rapport.h"
#include <QObject>
#include <QTableView>
#include <QStringList>
#include "arduino.h"
#include <QMainWindow>
QT_CHARTS_USE_NAMESPACE

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //void on_pushButton_clicked();

    void on_pb_ajouter_clicked();

    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_comboBoxTri_currentIndexChanged(int index);

    void on_on_pb_rembourser_clicked_clicked();

    void on_stati_clicked();

    void on_lineEdit_recherche_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_stati_2_clicked();

    void on_CoutTotal_clicked();

    void on_pushButton_7_clicked();

    void on_pb_supprimer_3_clicked();

    void on_lineEdit_recherche_3_textChanged(const QString &arg1);

    void on_comboBoxTri_3_currentIndexChanged(int index);

    void on_pdf_clicked();

    void on_stati_5_clicked();

    void on_pushButton_arduino_clicked();

    void on_pushButton_6_clicked();

    void on_qrcodegen_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_enregistrer_clicked();

    int compterCasesCocheesDansOnglet();

    void on_pb_modifier_4_clicked();

    void on_valider1_2_clicked();

    void on_reset_2_clicked();

    void on_pushButton_rapport_clicked();

    void on_pushButton_finance_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

private:
    Ui::MainWindow *ui;
    Finance Fimp;
    QSqlQueryModel *model;
    QByteArray data; // variable contenant les données reçues
    Arduino A; // objet temporaire
    int value;
    Rapport R;



};
#endif // MAINWINDOW_H
