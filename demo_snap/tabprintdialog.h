#ifndef TABPRINTDIALOG_H
#define TABPRINTDIALOG_H

#include <QWidget>
#include <LimeReport>
#include <QStringList>
#include <QStringListModel>
#include <QPrinterInfo>
#include <QPageSetupDialog>
#include <QByteArray>
#include <QFile>
#include <QMap>
#include <QString>
#include <QVariant>
#include "tabcommon.h"
#include "mainwindow.h"
#include <Windows.h>

namespace Ui {
class tabPrintDialog;
}

class tabPrintDialog : public tabCommon
{
    Q_OBJECT

public:
    explicit tabPrintDialog(MainWindow *parent = nullptr);
    ~tabPrintDialog();
    bool loadReportTemplate(QByteArray*);
    bool loadTmpReportTemplate(QString);
    QMap<QString, QVariant> *userData;

private:
    Ui::tabPrintDialog *ui;
    LimeReport::ReportEngine *m_report;
    LimeReport::ReportDesignWindowInterface* designerWindow;
    QByteArray *DataFile;
    LimeReport::PreviewReportWidget *previewWindow;
    QPrinter *printer;

private slots:
    void some_func();
    void on_pushButtonPrint_clicked();
    void on_comboBoxPrinters_currentIndexChanged(const QString &arg1);
    void on_comboBoxPrinters_currentTextChanged(const QString &arg1);
    void on_labelPrinterSettings_linkActivated(const QString &link);
    void pageSetupAccepted();
};

#endif // TABPRINTDIALOG_H
