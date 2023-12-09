#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned int create_shader( const char *vertex_shader_source, const char *fragment_shader_source ) {
	int error = 0;

	// Prevent morons from using uninitialized or dead strings.
	if ( vertex_shader_source == NULL ) {
		fprintf( stderr, "Vertex shader is null.\n" );
		error = 1;
	}

	if ( fragment_shader_source == NULL ) {
		fprintf( stderr, "Fragment shader is null.\n" );
		error = 1;
	}

	if ( error ) {
		return 0;
	}

	error = 0;

	// Another safety check because why not?
	if ( strlen( vertex_shader_source ) == 0 ) {
		fprintf( stderr, "Vertex shader is empty.\n" );
		error = 1;
	}

	if ( strlen( fragment_shader_source ) == 0 ) {
		fprintf( stderr, "Fragment shader is empty.\n" );
		error = 1;
	}

	if ( error ) {
		return 0;
	}

	// Now we have to make sure the runtime doesn't mess us up too.
	// Goddamn I hate this.
	if ( glCreateShader == NULL ) {
		fprintf( stderr, "\"glCreateShader\" doesn't exist.\n" );
		return 0;
	}

	if ( glShaderSource == NULL ) {
		fprintf( stderr, "\"glShaderSource\" doesn't exist.\n" );
		return 0;
	}

	if ( glCompileShader == NULL ) {
		fprintf( stderr, "\"glCompileShader\" doesn't exist.\n" );
		return 0;
	}

	if ( glGetShaderiv == NULL ) {
		fprintf( stderr, "\"glGetShaderiv\" doesn't exist.\n" );
		return 0;
	}

	if ( glGetShaderInfoLog == NULL ) {
		fprintf( stderr, "\"glGetShaderInfoLog\" doesn't exist.\n" );
		return 0;
	}

	if ( glDeleteShader == NULL ) {
		fprintf( stderr, "\"glDeleteShader\" doesn't exist.\n" );
		return 0;
	}

	// Now we get on to the real stuff.
	int success = 0; // This is both used for shader compilation and shader linkage.
	int shader_log_length = 0;
	char *shader_log = NULL;

	unsigned int vertex_shader = glCreateShader( GL_VERTEX_SHADER );

	// Don't question why OpenGL would ever fail this.
	if ( vertex_shader == 0 ) {
		fprintf( stderr, "OpenGL failed to create a vertex shader.\n" );
		return 0;
	}

	glShaderSource( vertex_shader, 1, &vertex_shader_source, NULL );
	glCompileShader( vertex_shader );
	glGetShaderiv( vertex_shader, GL_COMPILE_STATUS, &success );
	if ( !success ) {
		fprintf( stderr, "Vertex shader failed to compile.\n" );
		glGetShaderiv( vertex_shader, GL_INFO_LOG_LENGTH, &shader_log_length );
		shader_log = calloc( shader_log_length, sizeof( char ) );
		if ( shader_log == NULL ) {
			fprintf( stderr, "An error log couldn't be formed due to a memory error.\n" );
		} else {
			glGetShaderInfoLog( vertex_shader, shader_log_length, NULL, shader_log );
			fprintf( stderr, "%s\n", shader_log );
			free( shader_log );
		}
		glDeleteShader( vertex_shader );
		return 0;
	}

	// Quick reset.
	success = 0;
	shader_log_length = 0;

	// And we repeat the same process. Should really make a seperate function for this...
	unsigned int fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );

	if ( fragment_shader == 0 ) {
		fprintf( stderr, "OpenGL failed to create a fragment shader.\n" );
		return 0;
	}

	glShaderSource( fragment_shader, 1, &fragment_shader_source, NULL );
	glCompileShader( fragment_shader );
	glGetShaderiv( fragment_shader, GL_COMPILE_STATUS, &success );
	if ( !success ) {
		fprintf( stderr, "Fragment shader failed to compile.\n" );
		glGetShaderiv( fragment_shader, GL_INFO_LOG_LENGTH, &shader_log_length );
		shader_log = calloc( shader_log_length, sizeof( char ) );
		if ( shader_log == NULL ) {
			fprintf( stderr, "An error log couldn't be formed due to a memory error.\n" );
		} else {
			glGetShaderInfoLog( fragment_shader, shader_log_length, NULL, shader_log );
			fprintf( stderr, "%s\n", shader_log );
			free( shader_log );
		}
		glDeleteShader( vertex_shader );
		glDeleteShader( fragment_shader );
		return 0;
	}

	unsigned int shader_program = glCreateProgram( );
	if ( shader_program == 0 ) {
		fprintf( stderr, "OpenGL failed to create a shader program.\n" );
		return 0;
	}

	glAttachShader( shader_program, vertex_shader );
	glAttachShader( shader_program, fragment_shader );
	glDeleteShader( vertex_shader );
	glDeleteShader( fragment_shader );

	glLinkProgram( shader_program );
	glGetProgramiv( shader_program, GL_LINK_STATUS, &success );
	if ( !success ) {
		fprintf( stderr, "Shader program failed to link.\n" );
		glGetProgramiv( shader_program, GL_INFO_LOG_LENGTH, &shader_log_length );
		shader_log = calloc( shader_log_length, sizeof( char ) );
		if ( shader_log == NULL ) {
			fprintf( stderr, "An error log couldn't be formed due to a memory error.\n" );
		} else {
			glGetProgramInfoLog( shader_program, shader_log_length, NULL, shader_log );
			fprintf( stderr, "%s\n", shader_log );
			free( shader_log );
		}
		return 0;
	}

	return shader_program;
}

void draw_triangle( int x0, int y0, int x1, int y1, int x2, int y2 ) {
	const float verts[9] = { ( float )x0, ( float )y0, 0.0f, ( float )x1, ( float )y1, 0.0f, ( float )x2, ( float )y2, 0.0f };
	unsigned int buffer = 0;
	glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( verts ), verts, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), NULL );
	glEnableVertexAttribArray( 0 );
	glDrawArrays( GL_TRIANGLES, 0, 3 );
	glDeleteBuffers( 1, &buffer );
}

void draw_rectangle( int x, int y, int width, int height ) {
	draw_triangle( x, y, x + width, y, x, y + height );
	draw_triangle( x, y + height, x + width, y + height, x + width, y );
}

void draw_pixel( int x, int y ) {
	draw_rectangle( x, y, 1, 1 );
}

void draw_line( int x0, int y0, int x1, int y1 ) {
	const float verts[6] = { ( float )x0, ( float )y0, 0.0f, ( float )x1, ( float )y1, 0.0f };
	unsigned int buffer = 0;
	glGenBuffers( 1, &buffer );
	glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( verts ), verts, GL_STATIC_DRAW );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), NULL );
	glEnableVertexAttribArray( 0 );
	glDrawArrays( GL_LINES, 0, 3 );
}

void draw_circle( int cx, int cy, int radius ) {
	int x = radius;
	int y = 0;
	int err = 0;

	while ( x >= y ) {
		draw_pixel( cx + x, cy + y );
		draw_pixel( cx + y, cy + x );
		draw_pixel( cx - y, cy + x );
		draw_pixel( cx - x, cy + y );
		draw_pixel( cx - x, cy - y );
		draw_pixel( cx - y, cy - x );
		draw_pixel( cx + y, cy - x );
		draw_pixel( cx + x, cy - y );

		if ( err <= 0 ) {
			y++;
			err += 2 * y + 1;
		}

		if ( err > 0 ) {
			x--;
			err -= 2 * x + 1;
		}
	}
}

void draw_sun( int cx, int cy, int radius ) {
	int x = radius;
	int y = 0;
	int err = 0;

	while ( x >= y ) {
		draw_line( cx, cy, cx + x, cy + y );
		draw_line( cx, cy, cx + y, cy + x );
		draw_line( cx, cy, cx - y, cy + x );
		draw_line( cx, cy, cx - x, cy + y );
		draw_line( cx, cy, cx - x, cy - y );
		draw_line( cx, cy, cx - y, cy - x );
		draw_line( cx, cy, cx + y, cy - x );
		draw_line( cx, cy, cx + x, cy - y );

		if ( err <= 0 ) {
			y++;
			err += 2 * y + 1;
		}

		if ( err > 0 ) {
			x--;
			err -= 2 * x + 1;
		}
	}
}

int clamp( int value, int min, int max ) {
	if ( value >= max ) {
		return max;
	} else if ( value <= min ) {
		return min;
	}
	return value;
}

float clampf( float value, float min, float max ) {
	if ( value >= max ) {
		return max;
	} else if ( value <= min ) {
		return min;
	}
	return value;
}