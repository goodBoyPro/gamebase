#if !defined(GAMEEDITOR)
#define GAMEEDITOR
#include <GDebug.h>
#include <GObject.h>
#include <SFML/Graphics.hpp>
#include <set>
#include <vector>
#include "editorCommand.h"
#include <GSource.h>
namespace ens
{

    inline sf::RenderWindow &getEditorWindow()
    {
        static sf::RenderWindow window(sf::VideoMode(WINW, WINH), "editor");
        return window;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    class Info
    {
    public:
        struct Information
        {
            int fileID;
            int picIndex;
            FVector3 position;
        } info;
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    class EditorCamera : public GCameraInterface
    {
    public:
        EditorCamera() { GCameraInterface::setGameCamera(this); }
        static EditorCamera editorCamera;
        static void pollKey(sf::Event &event) {}
    };
    inline EditorCamera EditorCamera::editorCamera;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    class WindowFlag
    {
    public:
        static WindowFlag flag;
        gameSprite spr;
        sf::Texture tex;
        sf::ConvexShape shape;
        FVector3 posInWs = {0, 0, 0};
        WindowFlag()
        {
            shape.setPointCount(3);
            shape.setPoint(0, {0, 0});
            shape.setPoint(1, {4, 4});
            shape.setPoint(2, {-4, 4});
            shape.setFillColor(sf::Color(0, 255, 0, 255));
        }
        void draw()
        {
            IVector piwin = wsToWin(flag.posInWs);
            flag.shape.setPosition(piwin.x, piwin.y);
            getWindow()->draw(flag.shape);
        };
    };
    inline WindowFlag WindowFlag::flag;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    std::mutex mutForAllMeo;
    class MovableEditObj : public Info
    {

    public:
        struct Compare
        {
            bool operator()(const MovableEditObj *a,
                            const MovableEditObj *b) const
            {
                return a->posInWs.y < b->posInWs.y;
            }
        };

    public:
        static std::multiset<MovableEditObj *> allMEO;
        void allMeoInsert(MovableEditObj *ptr)
        {
            std::unique_lock lk(mutForAllMeo);
            allMEO.insert(ptr);
        };
        void allMeoRemove(MovableEditObj *ptr)
        {
            std::unique_lock lk(mutForAllMeo);
            allMEO.erase(ptr);
        };
        static MovableEditObj *selectedObjForMove;
        static MovableEditObj *selectedObjForEdit;

        sf::Sprite spr;
        sf::Texture tex;
        sf::ConvexShape centerPoint;
        sf::ConvexShape shapeForSelect;
        // IVector posInWin;
        FVector3 posInWs = {0, 0, 0};
        bool isValid = true;
        void destroy() { isValid = false; }
        static IVector mousePos;
        static IVector deltaMove;
        static IVector posPre;
        MovableEditObj(int fileId_, int picIndex_)
        {
            allMeoInsert(this);
            centerPoint.setPointCount(3);
            centerPoint.setPoint(0, {0, 0});
            centerPoint.setPoint(1, {4, 4});
            centerPoint.setPoint(2, {-4, 4});
            centerPoint.setFillColor(sf::Color(255, 0, 0));
            getSource().setSprite(spr, fileId_, picIndex_);
            shapeForSelect.setPointCount(4);
            sf::FloatRect lb = spr.getLocalBounds();
            shapeForSelect.setPoint(0, {lb.left, lb.top});
            shapeForSelect.setPoint(1, {lb.left + lb.getSize().x, lb.top});
            shapeForSelect.setPoint(2, {lb.left + lb.getSize().x, lb.top + lb.getSize().y});
            shapeForSelect.setPoint(3, {lb.left, lb.top + lb.getSize().y});
            shapeForSelect.setOrigin(spr.getOrigin());
            shapeForSelect.setFillColor(sf::Color(0, 0, 0, 0));
            shapeForSelect.setOutlineColor(sf::Color(255, 255, 0));
            shapeForSelect.setOutlineThickness(3);
        }
        void draw(sf::RenderWindow &window_)
        {
            IVector psInWin = wsToWin(posInWs);
            spr.setPosition(psInWin.x, psInWin.y);
            window_.draw(spr);
            centerPoint.setPosition(psInWin.x, psInWin.y);
            window_.draw(centerPoint);
            if (selectedObjForEdit == this)
            {
                shapeForSelect.setPosition(psInWin.x, psInWin.y);
                window_.draw(shapeForSelect);
            }
        }

        static void pollKey(sf::RenderWindow &window_, sf::Event &event_)
        {
            mousePos = sf::Mouse::getPosition(window_);
            deltaMove = mousePos - posPre;
            FVector3 deltaWorldMove = {deltaMove.x * pixSize, deltaMove.y * pixSize,
                                       0};
            posPre = sf::Mouse::getPosition(window_);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                if (!selectedObjForMove)
                {
                    selectedObjForEdit = nullptr;
                    for (auto ob : allMEO)
                    {
                        sf::FloatRect bound = ob->spr.getGlobalBounds();
                        if (bound.contains(window_.mapPixelToCoords(mousePos)))
                        {
                            selectedObjForMove = ob;
                            selectedObjForEdit = ob;
                            break;
                        }
                    }
                }
                else
                {
                    selectedObjForMove->posInWs += deltaWorldMove;
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                if (window_.hasFocus())
                    WindowFlag::flag.posInWs = {winToWs(sf::Mouse::getPosition(*getWindow())).x,
                                                winToWs(sf::Mouse::getPosition(*getWindow())).y,
                                                0};
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
            {
                EditorCamera::editorCamera.posInWs -= deltaWorldMove;
            }
            if (event_.type == sf::Event::MouseButtonReleased)
            {
                selectedObjForMove = nullptr;
            }
        }
    };

    inline std::multiset<MovableEditObj *> MovableEditObj::allMEO;
    inline MovableEditObj *MovableEditObj::selectedObjForMove;
    inline MovableEditObj *MovableEditObj::selectedObjForEdit;
    inline IVector MovableEditObj::mousePos;
    inline IVector MovableEditObj::deltaMove;
    inline IVector MovableEditObj::posPre;
    template <class T>
    inline T *createAtLocation(T *ptr, const FVector3 &pos_)
    {
        ptr->posInWs = pos_;
        return ptr;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    class Editor
    {
    public:
        sf::RenderWindow *window;
        sf::Event event;
        void setCommand();
        Editor()
        {
            initTools();

            window = &getEditorWindow();
            setWindow(window);
            setCommand();
            std::thread tInput(&editorCommand::loop, &editorCommand::edc);
            loop();
            tInput.join();
        }
        void loop()
        {
            sf::RenderWindow &window = getEditorWindow();
            auto it = MovableEditObj::allMEO.begin();
            while (window.isOpen())
            {
                if (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();

                    MovableEditObj::pollKey(window, event);
                }
                std::multiset<MovableEditObj *, MovableEditObj::Compare> allObj;
                std::unique_lock lk(mutForAllMeo);
                for (it = MovableEditObj::allMEO.begin();
                     it != MovableEditObj::allMEO.end();)
                {
                    if ((*it)->isValid)
                    {
                        allObj.insert(*it);
                        it++;
                    }
                    else
                    {
                        it = MovableEditObj::allMEO.erase(it);
                    }
                }
                lk.unlock();
                ///////////////////////////////////////////////////////////////////////////
                // 渲染///////////////////////////////////////////////////////////////////////////
                window.clear();
                PRINTDEBUG(L"键鼠位置：%f,%f",
                           winToWs(sf::Mouse::getPosition(*getWindow())).x,
                           winToWs(sf::Mouse::getPosition(*getWindow())).y);
                GCameraInterface::posForDraw = GCameraInterface::getGameCamera()->posInWs;
                for (auto obj : allObj)
                {
                    obj->draw(window);
                }
                WindowFlag::flag.draw();
                // 显示DEBUG////////////////////////////////
                GDebug::debugDisplay();
                window.display();
            }
            editorCommand::edc.bRun = false;
            for (auto obj : MovableEditObj::allMEO)
            {
                delete obj;
            }
        }
    };
    inline void Editor::setCommand()
    {
        editorCommand::edc.command["create"] = [&]()
        {
            MovableEditObj *obj = createAtLocation(new MovableEditObj(std::stoi(editorCommand::edc.input[1]),
                                                                      std::stoi(editorCommand::edc.input[2])),
                                                   WindowFlag::flag.posInWs);
            MovableEditObj::selectedObjForEdit = obj;
        };
        editorCommand::edc.command["createm"] = [&]()
        {
            MovableEditObj *obj = createAtLocation(new MovableEditObj(std::stoi(editorCommand::edc.input[1]),
                                                                      std::stoi(editorCommand::edc.input[2])),
                                                   {winToWs(sf::Mouse::getPosition(*getWindow())).x,
                                                    winToWs(sf::Mouse::getPosition(*getWindow())).y, 0});
            MovableEditObj::selectedObjForEdit = obj;
        };
    };
}; // namespace ens
#endif // GAMEEDITOR
