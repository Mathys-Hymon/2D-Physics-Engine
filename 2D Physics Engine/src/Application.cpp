#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

bool Application::IsRunning() {
    return running;
}


// Setup function (executed once in the beginning of the simulation)

void Application::Setup() {
    running = Graphics::OpenWindow();

    Particle* smallBall = new Particle(50, 100, 1.0);
    smallBall->radius = 4;
    particles.push_back(smallBall);
    Particle* bigBall = new Particle(250, 200, 3.0);
    bigBall->radius = 12;
    particles.push_back(bigBall);

    liquid.x = 0;
    liquid.y = Graphics::Height() / 2;
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2;
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


    for(auto particle: particles) {
        Vec2 weight = Vec2(0.0, 9.8 * particle->mass * PIXELS_PER_METER);
        particle->AddForce(weight);
        particle->AddForce(pushForce);



        if (particle->position.y >= liquid.y) {                                        //APPLY DRAG FORCE
            Vec2 drag = Force::GenerateDragForce(*particle, 0.03);
            particle->AddForce(drag);
        }

        particle->Integrate(deltaTime);

        if (particle->position.x - particle->radius <= 0) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.5;
        }
        else if (particle->position.x + particle->radius >= Graphics::Width()) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.5;
        }

        if (particle->position.y - particle->radius <= 0) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.8;
        }
        else if (particle->position.y + particle->radius >= Graphics::Height()) {
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.8;
        }
    }
}


// Render function (called several times per second to draw objects)

void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    Graphics::DrawFillRect(liquid.x + liquid.w /2, liquid.y + liquid.h /2, liquid.w, liquid.h, 0xFF6E3713);
    for (auto particle : particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}


// Destroy function to delete objects and close the window

void Application::Destroy() {
    // TODO: destroy all objects in the scene

    for (auto particle : particles) {
        delete particle;
    }

    Graphics::CloseWindow();
}