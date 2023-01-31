CC:=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic -g
PROJ_NAME=clli
OBJ_DIR=build
SRC_DIR=src
LIBS=-lm -lncurses

.PHONY: all
all: $(PROJ_NAME) $(PROJ_NAME)_tests test run

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/linked_list.o: $(SRC_DIR)/linked_list.c $(SRC_DIR)/linked_list.h
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/linked_list.h
$(OBJ_DIR)/test_utils.o: $(SRC_DIR)/test_utils.c $(SRC_DIR)/test_utils.h
$(OBJ_DIR)/tests.o: $(SRC_DIR)/tests.c $(SRC_DIR)/linked_list.h $(SRC_DIR)/test_utils.h

$(PROJ_NAME): $(OBJ_DIR)/main.o $(OBJ_DIR)/linked_list.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

$(PROJ_NAME)_tests: $(OBJ_DIR)/tests.o $(OBJ_DIR)/linked_list.o $(OBJ_DIR)/test_utils.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

.PHONY: run
run: $(PROJ_NAME)
	./$(PROJ_NAME)

.PHONY: test
test: $(PROJ_NAME)_tests
	./$(PROJ_NAME)_tests

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(PROJ_NAME) $(PROJ_NAME)_tests