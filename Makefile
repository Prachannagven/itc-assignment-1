TOP = main.c
EXEC = itc
AUX = huffman.c global_req.c
FLAGS = -Wall -Wextra -g

$(EXEC): $(TOP) $(AUX) 
	cc $(FLAGS) -o $(EXEC) $(TOP) $(AUX)

clean:
	rm -f $(EXEC) *.o

