all: clean socketManager.o server.o  compile clean

compile:	
	gcc socketManager.o server.o -o server -lm 
server.o:
	gcc -c server.c 
socketManager.o:
	gcc -c socketManager.c 
clean:
	find -name '*~' | xargs rm -rf {}
	find -name '*.o' | xargs rm -rf {}
run:
	./server
zip:
	zip -r server *.c *.h Makefile
