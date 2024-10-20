CXX = g++
CXXFLAGS =  -Wall -Wextra -Werror -std=c++17 -g

PROJECT = 3DViewer_v2_0
PROJECT_DIR = ${PROJECT}/
TEST_DIR =${PROJECT_DIR}model/tests

EXC := ${PROJECT_DIR}main.cc  ${PROJECT_DIR}view/view.cc ${PROJECT_DIR}view/render.cc ${PROJECT_DIR}model/tests/model_test.cc
SRC := $(filter-out $(EXC), $(shell find ${PROJECT} -type f -name "*.cc"))
OBJ = $(patsubst %.cc, %.o, $(SRC))

TEST_FILE := $(shell find ${TEST_DIR} -type f -name "*.cc" -o -name "*.h")
TAR := $(filter-out ${TEST_FILE}, $(shell find ${PROJECT} -type f -name "*.cc" -o -name "*.h" -o -name "*.c" -o -name "*.qrc" -o -name "pic.png" -o -name "*.ui" -o -name "CMakeLists.txt" -o -name "Makefile" -o -name "Doxyfile" -o -name "description.h" -not -name "model_test.cc" -not -name "model_test.h"))

SRC_TEST := $(shell find ${TEST_DIR} -type f -name "*.cc")
OBJ_TEST := $(patsubst %.cc, %.o, ${SRC_TEST})


NAME := $(shell uname -s)
ifeq (${NAME}, Linux)
	LDFLAGS= -lgtest -lgmock -lpthread
else ifeq (${NAME}, Darwin) 
	LDFLAGS= -lgtest -lgmock
endif

.PHONY: all clean test build

all: clean install

install: clean build
		@-mkdir ../../${PROJECT}
		cp -rf build/${PROJECT}.app ../../${PROJECT}/
uninstall: 
		rm -rf ../../${PROJECT}/
build:
		-mkdir build
		cd build && cmake -DCMAKE_BUILD_TYPE=Release ../3DViewer_v2_0/CMakeLists.txt
		cd build && make
		chmod 755 build/${PROJECT}.app
open:
		open ./../../${PROJECT}/${PROJECT}.app

dvi:
		-doxygen Doxyfile - w html
		open html/index.html
dviclean:
		rm -rf html/ latex/

dist:
		@tar cvf ../../${PROJECT}.tar ${TAR} ./Makefile ./description.h ./Doxyfile
dist2: build
		tar cvf ../../${PROJECT}.tar build/${PROJECT}.app
distclean:
		rm -rf ../../${PROJECT}.tar

start:
		open ${PROJECT_DIR}CMakeLists.txt -a "Qt Creator.app"

test: $(OBJ) $(OBJ_TEST)
		$(CXX) $^ ${LDFLAGS} -o $@
		./test
%.o: %.cc %.h
	${CXX} ${CXXFLAGS} -c $(filter-out %.h, $^) -o $@

leaks: $(OBJ) $(OBJ_TEST)
		$(CXX) $^ ${LDFLAGS} -o test
		leaks --atExit -- ./test
san: clean add_sanitaizer test

check:
		@cp ../materials/linters/.clang-format ./
		clang-format -n $(shell find ${PROJECT} -type f -name "*.cc" -o -name "*.h" -o -name "*.c")
		@rm -rf .clang-format
check2:
		@cp ../materials/linters/.clang-format ./
		clang-format -i $(shell find ${PROJECT} -type f -name "*.cc" -o -name "*.h" -o -name "*.c")
		@rm -rf .clang-format
clean:
		@rm -rf $(shell find ${PROJECT} -type f -name "*.o")
		@rm -rf test report test.info
		@rm -rf $(shell find ${PROJECT}/ -type f -name "*.info" -o -name "*.gcno" -o -name "*.gcda")
		@rm -rf test/*.info tests/*.gcda tests/*.gcno
		@rm -rf build

gcov_report: clean add_coverage $(OBJ) $(OBJ_TEST)
		@$(CXX) $(CXXFLAGS) $(OBJ) $(OBJ_TEST) $(LDFLAGS) -o test
		@./test
		@mkdir report
		lcov --branch-coverage --ignore-errors mismatch,mismatch,empty,inconsistent,usage --no-external --no-recursion -t "test" -q  -o test.info -c -directory 3DViewer_v2_0/model -directory 3DViewer_v2_0/model/command -directory 3DViewer_v2_0/model/tests -directory 3DViewer_v2_0/controller 
		lcov --r test.info "3DViewer_v2_0/model/tests/model_test.cc" -o test.info
		genhtml --ignore-errors mismatch -o report test.info
		open report/index.html
		rm -rf *.gcda *.gcno *.out

add_coverage:
	$(eval CXXFLAGS += -fprofile-arcs -ftest-coverage -O0)
add_sanitaizer:
	$(eval CXXFLAGS += -fsanitize=address)
	$(eval LDFLAGS +=	-fsanitize=address)

