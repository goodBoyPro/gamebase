#if !defined(CLASSREG_H)
#define CLASSREG_H
namespace nsReg {
enum ActorType {
    eActorStatic = 1, //
    ePlayer = 2,
    eLandBlock=3
};
}; // namespace nsReg
class GActor *createActorFromJson(int type_);
#endif // CLASSREG_H
