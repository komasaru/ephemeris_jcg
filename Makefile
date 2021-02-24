gcc_options = -std=c++17 -Wall -O2 --pedantic-errors

ephemeris_jcg: ephemeris_jcg.o eph_jcg.o file.o common.o
	g++92 $(gcc_options) -o $@ $^

ephemeris_jcg.o : ephemeris_jcg.cpp
	g++92 $(gcc_options) -c $<

eph_jcg.o : eph_jcg.cpp
	g++92 $(gcc_options) -c $<

file.o : file.cpp
	g++92 $(gcc_options) -c $<

common.o : common.cpp
	g++92 $(gcc_options) -c $<

run : ephemeris_jcg
	./ephemeris_jcg

clean :
	rm -f ./ephemeris_jcg
	rm -f ./*.o

.PHONY : run clean

