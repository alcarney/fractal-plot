WARN= -Wall -Wextra -Werror

#HEAD_PATH=/home/alex/.myheaders
#LIB_PATH=/home/alex/.mylibs

#HEADERS=
#LIB_COMPONENTS=

.PHONY: clean

fractal_plot:
	gcc $(WARN) fractal_plot.c  -o fractal_plot

clean:
	rm *.o
