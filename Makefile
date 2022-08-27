cc = gcc
cflags = -Wall -Werror --std=c11 -g
objs = main.o dfa.o map.o vector.o
exe = run

$(exe): $(objs)
	$(cc) $(cflags) $(objs) -o $(exe) 
main.o: main.c dfa.o
	$(cc) $(cflags) -c main.c
dfa.o: dfa.c dfa.h map.o
	$(cc) $(cflags) -c dfa.c
map.o: map.c map.h
	$(cc) $(cflags) -c map.c
vector.o: vector.c vector.h
	$(cc) $(cflags) -c vector.c
clean:
	rm $(objs) $(exe)
