all: derle calistir

derle:
	@if not exist bin mkdir bin
	gcc -I./include src/*.c -o bin/Odev.exe

calistir:
	./bin/Odev.exe

clean:
	@if exist bin\Odev.exe del bin\Odev.exe