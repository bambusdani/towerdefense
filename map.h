#ifndef MAP_H
#define MAP_H

#include <QGraphicsView>

#include <QDebug>

#include "mapscene.h"

class Map : public QGraphicsView
{
private:
   Mapscene* mMapscene;

public:
   Map(int x, int y, int w, int h);

   Mapscene* getScene();
protected:
   bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAP_H
