#include <includes.hpp>
#include <Screen.hpp>
#include <ScreenEvents.hpp>
#include <MainMenu.hpp>

int	main()
{
	bool		running = true;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "Error SDL_Init: " << SDL_GetError() << std::endl;
		return 1;
	}

	Screen	*mainScreen = new Screen(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT);
	// Screen	*toolboxScreen = new Screen(TB_TITLE, WIDTH + 400, SDL_WINDOWPOS_CENTERED, TB_WIDTH, TB_HEIGHT);
	Screen	*toolboxScreen = NULL;

	try
	{
		mainScreen->initScreen();
		//toolboxScreen->initScreen();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		SDL_Quit();
		return (1);
	}

	SDL_Event		event;

	ScreenEvents	screenEvents(event);
	MainMenu		mainMenu;

	Uint32	lastTick = SDL_GetTicks();

	while (running)
	{
		Uint32	now = SDL_GetTicks();
		float	deltaTime = (now - lastTick) / 1000.0f;

		lastTick = now;

		while (SDL_PollEvent(&screenEvents.getSDLEvent()))
		{
			screenEvents.handleEvents(running, mainScreen, toolboxScreen);
			if (toolboxScreen && screenEvents.switchClose())
			{
				delete toolboxScreen;
				toolboxScreen = NULL;
			}
			if (!toolboxScreen && screenEvents.switchOpen())
			{
				toolboxScreen = new Screen(TB_TITLE, WIDTH + 400, SDL_WINDOWPOS_CENTERED, TB_WIDTH, TB_HEIGHT);

				try
				{
					toolboxScreen->initScreen();
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << std::endl;
					delete toolboxScreen;
					delete mainScreen;
					SDL_Quit();
					return (1);
				}
			}
			mainMenu.handleEvents(screenEvents.getSDLEvent(), mainScreen, toolboxScreen);
		}

		SDL_SetRenderDrawColor(mainScreen->getRenderer(), BACKGROUND_COLOR);
		SDL_RenderClear(mainScreen->getRenderer());

		if (toolboxScreen)
		{
			SDL_SetRenderDrawColor(toolboxScreen->getRenderer(), BACKGROUND_COLOR);
			SDL_RenderClear(toolboxScreen->getRenderer());
		}

		mainMenu.run(deltaTime);
		mainMenu.render(&mainScreen, &toolboxScreen);
		if (mainMenu.getQuit() == true)
			running = false;

		SDL_RenderPresent(mainScreen->getRenderer());
		if (toolboxScreen)
			SDL_RenderPresent(toolboxScreen->getRenderer());
	}

	delete mainScreen;
	delete toolboxScreen;
	SDL_Quit();
}
