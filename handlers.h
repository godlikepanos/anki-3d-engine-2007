#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#define TIMER_TICK 33

extern void InitWindow( int w, int h );
extern void QuitApp( int code );
extern void WaitForNextTick();

#endif