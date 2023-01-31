CC:=gcc
CFLAGS=-Wall -Wextra -std=c11 -pedantic -g
PROJ_NAME=clli
LIBS=-lm -lncurses

.PHONY: all
all: $(PROJ_NAME) $(PROJ_NAME)_tests test run

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

build/utils/:
	mkdir -p build/utils
build/data/:
	mkdir -p build/data
build/ui/:
	mkdir -p build/data
# Utils
build/utils/test_utils.o: src/utils/test_utils.c src/utils/test_utils.h

# UI
build/ui/ui.o: src/ui/ui.c src/ui/ui.h src/data/linked_list.h

# Data
build/data/data.o: src/data/data.c src/data/data.h src/data/linked_list.h
build/data/linked_list.o: src/data/linked_list.c src/data/linked_list.h
build/data/file_parsing.o: src/data/file_parsing.c src/data/file_parsing.h src/data/linked_list.h src/data/data.h

# Tests
build/tests.o: src/tests.c src/data/linked_list.h src/utils/test_utils.h
# Main
build/main.o: src/main.c src/data/linked_list.h src/data/file_parsing.h src/data/data.h src/ui/ui.h

$(PROJ_NAME): build/main.o build/data/linked_list.o build/data/file_parsing.o build/data/data.o build/ui/ui.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

$(PROJ_NAME)_tests: build/tests.o build/data/linked_list.o build/utils/test_utils.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

.PHONY: run
run: $(PROJ_NAME)
	./$(PROJ_NAME)

.PHONY: test
test: $(PROJ_NAME)_tests
	./$(PROJ_NAME)_tests

.PHONY: clean
clean:
	rm -f build/*.o $(PROJ_NAME) $(PROJ_NAME)_tests