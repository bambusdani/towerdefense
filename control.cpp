#include "control.h"

Control::Control()
   : mCurrency(75), mCurrentLife(50), speed(1), spawnedMinions(0)
{
   //Initialization
   step = 0;
   round = 1;
   mTimer.setInterval(10);

   //Connects the timer with the timerEvent to get the game started
   connect(&mTimer, SIGNAL(timeout()), this, SLOT(timerEvent()));
}

/**
 * @brief Control::loadMap
 * Loads a map from an .txt file
 * Format: x;y
 */
void Control::loadMap(QString pfad = "map.txt")
{
   //Some protection
   try
   {
      QFile file(pfad);

      //Try to open the file
      //Will abort if the file isn't in the right path
      if(!file.open(QFile::ReadOnly))
      {
         throw std::runtime_error("Map couldn't loaded! Check file! Path-Error");
      }
      else
      {
         //Try to read the file
         QString str(file.readAll());

         //If the file doesn't contain data, it will also abort
         if(str.isEmpty())
         {
            throw std::runtime_error("Map couldn't loaded! Check file! File-Empty");
         }
         else
         {
            QStringList list = str.split("\n");

            //This loop will read the single coordinates and check the validity
            //If there are not exactly 2 coordinates per line, the loading process will be aborted
            for(int i = 0; i < list.size(); i++)
            {
               QStringList coord = list.at(i).split(";");
               if(coord.size() != 2) throw std::runtime_error("Map couldn't loaded! Check file! Invalid-Data");
               path.push_back(cord(coord.at(0).toInt(), coord.at(1).toInt()));
            }
         }
      }
   }
   catch(std::runtime_error &e)
   {
      //Print the error message into the console stream
      std::cerr << e.what();

      //A messagebox will appear which lets the user decide to close the programm
      //or search a map file
      QMessageBox msg(QMessageBox::Critical,
                      "Error!",
                      e.what(),
                      QMessageBox::Close | QMessageBox::Open);
      int ret = msg.exec();

      switch(ret)
      {
      case QMessageBox::Close:
         //This will close the programm in a correct way
         std::exit(2);
         break;
      case QMessageBox::Open:
         //open dialog

         QString fileName = QFileDialog::getOpenFileName(0, tr("Open Map"), "./", tr("Map File (*.txt)"));
         loadMap(fileName); //Call this method again
         break;
      }
   }
   catch(...) //This code should never be reached
   {
      std::cerr << "Something unexpected happened here Control::loadMap()";
      std::exit(2);
   }
}

/**
 * This is basically the initialization of the game
 * @brief Control::createView
 * @param parent
 */
void Control::createView(QWidget *parent)
{
   //Creates the main game frame
   mMap = new Map(0,0,W_MAP,H_MAP);
   mMap->setParent(parent);
   mMap->show();

   //At this point we want to load a map
   loadMap();

   //Here we do a pointer exchange
   mScene = mMap->getScene();
   mScene->setControl(this);

   //Create the grid fields
   createGrid();

   //This method will smooth the coordinates
   //See the method for more information
   smoothWaypoints();

   //And the game will begin
   mTimer.start();

   //Some connections for changing speed and earn money
   connect(mScene, SIGNAL(speedChange(int)), this, SLOT(changeSpeed(int)));
   connect(mScene, SIGNAL(earnMoney(double)), this, SLOT(earnMoney(double)));
}

/**
 * Create the grid
 * @brief Control::createGrid
 */
void Control::createGrid()
{
   //Scene is not Valid
   if(mScene == 0) throw std::runtime_error("Control::createGrid mScene is not set correct!");

   //This loop will create the gamefield with some magic numbers 25x15
   for(int x=0; x < 25; x++)
   {
      for(int y=0; y < 15; y++)
      {
         mScene->grid(x,y,NoBuilding);
      }
   }

   //The path will be set here
   for(int i = 0; i < path.size(); i++)
   {
      mScene->getBuilding(path[i].x, path[i].y)->setBuilding(Imbuildable);
      mWaypoints.push_back(QPointF(path[i].x*40, path[i].y*40));
   }
}

/**
 * This will smooth waypoints to distances of a maximum of 1 px
 * @brief Control::smoothWaypoints
 */
void Control::smoothWaypoints()
{
   if(mWaypoints.size() <= 1) return; //There are no Waypoints to smooth

   QList<QPointF> tmp;

   tmp.push_back(mWaypoints.at(0));

   for(int i=0; i < mWaypoints.size()-1; i++)
   {
      QPointF wp = mWaypoints.at(i);
      QPointF diff = (mWaypoints.at(i+1)-wp)/40;

      for(int j=1; j <= 40; j++)
      {
         tmp.push_back(wp+(j*diff));
      }
   }

   mWaypoints = tmp;
}

double Control::getLife()
{
   return mCurrentLife;
}

double Control::getCurrency()
{
   return mCurrency;
}

int Control::getRound()
{
   return round;
}

void Control::spendCurrency(double value)
{
   mCurrency -= value;
}

unsigned long Control::getSpawnedMinions()
{
    return spawnedMinions;
}

QTimer *Control::getTimer()
{
    return &mTimer;
}

void Control::changeSpeed(int value)
{
   //Changes the game speed
   if(value > 0)
   {
      speed = value;
   }
}

/**
 * The timerEvent is the main game update method
 * @brief Control::timerEvent
 */
void Control::timerEvent()
{
   for(int k = 0; k < speed; k++)
   {
      ++step;
      QList<Minion*>* minionList = mScene->getMinionListPtr();

      //Here we move existing minions and we check if they've reached the last coordinates
      for(int i = 0; i < minionList->size(); i++)
      {
         minionList->at(i)->move();
         int curPos = minionList->at(i)->getCurrentPos();

         //The minion is atleast on the last Waypoint
         if(curPos >= mWaypoints.size())
         {
            //Reduce the lifes the player has
            --mCurrentLife;
            //This minion will have to die
            minionList->at(i)->die();

            //If there are no lifes left, the player has lost the game
            if(mCurrentLife <= 0)
            {
               mTimer.stop();
               int ret = QMessageBox::information(0,
                                                  tr("Lost"),
                                                  tr("You lost the game...\nRound: " )+ QString::number(round));

               //Maybe add a restart option here
               if(ret == QMessageBox::Ok)
               {
                  //Exit the program
                  std::exit(0);
               }
            }
         }
         else
         {
            //The minion is not at the last waypoint, so set a new position for it
            minionList->at(i)->setPos(mWaypoints.at(curPos));
         }
      }

      //Add a new minion
      int spawndiff = 50 - static_cast<int>(round/20.0);

      if(spawndiff <= 5) spawndiff = 5;

      if(step%spawndiff == 0)
      {
         if(mScene->minion(mWaypoints.at(0)))
         {
            spawnedMinions++;
         }
      }

      if(step%500 == 0)
      {
         step = 0;
         round++; //Next round
         Minion::roundMultiply = round; //More minion health

         if(round%5 == 0)
         {
            Minion::startHp += 100; //Much more minion health
            mCurrency += round*20; //Add currency
         }
      }

      //Update the scene
      mScene->updateTimerEvent();
   }
   //Redraw the graphic
   mScene->updateGraphic();
}

void Control::earnMoney(double money)
{
   mCurrency += money;
}
