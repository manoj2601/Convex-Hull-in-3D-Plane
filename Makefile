all:
	g++ main.cpp functions.cpp -o exec

run:
	./exec CONVEX.IN CONVEX.OUT

clean:
	rm -f exec