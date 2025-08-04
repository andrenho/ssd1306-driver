CPPFLAGS = -Wall -Wextra

OBJ = example.o ../ssd1306.o

all: ssd1306-example

ssd1306-example: ${OBJ}
	gcc -o $@ $^ -lpigpio

clean:
	rm -f ${OBJ}
