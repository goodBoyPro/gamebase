#if !defined(GDBUG)
#define GDBUG
#include <GObject.h>
#include<mutex>
#include <set>
#define PRINTDEBUG(...)                \
  {static GDebug __g__;                 \
  swprintf(__g__.wchar_, __VA_ARGS__);} \
                                       \
// 示例： static GDebug gddd;
// swprintf(gddd.wchar_, L"场景对象数量：%d", actors.size());
class GDebug : public GObject
{
private:
  /* data */
public:
  GDebug();

  virtual ~GDebug();
  int yPosition = 0;
  int texHight = 10;
  wchar_t wchar_[128] = {0};
  void drawLog(int index);
  static void clearDebugs();
  static void debugDisplay();
  static std::mutex mtx;
};
extern std::set<GDebug *> debugs__;

#endif // GDBUG
