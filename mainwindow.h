#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QGroupBox>
#include <QMainWindow>
#include "rapport.h"
#include <QObject>
#include <QTableView>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int compterCasesCocheesDansOnglet();
   // void envoyerRapportParEmail(const QString &destinataire, const QString &objet, int RAPPORT_ID, QDate DATE_VISITE, QString RESULTAT_DE_RAPPORT, int EXPERTISE_ID, QFile *fichierPieceJointe)


private slots:
    void on_pushButton_clicked();

    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_valider1_clicked();

    void on_reset_clicked();

    //void on_comboBox_supprimer_activated(const QString &arg1);

    void on_comboBoxTri_currentIndexChanged(int index);

   // void on_pushButton_2_clicked();

    //void on_lineEdit_recherche_cursorPositionChanged(int arg1, int arg2);

    void on_lineEdit_recherche_textChanged(const QString &arg1);

    void on_pdf_clicked();

    void on_stati_clicked();

   // void on_pushButton_2_clicked();

    void on_pushButton_enregistrer_clicked();

private:
    Ui::MainWindow *ui;
Rapport R;
QSqlQueryModel *model;



};

#endif // MAINWINDOW_H
