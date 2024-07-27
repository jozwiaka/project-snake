.PHONY: install_dependencies prepare configure build test run callgrind all
install_dependencies:
	apt install_dependencies libsdl2-dev
prepare: install_dependencies
	rm -rf build
	mkdir build
configure: prepare
	cd build && cmake ..
build: configure
	cd build && cmake --build .
test: build
	# cd build && make test
run:
	cd build && ./exe
callgrind:
	cd build && valgrind --tool=callgrind ./exe && kcachegrin callgrind.out.*
all: install_dependencies prepare configure build test run
