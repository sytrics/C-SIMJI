
default: build run

build:
	@echo "----building c-simji----"
	@gcc -g -o bin/c-simji src/ISS.c

run:
	@echo "----running c-simji----"
	@./bin/c-simji
