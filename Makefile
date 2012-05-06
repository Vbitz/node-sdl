NODEPATH = /usr/local/Cellar/node/0.6.8/


build:
	g++ -g node-sdl.cpp -L$(NODEPATH)bin/ -I$(NODEPATH)include/node/ -lSDL -lSDLMain -lnode -lv8 -luv -o node-sdl