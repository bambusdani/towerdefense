#include "minion.h"

unsigned long Minion::roundMultiply = 1;
unsigned long Minion::startHp = 100;

/**
 * This creates a minion
 * AND even some nice magic numbers are included here </irony>
 * @brief Minion::Minion
 * @param maxHP
 */
Minion::Minion(double maxHP)
   : currentPosition(0),
     mHealthPoints(startHp*roundMultiply*0.25),
     mMaxHealthPoints(startHp*roundMultiply*0.25),
     mCurrency(1.5*roundMultiply),
     mArmorFactor(1/(1*exp(roundMultiply*0.004))),
     mDied(false),
     mStep(1),
     mMoveTemp(0)
{
}

void Minion::move()
{
   //Moving the minion per step
   if(mMoveTemp == mStep)
   {
      currentPosition++;
      mMoveTemp = 0;
      mStep = 1;
   }
   mMoveTemp++;
}

QRectF Minion::boundingRect() const
{
   return QRectF(0, 0, 40, 40);
}

void Minion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
   //Minions turn from red to black, based on their armor multiplier
   painter->setBrush(QColor(255*mArmorFactor, 0, 0));
   painter->setRenderHint(QPainter::Antialiasing, true);
   painter->drawEllipse(0, 0, 40, 40);

   //Draw Healthbar
   double hpWidth = ((boundingRect().width()-2) / mMaxHealthPoints)*mHealthPoints;

   painter->setBrush(Qt::green);
   painter->drawRect(0, 0, hpWidth, 5);

   //Draw Health in numbers
   QString maxHp;
   maxHp = QString::number(mHealthPoints, 'g', 2);

   painter->setPen(Qt::white);
   painter->setFont(QFont("Arial", 7));
   painter->drawText(QPointF(boundingRect().width()/8, boundingRect().height()/2), maxHp);
}

int Minion::getCurrentPos()
{
   return currentPosition;
}

void Minion::damage(double dmg, bool bypassArmor)
{
   //If a minion gets damage it's health will be reduced
   if(!bypassArmor)
   {
      mHealthPoints -= dmg*mArmorFactor;
   }
   else
   {
      mHealthPoints -= dmg;
   }
}

double Minion::getHealthPoints()
{
   return mHealthPoints;
}

bool Minion::died()
{
   return mDied;
}

void Minion::die()
{
   mDied = true;
}

void Minion::setStep(int step)
{
   //We don't want to let a lower value be set here
   if(mStep <= step)
   {
      mStep = step;
   }
}

double Minion::getCurrency()
{
   return mCurrency;
}
