#pragma once

#include <includes.hpp>
#include <cstdlib>
#include <ctime>
#include <Screen.hpp>

# define USER_FILL	1
# define OBSTACLE	2
# define SOLUTION	3

class Screen;

class BSQ
{

	public:

		~BSQ();
		BSQ();

		void	run();
		void	render(Screen **mainScreen, Screen **toolboxScreen);
		void	handleEvents(SDL_Event &event, Screen *mainScreen, Screen *toolboxScreen);

	private:

		typedef enum e_bsq_actions {
			NEW,
			VALIDATE,
			RESOLVE,
			ERASE
		}	BSQ_Actions;

		int		_board[50][50];
		int		_boardBuffer[50][50];
		int		_boardWidth;
		int		_boardHeight;
		int		_cellSize;

		// bool	_validate;
		// bool	_draw;
		// bool	_erase;

		std::map<SDL_Keycode, BSQ_Actions>	_keymap;
		std::map<BSQ_Actions, SDL_Rect>		_buttons;

		void	_eraseBoard(void);
		void	_copyBoard(void);
		void	_randomBoard(int probability);

		void	_eventsOnMainScreen(SDL_Event &event, Screen *mainScreen);
		void	_eventsOnToolboxScreen(SDL_Event &event, Screen *toolboxScreen);

};
