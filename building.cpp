#include "building.h"

Building::Building(Buildings type)
{
   //Initialization
   mRange = 0;
   mDamagePerShoot = 0;
   mBuilding = type;
   mCooldown = 0;
   mCooldownFreq = 0;
   mCostValue = 0;
   mUpgrade = 1;
   mCostBasic = 0;
   mDamageType = NoDamage;

   setFlags(ItemIsSelectable);
}

/**
 * Build a new tower, or upgrade an existing
 * @brief Building::setBuilding
 * @param type
 * @param bypassCheck
 */
void Building::setBuilding(Buildings type, bool bypassCheck)
{
   //Bypass is only used to create the dummy towers
   if(!bypassCheck && (mBuilding == Imbuildable))
   {
      //We'd try to build a tower on a place where we can't do that
      return;
   }
   //This is not a building, OR the type to build is the same we got here
   //in that case, we upgrade this tower to another level
   else if(mBuilding == NoBuilding || mBuilding == type)
   {

//########### TODO ###########
//Maybe the upgrade variables should be stored inside the defines.h
//Would be easier to add a new tower type if this switch was a loop

      switch(type)
      {
      case RangeTower: //Build or Upgrade the rangetower here
         if(!bypassCheck)
         {
            //Rearrange specific tower stats here
            mRange = 200+mUpgrade*5;
            if(mRange > 300) mRange = 300;
            mDamagePerShoot = mUpgrade*20;
            mCooldownFreq = 30-mUpgrade;
            if(mCooldownFreq <= 1) mCooldownFreq = 1;
            if(mUpgrade == 1) mCooldown = 0;

            if(mUpgrade >= 150)
            {
               mDamageType = SingleTarget | IgnoreArmor | OneHitKill;
               mCooldownFreq = 100;
            }
            else
            {
               mDamageType = SingleTarget;
            }
         }
         mCostBasic = 15;
         mCostValue = mCostBasic*mUpgrade;
         break;
      case MeleeTower: //Same as above
         if(!bypassCheck)
         {
            mRange = 140+mUpgrade;
            if(mRange > 220) mRange = 220;
            mDamagePerShoot = 100+mUpgrade*50;
            mCooldownFreq = 80-mUpgrade*2;
            if(mCooldownFreq <= 3) mCooldownFreq = 3;
            if(mUpgrade == 1) mCooldown = 0;

            mDamageType = AllInRange;
         }
         mCostBasic = 45;
         mCostValue = mCostValue*mUpgrade;
         break;
      case FrostTower: //Same as above
         if(!bypassCheck)
         {
            mRange = 160+mUpgrade*2;
            if(mRange > 260) mRange = 260;
            mDamagePerShoot = mUpgrade*5;
            mCooldownFreq = 3*mUpgrade;
            if(mUpgrade == 1) mCooldown = 0;
            mSlow = 3*mUpgrade;

            mDamageType = AllInRange | Slow;
         }
         mCostBasic = 200;
         mCostValue = mCostValue*mUpgrade;
         break;
      default: //Unknown Tower type? Maybe an Error should occur!
         mRange = 0;
      }

      if(!bypassCheck) ++mUpgrade; //A dummy tower shouldn't be upgraded
      mBuilding = type;
   }
}

Buildings Building::getBuilding()
{
   return mBuilding;
}

DamageType Building::getDamageType()
{
   return mDamageType;
}

QRectF Building::boundingRect() const
{
   return QRectF(0, 0, 40, 40);
}

void Building::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   //Painting the tower
   painter->setRenderHint(QPainter::Antialiasing);

   QString str;
   switch(mBuilding)
   {
   case NoBuilding: //White stands for placeable tower
      painter->setBrush(Qt::white);
      if(isSelected())
      {
         painter->setBrush(Qt::green); //Specific place is selected with mouse
         painter->setOpacity(0.4);
      }
      break;

   case RangeTower:
      painter->setBrush(Qt::darkGreen); //RangeTower will be drawn dark green
      str = " Range +"+QString::number(mUpgrade)+", "+QString::number(mDamagePerShoot)+"\n "+ QString::number(getCost()); //Upgrade, damage and cost numbers will be drawn here
      break;

   case MeleeTower:
      painter->setBrush(Qt::yellow); //MeleeTower will be drawn dark green
      str = " Melee +"+QString::number(mUpgrade)+", "+QString::number(mDamagePerShoot)+"\n "+ QString::number(getCost());
      break;

   case FrostTower:
      painter->setBrush(Qt::red); //FrostTower tower will be drawn dark green
      str = " Frost +"+QString::number(mUpgrade)+", "+QString::number(mDamagePerShoot)+"\n "+ QString::number(getCost());
      break;

   case Imbuildable:
      painter->setBrush(Qt::black);
      painter->setOpacity(0.2);
      break;
   }

   //In case of an OnehitKill
   if(mDamageType & OneHitKill)
   {
      painter->setPen(QPen(Qt::black, 3));
   }

   //Final painting
   painter->drawRect(0, 0, 40, 40);
   painter->setFont(QFont("Arial", 8));
   painter->drawText(boundingRect(), str);

   //This shows the range when a tower is clicked
   if(isSelected())
   {
      painter->setOpacity(0.3);
      painter->drawEllipse(boundingRect().width()/2-mRange/2, boundingRect().height()/2-mRange/2, mRange, mRange);
      setZValue(8);
   }
   else
   {
      setZValue(0);
   }
}

int Building::getUpgrade()
{
   return mUpgrade;
}

int Building::getSlow()
{
   return mSlow;
}

double Building::getRange()
{
   return mRange;
}

double Building::getDamage()
{
   return mDamagePerShoot;
}

double Building::getCost()
{
   return mCostBasic*mUpgrade;
}

void Building::cooldown()
{
   //This sets the cooldown of every tower
   if(mCooldown <= 0)
   {
      mCooldown = 0;
      return;
   }
   mCooldown--;
}

int Building::getCooldown()
{
   return mCooldown;
}

void Building::startCooldown()
{
   mCooldown = mCooldownFreq;
}

void Building::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsItem::mousePressEvent(event);
}

void Building::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
   QGraphicsItem::mouseDoubleClickEvent(event);
}
