#include <MainMenu.hpp>

MainMenu::MainMenu() :
	_select(HOME),
	_quit(false)
{
	_buttons[SELECT_GOL] = {20, 20, 50, 50};
	_buttons[SELECT_BSQ] = {20, 90, 50, 50};
	_buttons[QUIT] = {20, 160, 50, 50};
}

MainMenu::~MainMenu() {}

bool	MainMenu::getQuit() { return _quit; }

void	MainMenu::run()
{
	switch (_select)
	{
	case SELECT_GOL:
		_gol.run();
		break;
	case SELECT_BSQ:
		_bsq.run();
		break;
	default:
		break;
	}

}

void	MainMenu::render(Screen **mainScreen, Screen **toolboxScreen)
{
	switch (_select)
	{
	case HOME:
		for (std::map<MM_Actions, SDL_Rect>::iterator b = _buttons.begin(); b != _buttons.end(); ++b)
		{
			switch (b->first) {
				case SELECT_GOL:
					SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 0, 200, 0, 255);
					break;
				case SELECT_BSQ:
					SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 200, 0, 0, 255);
					break;
				case QUIT:
					SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 0, 0, 200, 255);
					break;
				default: break;
			}
			SDL_RenderFillRect((*mainScreen)->getRenderer(), &b->second);

			//contour?
			SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 30, 30, 30, 255);
			SDL_RenderDrawRect((*mainScreen)->getRenderer(), &b->second);
		}
		break;
	case SELECT_GOL:
		_gol.render(mainScreen, toolboxScreen);
		break;
	case SELECT_BSQ:
		_bsq.render(mainScreen, toolboxScreen);
		break;
	default:
		break;
	}
}

void	MainMenu::_selector(SDL_Event event, Screen *mainScreen)
{
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.windowID == mainScreen->getID())
	{
		int x = event.button.x;
		int y = event.button.y;

		for (std::map<MM_Actions, SDL_Rect>::iterator b = _buttons.begin(); b != _buttons.end(); ++b)
		{
			if (x >= b->second.x && x < b->second.x + b->second.w &&
				y >= b->second.y && y < b->second.y + b->second.h)
			{
				switch (b->first)
				{
					case SELECT_GOL:
						_select = SELECT_GOL;
						break;
					case SELECT_BSQ:
						_select = SELECT_BSQ;
						break;
					case QUIT:
						_select = QUIT;
						break;
					default:
						break;
				}
			}
		}
	}
}

void	MainMenu::_returnToMenu(SDL_Event &event)
{
	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_m)
			_select = HOME;
	}
}

void	MainMenu::handleEvents(SDL_Event &event, Screen *mainScreen, Screen *toolboxScreen)
{
	_returnToMenu(event);

	switch (_select)
	{
	case HOME:
		_selector(event, mainScreen);
		break;
	case SELECT_GOL:
		_gol.handleEvents(event, mainScreen, toolboxScreen);
		break;
	case SELECT_BSQ:
		_bsq.handleEvents(event, mainScreen, toolboxScreen);
		break;
	case QUIT:
		std::cout << "Quitting..." << std::endl;
		_quit = true;
		break;
	default:
		break;
	}
}
