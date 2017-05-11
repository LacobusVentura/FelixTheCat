/*!
	\file animation_fernfractal.c
	\brief Barnsley Fern Fractal Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <time.h>

#include "frame.h"
#include "animation.h"
#include "animation_fernfractal.h"

#define ANIMATION_FERN_FRACTAL_NAME               "FernFractal"
#define ANIMATION_FERN_FRACTAL_DEFAULT_FPS        (10)
#define ANIMATION_FERN_FRACTAL_POINTS_PER_FRAME   (500)


struct animation_fern_fractal_state_s
{
	double xn;
	double yn;
	int points;
};

typedef struct animation_fern_fractal_state_s animation_fern_fractal_state_t;


static animation_t * animation_fern_fractal_create( animation_t * parent );
static void animation_fern_fractal_destroy( animation_t * this );
static void animation_fern_fractal_next_frame( animation_t * this );
static void animation_fern_fractal_initialize( animation_t * this );
static void animation_fern_fractal_finish( animation_t * this );


animation_implementation_t * animation_fern_fractal_get_implementation( void )
{
	static animation_implementation_t impl;

	impl.create = animation_fern_fractal_create;
	impl.destroy = animation_fern_fractal_destroy;
	impl.initialize = animation_fern_fractal_initialize;
	impl.finish = animation_fern_fractal_finish;
	impl.first_frame = animation_fern_fractal_next_frame;
	impl.next_frame = animation_fern_fractal_next_frame;
	impl.previous_frame = animation_fern_fractal_next_frame;

	return &impl;
}


static animation_t * animation_fern_fractal_create( animation_t * parent )
{
	animation_fern_fractal_state_t * state = NULL;

	state = calloc( 1, sizeof(animation_fern_fractal_state_t) );

	if(!state)
		return NULL;

	animation_set_default_fps( parent, ANIMATION_FERN_FRACTAL_DEFAULT_FPS );
	animation_set_name( parent, ANIMATION_FERN_FRACTAL_NAME );
	animation_set_state( parent, (void*) state );

	return parent;
}


static void animation_fern_fractal_destroy( animation_t * this )
{
	free( animation_get_state( this ) );
}


static void animation_fern_fractal_initialize( animation_t * this )
{
	animation_fern_fractal_state_t * state = animation_get_state(this);

	srand(time(NULL));

	/* Initial State */
	state->xn = 0.0;
	state->yn = 0.0;
	state->points = 0;

	/* Initial Screen */
	frame_clear( animation_get_frame( this ) );
}


static void animation_fern_fractal_finish( animation_t * parent )
{
}


static void animation_fern_fractal_next_frame( animation_t * this )
{
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	double d = 0.0;
	double e = 0.0;
	double f = 0.0;
	double xn = 0.0;
	double yn = 0.0;
	int r = 0;
	int x = 0;
	int y = 0;
	int i = 0;
	int xmax = 0;
	int ymax = 0;
	int xmid = 0;
	int ymid = 0;
	frame_point_t pt;

	int size = 45; /* Leaf Size Factor */
	int p[] = { 0, 1, 85, 92, 100 }; /* Probability Factors */

	animation_fern_fractal_state_t * state = animation_get_state(this);
	frame_t * frm = animation_get_frame( this );
	console_t * con = animation_get_console( this );

	if( state->points > ANIMATION_FERN_FRACTAL_POINTS_PER_FRAME * 100 )
		animation_reinitialize(this);

	for( i = 0; i < ANIMATION_FERN_FRACTAL_POINTS_PER_FRAME; i++ )
	{
		r = rand() % 100;

		if( (r >= p[0] ) && (r <= p[1]) )
		{
			/* Portion: Stem */
			a = 0.0;
			b = 0.0;
			c = 0.0;
			d = 0.16;
			e = 0.0;
			f = 0.0;
		 }
		 else if( (r > p[1]) && (r <= p[2]) )
		 {
			/* Portion: Successively smaller leaflets */
			a = 0.85;
			b = 0.04;
			c = -0.04;
			d = 0.85;
			e = 0.0;
			f = 1.6;
		}
		else if( (r > p[2]) && (r <= p[3]) )
		{
			/* Portion: Largest left-hand leaflet */
			a = 0.2;
			b = -0.26;
			c = 0.23;
			d = 0.22;
			e = 0.0;
			f = 1.6;
		}
		else if( (r > p[3]) && (r <= p[4]) )
		{
			/* Portion: Largest right-hand leaflet */
			a = -0.15;
			b = 0.28;
			c = 0.26;
			d = 0.24;
			e = 0.0;
			f = 0.44;
		}

		/* Transformation */
		xn = ( a * state->xn ) + ( b * state->yn ) + e;
		yn = ( c * state->xn ) + ( d * state->yn ) + f;

		state->xn = xn;
		state->yn = yn;

		/* Message */
		console_add_line( con, "pts=%d / r=%d / xn=%0.3f / yn=%0.3f", state->points, r, xn, yn );

		/* Fit Figure to the Frame Coordinates */
		frame_get_dimensions( frm, &xmax, &ymax );

		xmid = xmax / 2;
		ymid = ymax / 2;

		x = ( ( xn - 0.4738 ) * size ) + xmid;
		y = ymax - (( ( yn - 4.9991 ) * size ) + ymid);

		/* Plot! */
		frame_make_point( &pt, 0, 0, 2, '.' );
		frame_set_point( frm, x, y, &pt );

		state->points++;
	}
}

/* $Id: animation_fernfractal.c 300 2015-07-31 04:49:42Z tiago.ventura $ */
