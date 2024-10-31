#ifndef GOBJECT_H
#define GOBJECT_H
#include "GBase.h"
class GObject
{

public:
  GObject(/* args */) = default;

  virtual ~GObject() = default;
  // std::atomic<bool> isValid;
  bool isValid = true;
};
class GComponent : public GObject
{
};



#endif