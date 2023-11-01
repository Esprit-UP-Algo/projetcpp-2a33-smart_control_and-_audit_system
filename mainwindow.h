#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rapport.h"
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
    void on_pushButton_clicked();

    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_valider1_clicked();

    void on_reset_clicked();

private:
    Ui::MainWindow *ui;
Rapport R;
};

#endif // MAINWINDOW_H
