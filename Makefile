OBJECTS = scheduler.o main.o

TARGET = scheduler
all = scheduler

shell.o: scheduler.c scheduler.h
	gcc -c scheduler.c

main.o: main.c shell.h 
	gcc -c main.c

schnoz: main.o scheduler.o
	gcc -o scheduler main.o scheduler.o

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)
