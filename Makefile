CC:=gcc
INCLUDES=-Isrc
CFLAGS=-Wall -Wextra -g
CFLAGS+=-std=c11 -pedantic
CFLAGS+=$(INCLUDES)
PROJ_NAME=clli
LIBS=-lm -lncurses -lcurl

.PHONY: all
all: test run

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Utils
build/utils/test_utils.o: src/utils/test_utils.c src/utils/test_utils.h
build/utils/editor_utils.o: src/utils/editor_utils.c src/utils/editor_utils.h
build/utils/fetch_readme.o: src/utils/fetch_readme.c src/utils/fetch_readme.h
build/utils/files.o: src/utils/files.c src/utils/files.h
build/utils/string_utils.o: src/utils/string_utils.c src/utils/string_utils.h

# UI
build/ui/ui.o: src/ui/ui.c src/ui/ui.h src/data/linked_list.h src/data/data.h src/utils/editor_utils.h
build/ui/ui_flow.o: src/ui/ui_flow.c src/ui/ui_flow.h src/ui/ui.h src/utils/files.h

# Data
build/data/data.o: src/data/data.c src/data/data.h src/data/linked_list.h
build/data/data_compare.o: src/data/data_compare.c src/data/data_compare.h src/data/data.h src/utils/string_utils.h
build/data/linked_list.o: src/data/linked_list.c src/data/linked_list.h
build/data/file_parsing.o: src/data/file_parsing.c src/data/file_parsing.h src/data/linked_list.h src/data/data.h

# Tests
build/tests.o: src/tests.c src/data/linked_list.h src/utils/test_utils.h src/tests/linkedlist_tests.h
build/tests/linkedlist_tests.o: src/tests/linkedlist_tests.c src/data/linked_list.h src/utils/test_utils.h
build/tests/string_function_tests.o: src/tests/string_function_tests.c src/tests/string_function_tests.h src/utils/string_utils.h
# Main
build/main.o: src/main.c src/data/linked_list.h src/data/file_parsing.h src/data/data.h src/ui/ui.h src/ui/ui_flow.h
# fetch_readme
build/fetch_readme_test.o: src/fetch_readme_test.c src/utils/fetch_readme.h

# Executables
$(PROJ_NAME): build/main.o build/data/linked_list.o build/data/file_parsing.o build/data/data.o build/ui/ui.o build/utils/editor_utils.o build/utils/fetch_readme.o build/ui/ui_flow.o build/utils/files.o build/utils/string_utils.o build/data/data_compare.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

$(PROJ_NAME)_tests: build/tests.o build/data/linked_list.o build/utils/test_utils.o build/tests/linkedlist_tests.o build/tests/string_function_tests.o build/utils/string_utils.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

fetch_readme: build/fetch_readme_test.o build/utils/fetch_readme.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

.PHONY: docs
docs:
	doxygen Doxyfile
	cd docs/latex && make

.PHONY: run
run: $(PROJ_NAME)
	./$(PROJ_NAME) test.data

.PHONY: test
test: $(PROJ_NAME)_tests
	./$(PROJ_NAME)_tests

.PHONY: clean
clean:
	rm -f build/*.o $(PROJ_NAME) $(PROJ_NAME)_tests
