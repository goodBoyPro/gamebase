#ifndef GOBJECT_H
#define GOBJECT_H
#include "GBase.h"
#include <GDebug.h>
#include <gridmap.h>
class GObject {

  public:
    GObject(/* args */) = default;
    virtual ~GObject() = default;
    // std::atomic<bool> isValid;
    bool isValid = true;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          GComponent
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GComponent : public GObject {
  public:
    virtual ~GComponent() {}
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          GControllerInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GIController : public GObject {
  private:
    static GIController *playerController;
    int id;

  protected:
    bool __focusOnWidget = false;

  public:
    void setFoucusOnWidget(bool bFocusOnUi) { __focusOnWidget = bFocusOnUi; }
    bool getIsFocusOnWidget() { return __focusOnWidget; }
    static std::vector<GIController *> &getAllController() {
        static std::vector<GIController *> allCtrl;
        return allCtrl;
    }
    void setActive() { getAllController()[id] = this; }
    void disableActive() { getAllController()[id] = nullptr; }

  public:
    GIController() {
        getAllController().push_back(this);
        id = getAllController().size() - 1;
        for (std::function<void()> &func : bindKey)
            func = []() { printf("No Key Bind\n"); };
        for (std::function<void(float)> &func : bindAxis)
            func = [](float) { printf("No Key Bind\n"); };
    }
    std::function<void()> bindKey[128];
    std::function<void(float)> bindAxis[128];
    virtual void pollKey(sf::RenderWindow &window_, sf::Event &event_) = 0;
    virtual ~GIController() { getAllController()[id] = nullptr; }

  public:
    static GIController *getPlayerController() { return playerController; }
    static void setPlayerController(GIController *ptr) {
        playerController = ptr;
    }
};
inline GIController *GIController::playerController = nullptr;
inline GIController *getPlayerController() {
    return GIController::getPlayerController();
}
inline void setPlayerController(GIController *ptr) {
    GIController::setPlayerController(ptr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          cameraInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GICamera : public GObject {
  public:
    static float sceneScale;
    IVector positionInWin = IVector(WINW / 2, WINH / 2);
    FVector3 posInWs = FVector3(0, 0, 0);
    ~GICamera() {};
};
inline float GICamera::sceneScale = 1;
inline FVector2 wsToWin(const FVector3 &PositionInWS,
                        const FVector3 &cameraPos_) {
    return {((PositionInWS.x - cameraPos_.x) / pixSize + WINW / 2.f),
            ((PositionInWS.y - cameraPos_.y) / pixSize + WINH / 2.f -
             (PositionInWS.z / pixSize))};
}
inline FVector3 winToWs(const IVector &positionInWin,
                        const FVector3 &cameraPos_) {
    return {(positionInWin.x - WINW / 2) * pixSize + cameraPos_.x,
            (positionInWin.y - WINH / 2) * pixSize + cameraPos_.y, 0};
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           WorldInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GIWorld : public GObject {
  public:
    RenderTex layerGameRtt;
    gameSprite layerGame;
    GridMap<GActor *> *spaceManager = nullptr;
    GIWorld();
    virtual ~GIWorld() { delete spaceManager; }
    virtual void render(sf::RenderWindow &window_);
    void createSpaceManager() {
        delete spaceManager;
        spaceManager =
            new GridMap<GActor *>(FVector2(-10, -10), 500, 500, 5, 5);
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           gameInterface
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class GIGame : public GObject {
  private:
    static GIGame *gameIns;
    GIWorld *worldActive = nullptr;

  public:
    sf::Event event;
    bool bGameContinue = 1;
    GIGame() { setGameIns(this); };
    virtual ~GIGame() {
        setGameIns(nullptr);
        delete worldActive;
        worldActive = nullptr;
    }
    void setDeltaTime();
    float deltaTime = 0;
    virtual void renderLoop2D() = 0;
    static GIGame *getGameIns() { return gameIns; }
    static void setGameIns(GIGame *ptr) { gameIns = ptr; }
    GIWorld *getWorldActive() { return worldActive; }
    // 说明： createWorld(new worldclass);
    GIWorld *createWorld(GIWorld *newWorld);
    void setWorldActive(GIWorld *ptr) { worldActive = ptr; }
};
inline GIGame *GIGame::gameIns = nullptr;

inline void GIGame::setDeltaTime() {
    static sf::Clock clock;
    deltaTime= clock.restart().asSeconds();
}

inline GIWorld *GIGame::createWorld(GIWorld *newWorld) { return newWorld; }
// 全局世界指针需要在最开始设置，而不能在创建完成后设置
inline GIWorld::GIWorld() {
    delete GIGame::getGameIns()->getWorldActive();
    GIGame::getGameIns()->setWorldActive(this);
    layerGameRtt.create(WINW, WINH);
    layerGameRtt.setSmooth(true);
    layerGame.setTexture(layerGameRtt.getTexture());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////UiInterface///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class safeMultiset {};
// 变换依赖容器的变换状态，不要在构造函数中调用变换
class GIUi : public GObject {
  public:
    struct compare {
        const bool operator()(GIUi *a, GIUi *b) const {
            return a->layer < b->layer;
        }
    };

  private:
    std::multiset<GIUi *, compare> viewPortChild;
    std::set<GIUi *> allChildUi;
    GIUi *__parent = nullptr;
    FVector2 __size = {50, 50};
    FVector2 __positionRltv = {0, 0};
    int layer = 0;

  public:
    bool isButton = false;
    FVector2 getPositionABS() {
        if (__parent)
            return __parent->getPositionABS() + __positionRltv;
        return __positionRltv;
    }
    void setLayer(int layer_) {
        if (viewPortChild.contains(this)) {
            viewPortChild.erase(this);
            layer = layer_;
            viewPortChild.insert(this);
        } else
            layer = layer_;
    }
    GIUi(GIUi *parent_) : __parent(parent_) {}
    GIUi() {}
    virtual ~GIUi() {
        for (GIUi *u : allChildUi) {
            delete u;
        }
    }
    gameSprite spr;
    void setSize(const FVector2 &size_) { __size = size_; };
    const FVector2 &getSize() const { return __size; }
    void setPosition(const FVector2 &pos_) {
        __positionRltv = pos_;
        for (GIUi *u : allChildUi) {
            u->setPosition(u->__positionRltv);
        }
    }
    bool isMouseOn(sf::RenderWindow &window_) {
        FVector2 mousePos = {sf::Mouse::getPosition(window_).x,
                             sf::Mouse::getPosition(window_).y};

        return spr.getGlobalBounds().contains(mousePos);
    }
    void addToViewPort() {
        if (__parent->viewPortChild.contains(this))
            return;
        __parent->viewPortChild.insert(this);
    }
    void removeFromViewPort() { __parent->viewPortChild.erase(this); }
    virtual void draw(sf::RenderWindow &window_) {
        const FVector2 &sizeTemp = spr.getLocalBounds().getSize();
        FVector2 scale = {__size.x / sizeTemp.x, __size.y / sizeTemp.y};
        spr.setScale(scale);
        spr.setPosition(getPositionABS());
        window_.draw(spr);
        for (GIUi *u : viewPortChild) {
            u->draw(window_);
        }
    }
    virtual void pollKey(sf::RenderWindow &window_, sf::Event &event_) {
        for (GIUi *u : viewPortChild) {
            u->pollKey(window_, event_);
        }
    }

    template <class T> GIUi *createUI() {
        GIUi *x = new T;
        x->__parent = this;
        allChildUi.insert(x);
        return x;
    }
    void destroy() {
        __parent->allChildUi.erase(this);
        __parent->viewPortChild.erase(this);
        delete this;
    }
};

class GIButton : public GIUi {

  public:
    struct textInfo {
        float textWidth = 0.8;
        float textHeight = 0.7;
        sf::Color textColor = {255, 255, 255};
    } textState;
    enum EState { idle, hover, clicked };
    EState state = idle;
    std::wstring text;

    GIButton() { isButton = true; }
    ~GIButton() {}
    std::function<void()> onMouseLeftClicked = []() {};
    virtual void setSprIdle() = 0;
    virtual void setSprHover() = 0;
    virtual void setSprClicked() = 0;
    virtual void showText(sf::RenderWindow &window_) {
        int x = spr.getPosition().x + spr.getGlobalBounds().getSize().x *
                                          (1.f - textState.textWidth) / 2.f;
        int y = spr.getPosition().y + spr.getGlobalBounds().getSize().y * 0.05;
        FVector2 size = {
            spr.getGlobalBounds().getSize().x * textState.textWidth,
            spr.getGlobalBounds().getSize().y * textState.textHeight};
        printTextLimit(window_, text, x, y, 30, size, textState.textColor);
    }
    virtual void draw(sf::RenderWindow &window_) override {

        switch (state) {
        case idle:
            setSprIdle();
            break;
        case hover:
            setSprHover();
            break;
        case clicked:
            setSprClicked();
            break;

        default:
            break;
        }
        GIUi::draw(window_);
        showText(window_);
    }
    bool isSelected = false;
    virtual void pollKey(sf::RenderWindow &window_,
                         sf::Event &event_) override {
        if (isMouseOn(window_)) {
            if (state != clicked)
                state = hover;
            if (event_.type == sf::Event::MouseButtonPressed) {
                switch (event_.mouseButton.button) {
                case sf::Mouse::Left:
                    isSelected = true;
                    state = clicked;
                    break;
                default:
                    break;
                }
            }
        } else {
            if (state != clicked)
                state = idle;
        }
        if (event_.type == sf::Event::MouseButtonReleased) {
            switch (event_.mouseButton.button) {
            case sf::Mouse::Left:
                if (isSelected) {
                    onMouseLeftClicked();
                    isSelected = false;
                }

                if (isMouseOn(window_))
                    state = hover;
                else
                    state = idle;
                break;
            default:
                break;
            }
        }
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////widgetInterface///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stack>
class GIWidget : public GObject {
  private:
    static std::vector<GIWidget *> __widgetForRender;
    static GIWidget *___focused___;
    int layer = 0;
    bool visible = true;
    bool isInViewPort = false;
    GIUi root;

  public:
    void setLayer(int layer_) { layer = layer_; }
    void setVisible(bool visible_) { visible = visible_; }
    void setPosition(const FVector2 &pos_) { root.setPosition(pos_); }
    virtual void onEventAny(sf::RenderWindow &window_, sf::Event &event_) {
        root.pollKey(window_, event_);
    }
    virtual ~GIWidget() {};
    GIWidget() {}
    template <class T> GIUi *createUI() { return root.createUI<T>(); }
    void addToViewPort() {
        if (isInViewPort)
            return;
        isInViewPort = true;
        __widgetForRender.push_back(this);
        ___focused___ = this;
    }
    GIWidget *pop() {

        if (!isInViewPort)
            return nullptr;
        isInViewPort = false;
        GIWidget *cur = *(__widgetForRender.end() - 1);
        __widgetForRender.pop_back();
        if (__widgetForRender.empty())
            ___focused___ = nullptr;
        else
            ___focused___ = *(__widgetForRender.end() - 1);
        return cur;
    }
    static GIWidget *getTop() { return ___focused___; }
    static void drawAllWidget(sf::RenderWindow &window_);
    virtual void draw(sf::RenderWindow &window_) { root.draw(window_); };
};
inline void GIWidget::drawAllWidget(sf::RenderWindow &window_) {
    for (GIWidget *w : GIWidget::__widgetForRender) {
        if (w->visible)
            w->draw(window_);
    }
}
inline GIWidget *GIWidget::___focused___ = nullptr;
inline std::vector<GIWidget *> GIWidget::__widgetForRender;
/////////////////////////////////////////////////////////////////////////////////////////////////////
/// GMouseInterface
/// ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
class GIMouse {
  public:
    static GIMouse *mousePtr;
    GIMouse() { mousePtr = this; }
    virtual ~GIMouse() {}
    virtual void drawMouseCusor(sf::RenderWindow &window_) {};
};
inline GIMouse *GIMouse::mousePtr = nullptr;
/////////////////////////////////////////////////////////////////////////////////////////////////////

#endif