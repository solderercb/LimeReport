#ifndef COM_SQL_QUERIES_H
#define COM_SQL_QUERIES_H

#include "appver.h"

#define QUERY_SEL_CLIENT(id)                QString("SELECT  `id`,  `name`,  `surname`,  `patronymic`,  `agent_phone_mask`,  `agent2_phone_mask`,  `address`,  `post_index`,  `type`,  `memorial`,  `notes`,  `prefer_cashless`,  `visit_source`,  `ur_name`,  `email`,  `balance_enable`,  `balance`,  `price_col`,  `repairs`,  `purchases` FROM `clients` WHERE `id` = %1 AND `state` = 1").arg((id))
#define QUERY_SEL_CLIENT_AD_TYPES           QString("SELECT `name`, `id` FROM visit_sources WHERE `enabled` ORDER BY `position` ASC;")
#define QUERY_SEL_CLIENT_MATCH              QString("SELECT t1.`id`, CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`) AS 'FIO', t1.`balance`, t1.`repairs`, t1.`purchases`, IF(t1.`type` = 1, 'Ю', '') AS 'type', GROUP_CONCAT(IFNULL(t2.`phone`, '') ORDER BY t2.`type` DESC, t2.`id` DESC SEPARATOR '\r\n')  AS 'phone' FROM `clients` AS t1 LEFT JOIN `tel` AS t2 ON t1.`id` = t2.`customer` WHERE `state` = 1 %1 GROUP BY t1.`id`;")
#define QUERY_SEL_CLIENT_PHONES(id)         QString("SELECT `phone`, `mask` FROM `tel` WHERE `customer` = " + QString::number((id)) + " ORDER BY `type` DESC, `id` DESC LIMIT 2")
#define QUERY_SEL_USER_STATE(username)      QString("SELECT `id` FROM `users` WHERE `username` = '%1' AND `state` = 1 AND `is_bot` = 0 LIMIT 1;").arg((username))
#define QUERY_SEL_USER_DATA(username)       QString("SELECT `users`.`id`, GROUP_CONCAT(`role_id`) AS 'roles', `name`,  `surname`,  `patronymic`, CONCAT(IF(users.`surname` IS NULL OR users.`surname` = '', '', CONCAT(users.`surname`, ' ')), IF(users.`name` IS NULL OR users.`name` = '', '', CONCAT(LEFT(users.`name`, 1), '. ')), IF(users.`patronymic` IS NULL OR users.`patronymic` = '', '', CONCAT(LEFT(users.`patronymic`, 1), '.'))) AS 'fio', `office`,  `def_office`,  `def_store`,  `def_item_state`,  `def_employee`,  `def_status`,  `def_ws_filter`,  `salary_rate`,  `pay_day`,  `pay_day_off`,  `pay_repair`,  `pay_repair_quick`,  `pay_sale`,  `pay_repair_q_sale`,  `pay_cartridge_refill`,  `pay_device_in`,  `pay_device_out`,  `pay_4_sale_in_repair`,  `row_color`,  `ge_highlight_color`,  `color_label_ws`,  `workspace_mode`,  `preview_before_print`,  `new_rep_doc_copies`,  `auto_refresh_workspace`,  `refresh_time`,  `xls_c1`,  `xls_c2`,  `xls_c3`,  `xls_c4`,  `xls_c5`,  `xls_c6`,  `xls_c7`,  `xls_c8`,  `xls_c9`,  `xls_c10`,  `xls_c11`,  `xls_c12`,  `xls_c13`,  `xls_c14`,  `xls_c15`,  `display_out`,  `display_complete`,  `new_on_top`,  `issued_color`, `fields_cfg`,  `save_state_on_close`,  `group_store_items`,  `track_activity`,  `card_on_call`,  `inn`,  `inform_comment`,  `inform_status`,  `kkt`,  `pinpad`,  `advance_disable`,  `salary_disable`,  `kkm_pass`,  `prefer_regular`,  `fontsize`,  `rowheight`,  `animation` FROM `users` LEFT JOIN `roles_users` ON `users`.`id` = `roles_users`.`user_id` WHERE `username` = '%1' GROUP BY `users`.`id`  LIMIT 1;").arg((username))
#define QUERY_SEL_ASC_SCHEMA_VER            QString("SELECT `scriptname` FROM schemaversions WHERE `scriptname` LIKE 'ASC%' ORDER BY LPAD(REPLACE(`scriptname`, 'ASC.Scripts.Script', ''), 6, '0') DESC;")
#define QUERY_SEL_APP_VER                   QString("SELECT `version_snap` FROM `config` WHERE `id` = 1;")
#define QUERY_SEL_SCHEMA_VER                QString("SELECT REPLACE(`scriptname`, 'SNAP.schema-updates.', '') FROM schemaversions WHERE `scriptname` LIKE 'SNAP%' ORDER BY LPAD(REPLACE(`scriptname`, 'SNAP.schema-updates.script', ''), 6, '0') ASC;")
#define QUERY_SEL_ACTIVE_USERS(db, user)    QString("SELECT GROUP_CONCAT(DISTINCT `USER`) AS 'users' FROM `information_schema`.`PROCESSLIST` WHERE `DB` = '%1' AND `USER` NOT IN ('root', '%2') GROUP BY `DB`;")\
                                                .arg((db))\
                                                .arg((user))
#define QUERY_SEL_GLOB_WAIT_TIMEOUT         QString("SELECT `VARIABLE_VALUE` FROM performance_schema.global_variables WHERE `VARIABLE_NAME` LIKE 'wait_timeout';")
#define QUERY_SEL_ACTIVE_USERS2(timeout)    QString("SELECT GROUP_CONCAT(`username`) FROM users WHERE `last_activity` > DATE_SUB(NOW(), INTERVAL %1 SECOND);").arg(timeout)
#define QUERY_SEL_DOC_TEMPL_CHECKSUM(doc)   QString("SELECT HEX(`checksum`) AS 'checksum' FROM `doc_templates` WHERE `name` = '%1' AND `app` = 'SNAP' LIMIT 1;").arg(doc)
#define QUERY_SEL_DOC_TEMPL_DATA(doc)       QString("SELECT `data` FROM `doc_templates` WHERE `name` = '%1' AND `app` = 'SNAP' LIMIT 1;").arg(doc)
#define QUERY_SEL_PERMISSIONS(roles)        QString("SELECT `permission_id` FROM `permissions_roles` WHERE `role_id` IN (%1)").arg((roles))
#define QUERY_SEL_COMMON_SETTINGS           QString("SELECT  `id`,  `default_works_warranty`,  `default_items_warranty`,  `currency`,  `configcol`,  `email_config`,  `qs_print_pko`,  `qs_print_rko`,  `qs_print_pn`,  `qs_print_rn`,  `print_rep_stickers`,  `rep_stickers_copy`,  `rep_auto_company`,  `config_modified`,  `close_clients`,  `aster_host`,  `aster_port`,  `aster_login`,  `aster_password`,  `aster_web_port`,  `time_zone`,  `realizator_enable`,  `online_store`,  `online_store_api`,  `online_store_key`,  `classic_kassa`,  `default_items_used_warranty`,  `default_items_rep_warranty`,  `default_items_razb_warranty`,  `default_items_other_warranty`,  `images_width`,  `item_img_limit`,  `rep_img_limit`,  `default_reserve_days`,  `it_vis_opt`,  `it_vis_opt2`,  `it_vis_opt3`,  `it_vis_rozn`,  `it_vis_price_for_sc`,  `give2user_time`,  `diag_accept_time`,  `salary_day`,  `parts_included`,  `visit_source_force`,  `is_patronymic_required`,  `is_photo_required`,  `is_photo_out_req`,  `is_sn_req`,  `is_cartridge_sn_req`,  `is_reason_req`,  `address_required`,  `email_required`,  `phone_required`,  `diag_required`,  `is_master_set_on_new`,  `print_warranty`,  `print_works`,  `print_diagnostic`,  `print_reject`,  `print_check`,  `rko_on_pn`,  `card_close_time`,  `phone_mask1`,  `phone_mask2`,  `auto_switch_layout`,  `rep_price_by_manager`,  `video_codec`,  `print_new_repair_report`,  `statuses`,  `sms_config`,  `debt_rep_2_salary`,  `exchange_type`,  `exchange_source`,  `exchange_rate`,  `history_by_sn`,  `clients_are_dealers`,  `regular_customer_step`,  `backup_enable`,  `backup_images`,  `backup_time`,  `manual_maker`,  `TimeOfWork`,  `cartridge_enable`,  `vw_enable`,  `termsControl`,  `version` FROM `config` WHERE `id` = 1;")
#define QUERY_SEL_COMMON_SETTINGS2          QString("SELECT  `name`,  `value` FROM `settings`")
#define QUERY_SEL_COMPANIES                 QString("SELECT  `name`,  `id`,  `type`,  `inn`,  `kpp`,  `ogrn`,  `ur_address`,  `site`,  `email`,  `logo`,  `banks`,  `is_default`,  `status`,  `director`,  `accountant`,  `tax_form`,  `description` FROM `companies` WHERE `id` = 1;")
#define QUERY_SEL_OFFICES(id)               QString("SELECT  t1.`name`, CONCAT(t1.`name`, ' (', t2.`name`, ')') AS 'name_full',  t1.`id`,  t1.`state`,  `address`,  `phone`,  t1.`logo` AS 'logo',  t2.`logo` AS 'companyLogo',  `administrator`,  `created`,  `phone2`,  `default_company`,  `card_payment`,  `use_boxes`,  `paint_repairs`,  `warranty_sn` FROM `offices` AS t1 LEFT JOIN `companies` AS t2 ON t1.`default_company` = t2.`id` WHERE t1.`state` = 1 AND `default_company`  = %1;").arg((id))
#define QUERY_SEL_WAREHOUSES(office)        QString("SELECT `name`, `id`, `type`, `description`, `sub_type`, `it_vis_pn`, `it_vis_notes`, `it_vis_description`, `it_vis_sn`, `it_vis_barcode`, `it_vis_photo`, `it_vis_warranty`, `it_vis_warranty_dealer`, `active`FROM `stores` WHERE `office` = %1;").arg((office))
#define QUERY_SEL_ALL_USERS                 QString("SELECT `username`, `id`,  `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` WHERE `is_bot` = 0;")
#define QUERY_SEL_USERS                     QString("SELECT `username`, `id`,  `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` WHERE `state` = 1 AND  `is_bot` = 0;")
#define QUERY_SEL_MANAGERS                  QString("SELECT `username`, t1.`id`, `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` AS t1 LEFT JOIN `roles_users` AS t2 ON t1.`id` = t2.`user_id` WHERE t1.`state` = 1 AND t2.role_id IN (3, 6) GROUP BY t1.`id`;")
#define QUERY_SEL_ENGINEERS                 QString("SELECT `username`, t1.`id`, `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` AS t1 LEFT JOIN `roles_users` AS t2 ON t1.`id` = t2.`user_id` WHERE t1.`state` = 1 AND t2.role_id IN (2, 5) GROUP BY t1.`id`;")
#define QUERY_SEL_ITEM_BOXES(warehouse)     QString("SELECT `name`, `id`, `places`, `color` FROM `boxes` WHERE `store_id` = %1 AND `non_items` = 0 ORDER BY `name`;").arg((warehouse))
#define QUERY_SEL_REPAIR_BOXES              QString("SELECT `name`, `id`,`places`, `color` FROM `boxes` WHERE `non_items` = 1 ORDER BY `name`;")
#define QUERY_SEL_PAYMENT_SYSTEMS           QString("SELECT `name`, `system_id`, `system_data`, `id` FROM `payment_systems` WHERE `is_enable` = 1 ORDER BY `name`;")
#define QUERY_SEL_DEVICE(id)                QString("SELECT  `id`,  `type`,  `maker`,  `model`, `serial_number`,  `client` FROM `workshop` WHERE `id` = %1").arg((id))
#define QUERY_SEL_DEVICES                   QString("SELECT `name`, `id`, `company_list` FROM `devices` WHERE `enable` = 1 AND `refill` = 0 ORDER BY `position`;")
#define QUERY_SEL_DEVICE_MAKERS(device)     QString("SELECT `name`, `id` FROM `device_makers` WHERE `id` IN (%1);").arg((device))
#define QUERY_SEL_DEVICE_MODELS             QString("SELECT `name`, `id` FROM `device_models` WHERE `device` = %1 AND `maker` = %2;")
#define QUERY_SEL_DEVICE_FAULTS(device)     QString(\
                                                "SELECT '' AS 'name'\n"\
                                                "UNION ALL\n"\
                                                "(SELECT\n"\
                                                "  TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`fault_list`,',',enumerator.`number`),',',1)) AS 'name'\n"\
                                                "FROM (SELECT `fault_list` FROM `devices` WHERE `id` = %1) AS t1\n"\
                                                "JOIN enumerator\n"\
                                                "ON\n"\
                                                "  (LENGTH(REPLACE(t1.`fault_list`, ',', ''))-LENGTH(t1.`fault_list`) <= enumerator.`number`+1));")\
                                                .arg((device))

#define QUERY_SEL_DEVICE_SET(device)        QString(\
                                                "SELECT"\
                                                "  '' AS 'name'\n"\
                                                "UNION ALL\n"\
                                                "(SELECT\n"\
                                                "  TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`complect_list`,',',enumerator.`number`),',',1)) AS 'name'\n"\
                                                "FROM (SELECT `complect_list` FROM `devices` WHERE `id` = %1) AS t1\n"\
                                                "JOIN enumerator\n"\
                                                "ON\n"\
                                                "  (LENGTH(REPLACE(t1.`complect_list`, ',', ''))-LENGTH(t1.`complect_list`) <= enumerator.`number`+1));")\
                                                .arg((device))
                                                
#define QUERY_SEL_DEVICE_EXTERIOR(device)   QString(\
                                                "SELECT '' AS 'name'\n"\
                                                "UNION ALL\n"\
                                                "(SELECT\n"\
                                                "  TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`look_list`,',',enumerator.`number`),',',1)) AS 'name'\n"\
                                                "FROM (SELECT `look_list` FROM `devices` WHERE `id` = %1) AS t1\n"\
                                                "JOIN enumerator\n"\
                                                "ON\n"\
                                                "  (LENGTH(REPLACE(t1.`look_list`, ',', ''))-LENGTH(t1.`look_list`) <= enumerator.`number`+1));\n")\
                                                .arg((device))

#define QUERY_SEL_DEVICE_ADD_FIELDS(device) QString("SELECT `name`, REPLACE(`def_values`,'\r','') AS 'def_values', `type`, `id`,  `required`,  `printable`  FROM `fields` WHERE `_f` = 0 AND FIND_IN_SET(%1,`devices`) AND `archive` = 0 ORDER BY `id`;").arg((device))
#define QUERY_SEL_DEVICE_MATCH(text)        QString(\
                                                "SELECT\n"\
                                                "  t1.`id`,\n"\
                                                "  CONCAT_WS(' ', t2.`name`,  t3.`name`,  t4.`name`) AS 'device',\n"\
                                                "  `fault`,\n"\
                                                "  `serial_number`,\n"\
                                                "  CONCAT_WS(' ', t5.surname, t5.name, t5.patronymic) AS 'client',\n"\
                                                "  t1.`type`,\n"\
                                                "  t1.`maker`,\n"\
                                                "  t1.`model`,\n"\
                                                "  t1.`client` AS 'client_id'\n"\
                                                "FROM `workshop` AS t1 LEFT JOIN `devices` AS t2 ON t1.`type` = t2.`id` LEFT JOIN `device_makers` AS t3 ON t1.maker = t3.`id` LEFT JOIN `device_models` AS t4 ON t1.model = t4.`id` LEFT JOIN `clients` AS t5 ON t1.`client` = t5.`id` WHERE LCASE(`serial_number`) REGEXP LCASE('%1') ORDER BY `id` DESC;")\
                                                .arg((text))

#define QUERY_SEL_CLIENTS_STATIC            QString(\
                                                "SELECT\n"\
                                                "  t1.`id`,\n"\
                                                "  CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`) AS 'FIO',\n"\
                                                "  t1.`balance`,\n"\
                                                "  t1.`repairs`,\n"\
                                                "  t1.`purchases`,\n"\
                                                "  IF(t1.`type` = 1, 'Ю', '') AS 'type',\n"\
                                                "  IFNULL(t2.`phone`, '') AS 'phone'\n"\
                                                "FROM `clients` AS t1\n"\
                                                "LEFT JOIN `tel` AS t2\n"\
                                                "ON t1.`id` = t2.`customer`\n"\
                                                "  AND t2.`type` = 1")   // Здесь не должно быть точки с запятой!

#define QUERY_SEL_WORKSHOP_STATIC           QString(\
                                                "SELECT\n"\
                                                "   CONCAT_WS(' ', IF(`is_warranty`, 'Г', ''), IF(`is_repeat`, 'П', ''), IF(`express_repair`, 'С', ''), IF(`informed_status`, '*', '')) AS 'marks',\n"\
                                                "   t1.`id`,\n"\
                                                "   `Title`,\n"\
                                                "   CONCAT_WS(' ', t5.surname, t5.name, t5.patronymic) AS 'client',\n"\
                                                "   CONCAT_WS(' ', t2.`name`,  t3.`name`,  t4.`name`) AS 'device',\n"\
                                                "   `serial_number`,\n"\
                                                "   `office`,\n"\
                                                "   `manager`,\n"\
                                                "   `master`,\n"\
                                                "   `in_date`,\n"\
                                                "   `out_date`,\n"\
                                                "   t1.`state` AS 'status',\n"\
                                                "   t1.`state`,\n"\
                                                "   `new_state`,\n"\
                                                "   `user_lock`,\n"\
                                                "   `lock_datetime`,\n"\
                                                "   `quick_repair`,\n"\
                                                "   `box`,\n"\
                                                "   `repair_cost`,\n"\
                                                "   `fault`,\n"\
                                                "   `thirs_party_sc`,\n"\
                                                "   `last_save`,\n"\
                                                "   `last_status_changed`,\n"\
                                                "   `warranty_days`,\n"\
                                                "   `color`,\n"\
                                                "   `early`,\n"\
                                                "   `ext_early`,\n"\
                                                "   `issued_msg`,\n"\
                                                "   `vendor_id`,\n"\
                                                "   `termsControl`,\n"\
                                                "   `Hidden`,\n"\
                                                "   `company`\n"\
                                                "FROM `workshop` AS t1\n"\
                                                "LEFT JOIN `devices` AS t2\n"\
                                                "   ON t1.`type` = t2.`id`\n"\
                                                "LEFT JOIN `device_makers` AS t3\n"\
                                                "   ON t1.maker = t3.`id`\n"\
                                                "LEFT JOIN `device_models` AS t4\n"\
                                                "   ON t1.model = t4.`id`\n"\
                                                "LEFT JOIN `clients` AS t5\n"\
                                                "   ON t1.`client` = t5.`id`")   // Здесь не должно быть точки с запятой!

#define QUERY_SEL_REPAIR_DATA(id)           QString(\
                                                "SELECT\n"\
                                                "  t2.PrepaidTypeStr,\n"\
                                                "  workshop.*\n"\
                                                "FROM\n"\
                                                "  workshop\n"\
                                                "LEFT JOIN (\n"\
                                                "  SELECT\n"\
                                                "    \"полная предоплата\" AS 'PrepaidTypeStr',\n"\
                                                "    0 AS 'id'\n"\
                                                "  UNION\n"\
                                                "  SELECT\n"\
                                                "    \"за детали\",\n"\
                                                "    1\n"\
                                                "  UNION\n"\
                                                "  SELECT\n"\
                                                "    \"за часть стоимости деталей\",\n"\
                                                "    2\n"\
                                                "  UNION\n"\
                                                "  SELECT\n"\
                                                "    \"за часть стоимости работ\",\n"\
                                                "    3\n"\
                                                "  UNION\n"\
                                                "  SELECT\n"\
                                                "    \"за диагностику\",\n"\
                                                "    4\n"\
                                                "  ) AS `t2`\n"\
                                                "ON\n"\
                                                "  workshop.prepaid_type = t2.`id`\n"\
                                                "  AND workshop.`is_prepaid` = 1\n"\
                                                "WHERE\n"\
                                                "  workshop.`id` = %1;")\
                                                .arg((id))

#define QUERY_SEL_REPAIR_COMMENTS(id)       QString("SELECT `created`, `user`, `text` FROM `comments` WHERE `remont` = %1 ORDER BY `id` DESC;").arg((id)), QSqlDatabase::database("connMain")

#define QUERY_SEL_REPAIR_WORKS_AND_PARTS(id) QString(\
                                                "SELECT\n"\
                                                "  'X  +' AS '*',\n"\
                                                "  t1.`name`,\n"\
                                                "  t1.`count`,\n"\
                                                "  ROUND(t1.`price`, 2) AS 'price',\n"\
                                                "  t1.`count`*ROUND(t1.`price`, 2) AS 'summ',\n"\
                                                "  t1.`warranty`,\n"\
                                                "  t1.`user`,\n"\
                                                "  '' AS 'item_sn',\n"\
                                                "  t1.`id`,\n"\
                                                "  t1.`id` AS 'work_id',\n"\
                                                "  '' AS 'item_rsrv_id'\n"\
                                                "FROM\n"\
                                                "  `works` AS t1\n"\
                                                "WHERE\n"\
                                                "  `repair` = %1\n"\
                                                "UNION ALL\n"\
                                                "SELECT\n"\
                                                "  '   X',\n"\
                                                "  t2.`name`,\n"\
                                                "  t2.`count`,\n"\
                                                "  ROUND(t2.`price`, 2),\n"\
                                                "  t2.`count`*ROUND(t2.`price`, 2),\n"\
                                                "  t2.`warranty`,\n"\
                                                "  t2.`to_user`,\n"\
                                                "  t2.`sn`,\n"\
                                                "  t2.`id`,\n"\
                                                "  t2.`work_id`,\n"\
                                                "  t2.`id`\n"\
                                                "FROM\n"\
                                                "  `store_int_reserve` AS t2\n"\
                                                "WHERE\n"\
                                                "  t2.work_id IN (SELECT `id` FROM `works` WHERE `repair` = %1)\n"\
                                                "ORDER BY\n"\
                                                "  `work_id` ASC,\n"\
                                                "  `item_rsrv_id` ASC;")\
                                                .arg((id)), QSqlDatabase::database("connMain")

#define QUERY_SEL_REPAIR_PREPAYS(R)         QString("SELECT IFNULL(SUM(`summa`), 0) AS 'summa' FROM `cash_orders` WHERE `repair` = %1;").arg((R))
#define QUERY_SEL_REPAIR_WORKS(R)           QString("SELECT SUM(`price` * `count`) AS 'summa' FROM `works` WHERE `repair` = %1 GROUP BY `repair`;").arg((R))
#define QUERY_SEL_REPAIR_PARTS(R)           QString("SELECT SUM(`price` * `count`) AS `summa` FROM `store_int_reserve` WHERE `state` IN (2, 3) AND `repair_id` = %1;").arg((R))
#define QUERY_SEL_REPAIR_ADD_FIELDS(R)      QString("SELECT t2.`name`, t1.`value`, '' AS 'comment' FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t1.`repair_id` = %1 ORDER BY t1.`field_id` ASC;").arg((R)), QSqlDatabase::database("connMain")
#define QUERY_SEL_IS_BALANCE_EN(C)          QString("SELECT SUM(`summ`) AS 'summa' FROM `balance` WHERE `client` = %1;").arg((C))
#define QUERY_SEL_DOC(id)                   QString("SELECT  `id`,  `type`,  `state`,  `is_realization`,  `payment_system`,  `company`,  `store`,  `user`,  `total`,  `notes`,  `created`,  `updated_at`,  `office`,  `dealer`,  `currency_rate`,  `reason`,  `order_id`,  `price_option`,  `return_percent`,  `reserve_days`, DiffDays(UTC_TIMESTAMP(), `created`) AS 'diff_days', `master_id`,  `repair_id`,  `works_included`,  `invoice`,  `track`,  `d_store`,  `d_pay` FROM `docs` WHERE `id` = %1;").arg((id))
#define QUERY_SEL_ITEMS_IN_DOC(id)          QString("SELECT t1.`id`, CONCAT(LPAD(t2.articul, 6, '0'), '-', LPAD(t2.id, 6, '0')) AS 'UID', t2.`name`, t1.`count`, t2.`count` - t2.`reserved` AS 'avail', t1.`price`, t1.`count`*t1.`price` AS 'summ', t2.`box`, t1.`sn`, t1.`warranty`, t2.`is_realization`, t2.`return_percent`, t1.`is_cancellation`, t1.`cancellation_reason`, t2.`id` AS 'item_id', t2.`in_price`, t2.`dealer` FROM store_sales AS t1 LEFT JOIN store_items AS t2 ON t1.`item_id`=t2.id WHERE (`document_id` = %1);").arg((id))
#define QUERY_SEL_ITEMS_IN_DOC_RSRV(id)     QString("SELECT t1.`id`, CONCAT(LPAD(t2.articul, 6, '0'), '-', LPAD(t2.id, 6, '0')) AS 'UID', t2.`name`, t1.`count`, t2.`count` - t2.`reserved` + t1.`count` AS 'avail', t1.`price`, t1.`count`*t1.`price` AS 'summ', t2.`box`, t1.`sn`, t1.`warranty`, t2.`is_realization`, t2.`return_percent`, t1.`is_cancellation`, t1.`cancellation_reason`, t2.`id` AS 'item_id', t2.`in_price`, t2.`dealer` FROM store_sales AS t1 LEFT JOIN store_items AS t2 ON t1.`item_id`=t2.id WHERE (`document_id` = %1);").arg((id))

/**************** SELECT queries for data models for reports ******************/
#define QUERY_SEL_REPAIR_RPRT(R)            QString("SELECT t2.PrepaidTypeStr, workshop.* FROM workshop LEFT JOIN (SELECT \"полная предоплата\" AS 'PrepaidTypeStr', 0 AS 'id' UNION SELECT \"за детали\", 1 UNION SELECT \"за часть стоимости деталей\", 2 UNION SELECT \"за часть стоимости работ\", 3 UNION SELECT \"за диагностику\", 4 ) AS `t2` ON workshop.prepaid_type = t2.`id` AND workshop.`is_prepaid` = 1 WHERE workshop.`id` = %1;").arg(R), QSqlDatabase::database("connMain")
#define QUERY_SEL_CLIENT_RPRT(C)            QString(\
                                                "SELECT\r\n"\
                                                "  t1.*,\r\n"\
                                                "  GROUP_CONCAT(t2.phone ORDER BY t2.`type` DESC SEPARATOR ',') AS 'phone',\r\n"\
                                                "  IF( t1.`type` = 1, t1.`ur_name`,\r\n"\
                                                "    CONCAT(\r\n"\
                                                "      IF(t1.`surname` IS NULL OR t1.`surname` = '',\r\n"\
                                                "        '',\r\n"\
                                                "        CONCAT(t1.`surname`,' ')\r\n"\
                                                "      ),\r\n"\
                                                "      t1.`name`,\r\n"\
                                                "      IF(t1.`patronymic` IS NULL OR t1.`patronymic` = '',\r\n"\
                                                "        '',\r\n"\
                                                "        CONCAT(' ', t1.`patronymic`)\r\n"\
                                                "      )\r\n"\
                                                "    )\r\n"\
                                                "  ) AS 'FioOrUrName'\r\n"\
                                                "FROM clients AS t1\r\n"\
                                                "LEFT JOIN tel AS t2\r\n"\
                                                "  ON t1.id = t2.customer\r\n"\
                                                "WHERE t1.`id` = %1\r\n"\
                                                "GROUP BY t1.`id`;").arg(C), QSqlDatabase::database("connMain")

#define QUERY_SEL_REP_FIELDS_RPRT(R)        QString("(SELECT 'Предоплата' AS 'name', CONCAT(ROUND(`prepaid_summ`, 0), '$D{config.currency}') AS 'value', `prepaid_type` AS 'comment' FROM workshop WHERE `id` = %1 AND `is_prepaid` = 1)\
                                                      UNION ALL\
                                                      (SELECT 'Ориентировочная стоимость ремонта', CONCAT(ROUND(`pre_agreed_amount`, 0), '$D{config.currency}'), '' FROM workshop WHERE `id` = %1 AND `is_pre_agreed` = 1)\
                                                      UNION ALL\
                                                      (SELECT '', 'Уст-во было в другом СЦ', '' FROM workshop WHERE `id` = %1 AND `thirs_party_sc` = 1)\
                                                      UNION ALL\
                                                      (SELECT t2.`name`, t1.`value`, '' AS 'comment' FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t2.`printable` = 1 AND t2.`archive` = 0 AND t1.`repair_id` = %1 ORDER BY t1.`field_id` ASC);\
                                                      ").arg((R)), QSqlDatabase::database("connMain")

//#define QUERY_SEL__RPRT()  QString("").arg(()), QSqlDatabase::database("connMain")
//#define QUERY_SEL__RPRT()  QString("").arg(()), QSqlDatabase::database("connMain")
//#define QUERY_SEL__RPRT()  QString("").arg(()), QSqlDatabase::database("connMain")
//#define QUERY_SEL__RPRT()  QString("").arg(()), QSqlDatabase::database("connMain")

/**************** Queries for data insertion ******************/
#define QUERY_SEL_LAST_INSERT_ID            QString("SELECT LAST_INSERT_ID();")

// ACHTUNG! ACHTUNG! если в выражениях ниже не закомментирован вызов функции qDebug(), а выражение используется в теле условного оператора БЕЗ ЗАКЛЮЧЕНИЯ в {}, то работа программы будет неправильной
#define QUERY_EXEC(obj,flag)                /*qDebug() << "(!!!) nDBErr=" << (flag) << ". Executing query...";*/ if ((flag)) (flag) = (obj)->exec

// верификация записанных в БД данных; производится примитивно, путём сравнения записанных данных с данными, которые были переданы в запросе;
// если данные совпадают, то сервер должен вернуть 55AAh (21930d), иначе 00h
// далее, если контрольное число совпадает, flag устанавливаем в 1, иначе — 0.
#define QUERY_EXEC_VRFY(obj,flag)           /*qDebug() << "(!!!) nDBErr=" << (flag) << ". Checking integrity...";*/ if ((flag)){\
                                                (obj)->first();\
                                                /*qDebug() << "Integrity magic number: " << (obj)->value(0).toInt();*/\
                                                (flag) = ((obj)->value(0).toInt() == 21930)?1:0;}

// QSqlQuery::lastInsertId() почему-то не работает, поэтому так:
#define QUERY_LAST_INS_ID(obj,flag,id)      /*qDebug() << "(!!!) nDBErr=" << (flag) << ". Executing SELECT LAST_INSERT_ID() query...";*/ if ((flag)){\
                                                (flag) = (obj)->exec("SELECT LAST_INSERT_ID();");\
                                                if ((flag)){(obj)->first();\
                                                    (id) = (obj)->value(0).toInt();}}
#define QUERY_BEGIN                         QString("BEGIN;")
#define QUERY_COMMIT                        QString("COMMIT;")
#define QUERY_ROLLBACK                      QString("ROLLBACK;")
#define QUERY_COMMIT_ROLLBACK(obj,flag)     if (flag == 0)   /* в случае ошибки выполнения запроса нужно всё откатить */\
                                            {\
                                                qDebug() << QString("Ошибка выполнения запроса: %1").arg(obj->lastError().text());\
                                                QMessageBox msgBox;\
                                                msgBox.setText(QString("Ошибка выполнения запроса\r\n%1").arg(obj->lastError().text()));\
                                                obj->exec(QUERY_ROLLBACK);\
                                                msgBox.exec();\
                                            }\
                                            else\
                                            {\
                                                obj->exec(QUERY_COMMIT);\
                                            }

#define QUERY_INS_DEVICE_MODEL              QString("INSERT INTO `device_models` (`name`, `position`, `maker`, `device`) VALUES ('%1', %2, %3, %4);")
#define QUERY_INS_CLIENT                    QString("INSERT INTO `clients` (`creator`, `name`, `surname`, `patronymic`, `agent_phone_mask`, `agent2_phone_mask`, `address`, `post_index`, `passport_num`, `passport_date`, `passport_organ`, `state`, `type`, `birthday`, `memorial`, `notes`, `is_regular`, `is_dealer`, `balance_enable`, `prefer_cashless`, `take_long`, `ignore_calls`, `is_bad`, `is_realizator`, `is_agent`, `visit_source`, `photo_id`, `INN`, `KPP`, `OGRN`, `web_password`, `ur_name`, `email`, `icq`, `skype`, `viber`, `telegram`, `site`, `whatsapp`, `agent_name`, `agent_surname`, `agent_patronymic`, `agent_phone`, `agent_phone_clean`, `agent2_name`, `agent2_surname`, `agent2_patronymic`, `agent2_phone`, `agent2_phone_clean`, `created`, `balance`, `price_col`, `repairs`, `purchases`, `token`) VALUES \n"\
                                                                            "(%1, '%2', '%3', '%4', 1, 1, '%5', NULL, NULL, NULL, NULL, 1, %6, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, %7, NULL, NULL, NULL, NULL, '%8', '', '%9', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, UTC_TIMESTAMP(), 0, 0, 0, 0, NULL);")
#define QUERY_INS_PHONE(P,p,M,C,T,t)        QString(\
                                                "INSERT INTO `tel` ("\
                                                "`phone`, `phone_clean`, `mask`, `customer`, `type`, `note`"\
                                                ") VALUES ("\
                                                "'%1','%2',%3,%4,%5,'%6');")\
                                                .arg(P).arg(p).arg(M).arg(C).arg(T).arg(t)
#define QUERY_INS_USER_ACTIVITY(action)     QString("INSERT INTO `users_activity` (`user_id`, `datetime_`, `address`, `notes`, `app_version`, `machine_name`) VALUES (%1, UTC_TIMESTAMP(), SUBSTRING_INDEX(USER(), '@', -1), '%2', '%3', '%4');").arg(userDbData->value("id").toInt()).arg((action)).arg(APP_VER).arg("TODO:hostname")
#define QUERY_UPD_LAST_USER_LOGIN(id)       QString("UPDATE `users` SET `last_login`=UTC_TIMESTAMP() WHERE `id` = %1;").arg((id))
#define QUERY_UPD_LAST_USER_ACTIVITY(id)    QString("UPDATE `users` SET `last_activity`=UTC_TIMESTAMP() WHERE `id` = %1;").arg((id))
#define QUERY_INS_WORKSHOP                  QString(\
                                                "INSERT INTO `workshop` (\n"\
                                                "`Hidden`, `Title`, `client`, `type`, `maker`, `model`, `serial_number`, `company`, `office`, `start_office`, `manager`, `current_manager`, `master`, `diagnostic_result`, `in_date`, `out_date`, `state`, `new_state`, `user_lock`, `lock_datetime`, `express_repair`, `quick_repair`, `is_warranty`, `is_repeat`, `payment_system`, `is_card_payment`, `can_format`, `print_check`, `box`, `warranty_label`, `ext_notes`, `is_prepaid`, `prepaid_type`, `prepaid_summ`, `prepaid_order`, `is_pre_agreed`, `is_debt`, `pre_agreed_amount`, `repair_cost`, `real_repair_cost`, `parts_cost`, `fault`, `complect`, `look`, `thirs_party_sc`, `last_save`, `last_status_changed`, `warranty_days`, `barcode`, `reject_reason`, `informed_status`, `image_ids`, `color`, `order_moving`, `early`, `ext_early`, `issued_msg`, `sms_inform`, `invoice`, `cartridge`, `vendor_id`, `termsControl`\n"\
                                                ") VALUES (\n"\
                                                "0, '%1', %2, %3, %4, %5, '%6', %7, %8, %8, %9, %9, %10, '', UTC_TIMESTAMP(), NULL, 0, 0, NULL, NULL, %11, %12, %13, %14, %15, %16, %17, %18, %19, NULL, '%20', %21, %22, %23, NULL, %24, 0, %25, 0, 0, 0, '%26', '%27', '%28', %29, UTC_TIMESTAMP(), NULL, 0, '%30', '', 0, '', NULL, NULL, %31, %32, NULL, 1, NULL, NULL, NULL, 1\n);")
#define QUERY_INS_REPAIR_COMMENT            QString(\
                                                "INSERT INTO `comments` (\n"\
                                                "`text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`\n"\
                                                ") VALUES (\n"\
                                                "'%1', UTC_TIMESTAMP(), %2, %3, NULL, NULL, NULL);")

// запись в журнал действий: Устройство принято в ремонт, Деталь добавлена в ремонт, Деталь продана в ремонте; UOCRI = user,office,client,repair,item, т. е. связанные ключи других таблиц
#define QUERY_INS_LOG(G,T,U,O,C,R,I,D,B,t)  QString(\
                                                "INSERT INTO `logs` ("\
                                                "`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`"\
                                                ") VALUES (\n"\
                                                "%1, %2, 0, %3, UTC_TIMESTAMP(), NULL, NULL, %4, %5, %6, %7, %8, NULL, %9, '%10'"\
                                                ");")\
                                                .arg(G)\
                                                .arg(T)\
                                                .arg(U)\
                                                .arg(O)\
                                                .arg(C)\
                                                .arg(R)\
                                                .arg(I)\
                                                .arg(D)\
                                                .arg(B)\
                                                .arg(t)
#define QUERY_INS_CASH(T,S,s,C,U,O,t,R,D,A)   QString(\
                                                "INSERT INTO `cash_orders` ("\
                                                "`created`, `type`, `summa`, `summa_str`, `invoice`, `client`, `to_user`, `user`, `company`, `office`, `notes`, `repair`, `document`, `img`, `payment_system`, `card_fee`, `is_backdate`, `card_info`, `customer_email`, `fdn`, `payment_item_sign`"\
                                                ") VALUES (\n"\
                                                "UTC_TIMESTAMP(), %1, %2, '%3', NULL, %4, NULL, %5, 1, %6, '%7', %8, %9, NULL, %10, 0, 0, NULL, '', NULL, NULL"\
                                                ");")\
                                                .arg(T)\
                                                .arg(S)\
                                                .arg(s)\
                                                .arg(C)\
                                                .arg(U)\
                                                .arg(O)\
                                                .arg(t)\
                                                .arg(R)\
                                                .arg(D)\
                                                .arg(A)
#define QUERY_VRFY_CASH(T,S,s,C,U,O,t,R,D,A)  QString(\
                                                "SELECT IF(`type` = %1 AND `summa` = %2 /*AND `summa_str` = '%3'*/ AND IFNULL(`client`,'NULL') = %4 AND `user` = %5 AND `office` = %6 /*AND `notes` = '%7'*/ AND IFNULL(`repair`,'NULL') = %8 AND IFNULL(`document`,'NULL') = %9 AND `payment_system` = %10, 21930, 0)\n"\
                                                "FROM `cash_orders`\n"\
                                                "WHERE `id` = LAST_INSERT_ID();")\
                                                .arg(T)\
                                                .arg(S)\
                                                .arg(s)\
                                                .arg(C)\
                                                .arg(U)\
                                                .arg(O)\
                                                .arg(t)\
                                                .arg(R)\
                                                .arg(D)\
                                                .arg(A)

#define QUERY_INS_FIELDS(T, R, I, t)        QString(\
                                                "INSERT INTO `field_values` (`field_id`, `repair_id`, `item_id`, `value`) VALUES (%1, %2, %3, '%4');")\
                                                .arg(T)\
                                                .arg(R)\
                                                .arg(I)\
                                                .arg(t)

#define QUERY_LOCK_REPAIR(R,U)              QString("UPDATE `workshop` SET `user_lock`=%1, `lock_datetime`=UTC_TIMESTAMP() WHERE `id` = %2;").arg((U)).arg((R))

#define QUERY_UPD_REPAIR_STATE(R, S)        QString("UPDATE `workshop` SET `out_date`=UTC_TIMESTAMP(), `state`=%1, `lock_datetime`=UTC_TIMESTAMP(), `box`=NULL, `last_status_changed`=UTC_TIMESTAMP() WHERE `id` = %2;").arg(S)\
                                                .arg(R)

#define QUERY_SET_REJ_REASON(R, t)          QString("UPDATE `workshop` SET `reject_reason` = '%2' WHERE `id` = %1;").arg((R)).arg((t))

#define QUERY_INS_WRKSHP_ISSUED(R, U)       QString("INSERT INTO `workshop_issued`(`repair_id`, `employee_id`, `created_at`) VALUES (%1, %2, UTC_TIMESTAMP());").arg((R))\
                                                .arg((U))

#define QUERY_INS_REPAIR_STATUS_LOG(R,S,U,Mngr,Mstr)  QString("INSERT INTO `repair_status_logs`(`created_at`, `repair_id`, `status_id`, `user_id`, `manager_id`, `master_id`) VALUES (UTC_TIMESTAMP(), %1, %2, %3, %4, %5);").arg((R)).arg((S)).arg((U)).arg((Mngr)).arg((Mstr))

#define QUERY_INS_BALANCE_LOG(C,S,D,t,O,U)  QString(\
                                                "INSERT INTO\n"\
                                                "  `balance` (\n"\
                                                "    `client`,\n"\
                                                "    `summ`,\n"\
                                                "    `direction`,\n"\
                                                "    `reason`,\n"\
                                                "    `created`,\n"\
                                                "    `office`,\n"\
                                                "    `uid`,\n"\
                                                "    `dealer_payment`\n"\
                                                "  )\n"\
                                                "VALUES (\n"\
                                                "    %1,\n"\
                                                "    %2,\n"\
                                                "    %3,\n"\
                                                "    '%4',\n"\
                                                "    UTC_TIMESTAMP(),\n"\
                                                "    %5,\n"\
                                                "    %6,\n"\
                                                "    NULL\n"\
                                                ");")\
                                                .arg((C))\
                                                .arg((S))\
                                                .arg((D))\
                                                .arg((t))\
                                                .arg((O))\
                                                .arg((U))

#define QUERY_UPDATE_BALANCE(C,S)           QString(\
                                                "UPDATE\n"\
                                                "  `clients`\n"\
                                                "SET\n"\
                                                "  `balance` = `balance` + (%2)\n"\
                                                "WHERE\n"\
                                                "  `id`=%1;")\
                                                .arg((C))\
                                                .arg((S))

#define QUERY_VRFY_BALANCE(C)               QString(\
                                                "SELECT\n"\
                                                "  IF(t1.`balance` = SUM(t2.`summ`), 21930, 0)\n"\
                                                "FROM\n"\
                                                "  `clients` AS t1\n"\
                                                "LEFT JOIN\n"\
                                                "  `balance` AS t2\n"\
                                                "ON\n"\
                                                "  t1.id = t2.`client`\n"\
                                                "WHERE\n"\
                                                "  t1.`balance_enable` = 1\n"\
                                                "  AND t1.`id` = %1;")\
                                                .arg((C))

// Зачисления за товары на реализации, использованные в ремонте: запись в журнал балансов
#define QUERY_INS_BALANCE_LOG2(O,U,R) QString(\
                                                "INSERT INTO\n"\
                                                "  `balance` (\n"\
                                                "    `client`,\n"\
                                                "    `summ`,\n"\
                                                "    `direction`,\n"\
                                                "    `reason`,\n"\
                                                "    `created`,\n"\
                                                "    `office`,\n"\
                                                "    `uid`,\n"\
                                                "    `dealer_payment`\n"\
                                                "  )\n"\
                                                "  SELECT\n"\
                                                "    `parts`.dealer,\n"\
                                                "    `parts`.`summ`,\n"\
                                                "    IF(`parts`.`summ` >= 0, 1, 0),\n"\
                                                "    CONCAT('Зачисление средств на баланса клиента №', `parts`.dealer, ' на сумму ', `parts`.`summ`, ' за проданные товары, находившиеся на реализации'),\n"\
                                                "    UTC_TIMESTAMP(),\n"\
                                                "    %1,\n"\
                                                "    %2,\n"\
                                                "    NULL\n"\
                                                "  FROM (\n"\
                                                "    SELECT \n"\
                                                "      t2.`dealer`,\n"\
                                                "      ROUND(SUM(t3.`count`*(t2.in_price + (t3.price - t2.in_price)*t2.return_percent/100)), 2) AS summ\n"\
                                                "    FROM\n"\
                                                "      `store_items` AS t2\n"\
                                                "    LEFT JOIN\n"\
                                                "      `store_int_reserve` AS t3\n"\
                                                "    ON\n"\
                                                "      t2.id = t3.item_id\n"\
                                                "    WHERE\n"\
                                                "      t3.repair_id = %3\n"\
                                                "      AND t2.is_realization = 1\n"\
                                                "      AND t3.state = 2\n"\
                                                "    GROUP BY\n"\
                                                "      t3.repair_id,\n"\
                                                "      t2.dealer\n"\
                                                "  ) AS `parts`;")\
                                                .arg((O))\
                                                .arg((U))\
                                                .arg((R))

// Зачисления за товары на реализации, использованные в ремонте: обновление балансов клиентов
#define QUERY_UPDATE_BALANCE2(R)            QString(\
                                                "UPDATE\n"\
                                                "  `clients` AS t1\n"\
                                                "CROSS JOIN (\n"\
                                                "  SELECT\n"\
                                                "    t2.`dealer`,\n"\
                                                "    ROUND(SUM(t3.`count`*(t2.in_price + (t3.price - t2.in_price)*t2.return_percent/100)), 2) AS summ\n"\
                                                "  FROM\n"\
                                                "    `store_items` AS t2\n"\
                                                "  LEFT JOIN\n"\
                                                "    `store_int_reserve` AS t3\n"\
                                                "  ON\n"\
                                                "    t2.id = t3.item_id\n"\
                                                "  WHERE\n"\
                                                "    t3.repair_id = %1\n"\
                                                "    AND t2.is_realization = 1\n"\
                                                "    AND t3.state = 2\n"\
                                                "  GROUP BY\n"\
                                                "    t3.repair_id,\n"\
                                                "    t2.dealer\n"\
                                                "  ) AS `parts`\n"\
                                                "ON\n"\
                                                "  t1.id = `parts`.dealer\n"\
                                                "SET\n"\
                                                "  t1.balance = t1.balance + `parts`.`summ`\n"\
                                                "WHERE\n"\
                                                "  t1.balance_enable = 1;")\
                                                .arg((R))

// Зачисления за товары на реализации, использованные в ремонте: верификация балансов
#define QUERY_VRFY_BALANCE2(R)              QString(\
                                                "SELECT\n"\
                                                "  IF(t1.`balance` = SUM(t2.`summ`), 21930, 0)\n"\
                                                "FROM\n"\
                                                "  `clients` AS t1\n"\
                                                "LEFT JOIN\n"\
                                                "  `balance` AS t2\n"\
                                                "ON\n"\
                                                "  t1.id = t2.`client`\n"\
                                                "WHERE\n"\
                                                "  t1.`id` IN (\n"\
                                                "    SELECT \n"\
                                                "      t2.`dealer`\n"\
                                                "    FROM\n"\
                                                "      `store_items` AS t2\n"\
                                                "    LEFT JOIN\n"\
                                                "      `store_int_reserve` AS t3\n"\
                                                "    ON\n"\
                                                "      t2.id = t3.item_id\n"\
                                                "    WHERE\n"\
                                                "      t3.repair_id = %1\n"\
                                                "      AND t2.is_realization = 1\n"\
                                                "      AND t3.state = 2\n"\
                                                "    GROUP BY\n"\
                                                "      t3.repair_id,\n"\
                                                "      t2.dealer\n"\
                                                "  )\n"\
                                                "GROUP BY\n"\
                                                "  t1.`id`;")\
                                                .arg((R))

// Зачисления за товары на реализации (обычная продажа): запись в журнал балансов
#define QUERY_INS_BALANCE_LOG3(O,U,D) QString(\
                                                "INSERT INTO\n"\
                                                "  `balance` (\n"\
                                                "    `client`,\n"\
                                                "    `summ`,\n"\
                                                "    `direction`,\n"\
                                                "    `reason`,\n"\
                                                "    `created`,\n"\
                                                "    `office`,\n"\
                                                "    `uid`,\n"\
                                                "    `dealer_payment`\n"\
                                                "  )\n"\
                                                "  SELECT\n"\
                                                "    `parts`.dealer,\n"\
                                                "    `parts`.`summ`,\n"\
                                                "    IF(`parts`.`summ` >= 0, 1, 0),\n"\
                                                "    CONCAT('Зачисление средств на баланса клиента №', `parts`.dealer, ' на сумму ', `parts`.`summ`, ' за проданные товары, находившиеся на реализации'),\n"\
                                                "    UTC_TIMESTAMP(),\n"\
                                                "    %1,\n"\
                                                "    %2,\n"\
                                                "    NULL\n"\
                                                "  FROM (\n"\
                                                "    SELECT\n"\
                                                "      t2.`dealer`,\n"\
                                                "      ROUND(SUM(t3.`count`*(t2.in_price + (t3.price - t2.in_price)*t2.return_percent/100)), 2) AS summ\n"\
                                                "    FROM\n"\
                                                "      `store_items` AS t2\n"\
                                                "    LEFT JOIN\n"\
                                                "      `store_sales` AS t3\n"\
                                                "    ON\n"\
                                                "      t2.id = t3.item_id\n"\
                                                "    WHERE\n"\
                                                "      t3.document_id = %3\n"\
                                                "      AND t2.is_realization = 1\n"\
                                                "      AND t3.is_cancellation = 0\n"\
                                                "    GROUP BY\n"\
                                                "      t3.document_id,\n"\
                                                "      t2.dealer\n"\
                                                "  ) AS `parts`;")\
                                                .arg((O))\
                                                .arg((U))\
                                                .arg((D))

// Зачисления за товары на реализации (обычная продажа): обновление балансов клиентов
#define QUERY_UPDATE_BALANCE3(D)            QString(\
                                                "UPDATE\n"\
                                                "  `clients` AS t1\n"\
                                                "CROSS JOIN (\n"\
                                                "  SELECT\n"\
                                                "    t2.`dealer`,\n"\
                                                "    ROUND(SUM(t3.`count`*(t2.in_price + (t3.price - t2.in_price)*t2.return_percent/100)), 2) AS summ\n"\
                                                "  FROM\n"\
                                                "    `store_items` AS t2\n"\
                                                "  LEFT JOIN\n"\
                                                "    `store_sales` AS t3\n"\
                                                "  ON\n"\
                                                "    t2.id = t3.item_id\n"\
                                                "  WHERE\n"\
                                                "    t3.document_id = %1\n"\
                                                "    AND t2.is_realization = 1\n"\
                                                "    AND t3.is_cancellation = 0\n"\
                                                "  GROUP BY\n"\
                                                "    t3.document_id,\n"\
                                                "    t2.dealer\n"\
                                                "  ) AS `parts`\n"\
                                                "ON\n"\
                                                "  t1.id = `parts`.dealer\n"\
                                                "SET\n"\
                                                "  t1.balance = t1.balance + `parts`.`summ`\n"\
                                                "WHERE\n"\
                                                "  t1.balance_enable = 1;")\
                                                .arg((D))

// Зачисления за товары на реализации (обычная продажа): верификация балансов
#define QUERY_VRFY_BALANCE3(D)              QString(\
                                                "SELECT\n"\
                                                "  IF(t1.`balance` = SUM(t2.`summ`), 21930, 0)\n"\
                                                "FROM\n"\
                                                "  `clients` AS t1\n"\
                                                "LEFT JOIN\n"\
                                                "  `balance` AS t2\n"\
                                                "ON\n"\
                                                "  t1.id = t2.`client`\n"\
                                                "WHERE\n"\
                                                "  t1.`id` IN (\n"\
                                                "    SELECT\n"\
                                                "      t2.`dealer`\n"\
                                                "    FROM\n"\
                                                "      `store_items` AS t2\n"\
                                                "    LEFT JOIN\n"\
                                                "      `store_sales` AS t3\n"\
                                                "    ON\n"\
                                                "      t2.id = t3.item_id\n"\
                                                "    WHERE\n"\
                                                "      t3.document_id = %1\n"\
                                                "      AND t2.is_realization = 1\n"\
                                                "      AND t3.is_cancellation = 0\n"\
                                                "    GROUP BY\n"\
                                                "      t3.document_id,\n"\
                                                "      t2.dealer\n"\
                                                "  )\n"\
                                                "GROUP BY\n"\
                                                "  t1.`id`;")\
                                                .arg((D))

#define QUERY_UPDATE_STORE_INT_RSRV(S,R)    QString("UPDATE `store_int_reserve` SET `state`=%1 WHERE `repair_id` = %2;").arg((S)).arg((R))

// выдача ремонта:
#define QUERY_SEL_PRE_UPD_STORE_ITEMS(R)    QString(\
                                                        "SELECT\n"\
                                                        "  t1.`count`-t2.`count` AS 'count',\n"\
                                                        "  t1.`reserved`-t2.`count` AS 'reserved',\n"\
                                                        "  t1.`sold`+t2.`count` AS 'sold'\n"\
                                                        "FROM\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "LEFT JOIN store_int_reserve AS t2\n"\
                                                        "  ON t2.item_id = t1.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t2.`repair_id` = %1;")\
                                                        .arg((R))
#define QUERY_UPD_STORE_ITEMS(R)            QString(\
                                                        "UPDATE\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "LEFT JOIN store_int_reserve AS t2\n"\
                                                        "  ON t2.item_id = t1.`id`\n"\
                                                        "SET\n"\
                                                        "  t1.`count`=t1.`count` - t2.`count`,\n"\
                                                        "  t1.`reserved`=t1.`reserved`-t2.`count`,\n"\
                                                        "  t1.`sold`=t1.`sold`+t2.`count`,\n"\
                                                        "  t1.`updated`=UTC_TIMESTAMP()\n"\
                                                        "WHERE\n"\
                                                        "  t2.`repair_id` = %1;")\
                                                        .arg((R))
#define QUERY_SEL_PST_UPD_STORE_ITEMS(R)    QString(\
                                                        "SELECT\n"\
                                                        "  t1.`count`,\n"\
                                                        "  t1.`reserved`,\n"\
                                                        "  t1.`sold`\n"\
                                                        "FROM\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "LEFT JOIN store_int_reserve AS t2\n"\
                                                        "  ON t2.item_id = t1.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t2.`repair_id` = %1;")\
                                                        .arg((R))

// для интерактивного обновления цен уже добавленных в список товаров используется prepaired statement
#define QUERY_SEL_STORE_ITEMS_ITEM_PRICE(column)    QString("SELECT %1 FROM store_items WHERE `id` = :id;").arg(column)

// резерв:
#define QUERY_SEL_PRE_UPD_STORE_ITEMS2(qty, id)  QString(\
                                                        "SELECT\n"\
                                                        "  t1.`count`,\n"\
                                                        "  t1.`count` AS 'count_',\n"\
                                                        "  t1.`sold`,\n"\
                                                        "  t1.`sold` AS 'sold_',\n"\
                                                        "  t1.`reserved`,\n"\
                                                        "  t1.`reserved` + %1 AS 'reserved_'\n"\
                                                        "FROM\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "WHERE\n"\
                                                        "  t1.`id` = %2;")\
                                                        .arg((qty))\
                                                        .arg((id))

#define QUERY_UPD_STORE_ITEMS2(qty, id)     QString(\
                                                        "UPDATE\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "SET\n"\
                                                        "  t1.`reserved`=t1.`reserved` + %1,\n"\
                                                        "  t1.`updated`=UTC_TIMESTAMP()\n"\
                                                        "WHERE\n"\
                                                        "  t1.`id` = %2;")\
                                                        .arg((qty))\
                                                        .arg((id))

#define QUERY_SEL_PST_UPD_STORE_ITEMS2(doc, item)  QString("#Верификация чисел в столбцах store_sales.count, store_items.count, store_items.sold и store_items.reserved после записи (одним запросом)\n"\
                                                        "SELECT\n"\
                                                        "  t2.`count`,\n"\
                                                        "  t2.`count` AS 'count_',\n"\
                                                        "  t2.`sold`,\n"\
                                                        "  t2.`sold` AS 'sold_',\n"\
                                                        "  t2.`reserved` - t1.`count` AS 'reserved',\n"\
                                                        "  t2.`reserved` AS 'reserved_'\n"\
                                                        "FROM `store_sales` AS t1\n"\
                                                        "LEFT JOIN `store_items` AS t2\n"\
                                                        "  ON t1.`item_id` = t2.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t1.`document_id` = %1\n"\
                                                        "  AND t1.`item_id` = %2;")\
                                                        .arg((doc))\
                                                        .arg((item))
// продажа (простая):
#define QUERY_SEL_PRE_UPD_STORE_ITEMS3(qty, id)  QString(\
                                                        "SELECT\n"\
                                                        "  t1.`count`,\n"\
                                                        "  t1.`count` - %1 AS 'count_',\n"\
                                                        "  t1.`sold`,\n"\
                                                        "  t1.`sold` + %1 AS 'sold_',\n"\
                                                        "  t1.`reserved`,\n"\
                                                        "  t1.`reserved` AS 'reserved_'\n"\
                                                        "FROM\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "WHERE\n"\
                                                        "  t1.`id` = %2;")\
                                                        .arg((qty)).arg((id))

#define QUERY_UPD_STORE_ITEMS3(qty, id)     QString(\
                                                        "UPDATE\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "SET\n"\
                                                        "  t1.`count`=t1.`count` - %1,\n"\
                                                        "  t1.`sold`=t1.`sold` + %1,\n"\
                                                        "  t1.`updated`=UTC_TIMESTAMP()\n"\
                                                        "WHERE\n"\
                                                        "  t1.`id` = %2;")\
                                                        .arg((qty)).arg((id))

#define QUERY_SEL_PST_UPD_STORE_ITEMS3(doc, item)  QString(\
                                                        "SELECT\n"\
                                                        "  t2.`count` + t1.`count` AS 'count',\n"\
                                                        "  t2.`count` AS 'count_',\n"\
                                                        "  t2.`sold` - t1.`count` AS 'sold',\n"\
                                                        "  t2.`sold` AS 'sold_',\n"\
                                                        "  t2.`reserved`,"\
                                                        "  t2.`reserved` AS 'reserved_'\n"\
                                                        "FROM\n"\
                                                        "  `store_sales` AS t1\n"\
                                                        "LEFT JOIN `store_items` AS t2\n"\
                                                        "  ON t1.`item_id` = t2.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t1.`document_id` = %1\n"\
                                                        "  AND t1.`item_id` = %2;")\
                                                        .arg((doc)).arg((item))

// продажа (ранее зарезервированные товары):
#define QUERY_SEL_PRE_UPD_STORE_ITEMS4(qty, id)  QString(\
                                                        "SELECT\n"\
                                                        "  t1.`count`,\n"\
                                                        "  t1.`count` - %2 AS 'count_',\n"\
                                                        "  t1.`sold`,\n"\
                                                        "  t1.`sold` + %2 AS 'sold_',\n"\
                                                        "  t1.`reserved` - t2.`count` AS 'reserved_',\n"\
                                                        "  t1.`reserved` - t2.`count` + %2 AS 'reserved'\n"\
                                                        "FROM\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "LEFT JOIN `store_sales` AS t2\n"\
                                                        "  ON t2.`item_id` = t1.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t2.`id` = %1\n"\
                                                        "  AND t2.`is_cancellation` = 0;")\
                                                        .arg((id)).arg((qty))

#define QUERY_UPD_STORE_ITEMS4(new_qty, id)     QString(\
                                                        "UPDATE\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "LEFT JOIN `store_sales` AS t2\n"\
                                                        "  ON t2.`item_id` = t1.`id`\n"\
                                                        "SET\n"\
                                                        "  t1.`reserved` = t1.`reserved` - t2.`count` + %1,\n"\
                                                        "  t1.`updated`=UTC_TIMESTAMP()\n"\
                                                        "WHERE\n"\
                                                        "  t2.`id` = %2\n"\
                                                        "  AND t2.`is_cancellation` = 0;\n")\
                                                        .arg((new_qty))\
                                                        .arg((id))

#define QUERY_UPD_STORE_ITEMS4_1(new_qty, id)     QString(\
                                                        "UPDATE\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "LEFT JOIN `store_sales` AS t2\n"\
                                                        "  ON t2.`item_id` = t1.`id`\n"\
                                                        "SET\n"\
                                                        "  t1.`count`=t1.`count` - %1,\n"\
                                                        "  t1.`sold`=t1.`sold` + %1,\n"\
                                                        "  t1.`reserved` = t1.`reserved` - t2.`count`,\n"\
                                                        "  t1.`updated`=UTC_TIMESTAMP()\n"\
                                                        "WHERE\n"\
                                                        "  t2.`id` = %2\n"\
                                                        "  AND t2.`is_cancellation` = 0;\n")\
                                                        .arg((new_qty))\
                                                        .arg((id))

#define QUERY_SEL_PST_UPD_STORE_ITEMS4(id)  QString(\
                                                        "SELECT\n"\
                                                        "  t2.`count` + t1.`count` AS 'count',\n"\
                                                        "  t2.`count` AS 'count_',\n"\
                                                        "  t2.`sold` - t1.`count` AS 'sold',\n"\
                                                        "  t2.`sold` AS 'sold_',\n"\
                                                        "  t2.`reserved` AS 'reserved_',\n"\
                                                        "  t2.`reserved` + t1.`count` AS 'reserved'\n"\
                                                        "FROM\n"\
                                                        "  `store_sales` AS t1\n"\
                                                        "LEFT JOIN `store_items` AS t2\n"\
                                                        "  ON t1.`item_id` = t2.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t1.`id` = %1\n"\
                                                        "  AND t1.`is_cancellation` = 0;")\
                                                        .arg((id))

// отмена резерва (в т. ч. частичная, перед проведением накладной)
#define QUERY_UPD_STORE_SALES_RESERVE_CANCELLATION(doc, items_list)  QString(\
                                                        "UPDATE\n"\
                                                        "  `store_sales`\n"\
                                                        "SET\n"\
                                                        "  `count` = 0,\n"\
                                                        "  `is_cancellation` = 1,\n"\
                                                        "  `cancellation_reason` = 'снятие резерва'\n"\
                                                        "WHERE\n"\
                                                        "  `document_id` = %1\n"\
                                                        "  AND `id` IN (%2);\n")\
                                                        .arg((doc))\
                                                        .arg((items_list))

#define QUERY_UPD_STORE_ITEMS_RESERVE_CANCELLATION(doc, items_list)  QString(\
                                                        "UPDATE\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "LEFT JOIN `store_sales` AS t2\n"\
                                                        "  ON t2.`item_id` = t1.`id`\n"\
                                                        "SET\n"\
                                                        "  t1.`reserved` = t1.`reserved` - t2.`count`,\n"\
                                                        "  t1.`updated`=UTC_TIMESTAMP()\n"\
                                                        "WHERE\n"\
                                                        "  t2.`document_id` = %1\n"\
                                                        "  #AND t2.`is_cancellation` = 1\n"\
                                                        "  AND t2.`id` IN (%2);\n")\
                                                        .arg((doc))\
                                                        .arg((items_list))

#define QUERY_SEL_PRE_UPD_STORE_ITEMS5(doc)  QString(\
                                                        "SELECT\n"\
                                                        "  t2.`reserved` - t1.`count` AS 'reserved'\n"\
                                                        "FROM `store_sales` AS t1\n"\
                                                        "LEFT JOIN `store_items` AS t2\n"\
                                                        "  ON t1.`item_id` = t2.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t1.`document_id` = %1;")\
                                                        .arg((doc))

#define QUERY_SEL_PST_UPD_STORE_ITEMS5(doc)  QString("#Верификация чисел в столбцах store_sales.count, store_items.count, store_items.sold и store_items.reserved после записи (одним запросом)\n"\
                                                        "SELECT\n"\
                                                        "  t2.`reserved`\n"\
                                                        "FROM `store_sales` AS t1\n"\
                                                        "LEFT JOIN `store_items` AS t2\n"\
                                                        "  ON t1.`item_id` = t2.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t1.`document_id` = %1;")\
                                                        .arg((doc))

// Возврат товара
#define QUERY_UPD_STORE_ITEMS_UNSALE(doc, items_list)  QString(\
                                                        "UPDATE\n"\
                                                        "  `store_items` AS t1\n"\
                                                        "LEFT JOIN `store_sales` AS t2\n"\
                                                        "  ON t2.`item_id` = t1.`id`\n"\
                                                        "SET\n"\
                                                        "  t1.`count` = t1.`count` + t2.`count`,\n"\
                                                        "  t1.`sold` = t1.`sold` - t2.`count`,\n"\
                                                        "  t1.`updated`=UTC_TIMESTAMP()\n"\
                                                        "WHERE\n"\
                                                        "  t2.`document_id` = %1\n"\
                                                        "  #AND t2.`is_cancellation` = 1\n"\
                                                        "  AND t2.`id` IN (%2);\n")\
                                                        .arg((doc))\
                                                        .arg((items_list))

#define QUERY_UPD_STORE_SALES_UNSALE(doc, items_list)  QString(\
                                                        "UPDATE\n"\
                                                        "  `store_sales`\n"\
                                                        "SET\n"\
                                                        "  `count` = 0,\n"\
                                                        "  `is_cancellation` = 1,\n"\
                                                        "  `cancellation_reason` = 'Возврат товара'\n"\
                                                        "WHERE\n"\
                                                        "  `document_id` = %1\n"\
                                                        "  AND `id` IN (%2);\n")\
                                                        .arg((doc))\
                                                        .arg((items_list))

#define QUERY_SEL_PRE_UPD_STORE_ITEMS6(doc, items_list)  QString(\
                                                        "SELECT\n"\
                                                        "  t2.`count` + t1.`count` AS 'count',\n"\
                                                        "  t2.`sold` - t1.`count` AS 'sold'\n"\
                                                        "FROM `store_sales` AS t1\n"\
                                                        "LEFT JOIN `store_items` AS t2\n"\
                                                        "  ON t1.`item_id` = t2.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t1.`document_id` = %1\n"\
                                                        "  AND t1.`id` IN (%2);")\
                                                        .arg((doc))\
                                                        .arg((items_list))

#define QUERY_SEL_PST_UPD_STORE_ITEMS6(doc, items_list)  QString("#Верификация чисел в столбцах store_sales.count, store_items.count, store_items.sold и store_items.reserved после записи (одним запросом)\n"\
                                                        "SELECT\n"\
                                                        "  t2.`count`,\n"\
                                                        "  t2.`sold`\n"\
                                                        "FROM `store_sales` AS t1\n"\
                                                        "LEFT JOIN `store_items` AS t2\n"\
                                                        "  ON t1.`item_id` = t2.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t1.`document_id` = %1\n"\
                                                        "  AND t1.`id` IN (%2);")\
                                                        .arg((doc))\
                                                        .arg((items_list))

#define QUERY_INS_LOG_PARTS_IN_REPAIR(U,O,C,R) QString("INSERT INTO `logs`(`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) SELECT \n"\
                                                    "NULL, 6, 0, %1, UTC_TIMESTAMP(), NULL, NULL, %2, %3, `repair_id`, `item_id`, NULL, NULL, NULL, CONCAT('Товар установленный в ремонт №', `repair_id`, ' продан. Ремонт выдан') FROM `store_int_reserve` WHERE `repair_id` = %4;").arg((U)).arg((O)).arg((C)).arg((R))

#define QUERY_SEL_PART_FOR_SALE(uid, price_field_name)   QString(\
                                                        "SELECT\n"\
                                                        "  0 AS 'id',\n"\
                                                        "  CONCAT(LPAD(t1.articul, 6, '0'), '-', LPAD(t1.id, 6, '0')) AS 'UID',\n"\
                                                        "  t1.`name`,\n"\
                                                        "  1 AS 'count',\n"\
                                                        "  t1.`count` - t1.`reserved` AS 'avail',\n"\
                                                        "  t1.%2 AS 'price',\n"\
                                                        "  t1.%2 AS 'summ',\n"\
                                                        "  t1.`box`,\n"\
                                                        "  t1.`SN` AS 'sn',\n"\
                                                        "  t1.`warranty`,\n"\
                                                        "  t1.`is_realization`,\n"\
                                                        "  t1.`return_percent`,\n"\
                                                        "  0 AS 'is_cancellation',\n"\
                                                        "  NULL AS 'cancellation_reason',\n"\
                                                        "  t1.`id` AS 'item_id',\n"\
                                                        "  t1.`in_price`,\n"\
                                                        "  t1.`dealer`\n"\
                                                        "FROM\n"\
                                                        "  store_items AS t1\n"\
                                                        "WHERE\n"\
                                                        "  `id` IN (%1);").arg(uid).arg((price_field_name))

#define QUERY_INS_LOG_PARTS_IN_DOC(U,O,D,t)   QString("INSERT INTO `logs`(`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) SELECT \n"\
                                                    "NULL, 6, 0, %1, UTC_TIMESTAMP(), NULL, NULL, %2, NULL, NULL, `item_id`, %3, NULL, NULL, %4 FROM `store_sales` WHERE `document_id` = %3;").arg((U)).arg((O)).arg((D)).arg((t))

#define QUERY_INS_LOG_PARTS_IN_DOC_RSRV(U,O,D,t,ic)   QString("INSERT INTO `logs`(`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) SELECT \n"\
                                                    "NULL, 6, 0, %1, UTC_TIMESTAMP(), NULL, NULL, %2, NULL, NULL, `item_id`, %3, NULL, NULL, %4 FROM `store_sales` WHERE `document_id` = %3 AND `is_cancellation` = %5;").arg((U)).arg((O)).arg((D)).arg((t)).arg((ic))

#define QUERY_INS_LOG_PARTS_IN_DOC_RET(U,O,D,t,items_list)   QString("INSERT INTO `logs`(`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) SELECT \n"\
                                                    "NULL, 6, 0, %1, UTC_TIMESTAMP(), NULL, NULL, %2, NULL, NULL, `item_id`, %3, NULL, NULL, %4 FROM `store_sales` WHERE `document_id` = %3 AND `id` IN (%5);").arg((U)).arg((O)).arg((D)).arg((t)).arg((items_list))

#define QUERY_UPD_CLIENT_PURCHASES(id, num)      QString("UPDATE `clients` SET `purchases`=`purchases`+%2 WHERE `id` = %1;").arg((id)).arg((num))
#define QUERY_UPD_CLIENT_REPAIRS(id)      QString("UPDATE `clients` SET `repairs`=`repairs`+1 WHERE `id` = %1;").arg((id))

#define QUERY_UPD_SALE_DOC_TYPE_N_STATE(id, new_type, new_state)  QString("UPDATE `docs` SET `type` = %2, `state`=%3 WHERE `id` = %1;").arg((id)).arg((new_type)).arg((new_state))

#define QUERY_UPD_SALE_DOC_STATE_N_SUMM(doc, new_state, new_summ)  QString("UPDATE `docs` SET `state`=%2, `total`=%3 WHERE `id` = %1;").arg((doc)).arg((new_state)).arg((new_summ))

#define QUERY_UPD_SALE_DOC_TYPE_STATE_SUMM(id, new_type, new_state, new_summ)  QString("UPDATE `docs` SET `type` = %2, `state`=%3, `total`=%4 WHERE `id` = %1;").arg((id)).arg((new_type)).arg((new_state)).arg((new_summ))

#define QUERY_UPD_SALE_DOC(doc,\
                   type, state, acc, summ, notes,\
                    upd, reason, track)  QString(\
                                                        "UPDATE\n"\
                                                        "  `docs`\n"\
                                                        "SET\n"\
                                                        "  `type` = %2,\n"\
                                                        "  `state` = %3,\n"\
                                                        "  `payment_system` = %4,\n"\
                                                        "  `total` = %5,\n"\
                                                        "  `notes` = %6,\n"\
                                                        "  `updated_at` = %7,\n"\
                                                        "  `reason` = %8,\n"\
                                                        "  `track` = %9\n"\
                                                        "WHERE\n"\
                                                        "  `id` = %1;")\
                                                        .arg((doc))\
                                                        .arg((type))\
                                                        .arg((state))\
                                                        .arg((acc))\
                                                        .arg((summ))\
                                                        .arg((notes))\
                                                        .arg((upd))\
                                                        .arg((reason))\
                                                        .arg((track))

#define QUERY_INS_DOCS(type, state, real,\
    acc, comp, store, U, sum, t, O, C,\
    cur, t2, p_opt, rsrv)       QString("INSERT INTO `docs`(`type`, `state`, `is_realization`, `payment_system`, `company`, `store`, `user`, `total`, `notes`, `office`, `dealer`, `currency_rate`, `reason`, `price_option`, `reserve_days`, `created`, `updated_at`, `order_id`, `return_percent`, `master_id`, `repair_id`, `works_included`, `invoice`, `track`, `d_store`, `d_pay`) VALUES (\n"\
                                                    "%1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, UTC_TIMESTAMP(), NULL, NULL, 0, NULL, NULL, 0, NULL, NULL, NULL, 0);")\
                                                    .arg((type))\
                                                    .arg((state))\
                                                    .arg((real))\
                                                    .arg((acc))\
                                                    .arg((comp))\
                                                    .arg((store))\
                                                    .arg((U))\
                                                    .arg((sum))\
                                                    .arg((t))\
                                                    .arg((O))\
                                                    .arg((C))\
                                                    .arg((cur))\
                                                    .arg((t2))\
                                                    .arg((p_opt))\
                                                    .arg((rsrv))

#define QUERY_INS_STORE_SALES(dealer, item, doc,\
    buyer, qty, in_price, price, warranty, is_realiz,\
    ret_percent, user, sn)      QString("INSERT INTO `store_sales`(`dealer`, `item_id`, `document_id`, `customer_id`, `count`, `in_price`, `price`, `warranty`, `realizator_payed`, `is_realization`, `dealer_payment`, `return_percent`, `user`, `is_cancellation`, `cancellation_reason`, `sn`, `d_category`) VALUES (\n"\
                                    "%1, %2, %3, %4, %5, %6, %7, %8, 0, %9, NULL, %10, %11, 0, NULL, '%12', NULL);")\
                                                    .arg((dealer))\
                                                    .arg((item))\
                                                    .arg((doc))\
                                                    .arg((buyer))\
                                                    .arg((qty))\
                                                    .arg((in_price))\
                                                    .arg((price))\
                                                    .arg((warranty))\
                                                    .arg((is_realiz))\
                                                    .arg((ret_percent))\
                                                    .arg((user))\
                                                    .arg((sn))

#define QUERY_UPD_STORE_SALES(id, buyer, qty,\
    price, warranty, user, is_cancel,\
    cancel_reason, sn)          QString("UPDATE `store_sales` SET `customer_id` = %2, `count` = %3, `price` = %4, `warranty` = %5,"\
                                    "`user` = %6, `is_cancellation` = %7, `cancellation_reason` = %8, `sn` = '%9' WHERE `id` = %1;")\
                                                    .arg((id))\
                                                    .arg((buyer))\
                                                    .arg((qty))\
                                                    .arg((price))\
                                                    .arg((warranty))\
                                                    .arg((user))\
                                                    .arg((is_cancel))\
                                                    .arg((cancel_reason))\
                                                    .arg((sn))

#define QUERY_UPD_SALE_DOC_SET_TRACK(id, track)    QString("UPDATE `docs` SET `track` = '%2' WHERE `id` = %1;")\
                                                    .arg((id))\
                                                    .arg((track))
#define QUERY_INS_SCHEMAVERSIONS(scriptName)       QString("INSERT INTO `schemaversions` (ScriptName, Applied) VALUES ('%1',UTC_TIMESTAMP());").arg(scriptName)
#define QUERY_UPD_APP_VER(appVer)                  QString("UPDATE `config` SET `version_snap`='%1' WHERE `id`=1;").arg(appVer)

#endif // COM_SQL_QUERIES_H
