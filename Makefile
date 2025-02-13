#remove the @ to see the command being printed
all:
	@g++ Game.cpp -L./lib -I./include -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main
