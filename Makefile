all: project2

project2:
	cc project2.c -o project2 -pedantic -O3

ff: clean project2
	./project2 FIRSTFIT 268435456  tests/input.txt
	make clean

test:
	cc test.c -o test -pedantic
	./test
	rm -f test

clean:
	rm -f project2

debug:
	cc project2.c -o project2 -g -O3

commit:
	git add .
	git commit -m "template commit from Makefile"
	git push
