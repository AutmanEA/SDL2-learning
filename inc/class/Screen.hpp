#pragma once

#include <includes.hpp>

class Screen
{

	public:

		Screen(const char *window_name, unsigned int x, unsigned int y, int w, int h);
		~Screen();

		void			initScreen(void);

		SDL_Window		*getWindow() const;
		SDL_Renderer	*getRenderer() const;
		Uint32			&getID();

	private:

		Screen();

		const char		*_windowName;
		unsigned int	_screenPositionX;
		unsigned int	_screenPositionY;
		int				_screenWidth;
		int				_screenHeight;
		Uint32			_id;
		SDL_Window		*_window;
		SDL_Renderer	*_renderer;


};
