#ifndef APPLICATION_H
#define APPLICATION_H


#include "./Graphics.h"
#include "./Physics/Body.h"
#include <vector>

class Application {
    private:
        bool running = false;
        std::vector<Body*> rb;
        Vec2 pushForce = Vec2(0,0);
        Vec2 mouseCursor = Vec2(0, 0);
        bool leftMouseButtonDown = false;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif