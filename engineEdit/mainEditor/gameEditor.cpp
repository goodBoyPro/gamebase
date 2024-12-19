#include <gameEditor.h>
namespace ens {
void Editor::setCommand() {
    editorCommand::edc.command["setmode"] = []() {
        editMode = std::stoi(editorCommand::edc.input[1]);
        MovableEditObj::selectedObjForEdit = nullptr;
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    editorCommand::edc.command["save"] = []() {
        editorSave::saveTofile(editorCommand::edc.input[1]);
        const std::string &message = nsg::combineStrings(
            {"success save to ", editorCommand::edc.input[1]});
        EditorServer::server.sendMesssage(message);
    };
    editorCommand::edc.command["open"] = []() {
        MovableEditObj::clearAllMeo();
        editorSave::loadFromFile(editorCommand::edc.input[1]);
        const std::string &message = nsg::combineStrings(
            {"success save to ", editorCommand::edc.input[1]});
        EditorServer::server.sendMesssage(message);
    };
    editorCommand::edc.command["newWorld"] = []() {
        MovableEditObj::clearAllMeo();
        EditorServer::server.sendMesssage("editor reset");
    };
     editorCommand::edc.command["copy"] = []() {
        if(MovableEditObj::selectedObjForEdit){}
    };
     editorCommand::edc.command["paste"] = []() {
       
    };
     editorCommand::edc.command["delete"] = []() {
        if(MovableEditObj::selectedObjForEdit){
            MovableEditObj::selectedObjForEdit->destroy();
            MovableEditObj::selectedObjForEdit = nullptr;
        }
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    editorCommand::edc.command["create"] = [&]() {
        int fileid = std::stoi(editorCommand::edc.input[1]);
        if (!getSource().checkTexArrayValid(fileid)) {
            EditorServer::server.sendMesssage("server:no such fileId");
            return;
        }
        if (getSource().checkTexArrayType(fileid) != nsReg::eActor &&
            editMode == ACTORMODE) {
            EditorServer::server.sendMesssage("server:not actor");
            return;
        }
        if (getSource().checkTexArrayType(fileid) != nsReg::eLandBlock &&
            editMode == LANDMODE) {
            EditorServer::server.sendMesssage("server:not landblock");
            return;
        }
        FVector3 size = {std::stof(editorCommand::edc.input[4]),
                         std::stof(editorCommand::edc.input[5]),
                         std::stof(editorCommand::edc.input[6])};
        nsReg::ActorType type = static_cast<nsReg::ActorType>(
            std::stoi(editorCommand::edc.input[7]));
        switch (type) {
        case 1:
            printf("create 1");
            break;
        default:
            break;
        }

        if (editMode == ACTORMODE)
            for (int i = 0; i < std::stoi(editorCommand::edc.input[3]); i++) {
                MovableEditObj *obj = createAtLocation(
                    new Actor(fileid, std::stoi(editorCommand::edc.input[2])),
                    WindowFlag::flag.posInWs);
                obj->setSizeWS(size);
                MovableEditObj::selectedObjForEdit = obj;
            }
        if (editMode == LANDMODE)
            for (int i = 0; i < std::stoi(editorCommand::edc.input[3]); i++) {
                MovableEditObj *obj = createAtLocation(
                    new LandBlock(fileid,
                                  std::stoi(editorCommand::edc.input[2])),
                    WindowFlag::flag.posInWs);
                MovableEditObj::selectedObjForEdit = obj;
            }
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    editorCommand::edc.command["createDefaultLand"] = [&]() {
        FVector3 bp_ = {std::stof(editorCommand::edc.input[1]),
                        std::stof(editorCommand::edc.input[2]),
                        std::stof(editorCommand::edc.input[3])};
        float blocksize_ = std::stof(editorCommand::edc.input[4]);
        int rows_ = std::stoi(editorCommand::edc.input[5]);
        int columns_ = std::stoi(editorCommand::edc.input[6]);
        LandBlock::setLandOptions(bp_, blocksize_, rows_, columns_);
        LandBlock::spawnRandomLandblocks();
        EditorServer::server.sendMesssage("land created");
    };
};
}; // namespace ens