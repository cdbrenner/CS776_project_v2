#W/O LOGGER CLASS
x: main.cpp evaluate.o individual.o helpers.o population.o test.o ga.o options.h
	g++ -o x main.cpp evaluate.o individual.o helpers.o population.o test.o ga.o

# WITH LOGGER CLASS
# x: main.cpp evaluate.o individual.o helpers.o population.o test.o ga.o logger.o options.h
# 	g++ -o x main.cpp evaluate.o individual.o helpers.o population.o test.o ga.o logger.o

evaluate.o: evaluate.cpp evaluate.h
	g++ -c evaluate.cpp

ga.o: ga.cpp ga.h
	g++ -c ga.cpp

individual.o: individual.cpp individual.h
	g++ -c individual.cpp

helpers.o: helpers.cpp helpers.h individual.h
	g++ -c helpers.cpp

population.o: population.cpp population.h
	g++ -c population.cpp

test.o: test.cpp test.h
	g++ -c test.cpp

# logger.o: logger.cpp logger.h
# 	g++ -c logger.cpp

clean:
	rm -rf x *.o