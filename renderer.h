#ifndef _RENDERER_H_
#define _RENDERER_H_


class renderer_t
{
	public:
		int w, h;
		int frames_num;
		renderer_t() { w=1024; h=768; }
		void Init();
		void Run();
		void DrawGrid();
};


extern renderer_t renderer;

#endif