#include "qtsqlitedb.h"

QtSqliteDb::QtSqliteDb(QString dbName, QString dirPath, QString ext)
{
    this->Connect(dbName, dirPath, ext);
}
QtSqliteDb::~QtSqliteDb()
{
    this->Disconnect();
}
void QtSqliteDb::Connect(QString dbName, QString dirPath, QString ext)
{
    if (!QSqlDatabase::contains(dbName)) {
        this->Disconnect();
        this->dbName = dbName;
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", this->dbName);
        QString dbPath = QDir(dirPath).filePath(this->dbName + "." + ext);
        db.setDatabaseName(dbPath);
        qDebug () << "dbPath:" << dbPath;
    }
}
void QtSqliteDb::Disconnect()
{
    QSqlDatabase::removeDatabase(this->dbName);
}
QSqlQuery QtSqliteDb::Query(QString query)
{
    QSqlDatabase db = QSqlDatabase::database(this->dbName);
    db.transaction();
    try {
        QSqlQuery q(db);
        q.exec(query);
        QSqlError err = q.lastError();
        if (!err.text().trimmed().isEmpty()) { qDebug() << "SQLieteError: " << err.text(); }
        db.commit();
        qDebug() << "QSqlDatabase::commit(): " << query;
        return q;
    } catch (...) {
        qDebug() << "QSqlDatabase::rollback(): " << query;
        db.rollback();
    }
}
void QtSqliteDb::Query(QStringList queries)
{
    QSqlDatabase db = QSqlDatabase::database(this->dbName);
    db.transaction();
    try {
        QSqlQuery q(db);
        foreach (QString query, queries) {
            q.exec(query);
            QSqlError err = q.lastError();
            if (!err.text().trimmed().isEmpty()) { qDebug() << "SQLieteError: " << err.text() << "\n" << query << ": "; }
        }
        qDebug() << "QSqlDatabase::commit()" << queries;
        db.commit();
    } catch (...) {
        qDebug() << "QSqlDatabase::rollback()" << queries;
        db.rollback();
    }
}
