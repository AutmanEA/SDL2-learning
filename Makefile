# -- VARIABLES -- #

NAME	= run

CXXC	= c++
CXXFLAGS= -Wall -Wextra -Werror -g

SRC		= src/main.cpp				\
		src/Screen.cpp				\
		src/ScreenEvents.cpp		\
		src/MainMenu.cpp			\
		src/games/GameOfLife.cpp	\
		src/games/BSQ.cpp			\
		src/games/Platformer.cpp	\
		src/games/Player.cpp

OBJ		= $(addprefix .obj/, $(SRC:.cpp=.o))
INC		= -I./inc -I./inc/class -I./inc/class/games

# -- RULES -- #

all		: $(NAME)

$(NAME)	: $(OBJ)
	$(CXXC) $(CXXFLAGS) $(shell sdl2-config --cflags) $^ $(shell sdl2-config --libs) -o $@

.obj/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXXC) $(INC) $(shell sdl2-config --cflags) -c $< -o $@

clean	:
	rm -rf $(OBJ)
	rm -rf .obj

fclean	: clean
	rm -f $(NAME)

re		: fclean all

docker	:
	docker build . -t runsdl
	xhost +local:docker
	docker run -it --rm -e DISPLAY=$$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix runsdl:latest

vg:
	valgrind ./run

.PHONY	: all clean fclean re vg docker
