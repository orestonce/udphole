all	: server clientA clientB

server	: server.c conf.h
	gcc -Wall server.c -o server

clientA	: clientA.c conf.h
	gcc -Wall clientA.c -o clientA

clientB	: clientB.c conf.h
	gcc -Wall clientB.c -o clientB

clean	:
	rm -f server clientA clientB
