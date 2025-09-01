#pragma once

#include <includes.hpp>
#include <cstdlib>
#include <ctime>
#include <Screen.hpp>

# define EMPTY		0
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
			CLEAR
		}	BSQ_Actions;

		int		_board[50][50];
		int		_boardBuffer[50][50];
		int		_heatmap[50][50];
		int		_boardWidth;
		int		_boardHeight;
		int		_cellSize;

		//! ATTENTION, LA RESOLUTION DU BSQ NE MARCHE PAS, J'AI INVERSE X et Y!!!

		// bool	_validate;
		bool	_draw;
		// bool	_erase;

		std::map<SDL_Keycode, BSQ_Actions>	_keymap;
		std::map<BSQ_Actions, SDL_Rect>		_buttons;

		int		_squareStartX;
		int		_squareStartY;
		int		_squareCurrentX;
		int		_squareCurrentY;

		void	_eraseBoard(void);
		void	_clearBoard(void);
		void	_copyBoard(void);
		void	_randomBoard(int probability);
		void	_newBoard(void);
		void	_initSolution(void);
		void	_initHeatmap(void);

		void	_eventsOnMainScreen(SDL_Event &event, Screen *mainScreen);
		void	_eventsOnToolboxScreen(SDL_Event &event, Screen *toolboxScreen);

};
