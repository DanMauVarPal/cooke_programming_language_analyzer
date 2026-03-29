NAME=cooke_analyzer

all: $(NAME)

$(NAME): front.o parser.o
	$(CC) -Wall front.o parser.o -o $(NAME)	

.c.o:
	$(CC) -Wall -c $<


test: $(NAME) front.in
	./$(NAME) front.in

clean:
	$(RM) parser.o front.o $(NAME)
