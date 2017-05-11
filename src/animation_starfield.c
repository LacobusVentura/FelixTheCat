/*!
	\file animation_starfield.c
	\brief Starfield Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>

#include "frame.h"
#include "animation.h"
#include "animation_starfield.h"


#define ANIMATION_STARFIELD_NAME          "Starfield"
#define ANIMATION_STARFIELD_COLOR_COUNT   (256)
#define ANIMATION_STARFIELD_DEFAULT_FPS   (20)
#define ANIMATION_STARFIELD_STAR_COUNT    (300)


struct animation_starfield_star_s
{
	double x;
	double y;
	double z;
	double speed;
	int screen_x;
	int screen_y;
};

typedef struct animation_starfield_star_s animation_starfield_star_t;


struct animation_starfield_state_s
{
	animation_starfield_star_t star[ ANIMATION_STARFIELD_STAR_COUNT ];
};

typedef struct animation_starfield_state_s animation_starfield_state_t;


static animation_t * animation_starfield_create( animation_t * parent );
static void animation_starfield_destroy( animation_t * this );
static void animation_starfield_next_frame( animation_t * this );
static void animation_starfield_initialize( animation_t * this );
static void animation_starfield_finish( animation_t * this );


animation_implementation_t * animation_starfield_get_implementation( void )
{
	static animation_implementation_t impl;

	impl.create = animation_starfield_create;
	impl.destroy = animation_starfield_destroy;
	impl.initialize = animation_starfield_initialize;
	impl.finish = animation_starfield_finish;
	impl.first_frame = animation_starfield_next_frame;
	impl.next_frame = animation_starfield_next_frame;
	impl.previous_frame = animation_starfield_next_frame;

	return &impl;
}


static animation_t * animation_starfield_create( animation_t * parent )
{
	animation_starfield_state_t * state = NULL;

	state = calloc( 1, sizeof(animation_starfield_state_t) );

	if(!state)
		return NULL;

	animation_set_default_fps( parent, ANIMATION_STARFIELD_DEFAULT_FPS );
	animation_set_name( parent, ANIMATION_STARFIELD_NAME );
	animation_set_state( parent, (void*) state );

	return parent;
}


static void animation_starfield_destroy( animation_t * this )
{
	free( animation_get_state( this ) );
}


static void animation_starfield_initialize( animation_t * this )
{
	int i = 0;
	animation_starfield_state_t * state = NULL;
	palette_t * pal = NULL;

	pal = animation_get_palette( this );
	state = animation_get_state(this);

	/* Grayscale Palette */
	for( i = 0; i < ANIMATION_STARFIELD_COLOR_COUNT; i++ )
		palette_set_color( pal, i, i, i, i );

	/* Initialize each star in the starfield */
	for( i = 0; i < ANIMATION_STARFIELD_STAR_COUNT; i++ )
	{
		state->star[i].x = (rand() % 2000) - 1000;
		state->star[i].y = (rand() % 2000) - 1000;
		state->star[i].z = (rand() % 900) + 100;
		state->star[i].speed = ((rand() % 4500) / 1000) + 0.5;
	}
}


static void animation_starfield_finish( animation_t * parent )
{
}


static void animation_starfield_next_frame( animation_t * this )
{
	int xmax = 0;
	int ymax = 0;
	int xmid = 0;
	int ymid = 0;
	int i = 0;
	int j = 0;
	int val = 0;
	frame_point_t pt;
	frame_t * frm = animation_get_frame( this );
	animation_starfield_state_t * state = animation_get_state( this );
	console_t * con = animation_get_console( this );

	frame_get_dimensions( frm, &xmax, &ymax );

	xmid = xmax / 2;
	ymid = ymax / 2;

	for( j = 0; j < 10; j++)
	{
		for( i = 0; i < ANIMATION_STARFIELD_STAR_COUNT; i++ )
		{
			pt.color = 0;
			pt.bgcolor = 0;
			pt.chr = 0;
			pt.value = 0;

			frame_set_point( frm, state->star[i].screen_x, state->star[i].screen_y, &pt );

			state->star[i].z = state->star[i].z - state->star[i].speed;

			state->star[i].screen_x = (state->star[i].x / state->star[i].z * 100) + xmid;
			state->star[i].screen_y = (state->star[i].y / state->star[i].z * 100) + ymid;

			if( (state->star[i].screen_x < 0) || ( state->star[i].screen_x >= xmax ) ||
				(state->star[i].screen_y < 0) || ( state->star[i].screen_y >= ymax ) ||
				(state->star[i].z < 1.0) )
			{
				state->star[i].x = (rand() % 1000) - 500;
				state->star[i].y = (rand() % 1000) - 500;
				state->star[i].z = (rand() % 900) + 100;
				state->star[i].speed = ((rand() % 4500) / 1000) + 0.5;
			}

			val = ( (ANIMATION_STARFIELD_COLOR_COUNT / 5) * state->star[i].speed ) * ( 1000 / state->star[i].z );

			frame_make_point( &pt, 0, 0, val, '.' );
			frame_set_point( frm, state->star[i].screen_x, state->star[i].screen_y, &pt );
		}
	}

	console_add_line( con, "stars=%d / colors=%d", ANIMATION_STARFIELD_STAR_COUNT, ANIMATION_STARFIELD_COLOR_COUNT );
}

/* $Id: animation_starfield.c 300 2015-07-31 04:49:42Z tiago.ventura $ */
