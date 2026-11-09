# Define o compilador
CC = gcc

CFLAGS = -Wall -std=c99 -O3 #normal
# CFLAGS = -Wall -std=c99 -g #valgrind
# CFLAGS = -Wall -std=c99 -g -pg #com tempo

LIBS = -lm
TARGET = healkristin
SRCS = healkristin.c tasks.c

OBJS = $(SRCS:.c=.o)

#make
all: $(TARGET)

#executavel
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean: #make clean apaga os ficheiros .results
	rm -f $(OBJS) $(TARGET) gmon.out massif.out.*