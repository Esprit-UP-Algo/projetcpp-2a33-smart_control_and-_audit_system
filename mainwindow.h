#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clients.h"
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
    void on_ajouter_clicked();

    void on_afficher_clicked();

    void on_modifier_clicked();

    void on_supp_clicked();

    void on_recherch_id_clicked();

    void on_recherche_nom_clicked();

    void on_recherche_prenom_clicked();

    void on_trie_id_clicked();

    void on_trie_nom_clicked();

    void on_trie_prenom_clicked();

    void on_PDF_clicked();

    void on_recherche_b_clicked();

    void on_tri_b_clicked();

    void on_stat_b_clicked();

    void on_avis_b_clicked();

private:
    Ui::MainWindow *ui;
    clients c;
};

#endif // MAINWINDOW_H
