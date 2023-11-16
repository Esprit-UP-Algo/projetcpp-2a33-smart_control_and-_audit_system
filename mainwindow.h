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

private:
    Ui::MainWindow *ui;
    Finance Fimp;
    QSqlQueryModel *model;




};
#endif // MAINWINDOW_H
