#ifndef DEFINES_H
#define DEFINES_H

//Imbuildable = Impossible to build
enum Buildings {NoBuilding, Imbuildable, RangeTower, MeleeTower, Upgrade, FrostTower};
enum DamageType {NoDamage = 0,
                 SingleTarget = 1,
                 AllInRange = 2,
                 Slow = 4,
                 IgnoreArmor = 8,
                 OneHitKill = 16};

inline DamageType operator | (DamageType lhs, DamageType rhs)
{
   return static_cast<DamageType>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

#endif // DEFINES_H
