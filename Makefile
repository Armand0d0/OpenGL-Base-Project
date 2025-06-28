

all: 
	g++ -I ./include -I ./imgui/ *.c *.cpp ./imgui/*.cpp -lglfw -lX11


run:
	g++ -I ./include -I ./imgui/ *.c *.cpp ./imgui/*.cpp -lglfw -lX11
	./a.out
val:
	g++ -g -I ./include -I ./imgui/ *.c *.cpp ./imgui/*.cpp -lglfw -lX11
	valgrind ./a.out
push:
	git add .
	git commit -m "doing something..."
	git push


