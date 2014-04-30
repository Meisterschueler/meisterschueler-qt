#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QSettings>
#include <QTranslator>

#include "QGuidoPainter.h"

#include "events.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGuidoPainter::startGuidoEngine();

    // Embedd the note font
    QFontDatabase::addApplicationFont(":/font/guido2.ttf");

    // For Settings...
    QCoreApplication::setApplicationName("Meisterschueler");
    QCoreApplication::setApplicationVersion(VER);
    QCoreApplication::setOrganizationName("Meisterschueler AG");
    QCoreApplication::setOrganizationDomain("gruendger.de");

    // Translation
    QSettings settings;
    QString locale = QLocale::system().name();
    locale = locale.split("_").at(0);   // from de_DE to de

    QString savedLocale = settings.value("language").toString();
    QTranslator translator;
    translator.load(QString(":/translations/meisterschueler_") + (savedLocale.isEmpty() ? locale : savedLocale));
    a.installTranslator(&translator);

    // Application Icon
    //app.setWindowIcon(QIcon(":/icons/Media3.png"));

    MainWindow w;
    w.show();

    int result = a.exec();

    QGuidoPainter::stopGuidoEngine();
    
    return result;
}
