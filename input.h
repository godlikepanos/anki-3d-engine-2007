#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL/sdl.h>
#include "local.h"

class input_t
{		
	public:
		input_t() { Reset(); }
		void Reset();
		void HandleEvents();
		
		// keys and btns
		short keys [SDLK_LAST];  // shows the current key state. 0: unpressed, 1: pressed once, n is >1: kept pressed 'n' times continucely
		short mouse_btns [8];    // mouse btns. Supporting 3 btns & wheel. Works the same as above
		
		// mouse movement	
		vec2_t mouse_pos; // the coords are in the Cartesian system
		vec2_t mouse_velocity;
		
};


extern input_t input;

#endif