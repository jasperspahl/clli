CC:=gcc
INCLUDES=-Isrc
CFLAGS=-Wall -Wextra -g
CFLAGS+=-std=c11 -pedantic
CFLAGS+=$(INCLUDES)
PROJ_NAME=clli
LIBS=-lm -lncurses -lcurl

.PHONY: all
all: test run

# Utils
build/test_utils.o: src/utils/test_utils.c src/utils/test_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/editor_utils.o: src/utils/editor_utils.c src/utils/editor_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/fetch_readme.o: src/utils/fetch_readme.c src/utils/fetch_readme.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/files.o: src/utils/files.c src/utils/files.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/string_utils.o: src/utils/string_utils.c src/utils/string_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<

# UI
build/ui.o: src/ui/ui.c src/ui/ui.h src/data/linked_list.h src/data/data.h src/utils/editor_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/ui_flow.o: src/ui/ui_flow.c src/ui/ui_flow.h src/ui/ui.h src/utils/files.h src/data/search.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Data
build/data.o: src/data/data.c src/data/data.h src/data/linked_list.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/data_compare.o: src/data/data_compare.c src/data/data_compare.h src/data/data.h src/utils/string_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/linked_list.o: src/data/linked_list.c src/data/linked_list.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/file_parsing.o: src/data/file_parsing.c src/data/file_parsing.h src/data/linked_list.h src/data/data.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/search.o: src/data/search.c src/data/search.h src/data/linked_list.h src/data/data.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Tests
build/tests.o: src/tests.c src/data/linked_list.h src/utils/test_utils.h src/tests/linkedlist_tests.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/linkedlist_tests.o: src/tests/linkedlist_tests.c src/data/linked_list.h src/utils/test_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<
build/string_function_tests.o: src/tests/string_function_tests.c src/tests/string_function_tests.h src/utils/string_utils.h
	$(CC) $(CFLAGS) -c -o $@ $<
# Main
build/main.o: src/main.c src/data/linked_list.h src/data/file_parsing.h src/data/data.h src/ui/ui.h src/ui/ui_flow.h
	$(CC) $(CFLAGS) -c -o $@ $<
# fetch_readme
build/fetch_readme_test.o: src/fetch_readme_test.c src/utils/fetch_readme.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Executables
$(PROJ_NAME): build/main.o build/linked_list.o build/file_parsing.o build/data.o build/ui.o build/editor_utils.o build/fetch_readme.o build/ui_flow.o build/files.o build/string_utils.o build/data_compare.o build/search.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

$(PROJ_NAME)_tests: build/tests.o build/linked_list.o build/test_utils.o build/linkedlist_tests.o build/string_function_tests.o build/string_utils.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

fetch_readme: build/fetch_readme_test.o build/fetch_readme.o
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

.PHONY: docs
docs:
	doxygen Doxyfile
	cd docs/latex && make

.PHONY: build-all
build-all: $(PROJ_NAME) $(PROJ_NAME)_tests
	./$(PROJ_NAME)_tests


.PHONY: run
run: $(PROJ_NAME)
	./$(PROJ_NAME) test.data

.PHONY: test
test: $(PROJ_NAME)_tests
	./$(PROJ_NAME)_tests

.PHONY: clean
clean:
	rm -f build/*.o $(PROJ_NAME) $(PROJ_NAME)_tests
