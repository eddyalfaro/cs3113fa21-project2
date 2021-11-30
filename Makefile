all: project2

project2:
	cc project2.c -o project2 -pedantic 

ff: clean project2
	./project2 FIRSTFIT 26000000 tests/input.txt
	nake clean

test:
	cc test.c -o test -pedantic
	./test
	rm -f test

clean:
	rm -f project2



commit:
	git add .
	git commit -m "template commit from Makefile"
	git push
