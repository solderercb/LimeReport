#include "mainwindow.h"
#include "ui_mainwindow.h"
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

    userData = new QMap<QString, QVariant>;
    userData->insert("defaultDocumentPrinter", QString("Microsoft Print To PDF"));  // это значение нужно получить из локального файла
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::some_func()
{
    qDebug() << "MainWindow::some_func()";
//    tabReportDesigner *reportDesigner = new tabReportDesigner(this);
//    reportDesigner->tabCloseRequest();
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    // TODO: проверка типа виджета на вкладке.

//    if (designerWindow->checkNeedToSave())
    static_cast<tabCommon*>(ui->tabWidget->widget(index))->tabCloseRequest();
    delete ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
}


void MainWindow::on_pushButton_clicked()
{
//    qDebug() << "MainWindow::on_pushButton_clicked()";
    tabReportDesigner *reportDesigner = new tabReportDesigner();
    ui->tabWidget->addTab(reportDesigner, "Designer");
    ui->tabWidget->setCurrentWidget(reportDesigner);
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << userData;
    tabPrintDialog *printDialog = new tabPrintDialog(this);
    ui->tabWidget->addTab(printDialog, "Print");
    ui->tabWidget->setCurrentWidget(printDialog);
}


void MainWindow::on_pushButton_3_clicked()
{
    tabPrintDialog *printDialog = new tabPrintDialog(this);

    DataFile = new QByteArray;
    qDebug() << "MainWindow::on_pushButton_3_clicked()";
    QFile  CurrentFile(QApplication::applicationDirPath() + "\\demo_reports\\priemka.lrxml");  // файл отчета (будет загружаться из БД)

    if(!CurrentFile.open(QIODevice::ReadOnly))
        qDebug() << "Can't open .lrxml";
    *DataFile = CurrentFile.readAll();
//    int S = DataFile->size();

//    printDialog->loadTmpReportTemplate(CurrentFile.fileName());
    printDialog->loadReportTemplate(DataFile);

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

