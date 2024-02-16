# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mnouchet <mnouchet@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/06 22:19:57 by mnouchet          #+#    #+#              #
#    Updated: 2023/06/19 00:56:12 by mnouchet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME				:= ircserv

SRC_DIR     := src
INC_DIR     := include
BUILD_DIR		:= .build

SRCS				:= $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS				:= $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

CXX					:= c++
CFLAGS			:= -Wall -Wextra -Werror -std=c++98 -I$(INC_DIR)

RM					:= rm -f
MAKEFLAGS		+= --silent --no-print-directory
DIR_DUP			= mkdir -p $(@D)

all: $(NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	echo "→ Compiling $<"
	$(DIR_DUP)
	$(CXX) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(NAME)
	echo "\033[0;32m✓ Compiled $@ successfully\033[0m"

bonus: all

clean:
	echo "→ Removing objects"
	$(RM) -r $(BUILD_DIR)

fclean: clean
	echo "→ Removing binaries"
	$(RM) $(NAME)

re: clean all

.PHONY: all bonus clean fclean re
