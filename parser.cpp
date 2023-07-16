#include "parser.h"

#include <QDirIterator>

QString currentMedianFolder;
QString currentBackupFolder;
std::unordered_map<QString,QFileInfo> medianFileInfo;
std::unordered_map<QString,QFileInfo> backupFileInfo;

void readMedianFolder() {
    medianFileInfo.clear();
    QDirIterator it(currentMedianFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QString extension = info.suffix();
            QString name = info.baseName();
            if (extension.toStdString() == "stash") {
                medianFileInfo.insert(std::make_pair(name,info));
            }
        }
    }
}

void readBackupFolder() {
    backupFileInfo.clear();
    QDirIterator it(currentBackupFolder);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = (it).fileInfo();
        if (info.isFile()) {
            QString extension = info.suffix();
            QString name = info.baseName();
            if (extension.toStdString() == "stash") {
                backupFileInfo.insert(std::make_pair(name,info));
            }
        }
    }
}
