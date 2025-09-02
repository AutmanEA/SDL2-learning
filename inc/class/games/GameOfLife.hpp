#pragma once

#include <includes.hpp>
#include <Screen.hpp>

class Screen;

class GameOfLife
{

	public:

		~GameOfLife();
		GameOfLife();

		void	run();
		void	render(Screen **mainScreen, Screen **toolboxScreen);
		void	handleEvents(SDL_Event &event, Screen *mainScreen, Screen *toolboxScreen);

	private:

		typedef enum e_gol_actions {
			PLAY,
			FRAME,
			ERASE
		}	GOL_Actions;

		int		_board[65][99];
		int		_boardBuffer[65][99];
		int		_boardWidth;
		int		_boardHeight;
		int		_cellSize;

		bool	_play;
		bool	_draw;
		bool	_erase;

		std::map<SDL_Keycode, GOL_Actions>	_keymap;
		std::map<GOL_Actions, SDL_Rect>		_buttons;

		void	_eraseBoard(void);
		void	_resolveOneStep(void);
		int		_countNeighbours(int x, int y);
		void	_copyBoard(void);

		void	_eventsOnMainScreen(SDL_Event &event, Screen *mainScreen);
		void	_eventsOnToolboxScreen(SDL_Event &event, Screen *toolboxScreen);

};
