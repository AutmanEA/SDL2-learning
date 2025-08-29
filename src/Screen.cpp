#include <Screen.hpp>

Screen::Screen() {}
Screen::Screen(const char	*window_name, unsigned int x, unsigned int y, int w, int h):
	_windowName(window_name),
	_screenPositionX(x),
	_screenPositionY(y),
	_screenWidth(w),
	_screenHeight(h),
	_id(0),
	_window(NULL),
	_renderer(NULL) {std::cout << "[" << _windowName << "] Constructor called" << std::endl;}

Screen::~Screen()
{
	std::cout << "[" << _windowName << "] Destructor called" << std::endl;
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}

void	Screen::initScreen(void)
{
	_window = SDL_CreateWindow(_windowName, _screenPositionX, _screenPositionY, _screenWidth, _screenHeight, SDL_WINDOW_SHOWN);
	if (!_window)
	{
		throw std::invalid_argument("Error: window fail");
	}
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (!_renderer)
	{
		SDL_DestroyWindow(_window);
		throw std::invalid_argument("Error: renderer fail");
	}
	_id = SDL_GetWindowID(_window);
}

SDL_Window		*Screen::getWindow() const { return (_window); }
SDL_Renderer	*Screen::getRenderer() const { return (_renderer); }
Uint32			&Screen::getID() { return (_id); }
