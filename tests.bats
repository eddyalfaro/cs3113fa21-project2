#!/usr/bin/env bats

setup()
{
	PATH=$(pwd):$PATH
}

teardown()
{
	rm -f testOutput*.txt
}

diff_files()
{
	diff -b -B -y --suppress-common-lines -w -i --strip-trailing-cr --suppress-common-lines $1 $2 
}

@test "compiletest" {
	make all
	run which project1
	[ "$status" -eq 0 ]
}

@test "test1" {
	./project1  tests/input.txt > testOutput1.txt
	run diff_files testOutput1.txt tests/output.txt
	echo $output
	[ "$status" -eq 0 ]
}


@test "testclean" {
	make clean
	run which project1
	[ "$status" -ne 0 ]
}
