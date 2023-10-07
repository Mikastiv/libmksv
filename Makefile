NAME		=	libmksv.a

SRC_DIR		=	src
INC_DIR		=	include/mksv
OBJ_DIR		=	obj

OS			= $(shell uname -s)

DIRS		=	$(shell find $(SRC_DIR) -type d)
INCS		=	$(shell find $(INC_DIR) -type f -name "*.hpp")
SRCS		=	$(shell find $(SRC_DIR) -type f -name "*.cpp")
SRCS_BASE	=	$(foreach file, $(SRCS), $(shell basename -a $(file)))
OBJS		=	$(SRCS_BASE:%=$(OBJ_DIR)/%.o)

VPATH		=	$(SRC_DIR) $(DIRS)

CXX			=	clang++
CXXFLAGS	=	-Wall -Werror -Wextra -Wpedantic -Wfloat-conversion -Wsign-conversion \
				-Wno-gnu-anonymous-struct -fno-rtti -fno-exceptions -std=c++20
AR			=	ar rcs
RM			=	rm -rf
MKDIR		=	mkdir -p

ifeq ($(OS),Linux)
SDK_PATH	=
else ifeq ($(OS),Darwin)
SDK_PATH	=	$(shell xcrun --show-sdk-path)
else
SDK_PATH	=
endif

$(OBJ_DIR)/%.cpp.o:	%.cpp
	$(CXX) $(CXXFLAGS) -c -I$(INC_DIR) -isystem$(SDK_PATH)/usr/include $< -o $@

all:		debug

release:	CXXFLAGS += -O3
release:	$(NAME)

debug:		CXXFLAGS += -g
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
