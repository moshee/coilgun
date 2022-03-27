test: ../graphics.c ../graphics.h ../font.h test.c
	gcc -c -g -D__PC_TEST__ -o graphics.o ../graphics.c
	gcc -c -g -D__PC_TEST__ -o test.o test.c
	gcc -o test graphics.o test.o

test.pbm: test
	./test

test.png: test.pbm
	convert $< $@