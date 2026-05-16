CC = gcc
CFLAGS =-Wall

SRCS = myMonitoringTool.c cpu.c memory.c core_count.c core_freq.c
OBJS = $(SRCS:.c=.o)

TARGET = myMonitoringTool

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
