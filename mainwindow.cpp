#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "parser.h"

#include <QFileDialog>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonDocument>

#include <QStandardItemModel>

#include <QDirIterator>

#include <iostream>
#include <vector>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionAbout_Qt_triggered() {
    QApplication::aboutQt();
}


void MainWindow::on_actionAbout_Me_triggered() {
    QMessageBox::information(this,"About Me","This tool was made by Sluggly.");
}

void MainWindow::setMedianPathLabel() {
    ui->pathLabel->setText(currentMedianFolder);
}

void replaceMedianPathInConfigFile(QString path) {
    QFile jsonConfigFile;
    jsonConfigFile.setFileName(QDir::currentPath() + "/config.json");
    if (!jsonConfigFile.open(QIODevice::WriteOnly)) {
        qDebug() << "JSON Config File Open Error.";
    }

    QJsonObject medianPathObject;
    medianPathObject.insert("medianFolder",path);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(medianPathObject);

    jsonConfigFile.write(jsonDoc.toJson());
    jsonConfigFile.close();
}

void MainWindow::refreshEverything() {
    fillMedianTableView();
    fillBackupTableView();
    ui->medianTableView->sortByColumn(1,Qt::SortOrder::DescendingOrder);
    ui->backupTableView->sortByColumn(1,Qt::SortOrder::DescendingOrder);
}

void MainWindow::fillMedianTableView() {
    readMedianFolder();
    if (medianModel != nullptr) { medianModel->clear(); }
    medianModel = new QStandardItemModel();
    medianModel->setColumnCount(2);
    QStringList headers;
    headers.append("Character");
    headers.append("Last Stash Change");
    medianModel->setHorizontalHeaderLabels(headers);
    int numeroElem = 0;
    for (auto elem : medianFileInfo) {
        medianModel->appendRow(nullptr);
        medianModel->setData(medianModel->index(numeroElem,0),elem.first);
        medianModel->setData(medianModel->index(numeroElem,1),elem.second.lastModified());
        numeroElem++;
    }
    ui->medianTableView->setModel(medianModel);
    ui->medianTableView->setColumnWidth(0,100);
    ui->medianTableView->setColumnWidth(1,201);
    ui->medianTableView->sortByColumn(1,Qt::SortOrder::DescendingOrder);
}

void MainWindow::fillBackupTableView() {
    readBackupFolder();
    if (backupModel != nullptr) { backupModel->clear(); }
    backupModel = new QStandardItemModel();
    backupModel->setColumnCount(2);
    QStringList headers;
    headers.append("Character");
    headers.append("Last Stash Change");
    backupModel->setHorizontalHeaderLabels(headers);
    int numeroElem = 0;
    for (auto elem : backupFileInfo) {
        backupModel->appendRow(nullptr);
        backupModel->setData(backupModel->index(numeroElem,0),elem.first);
        backupModel->setData(backupModel->index(numeroElem,1),elem.second.lastModified());
        numeroElem++;
    }
    ui->backupTableView->setModel(backupModel);
    ui->backupTableView->setColumnWidth(0,100);
    ui->backupTableView->setColumnWidth(1,201);
    ui->backupTableView->sortByColumn(1,Qt::SortOrder::DescendingOrder);
}


void MainWindow::on_actionOpen_triggered() {
    disableEveryButton();
    QString directoryName = QFileDialog::getExistingDirectory(this,"Select a directory","C://");
    if (!directoryName.isNull()) {
        currentMedianFolder = directoryName;
        ui->pathLabel->setText(currentMedianFolder);
        replaceMedianPathInConfigFile(currentMedianFolder);
        fillMedianTableView();
        enableGlobalButtons();
    }
}


void MainWindow::on_loadSharedStashButton_clicked() {
    disableEveryButton();
    QDirIterator it(currentBackupFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QString extension = info.suffix();
            QString name = info.baseName();
            if (extension.toStdString() == "shared") {
                QFile::remove(currentMedianFolder + "/" + info.fileName());
                QFile::copy(info.filePath(),currentMedianFolder + "/" + info.fileName());
                break;
            }
        }
    }
    refreshEverything();
    enableGlobalButtons();
}


void MainWindow::on_loadEverythingButton_clicked() {
    disableEveryButton();
    QDirIterator it(currentBackupFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QFile::remove(currentMedianFolder + "/" + info.fileName());
            QFile::copy(info.filePath(),currentMedianFolder + "/" + info.fileName());
        }
    }
    refreshEverything();
    enableGlobalButtons();
}


void MainWindow::on_loadCharacterButton_clicked() {
    disableEveryButton();
    QString selectedName = backupModel->item(currentSelectedIndex)->text();
    QDirIterator it(currentBackupFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QString name = info.baseName();
            if (selectedName == name) {
                QFile::remove(currentMedianFolder + "/" + info.fileName());
                QFile::copy(info.filePath(),currentMedianFolder + "/" + info.fileName());
            }
        }
    }
    refreshEverything();
    enableGlobalButtons();
}


void MainWindow::on_loadCharacterStashButton_clicked() {
    disableEveryButton();
    QString selectedName = backupModel->item(currentSelectedIndex)->text();
    QDirIterator it(currentBackupFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QString name = info.baseName();
            QString extension = info.suffix();
            if ((selectedName == name)&&(extension == "stash")) {
                QFile::remove(currentMedianFolder + "/" + info.fileName());
                QFile::copy(info.filePath(),currentMedianFolder + "/" + info.fileName());
            }
        }
    }
    refreshEverything();
    enableGlobalButtons();
}


void MainWindow::on_saveCharacterButton_clicked() {
    disableEveryButton();
    QString selectedName = medianModel->item(currentSelectedIndex)->text();
    QDirIterator it(currentMedianFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QString name = info.baseName();
            if (selectedName == name) {
                QFile::remove(currentBackupFolder + "/" + info.fileName());
                QFile::copy(info.filePath(),currentBackupFolder + "/" + info.fileName());
            }
        }
    }
    refreshEverything();
    enableGlobalButtons();
}


void MainWindow::on_saveCharacterStashButton_clicked() {
    disableEveryButton();
    QString selectedName = medianModel->item(currentSelectedIndex)->text();
    QDirIterator it(currentMedianFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QString name = info.baseName();
            QString extension = info.suffix();
            if ((selectedName == name)&&(extension == "stash")) {
                QFile::remove(currentBackupFolder + "/" + info.fileName());
                QFile::copy(info.filePath(),currentBackupFolder + "/" + info.fileName());
                break;
            }
        }
    }
    refreshEverything();
    enableGlobalButtons();
}


void MainWindow::on_saveSharedStashButton_clicked() {
    disableEveryButton();
    QDirIterator it(currentMedianFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QString extension = info.suffix();
            if (extension.toStdString() == "shared") {
                QFile::remove(currentBackupFolder + "/" + info.fileName());
                QFile::copy(info.filePath(),currentBackupFolder + "/" + info.fileName());
                break;
            }
        }
    }
    refreshEverything();
    enableGlobalButtons();
}


void MainWindow::on_saveEverythingButton_clicked() {
    disableEveryButton();
    QDirIterator it(currentMedianFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QFile::remove(currentBackupFolder + "/" + info.fileName());
            QFile::copy(info.filePath(),currentBackupFolder + "/" + info.fileName());
        }
    }
    refreshEverything();
    enableGlobalButtons();
}

void MainWindow::mainWindowRefresh() {
    disableEveryButton();
    refreshEverything();
    enableGlobalButtons();
}

void MainWindow::on_refreshButton_clicked() {
    mainWindowRefresh();
}

void MainWindow::disableEveryButton() {
    ui->loadEverythingButton->setDisabled(true);
    ui->saveEverythingButton->setDisabled(true);
    ui->refreshButton->setDisabled(true);
    ui->loadSharedStashButton->setDisabled(true);
    ui->saveSharedStashButton->setDisabled(true);

    ui->loadCharacterButton->setDisabled(true);
    ui->loadCharacterStashButton->setDisabled(true);
    ui->saveCharacterButton->setDisabled(true);
    ui->saveCharacterStashButton->setDisabled(true);

    ui->medianTableView->clearSelection();
    ui->backupTableView->clearSelection();
}

void MainWindow::enableGlobalButtons() {
    ui->loadEverythingButton->setDisabled(false);
    ui->saveEverythingButton->setDisabled(false);
    ui->refreshButton->setDisabled(false);
    ui->loadSharedStashButton->setDisabled(false);
    ui->saveSharedStashButton->setDisabled(false);
}

void MainWindow::on_medianTableView_clicked(const QModelIndex &index) {
    ui->backupTableView->clearSelection();
    ui->loadCharacterButton->setDisabled(true);
    ui->loadCharacterStashButton->setDisabled(true);
    ui->saveCharacterButton->setDisabled(false);
    ui->saveCharacterStashButton->setDisabled(false);

    currentSelectedIndex = index.row();
}


void MainWindow::on_backupTableView_clicked(const QModelIndex &index) {
    ui->medianTableView->clearSelection();
    ui->loadCharacterButton->setDisabled(false);
    ui->loadCharacterStashButton->setDisabled(false);
    ui->saveCharacterButton->setDisabled(true);
    ui->saveCharacterStashButton->setDisabled(true);

    currentSelectedIndex = index.row();
}

bool MainWindow::event(QEvent *event) {
    switch(event->type()) {
        case QEvent::WindowActivate:
        case QEvent::FocusIn:
        mainWindowRefresh();
        default:
            return QMainWindow::event(event);
    }
}
