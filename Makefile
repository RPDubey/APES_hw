main.elf: main.c threads.o
	gcc -g -Werror -o $@ $^ -lpthread

threads.o: threads.c threads.h
	gcc -g -c -o $@ $< -lpthread

clean:
	rm -f *.o *.elf
