TOP = main.c
EXEC = itc
AUX = huffman.c global_req.c shannon_type.c shannon.c fano.c
FLAGS = -Wall -Wextra -g
LINKERS = -lm

$(EXEC): $(TOP) $(AUX) 
	cc $(FLAGS) -o $(EXEC) $(TOP) $(AUX) $(LINKERS)

clean:
	rm -f $(EXEC) *.o

