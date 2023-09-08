#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include <QStringListModel>
#include <QFile>
#include <QByteArray>
#include <LimeReport>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QPrintPreviewDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "sqlcreds.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void some_func();
    QMap<QString, QVariant> *userData;

private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    LimeReport::ReportEngine *m_report;
    LimeReport::ReportDesignWindowInterface* designerWindow;
    QByteArray *DataFile;
};
#endif // MAINWINDOW_H
