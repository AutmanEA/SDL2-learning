#include <Player.hpp>

Player::Player() : _x(0), _y(0), _radius(1) {}

Player::Player(int posX, int posY, int radius) : _x(posX), _y(posY), _radius(radius) {}

Player::~Player() {}

int	Player::getX(void) const { return static_cast<int>(_x); }
int	Player::getY(void) const { return static_cast<int>(_y); }
int	Player::getRadius(void) const { return _radius; }

bool	Player::checkCollision(std::vector< std::vector<int> > board)
{
	return board[_y][_x] == WALL;
}

void	Player::update(const std::vector< std::vector<int> > &board, float deltaTime)
{
	const float	gravity = 200.0f;
	const float	maxFall = 500.0f;

	_velocityY += gravity * deltaTime;
	if (_velocityY > maxFall)
		_velocityY = maxFall;

	float newY = _y + _velocityY * deltaTime;

	int cellX = (int)(_x / (_radius * 2));
	int cellYBottom = (int)((newY + _radius) / (_radius * 2));

	int boardH = board.size();
	int boardW = board[0].size();

	if (cellYBottom >= boardH || board[cellYBottom][cellX] == WALL)
	{
		_y = cellYBottom * (_radius * 2) - _radius;
		_velocityY = 0;
	}
	else
	{
		_y = newY;
	}
}

void	Player::jump(void)
{
	if (_velocityY == 0)
	{
		_velocityY = -0.5f;
	}
}
