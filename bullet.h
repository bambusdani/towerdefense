#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsLineItem>

#include <QPainter>

class Bullet : public QGraphicsLineItem
{
private:
public:
   Bullet(QLineF line);
   ~Bullet();

   virtual QRectF boundingRect() const;
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // BULLET_H
