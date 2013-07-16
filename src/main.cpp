#include <QApplication>

#include <QGuidoPainter.h>
#include <QGuidoWidget.h>

int main(int argc, char *argv[])
{
    QGuidoPainter::startGuidoEngine();

    QApplication a(argc, argv);
    QGuidoWidget w;
    w.setGMNCode("[c d e f g]", "");
    w.show();

    int result = a.exec();

    QGuidoPainter::stopGuidoEngine();

    return result;
}
