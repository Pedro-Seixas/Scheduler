OBJECTS = scheduler.o main.o

TARGET = scheduler
all = scheduler

scheduler.o: scheduler.c scheduler.h
	gcc -c -lpthread scheduler.c

main.o: main.c scheduler.h 
	gcc -c main.c

scheduler: main.o scheduler.o
	gcc -o scheduler main.o scheduler.o

clean:
	rm -f $(TARGET)
	rm -f $(OBJECTS)
