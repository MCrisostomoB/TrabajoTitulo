CC = g++
CFLAGS = -Wall
LDFLAGS =
OBJFILES = utils.o greedy.o main.o
TARGET = GGen
all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~