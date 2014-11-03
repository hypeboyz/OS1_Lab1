CROSS_PREFIX=i686-gentoo-linux-gnu-
CC=$(CROSS_PREFIX)gcc
OPT=-march=core2 -msse2 
CC_FLAGS=-c -I ../include/ -Wall -Wextra --std=gnu99 $(OPT)
CC_SLIB_FLAGS=$(CC_FLAGS) -fPIC
CCLD_SLIB_FLAGS=-shared -Wl,-soname,liblab1.so
LIB_OBJS= error.o pathalloc.o lockreg.o

liblab1.so: $(LIB_OBJS)
	$(CC) $(CCLD_SLIB_FLAGS) -o liblab1.so.1.0.1 $^ -lc
	ln -s liblab1.so.1.0.1 $@

liblab1.a: $(LIB_OBJS)
	ar ruv liblab1.a $^
	ranlib liblab1.a
	
$(LIB_OBJS): $(wildcard ./*.c)
	$(CC) $(CC_SLIB_FLAGS) $(wildcard ./*.c)
	
clean: 
	rm -f $(wildcard ./*.o)
	rm -f $(wildcard ./*~)
	rm -f ./liblab1.a
	rm -f ./liblab1.so
	rm -f ./liblab1.so.1.0.1
