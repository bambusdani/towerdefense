#ifndef MAPSCENE_H
#define MAPSCENE_H

#define W_GRID 1200
#define H_GRID 600

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QLineF>

#include <QSlider>
#include <QKeyEvent>

#include <QDebug>

#include <iostream>
#include <vector>

#include <exception>

#include "minion.h"
#include "building.h"
#include "Defines.h"
#include "bullet.h"

class Control;

class Mapscene : public QGraphicsScene
{
   Q_OBJECT
private:
   int mGridSize;
   QGraphicsTextItem* mCurrency;

   Control* mControl;

   Building* mBuilding[3];

   Building* selectedItemTemp;
   QList<Minion *> mMinions;
   QList<Building*> mTower;

   QList<Bullet* > mLines;

   bool isBuildingClick(Building* ptr);

   void updateLines();
   void insertLine(QLineF line);
   void doTowerEffects(Building* tower, Minion* minion);
public:
   Mapscene(Control* control = 0);

   bool minion(QPointF startPos);
   void building();
   QPointF grid(int x, int y, Buildings type);

   Building* getBuilding(int x, int y);
   QList<Minion *>* getMinionListPtr();

   void mousePressEvent(QGraphicsSceneMouseEvent *event);
   void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
   void keyPressEvent(QKeyEvent* event);

   void setControl(Control* control);
public slots:
   void updateTimerEvent();
   void updateGraphic();
signals:
   void earnMoney(double money);
   void speedChange(int value);
};

#endif // MAPSCENE_H
