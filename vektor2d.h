#ifndef VEKTOR2D_H
#define VEKTOR2D_H

#include <cmath>

class Vektor2d
{
public:
   Vektor2d(double posX = 0,
            double posY = 0,
            double angleX = 1,
            double angleY = 1,
            double scalar = 0);

   virtual ~Vektor2d();

   double mPosX;
   double mPosY;
   double mAngleX;
   double mAngleY;
   double mScalar;
   bool move();
   void rotate(double degrees);
};

#endif // VEKTOR2D_H
