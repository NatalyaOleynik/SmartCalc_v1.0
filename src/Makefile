PROJECT_NAME = SmartCalc
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -g
SOURCE = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SOURCE))
LIB = $(PROJECT_NAME).a
TEST = test
GCOV = gcov_report
REP = report
UNAME := $(shell uname -s)
PREFIX = SmartCalc_app
DVI = dvi.html
ARCHIVE = Archive_SmartCalc_v1.0

ifeq ($(UNAME), Darwin)
LIBS = $(shell pkg-config --cflags --libs check)
OPEN_REPORT = open
LEAKS = CK_FORK=no leaks --atExit -- ./test
endif
ifeq ($(UNAME), Linux)
LIBS = -lcheck -lsubunit -lrt -lm -lpthread -g
OPEN_REPORT = xdg-open
LEAKS = CK_FORK=no valgrind -s --leak-check=full --track-origins=yes ./test
endif

all : clean $(LIB) $(TEST) $(GCOV)

clean : 
	rm -rf ${TEST}.c $(TEST) *.gcno *.gcda *.gch ${TEST}.info $(REP) $(LIB) $(OBJ) *.tar.gz

$(LIB) : $(OBJ)
	ar rc $@ $^
	ranlib $@

$(TEST) : 
	checkmk clean_mode=1 test.check > ${TEST}.c
	$(CC) $(SOURCE) ${TEST}.c -o $@ $(LIBS)
	./$@

$(GCOV): $(TEST)
	$(CC) --coverage ${TEST}.c $(SOURCE) -o $(TEST) $(LIBS)
	./$(TEST)
	lcov -t "test" -o ${TEST}.info -c -d .
	genhtml -o $(REP) ${TEST}.info
	$(OPEN_REPORT) report/index.html

install :
	mkdir $(PREFIX)
	cd $(PREFIX) && qmake6 ../SmartCalc_v1.0/SmartCalc.pro
	cd $(PREFIX) && make

run : 
	open $(PREFIX)/SmartCalc.app

uninstall :
	rm -rf $(PREFIX)

dvi :
	open $(DVI)

dist :
	mkdir $(ARCHIVE)
	mkdir $(ARCHIVE)/src
	mkdir $(ARCHIVE)/src/SmartCalc_v1.0
	cp Makefile *.c *.h $(DVI) $(ARCHIVE)/src
	cp SmartCalc_v1.0/*.cpp SmartCalc_v1.0/*.h SmartCalc_v1.0/*.ui SmartCalc_v1.0/*.pro $(ARCHIVE)/src/SmartCalc_v1.0
	tar cvzf Archive_SmartCalc_v1.0.tar.gz $(ARCHIVE)
	rm -rf $(ARCHIVE)

check:
	cp ../.clang-format ./
	clang-format -style=Google -n *.c *.h SmartCalc_v1.0/*.cpp SmartCalc_v1.0/*.h
	rm .clang-format

leaks:	
	$(LEAKS)