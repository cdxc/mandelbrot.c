CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm -lpthread
TARGET = mandelbrot.c
SRC = main.c
IMG = output.ppm
PNG= image.png

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

export: transparent normal 

transparent: $(IMG)
ifeq ($(wildcard $(IMG)),)
	@echo "no $(IMG). Run the program."
else
	ffmpeg -y -i $(IMG) -vf "colorkey=black:0.1:0.1" -c:v png $(PNG)

endif


normal: $(IMG)
ifeq ($(wildcard $(IMG)),)
	@echo "no $(IMG). Run the program."
else
	ffmpeg -y -i $(IMG) $(PNG)

endif
checksum: $(IMG)
ifeq ($(wildcard $(IMG)),)
	@echo "no $(IMG). Run the program."
else
	sha256sum $(IMG)
endif


clean:
	rm -f $(TARGET) $(IMG)

.PHONY: all export run open_image clean

