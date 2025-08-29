#include <BSQ.hpp>

BSQ::BSQ() :
	_board(),
	_boardBuffer(),
	_boardWidth(50),
	_boardHeight(50),
	_cellSize(12)
	// _validate(false),
	// _draw(false),
	// _erase(false)
{
	_keymap[SDLK_SPACE] = NEW;
	_keymap[SDLK_RETURN] = VALIDATE;
	_keymap[SDLK_r] = RESOLVE;
	_keymap[SDLK_BACKSPACE] = ERASE;

	//TODO: class Button pour dessiner les boutons, avec position et taille, et pourquoi pas type du bouton.
	_buttons[VALIDATE] = {20, 20, 50, 50};
	_buttons[NEW] = {20, 90, 50, 50};
	_buttons[RESOLVE] = {20, 160, 50, 50};
	_buttons[ERASE] = {20, 230, 50, 50};

	std::srand(static_cast<unsigned>(std::time(NULL)));
	_randomBoard(std::rand() % 20 + 5);
	_copyBoard();
}

BSQ::~BSQ() {}

void	BSQ::run()
{

}

void	BSQ::_copyBoard(void)
{
	for (int i = 0; i < _boardWidth; i++)
	{
		for (int j = 0; j < _boardHeight; j++)
			_boardBuffer[j][i] = _board[j][i];
	}
}

void	BSQ::_eraseBoard(void)
{
	for (int i = 0; i < _boardWidth; i++)
	{
		for (int j = 0; j < _boardHeight; j++)
			_board[j][i] = 0;
	}
}

void	BSQ::_randomBoard(int probability)
{
	std::srand(static_cast<unsigned>(std::time(NULL)));

	for (int i = 0; i < _boardWidth; i++)
	{
		for (int j = 0; j < _boardHeight; j++)
		{
			if ((std::rand() % 100) < probability)
				_board[j][i] = OBSTACLE;
		}
	}
}

void	BSQ::render(Screen **mainScreen, Screen **toolboxScreen)
{
	if (*toolboxScreen)
	{
		for (std::map<BSQ_Actions, SDL_Rect>::iterator b = _buttons.begin(); b != _buttons.end(); ++b)
		{
			switch (b->first) {
				case NEW:   SDL_SetRenderDrawColor((*toolboxScreen)->getRenderer(), 0, 200, 0, 255); break;
				case VALIDATE:  SDL_SetRenderDrawColor((*toolboxScreen)->getRenderer(), 0, 0, 200, 255); break;
				case ERASE:  SDL_SetRenderDrawColor((*toolboxScreen)->getRenderer(), 200, 200, 0, 255); break;
				default: break;
			}
			SDL_RenderFillRect((*toolboxScreen)->getRenderer(), &b->second);

			//contour?
			SDL_SetRenderDrawColor((*toolboxScreen)->getRenderer(), 30, 30, 30, 255);
			SDL_RenderDrawRect((*toolboxScreen)->getRenderer(), &b->second);
		}
	}

	for (int y = 0; y < _boardHeight; ++y)
	{
		for (int x = 0; x < _boardWidth; ++x)
		{
			SDL_Rect cell;
			cell.w = _cellSize;
			cell.h = _cellSize;
			cell.x = x * _cellSize + (WIDTH - (_boardWidth * _cellSize)) / 2;
			cell.y = y * _cellSize + (HEIGHT - (_boardHeight * _cellSize)) / 2;

			if (_board[y][x] == USER_FILL)
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 255, 255, 255, 255);
				SDL_RenderFillRect((*mainScreen)->getRenderer(), &cell);
			}
			else if (_board[y][x] == OBSTACLE)
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 120, 120, 120, 255);
				SDL_RenderFillRect((*mainScreen)->getRenderer(), &cell);
			}
			else
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 15, 5, 5, 20);
				SDL_RenderDrawRect((*mainScreen)->getRenderer(), &cell);
			}

			SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 255, 80, 80, 255);

			SDL_Rect rect;
			rect.w = _cellSize * _boardWidth;
			rect.h = _cellSize * _boardHeight;
			rect.x = (WIDTH - rect.w) / 2;
			rect.y = (HEIGHT - rect.h) / 2;
			SDL_RenderDrawRect((*mainScreen)->getRenderer(), &rect);

			int thickness = 5;

			for (int i = 0; i < thickness; ++i)
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 255 - 51 * i, 80 - 16 * i, 80 - 16 * i, 255 - 51 * i);
				SDL_Rect r = { rect.x - i, rect.y - i, rect.w + 2*i, rect.h + 2*i };
				SDL_RenderDrawRect((*mainScreen)->getRenderer(), &r);
			}
		}
	}
}

void	BSQ::_eventsOnMainScreen(SDL_Event &event, Screen *mainScreen)
{
	(void)mainScreen;
	
	if (event.type == SDL_KEYDOWN)
	{
		std::map<SDL_Keycode, BSQ_Actions>::iterator it = _keymap.find(event.key.keysym.sym);
		if (it != _keymap.end()) {
			switch (it->second) {
				case NEW:
					_eraseBoard();
					std::srand(static_cast<unsigned>(std::time(NULL)));
					_randomBoard(std::rand() % 20 + 5);
					_copyBoard();
					break;
				case VALIDATE:
					// _resolveOneStep();
					break;
				case ERASE:
					_eraseBoard();
					break;
				default:
					break;
			}
		}
	}
	// else if ((event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION) && event.button.windowID == mainScreen->getID())
	// {
	// 	if (event.type == SDL_MOUSEBUTTONDOWN)
	// 	{
	// 		if (event.button.button == SDL_BUTTON_LEFT)
	// 			_draw = true;
	// 		else if (event.button.button == SDL_BUTTON_RIGHT)
	// 			_erase = true;
	// 	}
	// 	else if (event.type == SDL_MOUSEBUTTONUP)
	// 	{
	// 		if (event.button.button == SDL_BUTTON_LEFT)
	// 			_draw = false;
	// 		else if (event.button.button == SDL_BUTTON_RIGHT)
	// 			_erase = false;
	// 	}
	// 	else if (event.type == SDL_MOUSEMOTION)
	// 	{
	// 		int mouseX = (event.button.x - ((WIDTH - (_boardWidth * _cellSize)) / 2)) / _cellSize;
	// 		int mouseY = (event.button.y - ((HEIGHT - (_boardHeight * _cellSize)) / 2)) / _cellSize;

	// 		if (_draw)
	// 		{
	// 			_board[mouseY][mouseX] = 1;
	// 		}
	// 		if (_erase)
	// 		{
	// 			_board[mouseY][mouseX] = 0;
	// 		}
	// 	}
	// }
}

void	BSQ::_eventsOnToolboxScreen(SDL_Event &event, Screen *toolboxScreen)
{
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.windowID == toolboxScreen->getID())
	{
		int x = event.button.x;
		int y = event.button.y;

		for (std::map<BSQ_Actions, SDL_Rect>::iterator b = _buttons.begin(); b != _buttons.end(); ++b)
		{
			if (x >= b->second.x && x < b->second.x + b->second.w &&
				y >= b->second.y && y < b->second.y + b->second.h) {
				switch (b->first)
				{
					case NEW:
						_eraseBoard();
						std::srand(static_cast<unsigned>(std::time(NULL)));
						_randomBoard(std::rand() % 20 + 5);
						_copyBoard();
						break;
					case ERASE:
						_eraseBoard();
						break;
					default:
						break;
				}
			}
		}
	}
}

void	BSQ::handleEvents(SDL_Event &event, Screen *mainScreen, Screen *toolboxScreen)
{
	_eventsOnMainScreen(event, mainScreen);
	if (toolboxScreen)
		_eventsOnToolboxScreen(event, toolboxScreen);
}
