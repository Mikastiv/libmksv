NAME			=	libmksv.a

CC				=	clang
CFLAGS			=	-Wall -Werror -Wextra -Wpedantic -Wfloat-conversion -Wsign-conversion -std=c2x
AR				=	ar rcs
RM				=	rm -rf
MKDIR			=	mkdir -p

SRC_DIR			=	src
OBJ_DIR			=	obj

DIRS			=	$(shell find $(SRC_DIR) -type d)
INCS			=	$(shell find . -type f -name "*.h")
SRCS			=	$(shell find $(SRC_DIR) -type f -name "*.c")
SRCS_BASE		=	$(foreach file, $(SRCS), $(shell basename -a $(file)))
OBJS			=	$(SRCS_BASE:%=$(OBJ_DIR)/%.o)

VPATH			=	$(SRC_DIR) $(DIRS)

$(OBJ_DIR)/%.c.o:	%.c
	$(CC) $(CFLAGS) -c -I. $< -o $@

all:		debug

release:	CFLAGS += -O3
release:	$(NAME)

debug:		CFLAGS += -g
debug:		$(NAME)

$(NAME):	$(OBJ_DIR) $(OBJ) $(OBJS)
	@$(AR) $(NAME) $(OBJS)
	@echo $(NAME) created

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

clean:
	@$(RM) $(OBJ_DIR)

fclean:		clean
	@$(RM) $(NAME)

re:			fclean all

fmt:
	clang-format -i $(SRCS) $(INCS)

.PHONY:		all release debug clean fclean re fmt
