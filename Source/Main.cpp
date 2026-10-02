#include "Application.h"
#include "Engine/Engine.h"

// Static

static Application sApplication{};
static Engine sEngine{};

static void initializeApplication()
{
    sApplication.initializeWindow();
}

static void initializeEngine()
{
    sEngine.initializeRenderer(sApplication.getWindow());
    sEngine.initializeScene();
    sEngine.initializeGui();
}

static void cleanupInitialized()
{
    sEngine.cleanupInitialized();
    sApplication.cleanupInitialized();
}

static void runLoop()
{
    sApplication.showWindow();

    // Loop
    while (sApplication.isOpen())
    {
        // Events
        Event event{};

        while (sApplication.pollEvents(event))
            sEngine.updateGui(event);

        if (sApplication.isMinimized())
            continue;

        // Rendering
        sEngine.prepareFrame();
        sEngine.drawFrame();
    }
}

// Main

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
