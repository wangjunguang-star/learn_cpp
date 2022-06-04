CC = g++
CFLAGS = -g -std=c++11
objects = main.o tree.o dataset.o utils.o

edit : $(objects)
	$(CC) $(CFLAGS) -o edit $(objects) -std=c++11

$(objects) : %.o : %.cc
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	rm $(objects)
