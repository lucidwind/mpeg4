CC := g++

FLAGS :=  \
	-w

INCLUDE := ./

FILES := \
	main.c \
	mpeg4.cpp \
	func.cpp \
	track.cpp


TARGET := mp4read

all:
	@$(CC) -I$(INCLUDE) $(FILES) $(FLAGS) -o $(TARGET)

clean:
	@rm -f *.o $(TARGET)
