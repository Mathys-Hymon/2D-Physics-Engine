#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"

bool Application::IsRunning() {
    return running;
}




void Application::Setup() {
    running = Graphics::OpenWindow();

    Particle* smallPlanet = new Particle(200, 200, 1.0);
    smallPlanet->radius = 6;
    particles.push_back(smallPlanet);

    Particle* bigPlanet = new Particle(500, 500, 20.0);
    bigPlanet->radius = 20;
    particles.push_back(bigPlanet);
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
                    Vec2 impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
                    float impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
                    particles[0]->velocity = impulseDirection * impulseMagnitude;
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


    for(auto particle: particles) {
        //Vec2 weight = Vec2(0.0, 9.8 * particle->mass * PIXELS_PER_METER);
        //particle->AddForce(weight);
        particle->AddForce(pushForce);

        Vec2 friction = Force::GenerateFriction(*particle, 5);
        particle->AddForce(friction);

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
    Vec2 attraction = Force::GenerateGravity(*particles[0], *particles[1], 1000.0, 5, 100);
    particles[0]->AddForce(attraction);
    particles[1]->AddForce(-attraction);



    for (auto particle : particles) {
        particle->Integrate(deltaTime);
    }
}



void Application::Render() {
    Graphics::ClearScreen(0xFF056263);

    for (auto particle : particles) {
        Graphics::DrawFillCircle(particle->position.x, particle->position.y, particle->radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}



void Application::Destroy() {

    for (auto particle : particles) {
        delete particle;
    }

    Graphics::CloseWindow();
}