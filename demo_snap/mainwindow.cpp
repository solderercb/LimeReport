#include "global.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tabcommon.h"
#include "tabprintdialog.h"
#include "tabreportdesigner.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->resize(1440,800);

    QVector<QSqlDatabase *> connections;

    QSqlDatabase connMain = QSqlDatabase::addDatabase("QMYSQL", "connMain");       // это соединение для получения данных (ремонты, клиенты и т. д.)
    connections.append(&connMain);
    QSqlDatabase connNtfy = QSqlDatabase::addDatabase("QMYSQL", "connThird");
    connections.append(&connNtfy);

    for (int i=0; i<connections.size(); i++)
    {
        connections[i]->setUserName(SQL_USER);
        connections[i]->setPassword(SQL_PSWD);
        connections[i]->setHostName(SQL_HOST);
        connections[i]->setPort(SQL_PORT);
        connections[i]->setDatabaseName(SQL_DB);
    }

    if (!connMain.open())
    {
        qDebug().noquote() << QString("%1 can't be established: ").arg(connMain.connectionName()) << connMain.lastError().driverText();
    }
    else
    {
        qDebug().noquote() << QString("%1 successfully established.").arg(connMain.connectionName());
        QSqlQuery queryCheckUser = QSqlQuery(QSqlDatabase::database("connMain"));   // проверка состояния учетной записи пользователя (база программы, а не mysql)
        queryCheckUser.exec(QString("SELECT `id` FROM `users` WHERE `username` = '%1' AND `state` = 1 AND `is_bot` = 0 LIMIT 1;").arg(QSqlDatabase::database("connMain").userName()));
        queryCheckUser.first();
        if (queryCheckUser.isValid())
        {
            for (int i = 1; i<connections.size(); i++)  // открываем вспомогательные соединения
            {
                if (!connections[i]->open())
                {
                    qDebug().noquote() << QString("%1 can't be established: ").arg(connections[i]->connectionName()) << connections[i]->lastError().driverText();
                }
                else
                {
                    qDebug().noquote() << QString("%1 successfully established.").arg(connections[i]->connectionName());
                }
            }
        }
    }

    userLocalData->insert("DocsPrinter", QString("Microsoft Print To PDF"));  // это значение нужно получить из локального файла

    initUserDbData();
    initPermissions();
    initCompanies();
    initOffices();
    initGlobalModels();

//    on_pushButton_clicked();
    on_pushButton_2_clicked();
//    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::some_func()
{
    qDebug() << "MainWindow::some_func()";
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    // TODO: проверка типа виджета на вкладке.

    tabCommon &tab = *(static_cast<tabCommon*>(ui->tabWidget->widget(index)));
    if (tab.tabCloseRequest())   // Перед закрытием  вкладки нужно проверить нет ли несохранённых данных
    {
        delete ui->tabWidget->widget(index);
    }
}


void MainWindow::on_pushButton_clicked()    // Дизайнер
{
//    qDebug() << "MainWindow::on_pushButton_clicked()";
    tabReportDesigner *reportDesigner = new tabReportDesigner(this, "new_rep");
    ui->tabWidget->addTab(reportDesigner, "Designer");
    ui->tabWidget->setCurrentWidget(reportDesigner);
}

void MainWindow::on_pushButton_2_clicked()  // Просмотр/печать
{

    QMap<QString, QVariant> report_vars;

//    qDebug() << "MainWindow::on_pushButton_2_clicked()";

    report_vars.insert("repair_id", 24901);    // устанавливаем различные переменные для отчёта. Т. о. я могу передать в отчёт почти всё что угодно
    report_vars.insert("type", "new_rep");
//    report_vars.insert("repair_id", 24972);
//    report_vars.insert("type", "rep_label");
//    report_vars.insert("ids_list", "27529,27523,27513,27528,27515,27531,27518,27525,27512,27505,27530");
//    report_vars.insert("type", "sticker1");
    tabPrintDialog *printDialog = new tabPrintDialog(this, report_vars);
    ui->tabWidget->addTab(printDialog, "Print");
    ui->tabWidget->setCurrentWidget(printDialog);
}


void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << "MainWindow::on_pushButton_3_clicked()";
    qDebug() << *userDbData;
//    QWidget *printDialo   g = new QWidget();
    tabPrintDialog *printDialog = new tabPrintDialog(nullptr);
    ui->tabWidget->addTab(printDialog, "Print");
    ui->tabWidget->setCurrentWidget(printDialog);
}



void MainWindow::on_pushButton_4_clicked()
{
//    QPrintPreviewDialog *printPreview = new QPrintPreviewDialog();
//    ui->tabWidget->addTab(printPreview, "QPrintPreviewWidget");
//    ui->tabWidget->setCurrentWidget(printPreview);

    LimeReport::ReportEngine *m_report = new LimeReport::ReportEngine;
    QPrinterInfo pi;
    QPrinter lp(QPrinter::HighResolution);

    if (!pi.defaultPrinter().isNull()){
#if QT_VERSION >= 0x050300
            lp.setPrinterName(pi.defaultPrinterName());
#else
            lp.setPrinterName(pi.defaultPrinter().printerName());
#endif
    }

    QPrinter* printer = /*m_defaultPrinter ? m_defaultPrinter : */&lp;

    qDebug() << printer->supportedResolutions();

    if (!m_report->loadFromFile(QApplication::applicationDirPath() + "\\demo_reports\\simple_list.lrxml"))
        qDebug() << "Can't open .lrxml";

//    LimeReport::PreviewReportWidget *preview = m_report->createPreviewWidget();
//    qDebug() << preview->scaleType() << preview->scalePercent();
//    preview->setScaleType(LimeReport::ScaleType::FitPage);
//    ui->tabWidget->addTab(preview,"123");
//    ui->tabWidget->setCurrentWidget(preview);

//    m_report->printPages();
    m_report->prepareReportPages();
//    m_report->previewReport();
//    m_report->printReport(printer);
}

void MainWindow::on_pushButton_5_clicked()
{
    printerSettings();
}

void MainWindow::printerSettings(QPrinter *printer)
{
    DWORD size;
    bool result;
    result = GetDefaultPrinter (NULL, &size);
//    LPWSTR pname = (LPWSTR)malloc(size+1);
//    LPTSTR pname = (LPTSTR)malloc(size+1);
    QString qString("doPDF 8 (перенаправлено 2)\0");
    LPWSTR pname = reinterpret_cast<wchar_t *>(qString.data());
    size = qString.length();
    qDebug() << "LEN: " << size;
    result = GetDefaultPrinter(pname, &size);
//    QString *printername = (QString*)(pname);

    QStringList printersList = QPrinterInfo::availablePrinterNames();
    qDebug() << printersList;

    if(!result){
       qDebug() << "GetDefaultPrinter err: " << GetLastError();
    }
    else {
        pname[size] = 0;
        qDebug() << "PRINTER NAME: " << ((QString)*(pname)) << ", LEN: " << size;
    }

//    QString name = mDefaultPrinter->printerName();

    //These give more or less odd results
    //wchar_t* w_str = const_cast<wchar_t*>(name.toStdWString().c_str());
    //LPWSTR w_str = (LPWSTR)name.utf16();
    /*wchar_t* w_str = (wchar_t*) malloc (sizeof(wchar_t)*name.length()+1);
    int len = name.toWCharArray(w_str);
    w_str[len]=0;*/

    //OpenPrinter example uses LPHANDLE but that didn't work
    HANDLE hPrinter = NULL;
    PRINTER_DEFAULTS pd;
    ZeroMemory(&pd, sizeof(pd));
    pd.DesiredAccess = PRINTER_ALL_ACCESS;

    qDebug() << "TRYING GET PRINTER: " /*<< name*/;
//    OpenPrinter(pname, &hPrinter, &pd)
    if(!OpenPrinter(pname, &hPrinter, NULL)) {
        qDebug() << "GOT PRINTER ERR STATE: " /*<< IsPrinterError(hPrinter)*/;
    }
    qDebug() << "hPrinter = " << hPrinter;

    qDebug() << "mainwindow winId(): " << winId();
    if(!PrinterProperties((HWND)winId(), hPrinter)) {
        qDebug() << "GOT ERR PrinterProperties() dialog: ";
    }
    else
        qDebug() << "PrinterProperties() dialog closed";

    ClosePrinter(hPrinter);
//    free(hPrinter);
//    free(pname);

//    emit printReceiptComplete(success);
}

