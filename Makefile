test: background project

p:
	./pro pro_out P0

pro: process.cpp
	g++ process.cpp -std=c++14 -o pro
	./pro pro_out P0

background:
	./executor 3 ./instructions.txt &

project: process.cpp watchdog.cpp
	g++ process.cpp -std=c++14 -o process
	g++ watchdog.cpp -std=c++14 -o watchdog
	./watchdog 3 process_output.txt watchdog_output.txt

execfile: executor.cpp
	g++ executor.cpp -std=c++14 -o executor