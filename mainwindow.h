#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QStandardItemModel>
#include <QItemSelectionModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void handleSelectFolderButton();
    void fillMedianTableView();
    void extracted(int &numeroElem);
    void fillBackupTableView();
    void setMedianPathLabel();
    void refreshEverything();
    void disableEveryButton();
    void enableGlobalButtons();
    void mainWindowRefresh();

    int currentSelectedIndex = -1;

    QStandardItemModel* medianModel = nullptr;
    QStandardItemModel* backupModel = nullptr;

private slots:
    void on_actionAbout_Qt_triggered();

    void on_actionAbout_Me_triggered();

    void on_actionOpen_triggered();

    void on_loadSharedStashButton_clicked();

    void on_loadEverythingButton_clicked();

    void on_loadCharacterButton_clicked();

    void on_loadCharacterStashButton_clicked();

    void on_saveCharacterButton_clicked();

    void on_saveCharacterStashButton_clicked();

    void on_saveSharedStashButton_clicked();

    void on_saveEverythingButton_clicked();

    void on_refreshButton_clicked();

    void on_medianTableView_clicked(const QModelIndex &index);

    void on_backupTableView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    // QObject interface
public:
    bool event(QEvent *event);
};
#endif // MAINWINDOW_H
