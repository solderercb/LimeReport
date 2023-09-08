#include "tabcommon.h"

struct tabCommonPrivate
{
    tabCommonPrivate(tabCommon *q) : q_ptr(q){ };
    tabCommon *q_ptr;
    bool tabCloseRequestHelper();
};

bool tabCommonPrivate::tabCloseRequestHelper()
{
    tabCommon *q = static_cast<tabCommon*>(q_ptr);
    return (q->*(q->tabCloseRequestRI_p))();    // вызываем функцию по адресу в указателе
}

tabCommon::tabCommon(MainWindow *p) :
    QWidget(p),
    d_ptr(new tabCommonPrivate(this))
{
}

tabCommon::~tabCommon()
{
    delete d_ptr;
}

bool tabCommon::tabCloseRequest()
{
    tabCommonPrivate *d = static_cast<tabCommonPrivate*>(d_ptr);
    return d->tabCloseRequestHelper();
}

void tabCommon::setTabCloseRequestRIPointer(bool (tabCommon::*pointer)(void))  // setter указателя на функцию setTabCloseRequestRI
{
    tabCloseRequestRI_p = pointer;
}

bool tabCommon::tabCloseRequestRI() // функция-пустышка, нужна для инициализации указателя на функцию
{
    qDebug() << "(default) tabCloseRequestRI()";
    return 0;
}
