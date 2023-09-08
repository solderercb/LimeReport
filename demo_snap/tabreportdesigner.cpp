#include "tabreportdesigner.h"
#include "ui_tabreportdesigner.h"

tabReportDesigner::tabReportDesigner(MainWindow *parent) :
    tabCommon(parent),
    ui(new Ui::tabReportDesigner)
{
    ui->setupUi(this);
    tabCommon::setTabCloseRequestRIPointer((bool (tabCommon::*)(void))(tmp_ptr));   // обновляем указатель адресом переопределённой функции
    report = new LimeReport::ReportEngine;
    reportDesigner = report->getDesignerWindow();

    DataFile = new QByteArray;
    CurrentFile.setFileName(QApplication::applicationDirPath() + "\\demo_reports\\priemka.lrxml");  // файл отчета (будет загружаться из БД)
    if(!CurrentFile.open(QIODevice::ReadOnly))
        qDebug() << "Can't open .lrxml";
    *DataFile = CurrentFile.readAll();
    CurrentFile.close();
    tmpFile.setFileName(QApplication::applicationDirPath() + "\\template.tmp");         // временный файл для редактирования (для того, чтобы работала кнопка Save)
    if (!tmpFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Can't open new file";
    }
    tmpFile.write(*DataFile);
    tmpFile.close();
    report->loadFromFile(tmpFile.fileName());

    ui->gridLayoutTab->addWidget(reportDesigner,0,0);
}

tabReportDesigner::~tabReportDesigner()
{
    delete ui;
//    delete DataFile;
    delete report;
}

bool tabReportDesigner::tabCloseRequestRI()
{
    qDebug() << "(reimplemented) tabCloseRequestRI()";

    if ( reportDesigner->checkNeedToSave() )
    {

    }

    DataFile = new QByteArray;
    tmpFile.setFileName(QApplication::applicationDirPath() + "\\template.tmp");         // временный файл для редактирования (для того, чтобы работала кнопка Save)
    if(!tmpFile.open(QIODevice::ReadOnly))
        qDebug() << "Can't open temp file";
    *DataFile = tmpFile.readAll();
    tmpFile.close();
    CurrentFile.setFileName(QApplication::applicationDirPath() + "\\demo_reports\\priemka.lrxml");  // файл отчета (будет загружаться из БД)
    if (!CurrentFile.open(QIODevice::ReadWrite))
    {
        qDebug() << "Can't open .lrxml";
    }
    CurrentFile.write(*DataFile);
    CurrentFile.close();
}
