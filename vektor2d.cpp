#include "vektor2d.h"

Vektor2d::Vektor2d(double posX, double posY, double angleX, double angleY, double scalar)
   : mPosX(posX),
     mPosY(posY),
     mAngleX(angleX),
     mAngleY(angleY),
     mScalar(scalar)
{
}

Vektor2d::~Vektor2d()
{

}

bool Vektor2d::move()
{
   mPosX += mAngleX*mScalar;
   mPosY += mAngleY*mScalar;

   return true;
}

void Vektor2d::rotate(double degrees)
{
   degrees = degrees * (3.1415926535/180);                         //deg to rad
   double x = (mAngleX * cos(degrees) - (mAngleY * sin(degrees)));
   double y = (mAngleY * cos(degrees) + (mAngleX * sin(degrees)));
   mAngleX = x;
   mAngleY = y;
}
