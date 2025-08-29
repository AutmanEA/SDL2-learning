# -- VARIABLES -- #

NAME	= run

CXXC	= c++
CXXFLAGS= -Wall -Wextra -Werror -g
CPPFLAGS= $(shell sdl2-config --cflags --libs)

SRC		= src/main.cpp				\
		src/Screen.cpp				\
		src/ScreenEvents.cpp		\
		src/MainMenu.cpp			\
		src/games/GameOfLife.cpp	\

OBJ		= $(addprefix .obj/, $(SRC:.cpp=.o))
INC		= -I./inc/ -I./class -I./class/games

# -- RULES -- #

all		: $(NAME)

$(NAME)	: $(OBJ)
	$(CXXC) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

.obj/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXXC) $(INC) $(CXXFLAGS) -c -o $@ $<

clean	:
	rm -rf $(OBJ)
	rm -rf .obj

fclean	: clean
	rm -f $(NAME)

re		: fclean all

vg:
	valgrind ./run

.PHONY	: all clean fclean re vg
