#if !defined(GCOLLISION_H)
#define GCOLLISION_H
#include <GDebug.h>
#include <GObject.h>

class polygon {
  private:
    float radius;
    int count;

  public:
    sf::ConvexShape *shape=nullptr;;
    ~polygon(){
       delete shape;};
    polygon(int count, float radius_, FVector pos_) {
      shape=new sf::ConvexShape;
        shape->setFillColor(sf::Color(255, 1, 1, 0));
        shape->setOutlineColor(sf::Color(255, 255, 255));
        shape->setOutlineThickness(2);
        shape->setPointCount(count);
        shape->setPosition(pos_.x, pos_.y);
        float angle = 2 * 3.1415926 / count;
        for (int i = 0; i < count; i++) {
            shape->setPoint(i, {sin(angle * i) * radius_ / pixSize,
                               cos(angle * i) * radius_ / pixSize});
        }
    }
    void setshape(int count, float radius_, FVector pos_) {
        shape->setPointCount(count);
        shape->setPosition(pos_.x, pos_.y);
        float angle = 2 * 3.1415926 / count;
        for (int i = 0; i < count; i++) {
            shape->setPoint(i, {sin(angle * i) * radius_ / pixSize,
                               cos(angle * i) * radius_ / pixSize});
        }
    }
    
    
};
class CollisionInterface : public GObject {
  private:
    FVector position;
    FVector origin;
    float radius = 1;
    bool isValidCollision = 1;
    polygon *poly = nullptr;

  public:
    CollisionInterface() {
        allCollisions.emplace(this);
        poly = new polygon(12, 1, getPosition());
        
    }
    virtual ~CollisionInterface() {
        allCollisions.erase(this);
        delete poly;
    }
    virtual bool isStock() = 0;
    virtual bool test(CollisionInterface *other) = 0;
    static std::set<CollisionInterface *> allCollisions;
    static void showCollisions() {
        for (auto c : allCollisions) {
          IVector pos = wsToWin(c->position);
            c->poly->shape->setPosition(pos.x, pos.y);
            getWindow()->draw(*c->poly->shape);
            
        }
    }
    FVector &getPosition() { return position; }
    void setPosition(const FVector &position_) {
        position = position_;
        IVector pos = wsToWin(position_);
        poly->shape->setPosition(pos.x, pos.y);
    }
    float getRadius() { return radius; }
    void setRadius(float radius_) {
        radius = radius_;
        poly->setshape(12, radius_, getPosition());
        
    }
    void setScale(float x_,float y_){poly->shape->setScale(x_,y_);}
    void setCollisionOn(bool collisionOn) { isValidCollision = collisionOn; }
    bool isCollisionOn() { return isValidCollision; }
};

/// @brief ///////////////////////////////////////////////////////////
class GCollision : public CollisionInterface {
  private:
  public:
    GCollision() = default;
    GCollision(float radius_, FVector position_) {
        setPosition(position_);
        setRadius(radius_);
    }
    virtual bool test(CollisionInterface *other) override {

        if (nsg::getDistance(other->getPosition(), getPosition()) <
            other->getRadius() + getRadius()) {
            return true;
        } else
            return false;
    }
    virtual bool isStock() override {
        for (auto c : allCollisions) {
            if (this == c)
                continue;
            if (test(c))
                return 1;
        }
        return 0;
    }

  private:
};

#endif // GCOLLISION_H