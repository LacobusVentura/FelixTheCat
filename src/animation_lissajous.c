/*!
	\file animation_lissajous.c
	\brief Lissajous Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "frame.h"
#include "animation.h"
#include "animation_lissajous.h"

#define ANIMATION_LISSAJOUS_NAME                     "Lissajous"
#define ANIMATION_LISSAJOUS_POINTS_PER_FRAME         (300)
#define ANIMATION_LISSAJOUS_FRAMES_PER_FIGURE        (30)
#define ANIMATION_LISSAJOUS_DEFAULT_FPS              (10)


struct animation_lissajous_state_s
{
	double theta;
	double phi;
	int a;
	int b;
	int color;
};

typedef struct animation_lissajous_state_s animation_lissajous_state_t;


static animation_t * animation_lissajous_create( animation_t * parent );
static void animation_lissajous_destroy( animation_t * this );
static void animation_lissajous_next_frame( animation_t * this );
static void animation_lissajous_initialize( animation_t * this );
static void animation_lissajous_finish( animation_t * this );


animation_implementation_t * animation_lissajous_get_implementation( void )
{
	static animation_implementation_t impl;

	impl.create = animation_lissajous_create;
	impl.destroy = animation_lissajous_destroy;
	impl.initialize = animation_lissajous_initialize;
	impl.finish = animation_lissajous_finish;
	impl.first_frame = animation_lissajous_next_frame;
	impl.next_frame = animation_lissajous_next_frame;
	impl.previous_frame = animation_lissajous_next_frame;

	return &impl;
}


static animation_t * animation_lissajous_create( animation_t * parent )
{
	animation_lissajous_state_t * state = NULL;

	state = calloc( 1, sizeof(animation_lissajous_state_t) );

	if(!state)
		return NULL;

	animation_set_default_fps( parent, ANIMATION_LISSAJOUS_DEFAULT_FPS );
	animation_set_name( parent, ANIMATION_LISSAJOUS_NAME );

	animation_set_state( parent, (void*) state );

	return parent;
}


static void animation_lissajous_destroy( animation_t * this )
{
	animation_lissajous_state_t * state = animation_get_state( this );
	free( state );
}


static void animation_lissajous_initialize( animation_t * this )
{
	animation_lissajous_state_t * state = animation_get_state( this );

	srand(time(NULL));

	state->theta = 0.0;
	state->phi = (((2*M_PI) * ((rand() % 100)/ (double)100)) - M_PI);
	state->color = (rand() % 15) + 1;
	state->a = (rand() % 10) + 1;
	state->b = (rand() % 10) + 1;

	frame_clear( animation_get_frame(this) );
}


static void animation_lissajous_finish( animation_t * parent )
{
}


static void animation_lissajous_next_frame( animation_t * this )
{
	double size = 0.0;
	double start = 0.0;
	double end = 0.0;
	double step = 0.0;
	int ncols = 0;
	int nrows = 0;
	int xmid = 0;
	int ymid = 0;
	double theta = 0.0;
	double phi = 0.0;
	int a = 0;
	int b = 0;
	int A = 0;
	int B = 0;
	int x = 0.0;
	int y = 0.0;
	frame_point_t pt;
	frame_t * frm = animation_get_frame( this );
	animation_lissajous_state_t * state = animation_get_state( this );
	console_t * con = animation_get_console( this );


	if( state->theta >= (2 * M_PI) )
		animation_reinitialize( this );

	frame_get_dimensions( frm, &ncols, &nrows );

	xmid = ncols / 2;
	ymid = nrows / 2;

	size = ((2 * M_PI) / ANIMATION_LISSAJOUS_FRAMES_PER_FIGURE);

	start = state->theta;

	step = size / ANIMATION_LISSAJOUS_POINTS_PER_FRAME;

	end = start + size;

	if( end > (2 * M_PI) )
		end = (2 * M_PI);

	A = (ncols / 2) - 1;
	B = (nrows / 2) - 1;

	a = state->a;
	b = state->b;

	phi = state->phi;

	frame_make_point( &pt, 0, 0, state->color, '*' );

	for( theta = start; theta < end; theta += step )
	{
		x = ( A * sin( theta * a + phi ) ) + xmid;
		y = ( B * sin( theta * b ) ) + ymid;

		frame_set_point( frm, x, y, &pt );
	}

	state->theta = theta;

	console_add_line( con, "theta=%0.03f / phi=%0.03f / a=%d / b=%d / color=%d", state->theta, state->phi, state->a, state->b, state->color );
}

/* $Id: animation_lissajous.c 304 2015-08-08 00:57:58Z tiago.ventura $ */
