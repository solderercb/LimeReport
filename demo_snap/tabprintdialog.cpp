#include "tabprintdialog.h"
#include "ui_tabprintdialog.h"

tabPrintDialog::tabPrintDialog(MainWindow *parent) :  tabCommon(parent),
    ui(new Ui::tabPrintDialog)
{
    ui->setupUi(this);
    printer = new QPrinter(QPrinter::HighResolution);
    userData = parent->userData;
//    parent->some_func();
    m_report = new LimeReport::ReportEngine();
    previewWindow =  m_report->createPreviewWidget();

    QStringList simpleData;
    simpleData << "value1" << "value2" << "value3";
    QStringListModel* stringListModel = new QStringListModel();
    stringListModel->setStringList(simpleData);
    stringListModel->setHeaderData(0,Qt::Horizontal, simpleData, Qt::DisplayRole);
    m_report->dataManager()->addModel("string_list",stringListModel,true);

    qDebug() << QPrinterInfo::availablePrinterNames();
    ui->comboBoxPrinters->addItems(QPrinterInfo::availablePrinterNames());
    if (userData->contains("defaultDocumentPrinter"))
    {
        qDebug() << "defaultDocumentPrinter: " << userData->value("defaultDocumentPrinter").toString();
        ui->comboBoxPrinters->setCurrentText(userData->value("defaultDocumentPrinter").toString());
    }

//    on_pushButtonPrint_clicked();

//    QPrinter *printer = new QPrinter(QPrinterInfo::printerInfo("Microsoft Print To PDF"));
//    printer->setPrinterName("Microsoft Print To PDF");

//    if (!m_report->loadFromFile(QApplication::applicationDirPath() + "\\demo_reports\\simple_list.lrxml"))
//        qDebug() << "Can't open .lrxml";
//    m_report->previewReport();
//    m_report->printReport(printer);
/*
    DataFile = new QByteArray;
    QFile CurrentFile(QApplication::applicationDirPath() + "\\demo_reports\\simple_group.lrxml");
    if(!CurrentFile.open(QIODevice::ReadOnly))
        qDebug() << "Can't open .lrxml";
    QByteArray fileData = CurrentFile.readAll();
    qDebug() << fileData;
    m_report->loadFromByteArray(&fileData);

    LimeReport::PreviewReportWidget *previewWindow =  m_report->createPreviewWidget(this);

    // Вкладка предпросмотра  на подобие MS Office: слева в столбик параметры печати, а справа непосредсвтенно превью
    ui->gridLayoutTab->addWidget(previewWindow, 0, 1);
    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
*/
}

tabPrintDialog::~tabPrintDialog()
{
    delete ui;
    delete m_report;
}

bool tabPrintDialog::loadReportTemplate(QByteArray *data)
{
    if (!m_report->loadFromByteArray(data))
        return 1;
    previewWindow->refreshPages();

    // Вкладка предпросмотра  на подобие MS Office: слева в столбик параметры печати, а справа непосредсвтенно превью
    ui->gridLayoutTab->addWidget(previewWindow, 0, 1);
    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
    return 0;
}

bool tabPrintDialog::loadTmpReportTemplate(QString filename)
{
    if (!m_report->loadFromFile(filename))
        qDebug() << "Can't open file";
    previewWindow->refreshPages();

    // Вкладка предпросмотра  на подобие MS Office: слева в столбик параметры печати, а справа непосредсвтенно превью
    ui->gridLayoutTab->addWidget(previewWindow, 0, 1);
    ui->gridLayoutTab->setColumnStretch(1, 1);
    ui->gridLayoutTab->setColumnMinimumWidth(0, 200);
}

void tabPrintDialog::some_func()
{

    if (!m_report->loadFromFile(QApplication::applicationDirPath() + "\\demo_reports\\simple_list.lrxml"))
        qDebug() << "Can't open .lrxml";

//    designerWindow = m_report->getDesignerWindow();
//    ui->tabWidget->addTab(designerWindow, "designer");
//    ui->tabWidget->setCurrentWidget(designerWindow);
//    m_report->designReport();

//
//    ui->tabWidget->addTab(previewWindow, "preview");
//    ui->tabWidget->setCurrentWidget(previewWindow);
//    m_report->previewReport();
}



void tabPrintDialog::on_pushButtonPrint_clicked()
{
    qDebug() << "printer name: " << printer->printerName();
    if (!ui->lineEditPagesToPrint->text().isEmpty())
    {
        printer->setPrintRange(QPrinter::PrintRange(2));
        QRegularExpression rx("\\d+-?(\\d)*");
        QValidator *validator = new QRegularExpressionValidator (rx, this);
        ui->lineEditPagesToPrint->setValidator(validator);
        if(ui->lineEditPagesToPrint->hasAcceptableInput())
        {
            qDebug() << "acceptable input";
            QStringList pages = ui->lineEditPagesToPrint->text().split('-');
            if (pages.count() == 1)
            {
                printer->setFromTo(pages.at(0).toInt(), pages.at(0).toInt());
            }
            else
            {
                printer->setFromTo(pages.at(0).toInt(), pages.at(1).toInt());
            }
        }
        else
            qDebug() << "unacceptable input";
    }
    // TODO: printer->setDocName("");

//    QPrinterInfo pi;
//    QPrinter lp(QPrinter::HighResolution);

//    if (!pi.defaultPrinter().isNull()){
//#if QT_VERSION >= 0x050300
//            lp.setPrinterName(pi.defaultPrinterName());
//#else
//            lp.setPrinterName(pi.defaultPrinter().printerName());
//#endif
//    }

//    QPrinter* printer = /*m_defaultPrinter ? m_defaultPrinter : */&lp;
//    printer->setPageSize(QPageSize(QPageSize::A4));
//    printer->setResolution(24);
    int mrgLeft, mrgTop, mrgRight, mrgBot;
    mrgLeft = ui->lineEditMrgLeft->text().toInt();
    mrgTop = ui->lineEditMrgTop->text().toInt();
    mrgRight = ui->lineEditMrgRight->text().toInt();
    mrgBot = ui->lineEditMrgBot->text().toInt();

    // по непонятной причине границы слева и справа должны быть минимум 5pt, иначе отчет, расчечатанный на виртуальном принтере Microsoft или doPDF оказывается скукорженным в несколько раз.
    // границы сверху и снизу могут быть равны нулю
//    printer->setPageLayout(QPageLayout(QPageSize::A5,QPageLayout::Portrait,QMargins(mrgLeft, mrgTop, mrgRight, mrgBot) ));
    printer->setPageLayout(QPageLayout(QPageSize::B5,QPageLayout::Portrait,QMargins(0, 0, 0, 0) ));
//    printer->setFullPage(true);
    qDebug() << "printing...";
    qDebug() << "printer->pageLayout().pageSize() = " << printer->pageLayout().pageSize();
//    previewWindow->printPages(printer);
    previewWindow->print();
//    m_report->printReport(printer);
//    QPrintDialog dialog(printer,QApplication::activeWindow());
//    if (dialog.exec()==QDialog::Accepted){
//        printPages(printer);
//    }
}


void tabPrintDialog::on_comboBoxPrinters_currentIndexChanged(const QString &arg1)
{
    qDebug() << "on_comboBoxPrinters_currentIndexChanged()";
    printer->setPrinterName(arg1);
}


void tabPrintDialog::on_comboBoxPrinters_currentTextChanged(const QString &arg1)
{
    printer->setPrinterName(arg1);
    qDebug() << "on_comboBoxPrinters_currentTextChanged(), printerName():" << printer->printerName();
    qDebug() << "pageLayout():" << printer->pageLayout();
    qDebug() << "outputFormat() :" << printer->outputFormat();
    qDebug() << "pageRect(QPrinter::Millimeter):" << printer->pageRect(QPrinter::Millimeter);
    qDebug() << "paperRect(QPrinter::Millimeter):" << printer->paperRect(QPrinter::Millimeter);
    qDebug() << "resolution():" << printer->resolution();
//    qDebug() << ":" << printer->;
//    qDebug() << ":" << printer->;
//    qDebug() << ":" << printer->;
//    qDebug() << ":" << printer->;
//    qDebug() << ":" << printer->;
//    qDebug() << ":" << printer->;
}


void tabPrintDialog::on_labelPrinterSettings_linkActivated(const QString &link)
{
    qDebug() << "on_labelPrinterSettings_linkActivated";
    QPageSetupDialog dialog(printer);
    dialog.open(this, SLOT(pageSetupAccepted()));

    // Вызов Win API диалога свойств принтера
//    LPHANDLE *pPrinter;
//    LPTSTR pPrinterName = (LPTSTR)"Microsoft Print To PDF";
//    OpenPrinterW(pPrinterName, pPrinter, NULL);
//    PrinterProperties(NULL, pPrinter);
}

void tabPrintDialog::pageSetupAccepted()
{
    qDebug() << "pageSetupAccepted";
}

