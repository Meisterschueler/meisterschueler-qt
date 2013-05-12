#include "bubblegraphicsitem.h"

#include <QPen>

void BubbleGraphicsItem::setPenColor(double value) {
    penColor = value;
    QPen pen;
    pen.setColor(calcColor(penColor));
    pen.setWidth(PENWIDTH);
    this->setPen(pen);
}

QColor BubbleGraphicsItem::calcColor(double value, double vMin, double vMax, double cMin, double cMax) {
    QColor color;

    double normValue = (qBound(vMin, value, vMax) - vMin) / (vMax-vMin);
    normValue = normValue*(cMax-cMin)+cMin;

    if (normValue >= 0.0 && normValue < 1.0)
        color = QColor(255*normValue, 0, 0);
    else if (normValue >= 1.0 && normValue < 2.0)
        color = QColor(255, 255*(normValue-1.0), 0);
    else if (normValue >= 2.0 && normValue < 3.0)
        color = QColor(255*(3.0-normValue), 255, 0);
    else if (normValue >= 3.0 && normValue < 4.0)
        color = QColor(0, 255, 255*(normValue-3.0));
    else if (normValue >= 4.0 && normValue < 5.0)
        color = QColor(0, 255*(5.0-normValue), 255);
    else if (normValue >= 5.0 && normValue < 6.0)
        color = QColor(0, 0, 255*(6.0-normValue));

    return color;
}
