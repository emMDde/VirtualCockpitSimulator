#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
//#include <iostream>
//#include <string>
#include <QDebug>

QTranslator *translator = nullptr;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    translator = new QTranslator(&app);
    QString localLang = QLocale().name();
    if (localLang.startsWith("en") && translator->load(":/translates/WDS_Projekt_en.qm)")) app.installTranslator(translator);
    else if(localLang.startsWith("de") && translator->load(":/translates/WDS_Projekt_de.qm)"))  app.installTranslator(translator);
    //const QStringList uiLanguages = QLocale::system().uiLanguages();
    //qDebug() << "Lista języków:" << uiLanguages;

    MainWindow appWindow;
    appWindow.show();
    return app.exec();
}
