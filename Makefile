all: main

main: myerror.o command.o process.o main.o
	gcc $^ -Wall -o $@

clean:
	rm -f main *.d *.o

.PHONY: clean all

sources = myerror.c command.c process.c main.c

include $(sources:.c=.d)

%.d: %.c
	set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.o: %.c
	$(COMPILE.c) -g -Wall $(OUTPUT_OPTION) $<
