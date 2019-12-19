all: control write

control: control.c
	gcc -o control control.c

write: write.c
	gcc -o write write.c

clean:
	rm *.o
	rm control
	rm write
	rm *.gch
