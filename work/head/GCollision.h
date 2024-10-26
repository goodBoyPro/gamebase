#if !defined(GCOLLISION_H)
#define GCOLLISION_H
#include <GObject.h>
class CollisionInterface : public GObject {
  public:
    FVector position;
    FVector origin;
    float radius;

  public:
    CollisionInterface() { allCollisions.emplace(this); }
    virtual ~CollisionInterface() { allCollisions.erase(this); }
    virtual bool isStock() = 0;
    virtual bool test(CollisionInterface*other) = 0;
    static std::set<CollisionInterface *> allCollisions;
   
    FVector &getPosition() { return position; }
};


/// @brief ///////////////////////////////////////////////////////////
class GCollision : public CollisionInterface {
  private:
  public:
    GCollision() = default;
    GCollision(float radius_, FVector position_){
        position = position_;
        radius = radius_;
    }
    virtual bool test(CollisionInterface*other)override{
         if(nsg::getDistance(other->getPosition(),getPosition())<other->radius+radius)
                return true;
    }
    virtual bool isStock()override {
        for (auto c : allCollisions) {
            if(test(c))return 1;
        }
    }

  private:
};

#endif // GCOLLISION_H
