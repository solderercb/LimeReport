#ifndef TABCOMMON_H
#define TABCOMMON_H

#include <qglobal.h>
#include <QWidget>
#include "mainwindow.h"

class tabCommonPrivate;

class tabCommon : public QWidget
{
    Q_OBJECT

    friend class tabCommonPrivate;

public:
    explicit tabCommon(MainWindow *p = nullptr);
    ~tabCommon();
    bool tabCloseRequest();
    void setTabCloseRequestRIPointer(bool (tabCommon::*pointer)(void));
    bool tabCloseRequestRI();
//    QWidget *p;
private:
    bool (tabCommon::*tabCloseRequestRI_p)(void) = &tabCommon::tabCloseRequestRI;    // указатель на функцию tabCloseRequestRI
    tabCommonPrivate *d_ptr;

};


#endif // TABCOMMON_H
