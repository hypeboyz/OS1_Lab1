CROSS_PREFIX=i686-gentoo-linux-gnu-
CC=$(CROSS_PREFIX)gcc
OPT=-march=core2 -msse2
CC_FLAGS= -c -I ./include/ -Wall -Wextra --std=gnu99 $(OPT)
LD_SLIB=-Wl,-rpath=./lib -L ./lib -llab1 
LIB_DIR=./lib/
LIB=$(LIB_DIR)liblab1.a
SLIB=$(LIB_DIR)liblab1.so
MAKE=make

all: $(SLIB) exercise1 exercise2 exercise3 exercise4

exercise1: exercise1.c
	$(CC) $(CC_FLAGS) -o $@.o $<
	$(CC) $@.o $(LD_SLIB) -o $@
exercise2: exercise2.c
	$(CC) $(CC_FLAGS) -o $@.o $<
	$(CC) $@.o $(LD_SLIB) -o $@
exercise3: exercise3.c
	$(CC) $(CC_FLAGS) -o $@.o $<
	$(CC) $@.o $(LD_SLIB) -o $@ -lrt
exercise4: exercise4.c
	$(CC) $(CC_FLAGS) -o $@.o $<
	$(CC) $@.o $(LD_SLIB) -o $@ -lrt -lpthread

%: %.c $(SLIB)
	$(CC) $(CC_FLAGS) -o $@.o $<
	$(CC) $@.o $(LD_SLIB) -o $@

$(SLIB):
	$(MAKE) liblab1.so -C ./lib

$(LIB):
	$(MAKE) liblab1.a -C ./lib

clean:
	$(MAKE) -C ./lib clean
	rm -f ./exercise1 \
	./exercise2 \
	./exercise3 \
	./exercise4

	rm -f sample
	rm -f $(wildcard ./*.o)
	rm -f $(wildcard ./*~)
