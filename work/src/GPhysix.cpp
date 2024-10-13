#include "GPhysix.h"
#include <GObject.h>

GPhysix::GPhysix()
{
}

GPhysix::~GPhysix()
{
}

void GPhysix::drop(float &z)
{
    if (tDrop.delay(10))
    {
        if (z > 0||speedZ>0)
        {
            speedZ=speedZ+gravity*0.01;
            z = z + speedZ * 0.01 + gravity *0.00005;
        }
        else {z=0;speedZ=0;}
    }
}
