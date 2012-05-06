var sdl = require("./node-sdl");
sdl.init();
screen = sdl.initVideoMode(800, 600);
running = true;
while (running) {
	evnt = sdl.getEvent();
	if (sdl.eventType(evnt) == sdl.event_quit) { running = false; }
	sdl.update(screen);
}