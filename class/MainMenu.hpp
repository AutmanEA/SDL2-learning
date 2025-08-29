#pragma once

#include <includes.hpp>
#include <GameOfLife.hpp>
#include <Screen.hpp>

class Screen;
class GameOfLife;

class MainMenu
{

	public:

		MainMenu();
		~MainMenu();

		void	run();
		void	render(Screen **mainScreen, Screen **toolboxScreen);

		void	handleEvents(SDL_Event &event, Screen *mainScreen, Screen *toolboxScreen);

		bool	getQuit(void);

	private:

		GameOfLife	_gol;

		typedef enum e_mm_actions {
			HOME,
			SELECT_GOL,
			SELECT_BSQ,
			QUIT
		}	MM_Actions;

		std::map<SDL_Keycode, MM_Actions>	_keymap;
		std::map<MM_Actions, SDL_Rect>		_buttons;

		MM_Actions	_select;
		bool		_quit;

		void		_selector(SDL_Event event, Screen *mainScreen);
		void		_returnToMenu(SDL_Event &event);

};
