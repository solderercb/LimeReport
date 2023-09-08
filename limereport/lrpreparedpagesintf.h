#ifndef LRPREPAREDPAGESINTF_H
#define LRPREPAREDPAGESINTF_H
#include "lrglobal.h"
namespace LimeReport {
class LIMEREPORT_EXPORT IPreparedPages{
public:
    virtual ~IPreparedPages(){};
    virtual bool loadFromFile(const QString& fileName) = 0;
    virtual bool loadFromString(const QString data) = 0;
    virtual bool loadFromByteArray(QByteArray* data) = 0;
    virtual bool saveToFile(const QString& fileName) = 0;
    virtual QString saveToString()  = 0;
    virtual QByteArray  saveToByteArray() = 0;
    virtual void clear() = 0;
    struct PageProps{
        QPageSize pageSize;
        int orientation;
        QRect geometry;
        int leftMargin;
        int topMargin;
        int rightMargin;
        int bottomMargin;
        bool isSetPageSizeToPrinter;
        bool endlessHeight;
        int heightMM;
        int widthMM;
    };
    virtual PageProps pageProperties(int page = 0) = 0;
};
} //namespace LimeReport
#endif // LRPREPAREDPAGESINTF_H
