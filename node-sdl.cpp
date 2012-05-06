// This is the main DLL file.
#include <v8.h>
#include <node.h>

#ifndef WIN32
#include <sdl/SDL.h>
#else
#include <SDL.h>
#endif

using namespace v8;
using namespace node;

SDL_Surface* Screen;

class SurfaceWrap {
public:
	SDL_Surface* Sur;

	static Handle<Value> Wrap(SDL_Surface* value) {
		Handle<ObjectTemplate> thisTemplate = ObjectTemplate::New();
		thisTemplate->SetInternalFieldCount(1);
		SurfaceWrap* wrap = new SurfaceWrap();
		wrap->Sur = value;
		Local<Object> obj = thisTemplate->NewInstance();
		obj->SetInternalField(0, External::New(wrap));
		return obj;
	}

	static Handle<Value> js_initVideoMode(const Arguments& args) {
		if (Screen != NULL) return Null();
		if (args.Length() != 2) return Null();
		if (!args[0]->IsInt32()) return Null();
		if (!args[1]->IsInt32()) return Null();
		int w = args[0]->IntegerValue();
		int h = args[1]->IntegerValue();
		Screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
		return Wrap(Screen);
	}

	static Handle<Value> js_update(const Arguments& args) {
		if (args[0]->IsObject()) {
			void* value = Local<External>::Cast(args[0]->ToObject()->GetInternalField(0))->Value();
			SurfaceWrap* sur = static_cast<SurfaceWrap*>(value);
			SDL_Flip(sur->Sur);
			return Boolean::New(true);
		} else {
			return Boolean::New(false);
		}
	}
};

class EventWrap {
public:
	SDL_Event Evnt;

	static Handle<Value> Wrap(SDL_Event value) {
		Handle<ObjectTemplate> thisTemplate = ObjectTemplate::New();
		thisTemplate->SetInternalFieldCount(1);
		EventWrap* wrap = new EventWrap();
		wrap->Evnt = value;
		Local<Object> obj = thisTemplate->NewInstance();
		obj->SetInternalField(0, External::New(wrap));
		return obj;
	}

	static Handle<Value> js_getEvent(const Arguments& args) {
		SDL_Event target;
		SDL_PollEvent(&target);
		return Wrap(target);
	}

	static Handle<Value> js_eventType(const Arguments& args) {
		if (args[0]->IsObject()) {
			void* value = Local<External>::Cast(args[0]->ToObject()->GetInternalField(0))->Value();
			EventWrap* evnt = static_cast<EventWrap*>(value);
			return Integer::New(evnt->Evnt.type);
		} else {
			return Boolean::New(false);
		}
	}
};

static Handle<Value> js_sdl_init(const Arguments& args) {
	SDL_Init(SDL_INIT_EVERYTHING);
	return Null();
}

extern "C" void NODE_EXTERN init (Handle<Object> target) {
	HandleScope scope;
	// consts
	target->Set(String::New("event_quit"), Integer::New(SDL_QUIT));

	// methods
	NODE_SET_METHOD(target, "init", js_sdl_init);

	NODE_SET_METHOD(target, "initVideoMode", SurfaceWrap::js_initVideoMode);
	NODE_SET_METHOD(target, "update", SurfaceWrap::js_update);

	NODE_SET_METHOD(target, "getEvent", EventWrap::js_getEvent);
	NODE_SET_METHOD(target, "eventType", EventWrap::js_eventType);
}