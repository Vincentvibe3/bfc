build:
	mkdir -p build
	gcc ./src/*.c -o ./build/bfc 

clean:
	rm -r build