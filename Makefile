CC = g++

CFLAGS = util.cpp -g -Wall -O3

TARGET = main

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET);\
	rm -f res/*;\

check:
	rm -f res/*;\
	./all_tests.sh;\
	./verify_results.sh;\
