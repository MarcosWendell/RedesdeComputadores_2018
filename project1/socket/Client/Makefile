all: clean socketManager.o client.o  compile

compile:	
	gcc socketManager.o client.o -o client.exe -lm 
socketManager.o:
	gcc -c socketManager.c 
client.o:
	gcc -c client.c	
clean:
	find -name '*~' | xargs rm -rf {}
	find -name '*.o' | xargs rm -rf {}
	find -name '*.exe' | xargs rm -rf {}
run:
	./client.exe
zip:
	zip -r client *.c *.h Makefile
