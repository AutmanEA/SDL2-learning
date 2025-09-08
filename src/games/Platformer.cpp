#include <Platformer.hpp>

Platformer::Platformer()
{
	_boardWidth = 100;
	_boardHeight = 80;
	_cellSize = std::min((WIDTH - 10) / _boardWidth, (HEIGHT - 10) / _boardHeight);

	_board.resize(_boardHeight, std::vector<int>(_boardWidth, EMPTY));
	for (int x = 0; x < _boardWidth; x++)
	{
		_board[_boardHeight - 1][x] = WALL;
	}

	uint	midX = _boardWidth / 2;
	uint	midY = _boardHeight / 2;

	_player = Player(midX, midY, _cellSize / 2);
}

Platformer::~Platformer()
{

}

void	Platformer::run(float deltaTime)
{
	_player.update(_board, deltaTime);
}

void	Platformer::render(Screen **mainScreen, Screen **toolboxScreen)
{
	(void)toolboxScreen;

	SDL_Renderer	*renderer = (*mainScreen)->getRenderer();

	SDL_Rect rect;
	rect.w = _cellSize * _boardWidth;
	rect.h = _cellSize * _boardHeight;
	rect.x = (WIDTH - rect.w) / 2;
	rect.y = (HEIGHT - rect.h) / 2;

	//border render
	SDL_SetRenderDrawColor(renderer, 80, 255, 80, 255);
	SDL_RenderDrawRect(renderer, &rect);

	int thickness = 5;

	for (int i = 0; i < thickness; ++i)
	{
		SDL_SetRenderDrawColor(renderer, 80 - 16 * i, 255 - 51 * i, 80 - 16 * i, 255 - 51 * i);
		SDL_Rect r = { rect.x - i, rect.y - i, rect.w + 2*i, rect.h + 2*i };
		SDL_RenderDrawRect(renderer, &r);
	}

	//cells render
	for (int y = 0; y < (int)_board.size(); y++)
	{
		for (int x = 0; x < (int)_board[y].size(); x++)
		{
			SDL_Rect cell;
			cell.w = _cellSize;
			cell.h = _cellSize;
			cell.x = x * _cellSize + (WIDTH - (_boardWidth * _cellSize)) / 2;
			cell.y = y * _cellSize + (HEIGHT - (_boardHeight * _cellSize)) / 2;

			if (_board[y][x] == WALL)
				SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
			else
				SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			SDL_RenderFillRect(renderer, &cell);
		}
	}

	//player render
	SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
	int	centerX = (_player.getX() * _cellSize) + rect.x;
	int	centerY = (_player.getY() * _cellSize) + rect.y;

	for (int w = 0; w < _player.getRadius() * 2; w++)
	{
		for (int h = 0; h < _player.getRadius() * 2; h++)
		{
			int	dx = _player.getRadius() - w;
			int	dy = _player.getRadius() - h;
			if ((dx * dx + dy * dy) <= (_player.getRadius() * _player.getRadius()))
			{
				SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
			}
		}
	}
}

void	Platformer::handleEvents(SDL_Event &event, Screen *mainScreen, Screen *toolboxScreen)
{
	(void)event;
	(void)mainScreen;
	(void)toolboxScreen;
}
