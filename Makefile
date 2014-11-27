WARN= -Wall -Wextra -Werror

HEAD_PATH=/home/alex/.myheaders
LIB_PATH=/home/alex/.mylibs

#HEADERS=
#LIB_COMPONENTS=

.PHONY: clean

fractal_plot:
	gcc -I$(HEAD_PATH) -L$(LIB_PATH) $(WARN) fractal_plot.c  -o fractal_plot -lm -lpnghelper

clean:
	rm *.o
