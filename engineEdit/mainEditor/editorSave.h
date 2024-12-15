#if !defined(EDITORSAVE_H)
#define EDITORSAVE_H
#include <fstream>
#include <nlohmann_json/json.hpp>
namespace ens {
class editorSave {
  private:
    /* data */
  public:
    editorSave(/* args */);
    ~editorSave();
    static void saveTofile(std::string path_);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////
inline editorSave::editorSave(/* args */) {}

inline editorSave::~editorSave() {}

} // namespace ens

#endif // EDITORSAVE_H
