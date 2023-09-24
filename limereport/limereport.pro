CONFIG(debug, debug|release) {
    TARGET = limereportd
} else {
    TARGET = limereport
}

TEMPLATE = lib

CONFIG += precompile_header

CONFIG(static_build) {
    CONFIG += staticlib
}

!CONFIG(staticlib) {
    CONFIG += lib
    CONFIG += dll
}

CONFIG += create_prl
CONFIG += link_prl

macx{
    CONFIG -= dll
    CONFIG += lib_bundle
    CONFIG += plugin
}

DEFINES += LIMEREPORT_EXPORTS

CONFIG(staticlib) {
    DEFINES += HAVE_STATIC_BUILD
    DEFINES += QZINT_STATIC_BUILD
    message(STATIC_BUILD)
    DEFINES -= LIMEREPORT_EXPORTS
}

INCLUDE_FILES += \
    lrglobal.h \
    lrdatasourceintf.h \
    lrdatasourcemanagerintf.h \
    lrreportengine.h \
    lrscriptenginemanagerintf.h \
    lrcallbackdatasourceintf.h \
    lrpreviewreportwidget.h \
    lrreportdesignwindowintrerface.h \
    lrpreparedpagesintf.h

include(limereport.pri)

DESTDIR = $${DEST_LIBS}

# копирование скомпилированных файлов в папку основного проекта
!isEmpty(EXPORT_LIBS) {
#    message(PWD = $${PWD})
#    message(EXPORT_LIBS = $$EXPORT_LIBS)
    TARGET_DIR = $${PWD}
    EXPORT_LIBS  ~= s,/,\\,g
    TARGET_DIR   ~= s,/,\\,g

    QMAKE_POST_LINK += $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += chcp 65001 >nul 2>&1 $$escape_expand(\\n\\t)
    !exists($${EXPORT_LIBS}\\include){   # подавление вывода mkdir с помощью >nul 2>&1 почему-то приводит к ошибке сборки, поэтому выполняется проверка наличия директории
       QMAKE_POST_LINK += mkdir \"$${EXPORT_LIBS}\\include\" $$escape_expand(\\n\\t)
    }

    for(FILE, INCLUDE_FILES) {
        QMAKE_POST_LINK += $$QMAKE_COPY \"$${TARGET_DIR}\\$${FILE}\" \"$${EXPORT_LIBS}\\include\\\" >nul 2>&1 $$escape_expand(\\n\\t)      # e.g. copy  <filename>    LimeReport\Desktop_Qt_5_15_4_mingw81_64\release\lib\include
    }
    QMAKE_POST_LINK += echo $${LITERAL_HASH}include \"lrreportengine.h\" > \"$${EXPORT_LIBS}\\include\\LimeReport\" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += $$QMAKE_COPY \"$${BUILD_DIR}\\lib\" \"$${EXPORT_LIBS}\" $$escape_expand(\\n\\t)
}

CONFIG(zint) {
    message(zint)
    INCLUDEPATH += $$ZINT_PATH/backend $$ZINT_PATH/backend_qt
    DEPENDPATH += $$ZINT_PATH/backend $$ZINT_PATH/backend_qt
    LIBS += -L$${DESTDIR}
    CONFIG(release, debug|release) {
        LIBS += -lQtZint
    } else {
        LIBS += -lQtZintd
    }
}

#### Install mkspecs, headers and libs to QT_INSTALL_DIR

headerFiles.path = $$[QT_INSTALL_HEADERS]/LimeReport/
headerFiles.files = $${DESTDIR}/include/*
INSTALLS += headerFiles

mkspecs.path = $$[QT_INSTALL_DATA]/mkspecs/features
mkspecs.files = limereport.prf
INSTALLS += mkspecs

target.path = $$[QT_INSTALL_LIBS]
INSTALLS += target

####Automatically build required translation files (*.qm)

CONFIG(build_translations) {
    LANGUAGES = ru es ar fr zh pl

    defineReplace(prependAll) {
        for(a,$$1):result += $$2$${a}$$3
        return($$result)
    }

    TRANSLATIONS = $$prependAll(LANGUAGES, \"$$TRANSLATIONS_PATH/limereport_,.ts\")

    qtPrepareTool(LUPDATE, lupdate)
    ts.commands = $$LUPDATE \"$$PWD\" -noobsolete -ts $$TRANSLATIONS
    TRANSLATIONS_FILES =
    qtPrepareTool(LRELEASE, lrelease)
    for(tsfile, TRANSLATIONS) {
        qmfile = $$tsfile
        qmfile ~= s,".ts\"$",".qm\"",
        qm.commands += $$LRELEASE -removeidentical $$tsfile -qm $$qmfile $$escape_expand(\\n\\t)
        TRANSLATIONS_FILES += $$qmfile
    }
    qm.depends = ts
    OTHER_FILES += $$TRANSLATIONS
    QMAKE_EXTRA_TARGETS += qm ts
    POST_TARGETDEPS +=  qm
}

#### EN AUTOMATIC TRANSLATIONS
