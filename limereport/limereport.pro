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

EXTRA_FILES += \
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

DESTDIR = $${DEST_LIBS} # $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}/lib
unix:{
    linux{
        # qmake need make mkdir -p on subdirs more than root/
        QMAKE_POST_LINK += mkdir -p \"$${DEST_INCLUDE_DIR}\" $$escape_expand(\\n\\t) # mkdir LimeReport/include
        QMAKE_POST_LINK += mkdir -p \"$${DESTDIR}/include\" $$escape_expand(\\n\\t) # mkdir $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}/lib/include
    }

        for(FILE,EXTRA_FILES) {
        QMAKE_POST_LINK += $$QMAKE_COPY \"$$FILE\" \"$${DEST_INCLUDE_DIR}\" $$escape_expand(\\n\\t) # copy  <filename>    LimeReport/include
        }
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$${DEST_INCLUDE_DIR}\" \"$${DESTDIR}\"    # copy  LimeReport/include    $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}/lib
}

win32 {
    contains(QMAKE_HOST.os, Linux) {
        # qmake need make mkdir -p on subdirs more than root/
        QMAKE_POST_LINK += mkdir -p \"$${DEST_INCLUDE_DIR}\" $$escape_expand(\\n\\t) # mkdir LimeReport/include
        QMAKE_POST_LINK += mkdir -p \"$${DESTDIR}/include\" $$escape_expand(\\n\\t) # mkdir $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}/lib/include
        for(FILE,EXTRA_FILES) {
            QMAKE_POST_LINK += $$QMAKE_COPY \"$$FILE\" \"$${DEST_INCLUDE_DIR}\" $$escape_expand(\\n\\t) # copy  <filename>    LimeReport/include
        }
        QMAKE_POST_LINK += $$QMAKE_COPY_DIR \"$${DEST_INCLUDE_DIR}\" \"$${DESTDIR}\"    # copy  LimeReport/include    $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}/lib
    } else {
        QMAKE_MKLINK = mklink /H

        # EXPORT_LIBS — путь к директории проекта, использующего LimeReport
        # EXPORT_BIN - путь к директории bin (в неё будут помещены все исполняемые файлы; для удобства)
        equals(QT_MAJOR_VERSION, 6){
            EXPORT_LIBS = $${PWD}/../../lib6    # LimeReport/limereport/../../lib
        }
        else {
            EXPORT_LIBS = $${PWD}/../../lib5
        }
        DEST_DIR = $$DESTDIR/include            # $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}/lib/include
        EXPORT_BIN = $${PWD}/../bin             # LimeReport/limereport/../bin

        # замена / на \
        EXPORT_LIBS ~= s,/,\\,g
        EXPORT_BIN ~= s,/,\\,g
        DEST_DIR ~= s,/,\\,g
	EXTRA_FILES ~= s,/,\\,g
	BUILD_DIR ~= s,/,\\,g
        DEST_LIBS ~=  s,/,\\,g
	DEST_INCLUDE_DIR ~= s,/,\\,g
        PWDW = $${PWD}
        PWDW ~= s,/,\\,g

        ADDITIONAL_DIRS = \
            \"$${EXPORT_LIBS}\" \               # LimeReport\limereport\..\..\lib
            \"$${EXPORT_LIBS}\\include\" \      # LimeReport\limereport\..\..\lib\include
            \"$${EXPORT_BIN}\" \                # LimeReport\bin
            \"$${DEST_INCLUDE_DIR}\" \          # LimeReport\include
            \"$${DEST_DIR}\"                    # $${BUILD_DIR}\$${ARCH_TYPE}\$${BUILD_TYPE}\lib\include
        # обязательно в кавычках

        CONFIG(debug, debug|release){
            LIB_FILES += \
                liblimereportd.a \
                libQtZintd.a \
                limereportd.dll \
                limereportd.prl \
                QtZintd.dll
        }else{
            LIB_FILES += \
                liblimereport.a \
                libQtZint.a \
                limereport.dll \
                limereport.prl \
                QtZint.dll
        }

        QMAKE_POST_LINK += $$escape_expand(\\n\\t)
        QMAKE_POST_LINK += chcp 65001 >nul 2>&1 $$escape_expand(\\n\\t)
        for(DIR_ITER, ADDITIONAL_DIRS) {
            DIR_ITER ~=  s,\",,g    # здесь кавычки нужно заменить, т. к. команда exists добавляет свои
            !exists($${DIR_ITER}){   # подавление вывода mkdir с помощью >nul 2>&1 почему-то приводит к ошибке сборки, поэтому выполняется проверка наличия директории
    #            message($${DIR_ITER})
                QMAKE_POST_LINK += mkdir \"$${DIR_ITER}\" $$escape_expand(\\n\\t)
            }
        }

        for(FILE,EXTRA_FILES) {
    #        # >nul 2>&1 отправляет stdout и stderr в NUL
    #        # include файлы мелкие, поэтому копируем их
            QMAKE_POST_LINK += $$QMAKE_COPY \"$${PWDW}\\$${FILE}\" \"$${DEST_INCLUDE_DIR}\\\" >nul 2>&1 $$escape_expand(\\n\\t)         # copy  <filename>    LimeReport\include
            QMAKE_POST_LINK += $$QMAKE_COPY \"$${PWDW}\\$${FILE}\" \"$${DEST_DIR}\\\" >nul 2>&1 $$escape_expand(\\n\\t)                 # copy  <filename>    $${BUILD_DIR}/$${ARCH_TYPE}/$${BUILD_TYPE}/lib/include
            QMAKE_POST_LINK += $$QMAKE_COPY \"$${PWDW}\\$${FILE}\" \"$${EXPORT_LIBS}\\include\\\" >nul 2>&1 $$escape_expand(\\n\\t)     # copy  <filename>    LimeReport\..\lib\include
        }

        for(FILE,LIB_FILES){
            # скомпилированные библиотеки (особенно debug версии) могут иметь внушительный размер, поэтому создаём ссылки на них
    #        QMAKE_POST_LINK += $$QMAKE_COPY \"$${FILE}\" \"$${EXPORT_LIBS}\\$${FILE}\" $$escape_expand(\\n\\t)
            QMAKE_POST_LINK += del \"$${EXPORT_LIBS}\\$${FILE}\" 2>NUL $$escape_expand(\\n\\t)                                          # del LimeReport\limereport\..\..\lib\<*.a; *.dll>
            QMAKE_POST_LINK += del \"$${EXPORT_BIN}\\$${FILE}\" 2>NUL $$escape_expand(\\n\\t)                                           # del LimeReport\bin\<*.a; *.dll>
            QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${EXPORT_LIBS}\\$${FILE}\" \"$$DEST_LIBS\\$${FILE}\" >nul 2>&1 $$escape_expand(\\n\\t) # mklink LimeReport\..\lib\<file> $${BUILD_DIR}\$${ARCH_TYPE}\$${BUILD_TYPE}\lib\<file>
            QMAKE_POST_LINK += $$QMAKE_MKLINK \"$${EXPORT_BIN}\\$${FILE}\" \"$$DEST_LIBS\\$${FILE}\" >nul 2>&1 $$escape_expand(\\n\\t)  # mklink LimeReport\bin\<file> $${BUILD_DIR}\$${ARCH_TYPE}\$${BUILD_TYPE}\lib\<file>
        }
    }
}

CONFIG(zint) {
    message(zint)
    INCLUDEPATH += $$ZINT_PATH/backend $$ZINT_PATH/backend_qt
    DEPENDPATH += $$ZINT_PATH/backend $$ZINT_PATH/backend_qt
    LIBS += -L$${DEST_LIBS}
    CONFIG(release, debug|release) {
        LIBS += -lQtZint
    } else {
        LIBS += -lQtZintd
    }
}

#### Install mkspecs, headers and libs to QT_INSTALL_DIR

headerFiles.path = $$[QT_INSTALL_HEADERS]/LimeReport/
headerFiles.files = $${DEST_INCLUDE_DIR}/*
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
