all: clean socketManager.o server.o  compile

compile:	
	gcc socketManager.o server.o -o server.exe -lm 
server.o:
	gcc -c server.c 
socketManager.o:
	gcc -c socketManager.c 
clean:
	find -name '*~' | xargs rm -rf {}
	find -name '*.o' | xargs rm -rf {}
	find -name '*.exe' | xargs rm -rf {}
run:
	./server.exe
zip:
	zip -r server *.c *.h Makefile
