#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

bool Application::IsRunning() {
    return running;
}




void Application::Setup() {
    running = Graphics::OpenWindow();

    Body* body = new Body(BoxShape(100,50), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    bodies.push_back(body);
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
                    Vec2 impulseDirection = (bodies[0]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (bodies[0]->position - mouseCursor).Magnitude() * 5.0;
                    bodies[0]->velocity = impulseDirection * impulseMagnitude;
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


    for(auto rigidBody : bodies) {
        //Vec2 weight = Vec2(0.0, 9.8 * rigidBody->mass * PIXELS_PER_METER);
       // rigidBody->AddForce(weight);
        rigidBody->AddForce(pushForce);

        /*float torque = 20;
        rigidBody->AddTorque(torque);*/

        //Vec2 drag = Force::GenerateDrag(*rigidBody, 0.005);

        //rigidBody->AddForce(drag);

        if (rigidBody->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*) rigidBody->shape;
            if (rigidBody->position.x - circleShape->radius <= 0) {
                rigidBody->position.x = circleShape->radius;
                rigidBody->velocity.x *= -0.5;
            }
            else if (rigidBody->position.x + circleShape->radius >= Graphics::Width()) {
                rigidBody->position.x = Graphics::Width() - circleShape->radius;
                rigidBody->velocity.x *= -0.5;
            }

            if (rigidBody->position.y - circleShape->radius <= 0) {
                rigidBody->position.y = circleShape->radius;
                rigidBody->velocity.y *= -0.8;
            }
            else if (rigidBody->position.y + circleShape->radius >= Graphics::Height()) {
                rigidBody->position.y = Graphics::Height() - circleShape->radius;
                rigidBody->velocity.y *= -0.8;
            }
        }
    }

    for (auto rigidBody : bodies) {
        rigidBody->Integrate(deltaTime);
        rigidBody->IntegrateAngular(deltaTime);
    }
}


void Application::Render() {
    Graphics::ClearScreen(0xFF056263);


    for (auto rigidBody : bodies) {
        if (rigidBody->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*)rigidBody->shape;
            Graphics::DrawCircle(rigidBody->position.x, rigidBody->position.y, circleShape->radius, rigidBody->rotation, 0xFFFFFFFF);
        }
        if (rigidBody->shape->GetType() == BOX) {
            BoxShape* boxShape = (BoxShape*)rigidBody->shape;
            Graphics::DrawPolygon(rigidBody->position.x, rigidBody->position.y, boxShape->vertices, 0xFFFFFFFF);
        }
    }

    Graphics::RenderFrame();
}



void Application::Destroy() {

    for (auto rigidBody : bodies) {
        delete rigidBody;
    }

    Graphics::CloseWindow();
}