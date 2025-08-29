#pragma once

#include <includes.hpp>
#include <Screen.hpp>

class Screen;

class ScreenEvents
{

	public:

		ScreenEvents(SDL_Event event);
		~ScreenEvents();

		void		handleEvents(bool &running, Screen *mainScreen, Screen *toolboxScreen);

		SDL_Event	&getSDLEvent(void);

		bool		&getOpenToolbox(void);
		bool		&getCloseToolbox(void);

		void		setOpenToolbox(bool const &b);
		void		setCloseToolbox(bool const &b);

		bool		switchOpen();
		bool		switchClose();

	private:

		ScreenEvents();

		SDL_Event	_event;
		bool		_openToolbox;
		bool		_closeToolbox;

};
