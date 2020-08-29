#ifndef __SYPHON__SERVER__
#define __SYPHON__SERVER__

void syphon_start();
void syphon_stop();
void syphon_exit();
void syphon_size(float w, float h);
void syphon_bind(float w, float h);
int syphon_pushTexture(int tex);
unsigned char *Screen_GrabRect(int x, int y, int w, int h);

#endif
