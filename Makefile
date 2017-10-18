all: main

main: error.o command.o process.o main.o
	gcc $^ -o -Wall $@

clean:
	rm -f main *.d *.o

.PHONY: clean all

sources = error.c command.c process.c main.c

include $(sources:.c=.d)

%.d: %.c
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.c
	$(COMPILE.c) $(OUTPUT_OPTION) -Wall $<
