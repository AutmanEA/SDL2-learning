#pragma once

#include <includes.hpp>

# define EMPTY 0
# define WALL 1

class Player
{

	public :

		Player();
		Player(int posX, int posY, int radius);
		~Player();

		int	getX(void) const;
		int	getY(void) const;
		int	getRadius(void) const;

		void	update(const std::vector< std::vector<int> > &board, float deltaTime);
		void	jump(void);

		bool	checkCollision(std::vector< std::vector<int> > board);

	private :

		float	_x, _y, _velocityX, _velocityY;
		int		_radius;

};
