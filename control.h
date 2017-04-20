#ifndef CONTROL_H
#define CONTROL_H

#define W_MAP 1200
#define H_MAP 600

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QPair>

#include <QVector>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>


#include <stdexcept>

#include <iostream>

#include "map.h"
#include "mapscene.h"
#include "Defines.h"
#include "minion.h"

class Control : public QObject
{
   Q_OBJECT
private:
   unsigned long step;
   unsigned long round;
   unsigned long spawnedMinions;

   Map* mMap;
   Mapscene* mScene;
   QTimer mTimer;
   QList<QPointF> mWaypoints;
   double mCurrency;
   double mCurrentLife;

   class cord
   {
   public:
      int x;
      int y;
      cord(int x=0, int y=0) : x(x), y(y) {}

   };

   QVector<cord> path;

   void loadMap(QString pfad);

   int speed;
public:
   Control();

   void createView(QWidget *parent = 0);
   void createGrid();
   void smoothWaypoints();

   double getLife();
   double getCurrency();
   int getRound();
   void spendCurrency(double value);
   unsigned long getSpawnedMinions();
   QTimer* getTimer();
signals:

public slots:
   void timerEvent();
   void earnMoney(double money);
   void changeSpeed(int value);
};

#endif // CONTROL_H
