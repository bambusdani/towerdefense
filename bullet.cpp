#include "bullet.h"

Bullet::Bullet(QLineF line)
   : QGraphicsLineItem(line)
{
}

Bullet::~Bullet()
{
}

QRectF Bullet::boundingRect() const
{
   return QRectF(0,0,10,10);
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   //Draws a line as a bullet
   painter->setRenderHint(QPainter::Antialiasing, true);
   painter->drawLine(line());
}
