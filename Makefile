diseaseMonitorMain: diseaseMonitorMain.o Date.o HashTable.o PatientList.o PatientRecord.o Tree.o MaxHeap.o Queue.o
	g++ -g diseaseMonitorMain.o Date.o HashTable.o PatientList.o PatientRecord.o Tree.o MaxHeap.o Queue.o -o diseaseMonitor

diseaseMonitorMain.o: diseaseMonitorMain.cpp
	g++ -std=c++11 -g3 -c  diseaseMonitorMain.cpp

Date.o: Date.cpp Date.h
	g++ -std=c++11 -g3 -c Date.cpp

HashTable.o: HashTable.cpp HashTable.h
	g++ -g3 -std=c++11 -c HashTable.cpp

PatientList.o: PatientList.cpp PatientList.h
	g++ -g3 -std=c++11 -c PatientList.cpp

PatientRecord.o: PatientRecord.cpp PatientRecord.h
	g++ -g3 -std=c++11 -c  PatientRecord.cpp

Tree.o: Tree.cpp Tree.h
	g++ -g3 -std=c++11 -c Tree.cpp

MaxHeap.o: MaxHeap.cpp MaxHeap.h
	g++ -g3 -std=c++11 -c MaxHeap.cpp

Queue.o: Queue.cpp Queue.h
	g++ -g3 -std=c++11 -c Queue.cpp

clean:
	rm *.o diseaseMonitor

execute:
	./diseaseMonitor -p input_files/medium.txt -h1 20 -h2 20 -b 10

valgrind:
	valgrind ./diseaseMonitor -p input_files/medium.txt -h1 20 -h2 20 -b 10