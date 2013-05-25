#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>

#include "events.h"

#define VER "0.1"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Embedd the note font
    //QFontDatabase::addApplicationFont(":/font/guido2.ttf");

    // For Settings...
    QCoreApplication::setApplicationName("Meisterschueler");
    QCoreApplication::setApplicationVersion(VER);
    QCoreApplication::setOrganizationName("Meisterschueler AG");
    QCoreApplication::setOrganizationDomain("gruendger.de");

    // Translation
    QSettings settings;
    QString locale = QLocale::system().name();
    QString savedLocale = settings.value("language").toString();
    QTranslator translator;
    translator.load(QString(":/translations/meisterschueler_") + (savedLocale.isEmpty() ? locale : savedLocale));
    a.installTranslator(&translator);

    // Application Icon
    //app.setWindowIcon(QIcon(":/icons/Media3.png"));

    MainWindow w;
    w.show();
    
    return a.exec();
}
