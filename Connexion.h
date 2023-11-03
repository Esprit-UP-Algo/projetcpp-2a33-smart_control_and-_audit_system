#ifndef CONNEXION_H
#define CONNEXION_H
#include <QtSql>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

class Connection
{   QSqlDatabase db;
public:
    Connection();
    bool createconnect();
    void closeconnect();
};

#endif // CONNEXION_H
