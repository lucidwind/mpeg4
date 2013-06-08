CC := gcc

FLAGS :=  \
	-w

INCLUDE := ./

FILES := \
	main.c \
	func.c \
	box.c

TARGET := mp4read

all:
	@$(CC) -I$(INCLUDE) $(FILES) $(FLAGS) -o $(TARGET)

clean:
	@rm -f *.o $(TARGET)
