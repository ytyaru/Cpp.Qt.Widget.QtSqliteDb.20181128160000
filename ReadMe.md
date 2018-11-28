# このソフトウェアについて

　Qt5学習。QtでSQLite3を使うラッパクラスを作ってみた2。

* SQL実行時エラーをqDebugで表示するようにした
* 複数行のSQLを実行するメソッドを作った: `void Query(QStringList)`

## 前回まで

* https://github.com/ytyaru/pp.Qt.Widget.QtSqliteDb.20181128120000
* https://github.com/ytyaru/Cpp.Qt.Widget.QSql.SQLite3.Transaction.20181128070000
* https://github.com/ytyaru/Cpp.Qt.Widget.QSql.SQLite3.Class.20181127180000
* https://github.com/ytyaru/Cpp.Qt.Widget.QSql.SQLite3.Class.20181127170000
* https://github.com/ytyaru/Cpp.Qt.Widget.QSql.SQLite3.Class.20181127160000
* https://github.com/ytyaru/Cpp.Qt.Widget.QSql.SQLite3.Class.20181127130000

## コード抜粋

### SQL実行時エラーをqDebugで表示するようにした

qtsqlitedb.cpp
```cpp
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
```

　`QSqlError`というやつ。

### 複数行のSQLを実行するメソッドを作った

　引数が`QString`でなく`QStringList`のメソッド。戻り値がない。複数のInsert文を実行することを想定。

```cpp
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
```

## Qt要素

* http://doc.qt.io/qt-5/qsql.html
    * http://doc.qt.io/qt-5/qsqldatabase.html
    * http://doc.qt.io/qt-5/qsqlquery.html
    * http://doc.qt.io/qt-5/qsqlerror.html

# 開発環境

* [Raspberry Pi](https://ja.wikipedia.org/wiki/Raspberry_Pi) 3 Model B+
    * [Raspbian](https://www.raspberrypi.org/downloads/raspbian/) GNU/Linux 9.0 (stretch) 2018-06-27
        * Qt 5.7.1

## 環境構築

* [Raspbian stretch で Qt5.7 インストールしたもの一覧](http://ytyaru.hatenablog.com/entry/2019/12/17/000000)

# ライセンス

　このソフトウェアはCC0ライセンスである。

[![CC0](http://i.creativecommons.org/p/zero/1.0/88x31.png "CC0")](http://creativecommons.org/publicdomain/zero/1.0/deed.ja)

## 利用ライブラリ

ライブラリ|ライセンス|ソースコード
----------|----------|------------
[Qt](http://doc.qt.io/)|[LGPL](http://doc.qt.io/qt-5/licensing.html)|[GitHub](https://github.com/qt)

* [参考1](https://www3.sra.co.jp/qt/licence/index.html)
* [参考2](http://kou-lowenergy.hatenablog.com/entry/2017/02/17/154720)
* [参考3](https://qiita.com/ynuma/items/e8749233677821a81fcc)
