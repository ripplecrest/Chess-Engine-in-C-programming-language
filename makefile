all:
	gcc -oFast ssc.c -o chess_engine
	
debug:	
	gcc ssc.c -o chess_engine