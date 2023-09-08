include(../common.pri)
QT += core gui

CONFIG(release, debug|release) {
    TARGET = demo_snap
} else {
    TARGET = demo_snapd
}

TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    global.cpp \
    clients4test.cpp \
    tabcommon.cpp \
    tabprintdialog.cpp \
    tabreportdesigner.cpp \
    models/ssqlquerymodel.cpp \
    models/sstandarditemmodel.cpp

HEADERS += \
    mainwindow.h \
    global.h \
    clients4test.h \
    com_sql_queries.h \
    tabcommon.h \
    tabprintdialog.h \
    tabreportdesigner.h \
    models/ssqlquerymodel.h \
    models/sstandarditemmodel.h

FORMS += \
    mainwindow.ui\
    tabprintdialog.ui \
    tabreportdesigner.ui

INCLUDEPATH += $$PWD/../include
DEPENDPATH  += $$PWD/../include

EXTRA_DIR     += $$PWD/demo_reports
DEST_DIR       = $${DEST_BINS}          # $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}/$${TARGET}
REPORTS_DIR    = $${DEST_DIR}           # e. g. build/6.2.1/win64/debug/demo_snap/
EXPORT_BIN     = $$PWD/../bin

macx{
    CONFIG += app_bundle
}

DESTDIR = $$DEST_DIR
unix:{
    #    QMAKE_POST_LINK += mkdir -p $$quote($$REPORTS_DIR) |
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$$EXTRA_DIR\" \"$$REPORTS_DIR\" $$escape_expand(\n\t)
		
    linux{
        #Link share lib to ../lib rpath
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/lib
        QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../lib
        QMAKE_LFLAGS_RPATH += #. .. ./libs
    }
    target.path = $${DEST_DIR}
    INSTALLS = target
}

win32 {
    contains(QMAKE_HOST.os, Linux) {
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$$EXTRA_DIR\" \"$$REPORTS_DIR\" $$escape_expand(\n\t)
    } else {
        QMAKE_MKLINK = mklink /H
        EXTRA_DIR ~= s,/,\\,g
        DEST_DIR ~= s,/,\\,g
        REPORTS_DIR ~= s,/,\\,g
        EXPORT_BIN ~= s,/,\\,g
        QMAKE_POST_LINK += $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += chcp 65001 >nul 2>&1 $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$$EXTRA_DIR\" \"$$REPORTS_DIR\\reports\" $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += $$QMAKE_COPY \"$${DEST_DIR}\\$${TARGET}.exe\" \"$${EXPORT_BIN}\\\" $$escape_expand(\\n\\t)
    }
}

LIBS += -L$${DEST_LIBS}         # $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}/lib
CONFIG(debug, debug|release) {
    LIBS += -llimereportd
} else {
    LIBS += -llimereport
}
LIBS += -L$${DEST_LIBS}
CONFIG(debug, debug|release) {
    LIBS += -lQtZintd
} else {
    LIBS += -lQtZint
}
LIBS += -lwinspool -lKernel32
message($$LIBS)

# Qt Creator v5/v6 при сохранении разрушает жёсткую связь файлов. Для того, чтобы в отдельном субрепозитории (субпроекте,
# предназначенном для разработки отдельного класса и позволяющем чуть-чуть сэкономить на времени компиляции) при изменении
# исходного кода в основном проекте также появились изменения, после линковки пересоздаём жесткую ссылку. У этого метода,
# есть, конечно, недостатки; например, если файл был изменён в соновном проекте, сохранён, а компиляция не производилась, то
# в случае внесения изменений в файл в субрепозитории и запуска компиляции, изменения в файле в основном репозитории могут
# быть утрачены.

SHARED_FILE_SRC_PATH = $${PWD}
SHARED_FILE_TRG_PATH = $${PWD}\\..\\..
SHARED_FILES = clients4test.cpp
SHARED_FILES += clients4test.h
SHARED_FILES += com_sql_queries.h
SHARED_FILES += global.cpp
SHARED_FILES += global.h
SHARED_FILES += tabcommon.cpp
SHARED_FILES += tabcommon.h
SHARED_FILES += tabprintdialog.cpp
SHARED_FILES += tabprintdialog.h
SHARED_FILES += tabprintdialog.ui
SHARED_FILES += tabreportdesigner.cpp
SHARED_FILES += tabreportdesigner.h
SHARED_FILES += tabreportdesigner.ui
for(FILE,SHARED_FILES){
    QMAKE_POST_LINK += del \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" 2>NUL $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" \"$${SHARED_FILE_SRC_PATH}\\$$FILE\" >nul 2>&1 $$escape_expand(\\n\\t)
}

SHARED_FILE_SRC_PATH = $${PWD}\\models
SHARED_FILE_TRG_PATH = $${PWD}\\..\\..\\models
SHARED_FILES = ssqlquerymodel.cpp
SHARED_FILES += ssqlquerymodel.h
SHARED_FILES += sstandarditemmodel.cpp
SHARED_FILES += sstandarditemmodel.h
for(FILE,SHARED_FILES){
    QMAKE_POST_LINK += del \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" 2>NUL $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${SHARED_FILE_TRG_PATH}\\$$FILE\" \"$${SHARED_FILE_SRC_PATH}\\$$FILE\" >nul 2>&1 $$escape_expand(\\n\\t)
}
