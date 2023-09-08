#include "lrpreparedpages.h"

#include "serializators/lrxmlreader.h"
#include "serializators/lrxmlwriter.h"

namespace LimeReport {

bool PreparedPages::loadFromFile(const QString &fileName)
{
    ItemsReaderIntf::Ptr reader = FileXMLReader::create(fileName);
    return readPages(reader);
}

bool PreparedPages::loadFromString(const QString data)
{
    ItemsReaderIntf::Ptr reader = StringXMLreader::create(data);
    return readPages(reader);
}

bool PreparedPages::loadFromByteArray(QByteArray *data)
{
    ItemsReaderIntf::Ptr reader = ByteArrayXMLReader::create(data);
    return readPages(reader);
}

bool PreparedPages::saveToFile(const QString &fileName)
{
    if (!fileName.isEmpty()){
        QScopedPointer< ItemsWriterIntf > writer(new XMLWriter());
        foreach (PageItemDesignIntf::Ptr page, *m_pages){
            writer->putItem(page.data());
        }
        return writer->saveToFile(fileName);
    }
    return false;
}

QString PreparedPages::saveToString()
{
    QScopedPointer< ItemsWriterIntf > writer(new XMLWriter());
    foreach (PageItemDesignIntf::Ptr page, *m_pages){
        writer->putItem(page.data());
    }
    return writer->saveToString();
}

QByteArray PreparedPages::saveToByteArray()
{
    QScopedPointer< ItemsWriterIntf > writer(new XMLWriter());
    foreach (PageItemDesignIntf::Ptr page, *m_pages){
        writer->putItem(page.data());
    }
    return writer->saveToByteArray();
}

void PreparedPages::clear()
{
    m_pages->clear();
}

IPreparedPages::PageProps PreparedPages::pageProperties(int page)
{
    PageItemDesignIntf::Ptr p;
    IPreparedPages::PageProps pageProps = {
                                .pageSize = QPageSize(),
                                .orientation = 0,
                                .geometry = QRect(),
                                .leftMargin = 0,
                                .topMargin = 0,
                                .rightMargin = 0,
                                .bottomMargin = 0,
                                .isSetPageSizeToPrinter = 0,
                                .endlessHeight = 0,
                                .heightMM = 0,
                                .widthMM = 0
                              };

    if (m_pages->count() > 0 && m_pages->count() > page && page >= 0)
        p = m_pages->at(page);
    else p = PageItemDesignIntf::Ptr(0);

    if(p)
    {
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 1))
        pageProps.orientation = (QPrinter::Orientation)p->pageOrientation();
#else
        pageProps.orientation = (QPageLayout::Orientation)p->pageOrientation();
#endif
        pageProps.pageSize = QPageSize((QPageSize::PageSizeId)p->pageSize());
        pageProps.geometry = p->geometry();
        pageProps.leftMargin = p->leftMargin();
        pageProps.topMargin = p->topMargin();
        pageProps.rightMargin = p->rightMargin();
        pageProps.bottomMargin = p->bottomMargin();
        pageProps.isSetPageSizeToPrinter = p->getSetPageSizeToPrinter();
        pageProps.endlessHeight = p->endlessHeight();
        pageProps.heightMM = p->heightMM();
        pageProps.widthMM = p->widthMM();
    }
    return pageProps;
}

bool PreparedPages::readPages(ItemsReaderIntf::Ptr reader)
{
    clear();
    if (reader->first()){
        PageItemDesignIntf::Ptr page = PageItemDesignIntf::create(0);
        if (!reader->readItem(page.data()))
            return false;
        else {
            m_pages->append(page);
            while (reader->next()){
                page = PageItemDesignIntf::create(0);
                if (!reader->readItem(page.data())){
                    m_pages->clear();
                    return false;
                } else {
                    m_pages->append(page);
                }
            }
        }

        return true;
    }
    return false;
}

}
