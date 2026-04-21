CC = gcc
CFLAGS = -Iincludes

TARGET = main
SRCS = $(wildcard src/*.c)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)