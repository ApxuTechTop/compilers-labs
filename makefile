antlr=antlr-4.13.0-complete.jar

.PHONY: build format test clean download

all: format download build test

download:
	curl -O https://www.antlr.org/download/$(antlr)

build:
	
	cmake --preset debug
	cmake --build build/debug

format:
	git ls-files *.cpp | xargs clang-format -i -style=file --verbose
	git ls-files *.hpp | xargs clang-format -i -style=file --verbose

test:
	ctest --preset debug

clean:
	cmake --build build/debug --target clean

