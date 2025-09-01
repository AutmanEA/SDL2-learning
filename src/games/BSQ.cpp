#include <BSQ.hpp>

BSQ::BSQ() :
	_board(),
	_boardBuffer(),
	_heatmap(),
	_boardWidth(50),
	_boardHeight(50),
	_cellSize(12),
	// _validate(false),
	_draw(false),
	// _erase(false)
	_squareStartX(0),
	_squareStartY(0),
	_squareCurrentX(0),
	_squareCurrentY(0)
{
	_keymap[SDLK_SPACE] = NEW;
	_keymap[SDLK_RETURN] = VALIDATE;
	_keymap[SDLK_r] = RESOLVE;
	_keymap[SDLK_BACKSPACE] = CLEAR;

	//TODO: class Button pour dessiner les boutons, avec position et taille, et pourquoi pas type du bouton.
	_buttons[VALIDATE] = {20, 20, 50, 50};
	_buttons[NEW] = {20, 90, 50, 50};
	_buttons[RESOLVE] = {20, 160, 50, 50};
	_buttons[CLEAR] = {20, 230, 50, 50};

	_newBoard();
	_clearBoard();
}

BSQ::~BSQ() {}

void	BSQ::run()
{

}

void	BSQ::_newBoard(void)
{
	_eraseBoard();
	std::srand(static_cast<unsigned>(std::time(NULL)));
	_randomBoard(std::rand() % 15 + 5);
	_initSolution();
	_copyBoard();
}

void	BSQ::_eraseBoard(void)
{
	for (int i = 0; i < _boardWidth; i++)
	{
		for (int j = 0; j < _boardHeight; j++)
			_board[j][i] = 0;
	}
}

void	BSQ::_clearBoard(void)
{
	for (int i = 0; i < _boardWidth; i++)
	{
		for (int j = 0; j < _boardHeight; j++)
			_board[j][i] = _boardBuffer[j][i];
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

void	BSQ::_initSolution(void)
{
	int	save_i = 0;
	int	save_j = 0;
	int	saveMax = 0;

	_initHeatmap();

	for (int i = 1; i < _boardWidth; i++)
	{
		for (int j = 1; j < _boardHeight; j++)
		{
			if (_board[j][i] == OBSTACLE)
				_heatmap[j][i] = 0;
			else
				_heatmap[j][i] = std::min(std::min(_heatmap[j][i - 1], _heatmap[j - 1][i]), _heatmap[j - 1][i - 1]) + 1;
			if (_heatmap[j][i] > saveMax)
			{
				saveMax = _heatmap[j][i];
				save_i = i;
				save_j = j;
			}
		}
	}

	//! faut que je change pour que ca trouve toutes les solutions, pas juste la premiere dispo

	for (int i = 0; i < saveMax; i++)
	{
		int	tmp = save_j;
		for (int j = 0; j < saveMax; j++)
		{
			_board[tmp][save_i] = SOLUTION;
			tmp--;
		}
		save_i--;
	}
}

void	BSQ::_initHeatmap(void)
{
	for (int i = 0; i < _boardWidth; i++)
	{
		if (_board[0][i] != OBSTACLE)
			_heatmap[0][i] = 1;
		else
			_heatmap[0][i] = 0;
	}
	for (int j = 0; j < _boardHeight; j++)
	{
		if (_board[j][0] != OBSTACLE)
			_heatmap[j][0] = 1;
		else
			_heatmap[j][0] = 0;
	}
}

void	BSQ::_copyBoard(void)
{
	for (int i = 0; i < _boardWidth; i++)
	{
		for (int j = 0; j < _boardHeight; j++)
			_boardBuffer[j][i] = _board[j][i];
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
				case RESOLVE:  SDL_SetRenderDrawColor((*toolboxScreen)->getRenderer(), 0, 200, 200, 255); break;
				case CLEAR:  SDL_SetRenderDrawColor((*toolboxScreen)->getRenderer(), 200, 200, 0, 255); break;
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

			if (_draw)
			{
				int x1 = std::min(_squareStartX, _squareCurrentX);
				int y1 = std::min(_squareStartY, _squareCurrentY);
				int x2 = std::max(_squareStartX, _squareCurrentX);
				int y2 = std::max(_squareStartY, _squareCurrentY);

				SDL_Rect rect = {
					x1 * _cellSize + (WIDTH - (_boardWidth * _cellSize)) / 2,
					y1 * _cellSize + (HEIGHT - (_boardHeight * _cellSize)) / 2,
					(x2 - x1 + 1) * _cellSize,
					(y2 - y1 + 1) * _cellSize
				};

				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 255, 0, 0, 255);
				SDL_RenderFillRect((*mainScreen)->getRenderer(), &rect);
			}
			if (_board[y][x] == USER_FILL)
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 255, 255, 255, 255);
				SDL_RenderFillRect((*mainScreen)->getRenderer(), &cell);
			}
			if (_board[y][x] == OBSTACLE)
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 120, 120, 120, 255);
				SDL_RenderFillRect((*mainScreen)->getRenderer(), &cell);
			}
			if (_board[y][x] == SOLUTION) //!a delete >
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 0, 255, 0, 255);
				SDL_RenderFillRect((*mainScreen)->getRenderer(), &cell);
			}//! <
			if (_board[y][x] == EMPTY)
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
	static bool	ignoreFirstMouseUp = true;

	if (event.type == SDL_KEYDOWN)
	{
		std::map<SDL_Keycode, BSQ_Actions>::iterator it = _keymap.find(event.key.keysym.sym);
		if (it != _keymap.end()) {
			switch (it->second) {
				case NEW:
					_newBoard();
					break;
				case VALIDATE:
					// _resolveOneStep();
					break;
				case CLEAR:
					_clearBoard();
					break;
				default:
					break;
			}
		}
	}
	else if ( (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION)
				&& event.button.windowID == mainScreen->getID() )
	{
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				_squareStartX = (event.button.x - ((WIDTH - (_boardWidth * _cellSize)) / 2)) / _cellSize;
				_squareStartY = (event.button.y - ((HEIGHT - (_boardHeight * _cellSize)) / 2)) / _cellSize;
				_draw = true;
			}
	// 		else if (event.button.button == SDL_BUTTON_RIGHT)
	// 			_erase = true;
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			_clearBoard();
			_draw = false;

			if (ignoreFirstMouseUp == true)
				ignoreFirstMouseUp = false;
			else if (event.button.button == SDL_BUTTON_LEFT)
			{
				int	x1 = std::min(_squareStartX, _squareCurrentX);
				int	y1 = std::min(_squareStartY, _squareCurrentY);
				int	x2 = std::max(_squareStartX, _squareCurrentX);
				int	y2 = std::max(_squareStartY, _squareCurrentY);

				for (int y = y1; y <= y2; ++y)
				{
					for (int x = x1; x <= x2; ++x)
					{
						if (x >= 0 && x < _boardWidth && y >= 0 && y < _boardHeight)
							_board[y][x] = USER_FILL;
					}
				}

			}
	// 		else if (event.button.button == SDL_BUTTON_RIGHT)
	// 			_erase = false;
		}
		else if (event.type == SDL_MOUSEMOTION && _draw)
		{
			_squareCurrentX = (event.motion.x - ((WIDTH - (_boardWidth * _cellSize)) / 2)) / _cellSize;
			_squareCurrentY = (event.motion.y - ((HEIGHT - (_boardHeight * _cellSize)) / 2)) / _cellSize;
	// 		if (_erase)
	// 		{
	// 			_board[_squareCurrentY][_squareCurrentX] = 0;
	// 		}
		}
	}
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
						_newBoard();
						break;
					case CLEAR:
						_clearBoard();
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
