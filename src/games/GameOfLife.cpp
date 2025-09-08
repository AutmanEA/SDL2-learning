#include <GameOfLife.hpp>

GameOfLife::GameOfLife() :
	_board(),
	_boardBuffer(),
	_boardWidth(MAP_W),
	_boardHeight(MAP_H),
	_cellSize(12),
	_play(false),
	_draw(false),
	_erase(false)
{
	_keymap[SDLK_SPACE] = PLAY;
	_keymap[SDLK_RETURN] = FRAME;
	_keymap[SDLK_BACKSPACE] = ERASE;

	//TODO: class Button pour dessiner les boutons, avec position et taille, et pourquoi pas type du bouton.
	_buttons[PLAY] = {20, 20, 50, 50};
	_buttons[FRAME] = {20, 90, 50, 50};
	_buttons[ERASE] = {20, 160, 50, 50};
}

GameOfLife::~GameOfLife() {}

int		GameOfLife::_countNeighbours(int x, int y)
{
	int	count = 0;

	for (int offsetY = -1; offsetY <= 1; offsetY++)
	{
		for (int offsetX = -1; offsetX <= 1; offsetX++)
		{
			if (offsetX == 0 && offsetY == 0)
				continue;

			int	neighborX = (x + offsetX + (_boardWidth)) % (_boardWidth);
			int	neighborY = (y + offsetY + (_boardHeight)) % (_boardHeight);

			if (_boardBuffer[neighborY][neighborX] == 1)
				count++;
		}
	}

	return (count);
}

void	GameOfLife::_resolveOneStep(void)
{
	int	neighbours = 0;

	for (int i = 0; i < _boardWidth; i++)
	{
		for (int j = 0; j < _boardHeight; j++)
		{
			neighbours = _countNeighbours(i, j);
			if ((neighbours < 2 || neighbours > 3) && _board[j][i] == 1)
				_board[j][i] = 0;
			else if ((neighbours == 3) && _board[j][i] == 0)
				_board[j][i] = 1;
		}
	}
	_copyBoard();
}

void	GameOfLife::_copyBoard(void)
{
	for (int i = 0; i < _boardWidth; i++)
	{
		for (int j = 0; j < _boardHeight; j++)
			_boardBuffer[j][i] = _board[j][i];
	}
}

void	GameOfLife::_eraseBoard(void)
{
	for (int i = 0; i < _boardWidth; i++)
	{
		for (int j = 0; j < _boardHeight; j++)
			_board[j][i] = 0;
	}
}

void		GameOfLife::run()
{
	if (_play)
	{
		_copyBoard();
		_resolveOneStep();
	}
}

void	GameOfLife::render(Screen **mainScreen, Screen **toolboxScreen)
{
	if (*toolboxScreen)
	{
		for (std::map<GOL_Actions, SDL_Rect>::iterator b = _buttons.begin(); b != _buttons.end(); ++b)
		{
			switch (b->first) {
				case PLAY:   SDL_SetRenderDrawColor((*toolboxScreen)->getRenderer(), 0, 200, 0, 255); break;
				case FRAME:  SDL_SetRenderDrawColor((*toolboxScreen)->getRenderer(), 0, 0, 200, 255); break;
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

			if (_board[y][x] == 1)
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 255, 255, 255, 255);
				SDL_RenderFillRect((*mainScreen)->getRenderer(), &cell);
			}
			else
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 5, 5, 15, 20);
				SDL_RenderDrawRect((*mainScreen)->getRenderer(), &cell);
			}

			SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 80, 80, 255, 255);

			SDL_Rect rect;
			rect.w = _cellSize * _boardWidth;
			rect.h = _cellSize * _boardHeight;
			rect.x = (WIDTH - rect.w) / 2;
			rect.y = (HEIGHT - rect.h) / 2;
			SDL_RenderDrawRect((*mainScreen)->getRenderer(), &rect);

			int thickness = 5;

			for (int i = 0; i < thickness; ++i)
			{
				SDL_SetRenderDrawColor((*mainScreen)->getRenderer(), 80 - 16 * i, 80 - 16 * i, 255 - 51 * i, 255 - 51 * i);
				SDL_Rect r = { rect.x - i, rect.y - i, rect.w + 2*i, rect.h + 2*i };
				SDL_RenderDrawRect((*mainScreen)->getRenderer(), &r);
			}
		}
	}
}

void	GameOfLife::_eventsOnMainScreen(SDL_Event &event, Screen *mainScreen)
{
	if (event.type == SDL_KEYDOWN)
	{
		std::map<SDL_Keycode, GOL_Actions>::iterator it = _keymap.find(event.key.keysym.sym);
		if (it != _keymap.end()) {
			switch (it->second) {
				case PLAY:
					if (_play)
						_play = false;
					else
						_play = true;
					break;
				case FRAME:
					_copyBoard();
					_resolveOneStep();
					break;
				case ERASE:
					_eraseBoard();
					break;
			}
		}
	}
	else if ((event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEMOTION) && event.button.windowID == mainScreen->getID())
	{
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				_draw = true;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				_erase = true;
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
				_draw = false;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				_erase = false;
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			int mouseX = (event.button.x - ((WIDTH - (_boardWidth * _cellSize)) / 2)) / _cellSize;
			int mouseY = (event.button.y - ((HEIGHT - (_boardHeight * _cellSize)) / 2)) / _cellSize;

			if (_draw)
			{
				_board[mouseY][mouseX] = 1;
			}
			if (_erase)
			{
				_board[mouseY][mouseX] = 0;
			}
		}
	}
}

void	GameOfLife::_eventsOnToolboxScreen(SDL_Event &event, Screen *toolboxScreen)
{
	if (event.type == SDL_MOUSEBUTTONDOWN && event.button.windowID == toolboxScreen->getID())
	{
		int x = event.button.x;
		int y = event.button.y;

		for (std::map<GOL_Actions, SDL_Rect>::iterator b = _buttons.begin(); b != _buttons.end(); ++b)
		{
			if (x >= b->second.x && x < b->second.x + b->second.w &&
				y >= b->second.y && y < b->second.y + b->second.h) {
				switch (b->first)
				{
					case PLAY:
						if (_play)
							_play = false;
						else
							_play = true;
						break;
					case FRAME:
						_resolveOneStep();
						break;
					case ERASE:
						_eraseBoard();
						break;
				}
			}
		}
	}
}

void	GameOfLife::handleEvents(SDL_Event &event, Screen *mainScreen, Screen *toolboxScreen)
{
	_eventsOnMainScreen(event, mainScreen);
	if (toolboxScreen)
		_eventsOnToolboxScreen(event, toolboxScreen);
}
