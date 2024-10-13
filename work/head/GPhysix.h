#if !defined(GPHYSIX_H)
#define GPHYSIX_H
#include <GObject.h>
class GPhysix
{
private:
    /* data */
public:
    float gravity =-9.8f;
    float speedZ;

    GPhysix(/* args */);
    ~GPhysix();
    class canRun tDrop;
    void drop(float& z);
};

#endif // GPHYSIX_H
