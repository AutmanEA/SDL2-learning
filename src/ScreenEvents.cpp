#include <ScreenEvents.hpp>
#include <unistd.h>

ScreenEvents::ScreenEvents() {}

ScreenEvents::ScreenEvents(SDL_Event event) :
	_event(event),
	_openToolbox(false),
	_closeToolbox(false) {}

ScreenEvents::~ScreenEvents() {}

SDL_Event	&ScreenEvents::getSDLEvent(void) { return (_event); }

bool	&ScreenEvents::getOpenToolbox(void) { return (_openToolbox); }
void	ScreenEvents::setOpenToolbox(bool const &b) { _openToolbox = b; }

bool	&ScreenEvents::getCloseToolbox(void) { return (_closeToolbox); }
void	ScreenEvents::setCloseToolbox(bool const &b) { _closeToolbox = b; }

bool	ScreenEvents::switchOpen() {
	if (_openToolbox)
	{
		_openToolbox = false;
		return (true);
	}
	return (false);
}

bool	ScreenEvents::switchClose() {
	if (_closeToolbox)
	{
		_closeToolbox = false;
		return (true);
	}
	return (false);
}

void	ScreenEvents::handleEvents(bool &running, Screen *mainScreen, Screen *toolboxScreen)
{
			if (_event.type == SDL_QUIT)
				running = false;
			else if (_event.type == SDL_WINDOWEVENT)
			{
				if (_event.window.event == SDL_WINDOWEVENT_CLOSE) {
					if (_event.window.windowID == mainScreen->getID()) {
						running = false;
					}
					else if (_event.window.windowID == toolboxScreen->getID())
					{
						_closeToolbox = true;
					}
				}
			}
			else if (_event.type == SDL_KEYUP && _event.key.repeat == 0)
			{
				if (_event.key.keysym.sym == SDLK_ESCAPE)
					running = false;
				else if (_event.key.keysym.sym == SDLK_t && !toolboxScreen)
				{
					_openToolbox = true;
				}
				else if (_event.key.keysym.sym == SDLK_t && toolboxScreen)
				{
					_closeToolbox = true;
				}
			}
}
