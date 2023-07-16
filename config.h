#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <unordered_map>
#include <QFileInfo>

extern QString currentMedianFolder;
extern QString currentBackupFolder;

extern std::unordered_map<QString,QFileInfo> medianFileInfo;
extern std::unordered_map<QString,QFileInfo> backupFileInfo;

#endif // CONFIG_H
