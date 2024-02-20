NAME		=	webserv

CC			=	c++

CFLAGS		=	-std=c++11 -Wall -Werror -Wextra -fsanitize=address,undefined

# directories
SRC_DIR		=	src/
OBJ_DIR		=	obj/
INC_DIR		=	include/

# color codes
RESET		=	\033[0m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
RED			=	\033[31m
UP			=	\033[A
CUT			=	\033[K

#source files
SRC_FILES	=	main \
				Util \
				CgiHandler \
				HttpRequest \
				HttpRequestParser \
				HttpRequestHandler \
				HttpResponse \
				Socket \
				Server \
				ExceptionManager \
				HttpResponseParser \
				FileHandler \
				ConfigParser \
				Client \
				Connection \
				ServerHandler

H_FILES =	Exceptions \
			Util \
			CgiHandler \
			HttpRequest \
			HttpRequestParser \
			HttpRequestHandler \
			HttpResponse \
			Socket \
			Server \
			ExceptionManager \
			HttpResponseParser \
			FileHandler \
			ConfigParser \
			Client \
			Connection \
			ServerHandler

#paths
SRC	=	$(addprefix $(SRC_DIR), $(addsuffix .cpp, $(SRC_FILES)))
OBJ	=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
HS	=	$(addprefix $(INC_DIR), $(addsuffix .hpp, $(H_FILES)))

#all rule
all: $(NAME)

$(NAME): $(OBJ) $(HS)
	@echo "$(YELLOW)Compiling $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo "$(GREEN)Finished $(NAME)$(RESET)"

#compile objects
$(OBJ_DIR)%.o:$(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling $@...$(RESET)"
	@$(CC) $(CFLAGS) -I $(INC_DIR) -o $@ -c $<
	@printf "$(UP)$(CUT)"
	@echo "$(GREEN)Finished $@$(RESET)"

#clean rule
clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
	rm -rf $(OBJ_DIR); \
	echo "$(BLUE)Deleting all objects$(RESET)"; else \
	echo "No objects found"; \
	fi;

#fclean rule
fclean: clean
	@if [ -f "$(NAME)" ]; then \
	rm -f $(NAME); \
	echo "$(BLUE)Deleting $(NAME)$(RESET)"; else \
	echo "No executable found"; \
	fi;

#re rule
re: fclean all

#phony
.PHONY: all clean fclean re
