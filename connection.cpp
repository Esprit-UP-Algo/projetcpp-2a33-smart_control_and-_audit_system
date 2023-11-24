#include "connection.h"
#include <QSqlDatabase>
Connection::Connection()
{

}

bool Connection::createconnect()
{

    bool test=false;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_projet_2A");
    db.setUserName("system");//inserer nom de l'utilisateur
    db.setPassword("chouchou");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;





    return  test;
}
void Connection ::closeconnection(){db.close();}
