# Program name
NAME = philo

# Compiler
CC = cc

# Compiling flags
CFLAGS = -Wall -Wextra -Werror

# Directories
OBJ_DIR = objects
INCLUDES = ./philo.h

# Source files
CFILES = 	\
			./main.c \
			./utils.c 

# Define objects
OBJECTS = $(CFILES:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)
	@echo "$(NAME) built successfully!"

# Build the project
$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS)

# Create directory for objects, and compile source files into object files
$(OBJ_DIR)/%.o: %.c $(INCLUDES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean the project
clean:
	rm -rf $(OBJ_DIR)

# Clean project and object files
fclean: clean
	rm -rf $(NAME)

# Clean and rebuild project rule
re: fclean all

.PHONY: all clean fclean