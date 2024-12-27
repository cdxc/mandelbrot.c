CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm
TARGET = mandelbrot.c
SRC = main.c
IMG = mandelbrot.ppm

all: $(TARGET) run checksum open_image 

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

open_image: $(IMG)
ifeq ($(wildcard $(IMG)),)
	@echo "no $(IMG). Run the program."
else
	feh $(IMG)
endif

checksum: $(IMG)
ifeq ($(wildcard $(IMG)),)
	@echo "no $(IMG). Run the program."
else
	sha256sum $(IMG)
endif


clean:
	rm -f $(TARGET) $(IMG)

.PHONY: all run open_image clean

