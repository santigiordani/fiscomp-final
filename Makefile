# Compilador y flags
CC := gcc
CFLAGS := -Wall -Wextra -O2 -Iinclude

# Carpetas
SRC_DIR := src
OBJ_DIR := bin/obj
BIN_DIR := bin

# Ejecutable final
TARGET := $(BIN_DIR)/app

# Buscar todos los .c
SRCS := $(shell find $(SRC_DIR) -name '*.c')

# Generar la lista de .o correspondiente en bin/obj
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Regla por defecto
all: $(TARGET)

# Link final
$(TARGET): $(OBJS)
	@echo " [LINK] $@"
	@$(CC) $(OBJS) -o $@

# Compilación de cada .c a .o, replicando estructura
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo " [CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Limpieza
clean:
	@echo " [CLEAN]"
	@rm -rf $(OBJ_DIR) $(TARGET)

# Ejemplo para correr tests si los juntás en test/
test: $(TARGET)
	@echo " [RUN TESTS]"
	@./$(TARGET) --run-tests

.PHONY: all clean test
