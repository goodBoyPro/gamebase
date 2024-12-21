#include <gameEditor.h>
namespace ens {
void Editor::setCommand() {
    editorCommand::edc.command["setmode"] = []() {
        editMode = std::stoi(editorCommand::edc.input[1]);
        std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
        MovableEditObj::selectedObjs.clear();
        lk.unlock();
        // SelectRect::rect.bDraw = false;
        // SelectRect::rect.bSetBegin = true;
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
        const std::string &message =
            nsg::combineStrings({"success open ", editorCommand::edc.input[1]});
        EditorServer::server.sendMesssage(message);
    };
    editorCommand::edc.command["newWorld"] = []() {
        MovableEditObj::clearAllMeo();
        EditorServer::server.sendMesssage("editor reset");
    };
    editorCommand::edc.command["copy"] = []() {

    };
    editorCommand::edc.command["paste"] = []() { int a = 1; };
    editorCommand::edc.command["delete"] = []() {
        std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
        for (MovableEditObj *meo : MovableEditObj::selectedObjs) {
            meo->destroy();
        }
        MovableEditObj::selectedObjs.clear();
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
        case nsReg::eActor:
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
                std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
                MovableEditObj::selectedObjs.insert(obj);
                lk.unlock();
            }
        if (editMode == LANDMODE)
            for (int i = 0; i < std::stoi(editorCommand::edc.input[3]); i++) {
                MovableEditObj *obj = createAtLocation(
                    new LandBlock(fileid,
                                  std::stoi(editorCommand::edc.input[2])),
                    WindowFlag::flag.posInWs);
                std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
                MovableEditObj::selectedObjs.insert(obj);
                lk.unlock();
            }
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    editorCommand::edc.command["setActor"] = []() {
        int fileid = std::stoi(editorCommand::edc.input[1]);
        if (!getSource().checkTexArrayValid(fileid)) {
            EditorServer::server.sendMesssage("server:no such fileId");
            return;
        }
        if (getSource().checkTexArrayType(fileid) == nsReg::eLandBlock &&
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
        case nsReg::eActor:
            printf("create 1");
            break;
        default:
            break;
        }

        if (editMode == ACTORMODE) {
            std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
            for (MovableEditObj *obj : MovableEditObj::selectedObjs) {
                obj->setSizeWS(size);
                getSource().setSprite(obj->spr, fileid,
                                      std::stoi(editorCommand::edc.input[2]));
            }
            lk.unlock();
        }

        if (editMode == LANDMODE) {
            std::unique_lock lk(MovableEditObj::mutForSelectedObjs);
            for (MovableEditObj *obj : MovableEditObj::selectedObjs) {
                obj->setSizeWS(size);
                getSource().setSprite(obj->spr, fileid,
                                      std::stoi(editorCommand::edc.input[2]));
            }
            lk.unlock();
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
}
bool Editor::isMouseInWindow(sf::RenderWindow &window_) {
    IVector mousePosition = sf::Mouse::getPosition(window_);
    sf::Vector2u windowSize = window_.getSize();
    if (mousePosition.x >= 0 && mousePosition.x < windowSize.x &&
        mousePosition.y >= 0 && mousePosition.y < windowSize.y) {
        return true;
    }
    return false;
};
}; // namespace ens