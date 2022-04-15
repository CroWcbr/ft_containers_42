NAME	=	ft_containers

CC		=	clang++ -std=c++98
CFLAGS	=	-Wall -Werror -Wextra -g

H_CONT	=	vector.hpp \
			stack.hpp \
			map.hpp \
			set.hpp
			
H_UTILS	=	./utils/utils.hpp

H_ITER	=	./iter/iterator_random_access.hpp \
			./iter/iterator_reverse.hpp \
			./iter/iterator.hpp \
			./iter/RBTree_iterator.hpp 

H_TREE	=	./tree/RBTree.hpp \
			./tree/RBTree_Node.hpp

HEADER	=	$(H_CONT) $(H_ITER) $(H_TREE) $(H_UTILS)

SRC		=	main.cpp

OBJ_DIR :=	./obj
OBJ		=	$(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

RM_DIR	=	rm -rf
RM_FILE	=	rm -f

#COLORS
C_NO	=	"\033[00m"
C_OK	=	"\033[32m"
C_GOOD	=	"\033[32m"

#DEBUG
SUCCESS	=	$(C_GOOD)SUCCESS$(C_NO)
OK		=	$(C_OK)OK$(C_NO)

all		:	$(NAME)

$(OBJ): 	| $(OBJ_DIR)	

$(OBJ_DIR):
			@mkdir -p $(OBJ_DIR)
			
$(OBJ_DIR)/%.o		:	%.cpp ${HEADER} Makefile
					$(CC) $(CFLAGS) -c $< -o $@

$(NAME)	:	$(OBJ)
			$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
			@echo "\tCompiling...\t" [ $(NAME) ] $(SUCCESS)
			@$(RM_DIR) $(OBJ_DIR)

clean	:
			@$(RM_DIR) $(OBJ_DIR)
			@echo "\tCleaning...\t" [ $(OBJ_DIR) ] $(OK)

fclean	:	clean
			@$(RM_FILE) $(NAME)
			@echo "\tDeleting...\t" [ $(NAME) ] $(OK)

re		:	fclean all

.PHONY	:	all, clean, fclean, re, test
