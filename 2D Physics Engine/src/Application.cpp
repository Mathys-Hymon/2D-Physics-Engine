#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

bool Application::IsRunning() {
    return running;
}




void Application::Setup() {
    running = Graphics::OpenWindow();

}


// Input processing
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = -50 * PIXELS_PER_METER;
                
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0;
                break;
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouseCursor.x = x;
                    mouseCursor.y = y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    Vec2 impulseDirection = (rb[0]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (rb[0]->position - mouseCursor).Magnitude() * 5.0;
                    rb[0]->velocity = impulseDirection * impulseMagnitude;
                }
                break;
        }
    }
}


// Update function (called several times per second to update objects)

void Application::Update() {

    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) SDL_Delay(timeToWait);
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016f) deltaTime = 0.016f;
    timePreviousFrame = SDL_GetTicks();


    for(auto rigidBody : rb) {
        Vec2 weight = Vec2(0.0, 9.8 * rigidBody->mass * PIXELS_PER_METER);
        rigidBody->AddForce(weight);
        rigidBody->AddForce(pushForce);

        Vec2 drag = Force::GenerateDrag(*rigidBody, 0.002);
        rigidBody->AddForce(drag);




        if (rigidBody->position.x - rigidBody->radius <= 0) {
            rigidBody->position.x = rigidBody->radius;
            rigidBody->velocity.x *= -0.5;
        }
        else if (rigidBody->position.x + rigidBody->radius >= Graphics::Width()) {
            rigidBody->position.x = Graphics::Width() - rigidBody->radius;
            rigidBody->velocity.x *= -0.5;
        }

        if (rigidBody->position.y - rigidBody->radius <= 0) {
            rigidBody->position.y = rigidBody->radius;
            rigidBody->velocity.y *= -0.8;
        }
        else if (rigidBody->position.y + rigidBody->radius >= Graphics::Height()) {
            rigidBody->position.y = Graphics::Height() - rigidBody->radius;
            rigidBody->velocity.y *= -0.8;
        }
    }



    for (auto rigidBody : rb) {
        rigidBody->Integrate(deltaTime);
    }
}


void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    for (auto rigidBody : rb) {
        Graphics::DrawFillCircle(rigidBody->position.x, rigidBody->position.y, rigidBody->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}



void Application::Destroy() {

    for (auto rigidBody : rb) {
        delete rigidBody;
    }

    Graphics::CloseWindow();
}