

all: 
	 g++ -I ./include *.c *.cpp -lglfw3 -lX11


run:
	g++ -I ./include *.c *.cpp -lglfw3 -lX11
	./a.out

push:
	git add .
	git commit -m "doing something..."
	git push


