#include "connection.h"

Connection::Connection()
{

}

bool Connection::createconnect()
{
    db = QSqlDatabase::addDatabase("QODBC");
    bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("Source_Projet2A");
db.setUserName("Lamiss");//inserer nom de l'utilisateur
db.setPassword("lamis123");//inserer mot de passe de cet utilisateur

if (db.open()) test=true;
test=true;





    return  test;
}
//void Connection::closeconnect(){db.close();}
