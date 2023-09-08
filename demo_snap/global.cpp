#include "global.h"
#include "com_sql_queries.h"

QLocale sysLocale = QLocale::system();
QVector<QSqlDatabase *> connections;    // массив указателей на соединения (для установки всем соединениям одинаковых параметров)
QMap<QString, QVariant> *userDbData = new QMap<QString, QVariant>;
QSqlQueryModel *userDbDataModel = new QSqlQueryModel();
QMap<QString, QVariant> *userLocalData = new QMap<QString, QVariant>;
QMap<QString, bool> *permissions = new QMap<QString, bool>;
QMap<QString, QVariant> *comSettings = new QMap<QString, QVariant>;
SStandardItemModel *clientPhoneTypesModel = new SStandardItemModel();
SSqlQueryModel *companiesModel = new SSqlQueryModel;
QSqlQueryModel *officesModel = new QSqlQueryModel;
QSqlQueryModel *warehousesModel = new QSqlQueryModel;
QSqlQueryModel *allUsersModel = new QSqlQueryModel;
QMap<int, QString> *allUsersMap = new QMap<int, QString>;
QSqlQueryModel *usersModel = new QSqlQueryModel;
QSqlQueryModel *managersModel = new QSqlQueryModel;
QSqlQueryModel *engineersModel = new QSqlQueryModel;
SSqlQueryModel *itemBoxesModel = new SSqlQueryModel;
SSqlQueryModel *repairBoxesModel = new SSqlQueryModel;
SSqlQueryModel *paymentSystemsModel = new SSqlQueryModel;
QStandardItemModel* clientsTypesList = new QStandardItemModel;
SSqlQueryModel* clientAdTypesList = new SSqlQueryModel;
QStandardItemModel *statusesModel = new QStandardItemModel();
QStandardItemModel *notifyStatusesModel = new QStandardItemModel();
SStandardItemModel *warrantyTermsModel = new SStandardItemModel();
QMap<int, QString> *warrantyTermsMap = new QMap<int, QString>;

// фактическая высота ComboBox'а будет на 4 пикселя больше; высоту нужно задать в styleSheet, т. к. Qt5 и Qt6 без этого параметра рисуют виджеты чуть-чуть разной высоты
QString commonComboBoxStyleSheet = "QComboBox {  height: 18px; border: 1px solid gray;  padding: 1px 18px 1px 3px;}\
        QComboBox::drop-down {  border: 0px;}\
        QComboBox::down-arrow{  image: url(:/icons/light/down-arrow.png);  width: 16px;  height: 20px;}\
        QComboBox:!editable:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";
QString commonComboBoxStyleSheetRed = "QComboBox {  height: 18px; border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}\
        QComboBox::drop-down {  border: 0px;}\
        QComboBox::down-arrow{  image: url(:/icons/light/down-arrow.png);  width: 16px;  height: 20px;}\
        QComboBox:!editable:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}\
        QComboBox::down-arrow:hover{  border: 1px solid #0078D7;  background-color: #E5F1FB;}";

QString commonLineEditStyleSheet = "QLineEdit {  height: 18px; border: 1px solid gray;  padding: 1px 18px 1px 3px; background: #FFFFFF;}";
QString commonLineEditStyleSheetRed = "QLineEdit {  height: 18px; border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
QString commonTextEditStyleSheet = "";
QString commonTextEditStyleSheetRed = "QTextEdit {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
QString commonDateEditStyleSheet = "";
QString commonDateEditStyleSheetRed = "QDateEdit {  border: 1px solid red;  padding: 1px 18px 1px 3px; background: #FFD1D1;}";
SStandardItemModel *rejectReasonModel = new SStandardItemModel;
SStandardItemModel *priceColModel = new SStandardItemModel;
SStandardItemModel *itemUnitsModel = new SStandardItemModel;

//QWidget *modalWidget = nullptr;

bool readStatuses(QStandardItemModel &model, QJsonArray &jsonArray)
{
    QList<QStandardItem*> *list;

    if (!jsonArray.empty())
    {
        QJsonArray::iterator i = jsonArray.begin();
        do
        {
            QJsonObject jsonObj = i->toObject();
            list = new QList<QStandardItem*>;
            *list << new QStandardItem(jsonObj["Name"].toString())\
                  << new QStandardItem(QString::number(jsonObj["Id"].toInt()))\
                  << new QStandardItem(jsonObj["Color"].toString())\
                  << new QStandardItem(jsonObj["Terms"].toString())\
                  << new QStandardItem(jsonArrayJoin(jsonObj["Contains"],"|"))\
                  << new QStandardItem(jsonArrayJoin(jsonObj["Actions"],"|"))\
                  << new QStandardItem(jsonArrayJoin(jsonObj["Roles"],"|"));
            model.appendRow(*list);
            i++;
        }
        while ( i != jsonArray.end());
    }
    return 1;
}

/* Объединение элементов массива в строку с разделителем "|".
 * Такой формат позволит использовать полученный список в качестве
 * шаблона регулярного выражения (в частности, при переключении статуса ремонта).
 */
QString jsonArrayJoin(QJsonValue value, const QString)
{
    if(value.isArray())
    {
        QString out = "";
        QVariantList array = value.toArray().toVariantList();
        if (!array.empty())
        {
            QVariantList::iterator i = array.begin();
            do
            {
                if (out != "")
                    out += "|";
                out += i->toString();
                i++;
            }
            while ( i != array.end());
        }
        return out;
    }
    return "not an array";
}

void initGlobalModels()
{
    QString query;

    QSqlQuery *queryCommonSettings = new QSqlQuery(QSqlDatabase::database("connMain"));

    queryCommonSettings->exec(QUERY_SEL_COMMON_SETTINGS);
    comSettings = new QMap<QString, QVariant>;
    queryCommonSettings->first();
    // Переписываем результаты запроса в специальный массив
    // это необходимо, т. к. данные общих настроек могут быть дополнены.
    // Кроме того, есть параметры, хранящиеся в AppData и эти настройки превалируют над настройками, сохранёнными в БД.
    // TODO: чтение файла настроек в AppData
    // TODO: чтение файлов настроек пользователя из каталога приложения (например, program files): RepairsGrid-user.xml, SaleGrid-user.xml и многие другие
    for (int i = 0; i < queryCommonSettings->record().count(); i++)
    {
        comSettings->insert(queryCommonSettings->record().fieldName(i), queryCommonSettings->value(i));
    }
    queryCommonSettings->exec(QUERY_SEL_COMMON_SETTINGS2);  // вторая "порция" общих настроек ( в ASC CRM настроки из табил. config начиная с v3.7.18.969 schemaversion 258 постепенно "переезжают" в табл. settings; нова ятаблица имеет приоритет
    while(queryCommonSettings->next())
    {
        comSettings->insert(queryCommonSettings->value(0).toString(), queryCommonSettings->value(1));
    }

    for (int i=0; i < comSettings->value("phone_mask1").toString().size(); i++)     // В ASC CRM (а точнее в DevExpress) маски ввода в lineEdit работают не так, как в Qt
        if (comSettings->value("phone_mask1").toString().at(i) == '0')              // В Qt: 0 - это необязательная цифра, а 9 — обязательная цифра. Чтобы работала проверка
        {                                                                           // введённого номера телефона (если в настройках установлен флаг обязательности), нужно изменить маску.
            qDebug() << "МАСКА ТЕЛЕФОНА 1!!!";                                      // (это заглушка для отладки)
            break;                                                                  // TODO: добавить хитрую подсказку для полей ввода номера, если обнаруживается такое несовпадение
        }
    for (int i=0; i < comSettings->value("phone_mask2").toString().size(); i++)
        if (comSettings->value("phone_mask2").toString().at(i) == '0')
        {
            qDebug() << "МАСКА ТЕЛЕФОНА 2!!!";
            break;
        }
    comSettings->insert("phone_mask1", comSettings->value("phone_mask1").toString().replace('0', '9')); // предупреждалка пусть орёт, но чтобы работала проверка, изменим маску внаглую
    comSettings->insert("phone_mask2", comSettings->value("phone_mask2").toString().replace('0', '9'));

    QList<QStandardItem*> *clientPhoneTypesSelector;
    clientPhoneTypesSelector = new QList<QStandardItem*>();
    *clientPhoneTypesSelector << new QStandardItem("мобильный") << new QStandardItem("1") << new QStandardItem(comSettings->value("phone_mask1").toString()); // в ASC формат задаётся нулями, но в поиске совпадающих клиентов  это предусмотрено
    clientPhoneTypesModel->appendRow( *clientPhoneTypesSelector );
    clientPhoneTypesSelector = new QList<QStandardItem*>();;
    *clientPhoneTypesSelector << new QStandardItem("городской") << new QStandardItem("2") << new QStandardItem(comSettings->value("phone_mask2").toString());
    clientPhoneTypesModel->appendRow( *clientPhoneTypesSelector );

    warehousesModel->setQuery(QUERY_SEL_WAREHOUSES(userDbData->value("current_office").toInt()), QSqlDatabase::database("connMain"));
    allUsersModel->setQuery(QUERY_SEL_ALL_USERS, QSqlDatabase::database("connMain"));
    for(int i = 0; i < allUsersModel->rowCount(); i++)
    {
        allUsersMap->insert(allUsersModel->record(i).value("id").toInt(), allUsersModel->record(i).value("username").toString());
    }
    usersModel->setQuery(QUERY_SEL_USERS, QSqlDatabase::database("connMain"));
    managersModel->setQuery(QUERY_SEL_MANAGERS, QSqlDatabase::database("connMain"));
    engineersModel->setQuery(QUERY_SEL_ENGINEERS, QSqlDatabase::database("connMain"));
    itemBoxesModel->setQuery(QUERY_SEL_ITEM_BOXES(userDbData->value("current_office").toInt()), QSqlDatabase::database("connMain"));
    repairBoxesModel->setQuery(QUERY_SEL_REPAIR_BOXES, QSqlDatabase::database("connMain"));
    paymentSystemsModel->setQuery(QUERY_SEL_PAYMENT_SYSTEMS, QSqlDatabase::database("connMain")); // TODO: нужна прокси-модель для отображения платёжных систем в соответствии с правами пользователя
    clientAdTypesList->setQuery(QUERY_SEL_CLIENT_AD_TYPES, QSqlDatabase::database("connMain"));

    QVector<QString> clientTypesList = {"Все клиенты", "Организации", "Посредники", "Поставщики", "Постоянные клиенты", "Проблемные клиенты", "Реализаторы"};
    QVector<QString> clientTypesQueryFilterList = {"`type` = '%'", "`type` = 1", "`is_agent` = 1", "`is_dealer` = 1", "`is_regular` = 1", "`is_bad` = 1", "`is_realizator` = 1"};
    for (int i=0; i<clientTypesList.size(); i++)
    {
        QList<QStandardItem*> *clientTypeSelector = new QList<QStandardItem*>();
        *clientTypeSelector << new QStandardItem(clientTypesList.at(i)) << new QStandardItem(QString::number(i)) << new QStandardItem(clientTypesQueryFilterList.at(i));
        clientsTypesList->appendRow(*clientTypeSelector);
    }

    QJsonDocument jsonDoc(QJsonDocument::fromJson(comSettings->value("statuses").toByteArray()));
    if (jsonDoc.isArray())
    {
        QJsonArray jsonArray(jsonDoc.array());
        readStatuses(*statusesModel, jsonArray);
    }
    else
    {
        QList<QStandardItem*> *el = new QList<QStandardItem*>({new QStandardItem("Model init failed")});
        statusesModel->appendRow(*el);
    }

    QVector<QString> notifyStatusesList = {"---", "Клиент оповещён", "Клиент не отвечает", "Клиент не доступен", "Не оповещён прочее"};
    for (int i=0; i<notifyStatusesList.size(); i++)
    {
        QList<QStandardItem*> *notifyStatusSelector = new QList<QStandardItem*>();
        *notifyStatusSelector << new QStandardItem(notifyStatusesList.at(i)) << new QStandardItem(QString::number(i));
        notifyStatusesModel->appendRow(*notifyStatusSelector);
    }

    // TODO: В АСЦ в списке сроков гарантии также присутствуют "Согласно гарантии производителя", "Согласно ЗоЗПП" и "Согласно ФГТ", но, похоже, их выбор не реализован
    QVector<QString> warrantyTermsList = {"нет", "7 дней", "14 дней", "1 мес", "2 мес", "3 мес", "4 мес", "6 мес", "1 год", "2 года", "3 года"};
    int warrantyTerms[] = {0, 7, 14, 31, 62, 93, 124, 186, 365, 730, 1095};
    // Модель для комбобоксов при редактировании, а QMap для отображения
    for (int i=0; i<warrantyTermsList.size(); i++)
    {
        QList<QStandardItem*> *warrantyTermSelector = new QList<QStandardItem*>();
        *warrantyTermSelector << new QStandardItem(warrantyTermsList.at(i)) << new QStandardItem(QString::number(warrantyTerms[i]));
        warrantyTermsModel->appendRow(*warrantyTermSelector);
        warrantyTermsMap->insert(warrantyTerms[i], warrantyTermsList.at(i));
    }

    QVector<QString> rejectReasonList = {"отказ от ремонта", "ремонт не возможен", "ремонт не возможен из-за отсутствия запчастей", "ремонт не рентабелен", "неисправносте не проявилась", "другие причины"};
    QList<QStandardItem*> *rejectReasonSelector;
    for (int i=0; i<rejectReasonList.size(); i++)
    {
        rejectReasonSelector = new QList<QStandardItem*>();
        *rejectReasonSelector << new QStandardItem(rejectReasonList.at(i)) << new QStandardItem(QString::number(i));
        rejectReasonModel->appendRow(*rejectReasonSelector);
    }

    QVector<QString> priceColNamesList = {"Цена для сервиса", "Цена розница", "Цена опт", "Цена опт2", "Цена опт3"};
    QVector<QString> priceColIdsList = {"1", "2", "3", "4", "5"};
    QVector<QString> priceColDBFieldsList = {"price", "price2", "price3", "price4", "price5"};
    QList<QStandardItem*> *priceColSelector;
    for (int i=0; i<priceColNamesList.size(); i++)
    {
        priceColSelector = new QList<QStandardItem*>();
        *priceColSelector << new QStandardItem(priceColNamesList.at(i)) << new QStandardItem(priceColIdsList.at(i)) << new QStandardItem(priceColDBFieldsList.at(i));
        priceColModel->appendRow(*priceColSelector);
    }

    QVector<QString> itemUnitsList = {"шт", "г", "м", "см", "л"};
    QVector<QString> itemUnitsIdsList = {"1", "2", "3", "4", "5"};
    QList<QStandardItem*> *itemUnitsSelector;
    for (int i=0; i<itemUnitsList.size(); i++)
    {
        itemUnitsSelector = new QList<QStandardItem*>();
        *itemUnitsSelector << new QStandardItem(itemUnitsList.at(i)) << new QStandardItem(itemUnitsIdsList.at(i));
        itemUnitsModel->appendRow(*itemUnitsSelector);
    }

#ifdef QT_DEBUG
    initClients4Test();
#endif
}

void initUserDbData()
{
    userDbDataModel->setQuery(QUERY_SEL_USER_DATA(QSqlDatabase::database("connMain").userName()), QSqlDatabase::database("connMain"));

    // Переписываем результаты запроса в специальный массив
    // это необходимо, т. к. данные пользователя могут быть дополнены (например, кодом текущего офиса, если у пользователя есть право выбора офиса при входе)
    // Кроме того, есть параметры, хранящиеся в AppData и эти настройки превалируют над настройками, сохранёнными в БД (например, ширины столбцов таблиц, которые могут иметь разные значения в случае если пользователь работает на разных ПК).
    for (int i = 0; i < userDbDataModel->record(0).count(); i++)
    {
        userDbData->insert(userDbDataModel->record(0).fieldName(i), userDbDataModel->record(0).value(i));
    }
}

void initPermissions()
{
    QSqlQuery *queryPermissions = new QSqlQuery(QSqlDatabase::database("connMain"));

    queryPermissions->exec(QUERY_SEL_PERMISSIONS(userDbData->value("roles").toString()));
    while (queryPermissions->next())
    {
        permissions->insert(queryPermissions->value(0).toString(), 1);    // разрешённые пользователю действия хранятся в объекте QMap, не перечисленные действия не разрешены
    }

    delete queryPermissions;
}

void initCompanies()    // Список компаний.
{
    companiesModel->setQuery(QUERY_SEL_COMPANIES, QSqlDatabase::database("connMain"));
}

void initOffices()      // Список офисов
{
    officesModel->setQuery(QUERY_SEL_OFFICES(1), QSqlDatabase::database("connMain"));
}
