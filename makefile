clean:
	rm -f a.out binary_tree


binary_tree:	binary_tree.c elapsed_time.h
	cc -Wall -O2 binary_tree.c -o binary_tree -lm

run: 
	binary_tree
