#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


/*void Dialog::setfinance(Finance f)
{
   ui->lineEdit_cinD->setText(f.get_cin());
   ui->lineEdit_nomD->setText(f.get_nom());
   ui->lineEdit_prenomD->setText(f.get_prenom());
   ui->lineEdit_datedD->setText(f.get_date_debut());
   ui->lineEdit_datefD->setText(f.get_date_fin());


}*/
