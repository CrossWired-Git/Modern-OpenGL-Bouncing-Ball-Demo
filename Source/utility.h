#ifndef UTILITY_H
#define UTILITY_H

#include <GL/glew.h>
extern unsigned int create_shader( const char *vertex_shader_source, const char *fragment_shader_source );
extern void draw_triangle( int x0, int y0, int x1, int y1, int x2, int y2 );
extern void draw_rectangle( int x, int y, int width, int height );
extern void draw_pixel( int x, int y );
extern void draw_line( int x0, int y0, int x1, int y1 );
extern void draw_circle( int cx, int cy, int radius );
extern void draw_sun( int cx, int cy, int radius );
extern int clamp( int value, int min, int max );
extern float clampf( float value, float min, float max );
#endif // UTILITY_H
