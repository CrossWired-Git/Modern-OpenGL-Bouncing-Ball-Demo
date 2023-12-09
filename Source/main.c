#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <corecrt_wstdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include <cglm/cglm.h>
#include <cglm/call.h>
#include "utility.h"
#define RESOLUTION_WIDTH 640
#define RESOLUTION_HEIGHT 480

#define OPENGL_VERSION_MAJOR 2
#define OPENGL_VERSION_MINOR 0

#define BALL_RADIUS 32
#define BALL_SPEED 4

#define BALL_MIN_X 0
#define BALL_MIN_Y 0
#define BALL_MAX_X RESOLUTION_WIDTH
#define BALL_MAX_Y RESOLUTION_WIDTH

#define BALL_START_X ( RESOLUTION_WIDTH / 2 )
#define BALL_START_Y ( RESOLUTION_HEIGHT / 2 )
#define BALL_START_DIRECTION 1

#define NORMALIZE( )

unsigned long get_time( );

int main( int argc, char **argv ) {
	if ( !glfwInit( ) ) {
		const char *error = NULL;
		glfwGetError( &error );
		fprintf( stderr, "GLFW failed to initialize.\nError: %s\n", error );
		return EXIT_FAILURE;
	}
	fprintf( stdout, "Using GLFW %s\n", glfwGetVersionString( ) );

	glfwDefaultWindowHints( );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR );

	GLFWwindow *window = glfwCreateWindow( RESOLUTION_WIDTH, RESOLUTION_HEIGHT, "", NULL, NULL );
	if ( window == NULL ) {
		const char *error = NULL;
		glfwGetError( &error );
		fprintf( stderr, "GLFW failed to create a window or context.\nError: %s\n", error );
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent( window );

	glViewport( 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT );

	GLenum glew_err = glewInit( );
	if ( glew_err != GLEW_OK ) {
		fprintf( stdout, "GLEW failed to initialize.\n%s\n", glewGetErrorString( glew_err ) );
		return EXIT_FAILURE;
	}

	const char *vertex_shader =
	    "\
#version 110\n\
attribute vec3 position;\n\
uniform mat4 projection;\n\
void main() {\n\
\tgl_Position = projection * vec4(position, 1.0);\n\
}\n\0\
";

	const char *fragment_shader =
	    "\
#version 110\n\
uniform vec4 color;\n\
void main() {\n\
\tgl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n\
}\n\0\
";

	unsigned int shader = create_shader( vertex_shader, fragment_shader );

	unsigned long current_time = get_time( );
	unsigned long previous_time = 0;
	float delta = 0.0f;

	int ball_rad = BALL_RADIUS;

	struct {
		int x;
		int y;
		int direction_x;
		int direction_y;
	} ball;

	ball.x = BALL_START_X;
	ball.y = BALL_START_Y;
	ball.direction_x = BALL_START_DIRECTION;
	ball.direction_y = BALL_START_DIRECTION;

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	while ( !glfwWindowShouldClose( window ) ) {
		previous_time = current_time;
		current_time = get_time( );
		delta = ( ( float )current_time - ( float )previous_time ) / 16.6f;

		glfwPollEvents( );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram( shader );
		mat4 projection_matrix = GLM_MAT4_IDENTITY_INIT;
		glmc_ortho( 0.0f, ( float )RESOLUTION_WIDTH, ( float )RESOLUTION_HEIGHT, 0.0f, 0.0f, 1.0f, projection_matrix );
		glUniformMatrix4fv( glGetUniformLocation( shader, "projection" ), 1, GL_FALSE, projection_matrix[0] );

		ball.x += ( int )( ( float )( BALL_SPEED * ball.direction_x ) * delta );
		ball.y += ( int )( ( float )( BALL_SPEED * ball.direction_y ) * delta );

		if ( ( ball.x + ball_rad ) >= RESOLUTION_WIDTH ) {
			ball.direction_x = -1;
		} else if ( ( ball.x - ball_rad ) <= 0 ) {
			ball.direction_x = 1;
		}

		if ( ( ball.y + ball_rad ) >= RESOLUTION_HEIGHT ) {
			ball.direction_y = -1;
		} else if ( ( ball.y - ball_rad ) <= 0 ) {
			ball.direction_y = 1;
		}

		ball.x = clamp( ball.x, BALL_MIN_X, BALL_MAX_X );
		ball.y = clamp( ball.y, BALL_MIN_Y, BALL_MAX_Y );

		draw_circle( ball.x, ball.y, BALL_RADIUS );
		glfwSwapBuffers( window );
		Sleep( 13 );
	}

	glDeleteShader( shader );
	glfwDestroyWindow( window );
	glfwTerminate( );
}

unsigned long get_time( ) {
	return ( unsigned long )( glfwGetTime( ) * 1000.0f );
}
