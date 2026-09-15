#include "Application.h"
#include "Engine/Engine.h"

int main()
{
    // Program initialization
    Application application;
    Engine engine;

    // Application initialization
    application.initializeWindow();

    // Engine initialization
    engine.initializeRenderer(application.getWindow());
    engine.initializeGui();
    engine.initializeScene();

    // Program loop
    application.showWindow();
    
    while (application.isOpen())
    {
        // Event loop
        Event event;

        while (application.pollEvents(event))
            engine.updateGui(event);

        if (application.isMinimized())
            continue;

        // Rendering
        engine.drawFrame();
    }

    // Program cleanup
    engine.cleanupInitialized();
    application.cleanupInitialized();
}
