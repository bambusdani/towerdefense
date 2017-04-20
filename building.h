#ifndef BUILDING_H
#define BUILDING_H

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <cmath>

#include <QDebug>

#include "Defines.h"

class Building : public QGraphicsItem
{
private:
   int mCooldownFreq;
   int mCooldown;
   int mUpgrade;
   int mSlow;

   double mRange;
   double mDamagePerShoot;
   double mCostValue;
   double mCostBasic;

   Buildings mBuilding;
   DamageType mDamageType;
public:
   Building(Buildings type);

   void setBuilding(Buildings type, bool bypassCheck = false);
   Buildings getBuilding();
   DamageType getDamageType();

   virtual QRectF boundingRect() const;
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

   int getUpgrade();
   int getSlow();
   double getRange();
   double getDamage();
   double getCost();
   void cooldown();
   int getCooldown();
   void startCooldown();
protected:
   void mousePressEvent(QGraphicsSceneMouseEvent *event);
   void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BUILDING_H
