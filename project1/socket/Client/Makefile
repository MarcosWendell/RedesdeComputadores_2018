all: clean socketManager.o client.o  compile clean

compile:	
	gcc socketManager.o client.o -o client -lm 
socketManager.o:
	gcc -c socketManager.c 
client.o:
	gcc -c client.c	
clean:
	find -name '*~' | xargs rm -rf {}
	find -name '*.o' | xargs rm -rf {}
run:
	./client
zip:
	zip -r client *.c *.h Makefile
