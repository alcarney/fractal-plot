WARN= -Wall -Wextra -Werror

#HEAD_PATH=/home/alex/.myheaders
#LIB_PATH=/home/alex/.mylibs

#HEADERS=
#LIB_COMPONENTS=

.PHONY: clean

fractal_plot: newtons_method.o
	gcc $(WARN) newtons_method.o fractal_plot.c  -o fractal_plot

newtons_method.o:
	gcc -c $(WARN) newtons_method.c -o newtons_method.o

clean:
	rm *.o
