#include "map.h"

Map::Map(int x, int y, int w, int h)
{
   //Sets x-position, y-position, width and height of the map
   this->setGeometry(x,y,w,h);
   //Disables the scroll bars
   this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

   mMapscene = new Mapscene();
   mMapscene->setSceneRect(-x,y,w,h);
   this->setScene(mMapscene);
}

Mapscene *Map::getScene()
{
   return mMapscene;
}

bool Map::eventFilter(QObject *obj, QEvent *event)
{
   //This had to be done to prevent moving the map with the mouse wheel
   if(event->type() == QEvent::Wheel)
   {
      return true;
   }
   return false;
}
