graphexec:	main.o
	gcc main.o -o graphexec

main.o:	main.c
	gcc -c main.c

run-tests:	graphexec
	./graphexec tests/testgraph1.txt
	./graphexec tests/testgraph2.txt

clean:
	rm -Rf *.o *.txt stdin stdout\
		graphexec\