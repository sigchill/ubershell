CC     = gcc
CFLAGS = -Wall -Wextra -g
TARGET = ubershell
SRCS   = main.c builtin.c
OBJS   = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
