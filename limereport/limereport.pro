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
unix:{
    linux{
        # qmake need make mkdir -p on subdirs more than root/
        QMAKE_POST_LINK += mkdir -p \"$${DESTDIR}/include\" $$escape_expand(\\n\\t)
    }

    for(FILE,INCLUDE_FILES) {
        QMAKE_POST_LINK += $$QMAKE_COPY \"$$FILE\" \"$${DESTDIR}/include\" $$escape_expand(\\n\\t)
    }
}

win32 {
    contains(QMAKE_HOST.os, Linux) {
        # qmake need make mkdir -p on subdirs more than root/
        QMAKE_POST_LINK += mkdir -p \"$${DESTDIR}/include\" $$escape_expand(\\n\\t)

        for(FILE,INCLUDE_FILES) {
            QMAKE_POST_LINK += $$QMAKE_COPY \"$$FILE\" \"$${DESTDIR}/include\" $$escape_expand(\\n\\t)
        }
    } else {
        DEST_LIBS_ = $${DEST_LIBS} # / будут заменены на \
        PWD_       = $${PWD}

        QMAKE_MKLINK = mklink /H

        # EXPORT_LIBS — путь к директории проекта, использующего LimeReport
        EXPORT_LIBS = $$section(BUILD_DIR, /, -1, -1)
        EXPORT_LIBS = $${SRC_ROOT}/../lib/$$replace(EXPORT_LIBS, "Desktop", $$lower($$QMAKE_HOST.name))
#        message($$EXPORT_LIBS)

        # замена / на \
        EXPORT_LIBS   ~= s,/,\\,g
        DEST_LIBS_    ~= s,/,\\,g
        INCLUDE_FILES ~= s,/,\\,g
        PWD_          ~= s,/,\\,g

        QMAKE_POST_LINK += $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += chcp 65001 >nul 2>&1 $$escape_expand(\\n\\t)
        !exists($${DEST_LIBS_}\\include){   # подавление вывода mkdir с помощью >nul 2>&1 почему-то приводит к ошибке сборки, поэтому выполняется проверка наличия директории
#           message($${DEST_LIBS_})
           QMAKE_POST_LINK += mkdir \"$${DEST_LIBS_}\\include\" $$escape_expand(\\n\\t)
        }

        for(FILE,INCLUDE_FILES) {
#             >nul 2>&1 отправляет stdout и stderr в NUL
            QMAKE_POST_LINK += $$QMAKE_COPY \"$${PWD_}\\$${FILE}\" \"$${DEST_LIBS_}\\include\\\" >nul 2>&1 $$escape_expand(\\n\\t)      # e.g. copy  <filename>    LimeReport\Desktop_Qt_5_15_4_mingw81_64\release\lib\include
        }


#        CONFIG(release, debug|release){
            # символическая ссылка на папку с либами, например LimeReport\..\libQt5 <<===>> LimeReport\Desktop_Qt_5_15_4_mingw81_64\release\lib  
#            !exists($${EXPORT_LIBS}){
#                QMAKE_POST_LINK += mklink /D \"$${EXPORT_LIBS}\" \"$${DEST_LIBS_}\"  2>NUL $$escape_expand(\\n\\t)
#            }
            # или копирование всей папки (release версии не большие, много времени не займёт)
            QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$${DEST_LIBS_}\" \"$${EXPORT_LIBS}\"
#        }
    }
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
