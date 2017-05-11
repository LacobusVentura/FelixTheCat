/*!
	\file animation_spirograph.c
	\brief Spirograph Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "frame.h"
#include "animation.h"
#include "animation_spirograph.h"

#define ANIMATION_SPIROGRAPH_NAME          "Spirograph"
#define ANIMATION_SPIROGRAPH_DEFAULT_FPS   (10)
#define ANIMATION_SPIROGRAPH_POINTS_PER_FRAME (1000)

struct animation_spirograph_state_s
{
	int R;
	int r;
	double theta;
	int diameter;
};

typedef struct animation_spirograph_state_s animation_spirograph_state_t;


static animation_t * animation_spirograph_create( animation_t * parent );
static void animation_spirograph_destroy( animation_t * this );
static void animation_spirograph_next_frame( animation_t * this );
static void animation_spirograph_initialize( animation_t * this );
static void animation_spirograph_finish( animation_t * this );


animation_implementation_t * animation_spirograph_get_implementation( void )
{
	static animation_implementation_t impl;

	impl.create = animation_spirograph_create;
	impl.destroy = animation_spirograph_destroy;
	impl.initialize = animation_spirograph_initialize;
	impl.finish = animation_spirograph_finish;
	impl.first_frame = animation_spirograph_next_frame;
	impl.next_frame = animation_spirograph_next_frame;
	impl.previous_frame = animation_spirograph_next_frame;

	return &impl;
}


static animation_t * animation_spirograph_create( animation_t * parent )
{
	animation_spirograph_state_t * state = NULL;

	state = calloc( 1, sizeof(animation_spirograph_state_t) );

	if(!state)
		return NULL;

	animation_set_default_fps( parent, ANIMATION_SPIROGRAPH_DEFAULT_FPS );
	animation_set_name( parent, ANIMATION_SPIROGRAPH_NAME );

	animation_set_state( parent, (void*) state );

	return parent;
}


static void animation_spirograph_destroy( animation_t * this )
{
	free( animation_get_state( this ) );
}


static void animation_spirograph_initialize( animation_t * this )
{
	animation_spirograph_state_t * state = animation_get_state(this);

	srand(time(NULL));

	state->theta = 0.0;
	state->R = ( rand() % 50 ) + 1;
	state->r = ( rand() % state->R ) + 1;
}


static void animation_spirograph_finish( animation_t * parent )
{
}


static void animation_spirograph_next_frame( animation_t * this )
{
	int ncols = 0;
	int nrows = 0;
	int xmid = 0;
	int ymid = 0;
	frame_point_t pt;
	animation_spirograph_state_t * state = animation_get_state(this);
	frame_t * frm = animation_get_frame(this);
	console_t * con = animation_get_console( this );

	frame_get_dimensions( frm, &ncols, &nrows );

	xmid = ncols / 2;
	ymid = nrows / 2;

	if( state->theta > (M_PI * 2) )
	{
		state->theta = 0.0;
		state->R = ( rand() % 100 ) + 20;
		state->r = ( rand() % 20 ) + 1;
		frame_clear(frm);
	}

	int R = state->R;
	int r = state->r;
	double t = 0.0;
	double d = nrows / 3;
	int x = 0;
	int y = 0;
	double b = 0.0;
	double start = 0.0;
	double end = 0.0;
	double step = 0.0;

	start = state->theta;
	end = start + (M_PI / 10);
	step = (end - start) / ANIMATION_SPIROGRAPH_POINTS_PER_FRAME;

	frame_make_point( &pt, 0, 0, 2, '*' );

	for( t = start; t < end; t += step )
	{
		b = ((R - r) * t) / r;

		x = ( (R - r) * cos(t) ) + ( d * cos(b) );
		y = ( (R - r) * sin(t) ) + ( d * sin(b) );

		frame_set_point( frm, x + xmid, y + ymid, &pt );
	}

	state->theta = end;

	console_add_line( con, "theta=%f / R=%d / r=%d", state->theta, state->R, state->r );
}

/* $Id: animation_spirograph.c 304 2015-08-08 00:57:58Z tiago.ventura $ */
