#ifndef TABREPORTDESIGNER_H
#define TABREPORTDESIGNER_H

#include <LimeReport>
#include "tabcommon.h"
#include "mainwindow.h"

namespace Ui {
class tabReportDesigner;
}

class tabReportDesigner : public tabCommon
{
    Q_OBJECT

public:
    explicit tabReportDesigner(MainWindow *parent = nullptr);
    ~tabReportDesigner();
    bool tabCloseRequestRI();
    LimeReport::ReportEngine *report;
    LimeReport::ReportDesignWindowInterface *reportDesigner;

private:
    Ui::tabReportDesigner *ui;
    tabCommon *d_ptr;
    QByteArray *DataFile;
    bool (tabReportDesigner::*tmp_ptr)(void) = &tabReportDesigner::tabCloseRequestRI;
    QFile CurrentFile;
    QFile tmpFile;
};

#endif // TABREPORTDESIGNER_H
