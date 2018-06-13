EXEC = hashtest

SRCS=$(wildcard *.c)

OBJS=$(SRCS:.c=.o)

all: $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

clean:
	$(RM) $(OBJS)