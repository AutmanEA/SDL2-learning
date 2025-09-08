#pragma once

#include <includes.hpp>
#include <Screen.hpp>
#include <Player.hpp>

# define EMPTY 0
# define WALL 1

class Screen;
class Player;

class Platformer
{

	public :

		Platformer();
		~Platformer();

		void	run(float deltaTime);
		void	render(Screen **mainScreen, Screen **toolboxScreen);
		void	handleEvents(SDL_Event &event, Screen *mainScreen, Screen *toolboxScreen);

	private :

		std::vector< std::vector<int> >	_board;
		uint							_boardWidth;
		uint							_boardHeight;
		int								_cellSize;

		Player							_player;

};
