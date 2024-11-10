#if !defined(GDBUG)
#define GDBUG
#include <GObject.h>
#include <mutex>
#include <set>
#define PRINTDEBUG(...)                                                        \
    {                                                                          \
        static GDebug __g__;                                                   \
        swprintf(__g__.wchar_, __VA_ARGS__);                                   \
    }                                                                          \
                                                                               \
// 已实现线程安全
class GDebug : public GObject {

  private:
    int yPosition = 0;
    int texHight = 10;

    void drawLog(int index);
    static std::mutex mtx;

  public:
    wchar_t wchar_[128] = {0};
    GDebug();
    virtual ~GDebug();
    static void debugDisplay();
};
extern std::set<GDebug *> debugs__;

#endif // GDBUG
