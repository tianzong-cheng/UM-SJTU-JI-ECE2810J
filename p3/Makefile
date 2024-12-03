CC = clang++
CFLAGS = -std=c++1z -g -O2 -Wall -Wextra -Werror -pedantic -Wno-unused-result -Wconversion -Wvla

TARGET = main
HEADERS = sokoban.hpp

all: $(TARGET)

$(TARGET): main.cpp $(HEADERS)
	$(CC) $(CFLAGS) main.cpp -o $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
