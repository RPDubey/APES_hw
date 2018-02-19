main.elf: main.c threads.o dlink_list.o
	gcc -g -o $@ $^ -lpthread

threads.o: threads.c threads.h
	gcc -g -c -o $@ $< -lpthread

dlink_list.o: dlink_list.c dlink_list.h
	gcc -g -c -o $@ $< 

clean:
	rm -f *.o *.elf
