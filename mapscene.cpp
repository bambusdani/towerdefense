#include "mapscene.h"
#include "control.h"

/**
 * The default constructor for mapscene
 * @brief Mapscene::Mapscene
 * @param control
 */
Mapscene::Mapscene(Control* control)
   : mGridSize(50), mControl(control)
{
   building();
   selectedItemTemp = 0;

   //Adds the currency text to the GraphicsScene
   mCurrency = new QGraphicsTextItem;
   mCurrency->setPos(1030,120);
   this->addItem(mCurrency);

   //Add the speed slider widget to the GraphicsView
   QSlider* slider = new QSlider;
   slider->setMinimum(1);
   slider->setMaximum(500);
   slider->setOrientation(Qt::Horizontal);
   slider->setGeometry(1030, 250, 150, 30);
   addWidget(slider);

   //Connection to the slider to change gamespeed
   connect(slider, SIGNAL(sliderMoved(int)), this, SIGNAL(speedChange(int)));
}

/**
 * Adds a new minion at it's starting position
 * @brief Mapscene::minion
 * @param startPos
 */
bool Mapscene::minion(QPointF startPos)
{
   if(mMinions.size() >= 150) return false;
   Minion* minion = new Minion();
   minion->setPos(startPos);
   minion->setZValue(10); //To be secure that the building is in the foreground
   mMinions.push_back(minion);
   this->addItem(minion);

   return true;
}

/**
 * Here are the dummy towers created
 * @brief Mapscene::building
 */
void Mapscene::building()
{
   //Create the MeleeTower dummy
   mBuilding[0] = new Building(MeleeTower);
   mBuilding[0]->setPos(1030,20); //Positioning
   this->addItem(mBuilding[0]);

   //And the RangeTower dummy
   mBuilding[1] = new Building(RangeTower);
   mBuilding[1]->setPos(1080,20); //Positioning
   this->addItem(mBuilding[1]);

   //FrostTower dummy
   mBuilding[2] = new Building(FrostTower);
   mBuilding[2]->setPos(1030,70); //Positioning
   this->addItem(mBuilding[2]);

   //Sets the dummy's into their required state
   //Bypass is on, check setBuilding if you wanna know what that means
   mBuilding[0]->setBuilding(MeleeTower, true);
   mBuilding[1]->setBuilding(RangeTower, true);
   mBuilding[2]->setBuilding(FrostTower, true);
}

/**
 * This method is used to create the gamefield grid
 * @brief Mapscene::grid
 * @param x
 * @param y
 * @param type
 * @return
 */
QPointF Mapscene::grid(int x, int y, Buildings type)
{
   //Create a new Building, set the position to the specific coordinates
   Building* building = new Building(type);
   building->setPos(QPointF(x*building->boundingRect().width(), y*building->boundingRect().height()));
   addItem(building);

   //Each possible Tower is added into mTower
   if(type != Imbuildable)
   {
      mTower.push_back(building);
   }

   return building->pos();
}

/**
 * This method gives direct access to grid items by their coordinates
 * It's used when the map is loaded
 * @brief Mapscene::getBuilding
 * @param x
 * @param y
 * @return
 */

Building *Mapscene::getBuilding(int x, int y)
{
   return static_cast<Building*>(itemAt(QPointF(x*40, y*40), QTransform()));
}

QList<Minion *> *Mapscene::getMinionListPtr()
{
   return &mMinions;
}

void Mapscene::updateLines()
{
   //Updates and deletes the Bullets
   if(mLines.size() != 0)
   {
      for(int i = 0; i < mLines.size(); i++)
      {
         //Reduce the opacity of the Line
         mLines.at(i)->setOpacity(mLines.at(i)->opacity()-0.2);

         //Remove old lines
         if(mLines.at(i)->opacity() <= 0)
         {
            removeItem(mLines.at(i));
            delete mLines.at(i);
            mLines.removeAt(i);
            i--;
         }
      }
   }
}

void Mapscene::insertLine(QLineF line)
{
   if(mLines.size() < 2000) //Some performance security here
   {
      mLines.push_back(new Bullet(line.translated(20, 20)));
      mLines.back()->setZValue(15);
      addItem(mLines.back());
   }
}

void Mapscene::doTowerEffects(Building* tower, Minion* minion)
{
   //Slow Effect
   if(tower->getDamageType() & Slow)
      minion->setStep(tower->getSlow()); //Slow the minion

   bool bypass = false; //Bypass the armor?
   if(tower->getDamageType() & IgnoreArmor) bypass = true;

   //Shoot this minion
   minion->damage(tower->getDamage(), bypass);

   //One Hit Kill
   if(tower->getDamageType() & OneHitKill)
   {
      minion->damage(minion->getHealthPoints(), true);
   }
}

void Mapscene::updateTimerEvent()
{
   updateLines();

   if(mTower.size() != 0)
   {
      //This is the shoot cooldown
      for(int i = 0; i < mTower.size(); i++)
      {
         mTower.at(i)->cooldown();
      }

      //If there are Minions, we check if any tower will do damage to them
      if(mMinions.size() != 0)
      {
         //Protection
         try
         {
            //For each tower
            for(int i = 0; i < mTower.size(); i++)
            {
               //Tower has no cooldown, and is able to do damage
               if(mTower.at(i)->getCooldown() != 0 || mTower.at(i)->getDamageType() & NoDamage) continue;

               QPointF towerPos = mTower.at(i)->pos();
               QList<QPair<Minion*, QLineF> > minionsInRange;

               //We'll iterate all minions and check the distance between the tower and the minion
               for(int j = 0; j < mMinions.size(); j++)
               {
                  //Check if the minion is already dead
                  if(mMinions.at(j)->getHealthPoints() <= 0) continue;

                  QPointF minionPos = mMinions.at(j)->pos();
                  QLineF line(towerPos, minionPos); //Calculates a line between tower and minion

                  double length = (line.x1()-line.x2())*(line.x1()-line.x2())+
                        (line.y1()-line.y2())*(line.y1()-line.y2());

                  double radius = (mTower.at(i)->getRange()*mTower.at(i)->getRange())/4;
                  //If the line length is shorter than the range radius
                  if(length <= radius)
                  {
                     //Add this minion to the array of possible targets of the tower
                     minionsInRange.push_back(QPair<Minion*, QLineF>(mMinions.at(j), line));
                  }
               }

               //Check if we got minions in range of the tower
               if(minionsInRange.size() != 0)
               {
                  if(mTower.at(i)->getDamageType() & AllInRange) //Damage to ALL minions in range
                  {
                     //Shoot all the minions in range
                     for(int j = 0; j < minionsInRange.size(); j++)
                     {
                        insertLine(minionsInRange.at(j).second);
                        doTowerEffects(mTower.at(i), minionsInRange.at(j).first);
                     }
                     mTower.at(i)->startCooldown(); //Finally start the cooldown
                  }
                  else if(mTower.at(i)->getDamageType() & SingleTarget) //Single minion damage to random Target in range
                  {
                     int anz = minionsInRange.size();
                     int index = 0;
                     if(anz > 1) //We got more than one minion in range, so choose one randomized
                     {
                        index = qrand()%(minionsInRange.size()-1);
                     }

                     insertLine(minionsInRange.at(index).second);
                     doTowerEffects(mTower.at(i), minionsInRange.at(index).first);

                     mTower.at(i)->startCooldown();
                  }
               }
            }
         }
         catch(std::exception &e)
         {
            //Something went terribly wrong
            std::cerr << "Mapscene::updateTimerEvent " << e.what();
         }

         //Earn money for the killed minions
         for(int i = 0; i < mMinions.size(); i++)
         {
            if(mMinions.at(i)->getHealthPoints() <= 0 && !mMinions.at(i)->died())
            {
               mMinions.at(i)->die();
               emit earnMoney(mMinions.at(i)->getCurrency());
            }
         }
      }
   }

   //There are minions left
   if(mMinions.size() != 0)
   {
      QList<Minion*> tmp;

      for(int i = 0; i < mMinions.size(); i++)
      {
         if(!mMinions.at(i)->died()) //This minion is still alive
         {
            tmp.push_back(mMinions.at(i));
         }
         else //This not.
         {
            removeItem(mMinions.at(i));
            delete mMinions.at(i);
         }
      }

      mMinions = tmp; //Update the minion list
   }


}

void Mapscene::updateGraphic()
{
   //Pointer check, well, not really required cause without the pointer, nothing would work
   //However, more security!
   if(mControl != 0)
   {
      //Updates current currency, life, spawned minions and the current round
      mCurrency->setPlainText("Currency: " +
                              QString::number(mControl->getCurrency()) +
                              "\nLife: " +
                              QString::number(mControl->getLife()) +
                              "\nMinions: " +
                              QString::number(mMinions.size()) +
                              "\nSpawned Minions: " +
                              QString::number(mControl->getSpawnedMinions()) +
                              "\nRound: " + QString::number(mControl->getRound()));
   }

   update();
}

bool Mapscene::isBuildingClick(Building* ptr)
{
   //A dummy tower is selected
   return (ptr == mBuilding[0] || ptr == mBuilding[1] || ptr == mBuilding[2]);
}

void Mapscene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsScene::mousePressEvent(event);

   //If the alt key is pressed the event will be redirected to the mouseDoubleClickEvent
   //which will basically upgrade the selected building (if one is selected)
   if(event->modifiers() == Qt::AltModifier)
   {
      mouseDoubleClickEvent(event);
      return;
   }

   QList<QGraphicsItem *> selectedItem = selectedItems();

   //Some protection here
   try
   {
      //We will only proceed if there is an item selected
      if(selectedItem.size() != 0)
      {
         //Here we are going to build a new tower
         //It's designed like this:
         //First: Click on a free field
         //Second: Click on the tower that should be built

         //This is the first step of building a tower, there is no item selected
         //OR the selected field is not an dummy tower
         if(selectedItemTemp == 0 || !isBuildingClick(static_cast<Building*>(selectedItem.first())))
         {
            //Save the field position
            selectedItemTemp = static_cast<Building*>(selectedItem.first()); //Temp pointer to Item
            //qDebug() << "Selected item: " << selectedItemTemp;
         }

         //Here we go into the second step of building a tower
         //There is actually a tower selected AND the tower to build we choose is not the field itself
         if(selectedItemTemp != 0 && static_cast<Building*>(selectedItem.first()) != selectedItemTemp)
         {
            //Following the final step to build the tower
            //Here we're going to build a tower of the type of the dummy tower

            //Temp ptr to the clicked item
            Building* ptr = static_cast<Building*>(selectedItem.first());

            //We clicked on an dummy tower
            if(isBuildingClick(ptr))
            {
               //Simply build this
               if(mControl->getCurrency() >= ptr->getCost()*selectedItemTemp->getUpgrade())
               {
                  //By setting the type of the Building we've built the tower
                  selectedItemTemp->setBuilding(ptr->getBuilding());
                  //And the last step is to reduce the currency
                  mControl->spendCurrency(ptr->getCost()*(selectedItemTemp->getUpgrade()-1));
               }
            }

            //Reset the state of selectetItemTemp to reenable the build method
            selectedItemTemp = 0;
         }
      }
   }
   catch(std::exception &e)
   {
      std::cerr << e.what();
   }

   update();
}

void Mapscene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsScene::mouseDoubleClickEvent(event);
   QList<QGraphicsItem *> selectedItem = selectedItems();

   //Some items are selected
   if(selectedItem.size() != 1) return;

   //Protect this from exceptions
   //If errors occur here, it may be caused by the ptr cast
   try
   {
      //Cast the selected item into a Building ptr
      Building* selectedBuilding = static_cast<Building*>(selectedItem.front());

      //The selected building is the dummy building
      if(isBuildingClick(selectedBuilding)) return;

      //If shift is pressed, the tower will be upgraded by a maximum of 20 levels
      int cnt = 1;
      if(event->modifiers() == Qt::ShiftModifier) cnt = 20;

      //Upgrades the selected building cnt times, as far as there is enough currency
      for(int i = 0; i < cnt; i++)
      {
         //Checks if there is enough currency to upgrade the tower
         if(mControl->getCurrency() >= selectedBuilding->getCost())
         {
            //Reduces the currency, managed by the control class and finally upgrades the tower
            mControl->spendCurrency(selectedBuilding->getCost());
            selectedBuilding->setBuilding(selectedBuilding->getBuilding());
         }
         else
         {
            //There is not enough currency to upgrade the tower
            //Loop is interrupted here
            break;
         }
      }
   }
   catch(...)
   {
      //Hopefully this code will never be reached
      //If so, something went totally wrong
      std::cerr << "Mapscene::mouseDoubleClickEvent(): Something went totally wrong here!";
   }
}

void Mapscene::keyPressEvent(QKeyEvent *event)
{
   //Start/Stop
   //If the P key is pressed it stops the timer
   //If the P key is pressed again is starts the timer
   if(event->key() == Qt::Key_P && mControl->getTimer()->isActive())
   {
      mControl->getTimer()->stop();
   }
   else
   {
      mControl->getTimer()->start();
   }
}

/**
 * @brief Mapscene::setControl
 * This method is extremely important, without setting the mControl pointer to
 * our control object, nothing will ever work
 * @param control
 */
void Mapscene::setControl(Control *control)
{
   //It's needed to use control methods in this class (mapscene)
   mControl = control;
}
