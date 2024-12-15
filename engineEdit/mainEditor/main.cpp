#include "editorServer.h"
#include "gameEditor.h"
int main() {
    std::thread pad([]() { system("python eidtorPad.py"); });
    ens::Editor editor;
    EditorServer::server.close();
    pad.join();
    return 0;
}