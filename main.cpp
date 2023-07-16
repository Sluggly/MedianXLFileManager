#include "mainwindow.h"
#include "config.h"

#include <QApplication>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <iostream>

void startUpReadConfigFile(MainWindow& w) {
    QFile jsonConfigFile;
    jsonConfigFile.setFileName(QDir::currentPath() + "/config.json");
    if (!jsonConfigFile.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qDebug() << "JSON Config File Open Error.";
    }
    QString content = jsonConfigFile.readAll();
    jsonConfigFile.close();

    QJsonDocument jsonD = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject jsonObj = jsonD.object();
    QString medianFolder = jsonObj["medianFolder"].toString();
    if (!medianFolder.isNull()) {
        currentMedianFolder = medianFolder;
        w.setMedianPathLabel();
        w.fillMedianTableView();
    }

    currentBackupFolder = QDir::currentPath() + "/save";
    if (!QDir(QDir::currentPath() + "/save").exists()) {
        QDir().mkdir(QDir::currentPath() + "/save");
    }
    else {
        w.fillBackupTableView();
    }

}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    startUpReadConfigFile(w);
    return a.exec();
}
