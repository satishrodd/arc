BIN=bin
bin:
	gcc -g main.c prefix_tree.c -o bin
clean:
	rm -rf bin

