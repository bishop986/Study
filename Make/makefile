helloworld:Home.o func.o
	g++ -o helloworld Home.o func.o
Home.o:Home.cpp Print.h
	g++ -c Home.cpp
func.o:func.cpp Print.h
	g++ -c func.cpp
clean:
	rm Home func.o Home.o 
