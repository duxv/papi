SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(shell find $(SRC_DIR) -name "*.c")
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
LDFLAGS :=
TARGET := main
CC := clang

$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $<

clean:
	rm main obj/*.o
