#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "employe.h"
#include <QMainWindow>
#include"arduino.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pb_ajouter_clicked();

    void on_pb_supp_clicked();
    void on_pb_ajouter_2_clicked();
    void on_le_recherche_clicked();
    void on_le_annuler_clicked();
    //void on_le_ann_mod_clicked();
    void on_pb_tri_clicked();
    void on_l_pdf_clicked();
    void on_pb_confirmer_clicked();
    void on_pb_annuler_clicked();



private:
    Ui::MainWindow *ui;
    employe S;
    Arduino A;
};

#endif // MAINWINDOW_H
