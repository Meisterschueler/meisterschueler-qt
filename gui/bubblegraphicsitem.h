#ifndef BUBBLEGRAPHICSITEM_H
#define BUBBLEGRAPHICSITEM_H

#include <QGraphicsEllipseItem>

class BubbleGraphicsItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
    Q_PROPERTY( double penColor READ getPenColor WRITE setPenColor )
    Q_PROPERTY( QRectF rect READ rect WRITE setRect )
    Q_PROPERTY( qreal opacity READ opacity WRITE setOpacity )

    static const int PENWIDTH = 3;

public:  
    double getPenColor() const { return penColor; }
    void setPenColor( double value );

private:
    double penColor;

    QColor calcColor(double value, double vMin = 0.0, double vMax = 1.0, double cMin = 1.0, double cMax = 5.0);
    
};

#endif // BUBBLEGRAPHICSITEM_H
