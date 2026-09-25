#include "Application.h"
#include "Engine/Engine.h"

static Application sApplication{};
static Engine sEngine{};

void initializeApplication()
{
    sApplication.initializeWindow();
}

void initializeEngine()
{
    sEngine.initializeRenderer(sApplication.getWindow());
    sEngine.initializeGui();
    sEngine.initializeScene();
}

void cleanupInitialized()
{
    sEngine.cleanupInitialized();
    sApplication.cleanupInitialized();
}

void runLoop()
{
    sApplication.showWindow();

    // Loop
    while (sApplication.isOpen())
    {
        Event event;

        // Events
        while (sApplication.pollEvents(event))
            sEngine.updateGui(event);

        if (sApplication.isMinimized())
            continue;

        // Rendering
        sEngine.beginFrame();
        sEngine.drawFrame();
        sEngine.endFrame();
    }
}

int main()
{
    // Initialization
    initializeApplication();
    initializeEngine();

    // Execution
    runLoop();

    // Cleanup
    cleanupInitialized();

    return 0;
}
