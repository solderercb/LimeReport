chcp 65001 >nul 2>&1

sed -i -z "s/#include <LimeReport>/#include <include\/LimeReport>/g" mainwindow.h
sed -i -z "s/#include <LimeReport>/#include <include\/LimeReport>/g" tabprintdialog.h
sed -i -z "s/#include <LimeReport>/#include <include\/LimeReport>/g" tabreportdesigner.h
