# -----------------------------
# Compiler and Tools
# -----------------------------
CC := aarch64-linux-gnu-gcc

# -----------------------------
# Directories
# -----------------------------
SRC_DIRS := . i2c htu21d bmp280 db display display/lcd_16x2 display/oled_128x32
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj

# -----------------------------
# Compilation Flags
# -----------------------------
CFLAGS := -Wall -Wextra $(addprefix -I, $(SRC_DIRS))
LDFLAGS := -lsqlite3

# -----------------------------
# Source and Object Files
# -----------------------------
SRCS := main.c \
        i2c/i2c.c \
        htu21d/htu21d.c \
        bmp280/bmp280.c \
        db/db.c \
		display/lcd_16x2/lcd_16x2.c \
		display/lcd_16x2/low_level/low_level.c \
		display/oled_128x32/oled_128x32.c

OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))

# -----------------------------
# Output Executable
# -----------------------------
TARGET := $(BIN_DIR)/pi-home-sensors

# -----------------------------
# Default Target
# -----------------------------
all: directories $(TARGET)

# -----------------------------
# Create necessary directories
# -----------------------------
directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR) $(OBJ_DIR)/i2c $(OBJ_DIR)/htu21d $(OBJ_DIR)/bmp280 $(OBJ_DIR)/db $(OBJ_DIR)/display/lcd_16x2 $(OBJ_DIR)/display/lcd_16x2/low_level $(OBJ_DIR)/display/oled_128x32

# -----------------------------
# Link the final binary
# -----------------------------
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# -----------------------------
# Compile each .c into .o
# -----------------------------
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

# -----------------------------
# Clean
# -----------------------------
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
