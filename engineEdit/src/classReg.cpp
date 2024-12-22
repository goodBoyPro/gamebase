#include "classReg.h"
#include <GActor.h>
#include <GActorStatic.h>
#include <GLandBlock.h>
GActor *createActorFromJson(int type_) {
    nsReg::ActorType type = (nsReg::ActorType)type_;
    GActor *actor = nullptr;
    switch (type) {
    case nsReg::eActorStatic:
        actor = new GActorStatic();
        break;
    case nsReg::eLandBlock:
        actor = new GLandBlock();
        break;
    default:
        break;
    }
    return actor;
}
