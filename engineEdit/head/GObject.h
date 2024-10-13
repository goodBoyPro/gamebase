#ifndef GOBJECT_H
#define GOBJECT_H
#include "GBase.h"
class GObject {    

  public:
    GObject(/* args */)=default;

    ~GObject()=default;
    //std::atomic<bool> isValid;
    bool isValid = true;
};

#endif