#include <QApplication>
#include "mainwindow.h"

#include <QGuidoPainter.h>
#include <QGuidoWidget.h>

#include "guidoservice.h"

int main(int argc, char *argv[])
{
    QGuidoPainter::startGuidoEngine();

//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    QApplication a(argc, argv);
    QGuidoWidget w;
    w.setGMNCode("[c d e f g]");
    w.show();

    int result = a.exec();

    QGuidoPainter::stopGuidoEngine();

    return result;
}
