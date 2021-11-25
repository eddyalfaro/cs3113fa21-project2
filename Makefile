all: project2

project1:
	cc project2.c -o project2 -pedantic 

run: clean project2
	./project2 tests/input.txt
	make clean

test:
	bats tests.bats

clean:
	rm -f project2

commit:
	git add .
	git commit -m "template commit from Makefile"
	git push
