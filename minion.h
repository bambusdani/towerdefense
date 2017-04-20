#ifndef MINION_H
#define MINION_H

#include <QGraphicsItem>
#include <QPainter>

#include <QDebug>
#include <cmath>

class Minion : public QGraphicsItem
{
public:
   static unsigned long roundMultiply;
   static unsigned long startHp;
private:
   int currentPosition;
   double mHealthPoints;
   double mMaxHealthPoints;
   double mCurrency;
   double mStep;
   double mMoveTemp;

   double mArmorFactor;

   bool mDied;

public:
   Minion(double maxHP = 100);

   void move();

   virtual QRectF boundingRect() const;
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

   int getCurrentPos();

   void damage(double dmg, bool bypassArmor = false);
   double getHealthPoints();

   bool died();
   void die();

   void setStep(int step);
   double getCurrency();
};

#endif // MINION_H
