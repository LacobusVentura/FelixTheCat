/*!
	\file animation_swarm.c
	\brief Swarm Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "frame.h"
#include "animation.h"
#include "animation_swarm.h"


#define ANIMATION_SWARM_NAME          "Swarm"
#define ANIMATION_SWARM_DEFAULT_FPS   (2)
#define ANIMATION_SWARM_MIN_SPEED     (50)
#define ANIMATION_SWARM_MAX_SPEED     (200)
#define ANIMATION_SWARM_MAX_ACCEL     (25)
#define ANIMATION_SWARM_MAX_TURN      (M_PI)
#define ANIMATION_SWARM_OUTER_RANGE   (64)
#define ANIMATION_SWARM_INNER_RANGE   (5)
#define ANIMATION_SWARM_BEES_COUNT    (4000)


enum animation_swarm_bee_mode_e
{
	mode_attack,
	mode_evade
};

typedef enum animation_swarm_bee_mode_e animation_swarm_bee_mode_t;


struct animation_swarm_bee_s
{
	int x;
	int y;
	double theta;
	double speed;
	animation_swarm_bee_mode_t mode;
};

typedef struct animation_swarm_bee_s animation_swarm_bee_t;


struct animation_swarm_s
{
	int xpos;
	int ypos;
	animation_swarm_bee_t bees[ ANIMATION_SWARM_BEES_COUNT ];
};

typedef struct animation_swarm_s animation_swarm_t;


struct animation_swarm_state_s
{
	animation_swarm_t swarm;
};

typedef struct animation_swarm_state_s animation_swarm_state_t;


static animation_t * animation_swarm_create( animation_t * parent );
static void animation_swarm_destroy( animation_t * this );
static void animation_swarm_next_frame( animation_t * this );
static void animation_swarm_initialize( animation_t * this );
static void animation_swarm_finish( animation_t * this );


animation_implementation_t * animation_swarm_get_implementation( void )
{
	static animation_implementation_t impl;

	impl.create = animation_swarm_create;
	impl.destroy = animation_swarm_destroy;
	impl.initialize = animation_swarm_initialize;
	impl.finish = animation_swarm_finish;
	impl.first_frame = animation_swarm_next_frame;
	impl.next_frame = animation_swarm_next_frame;
	impl.previous_frame = animation_swarm_next_frame;

	return &impl;
}


static animation_t * animation_swarm_create( animation_t * parent )
{
	animation_swarm_state_t * state = NULL;

	state = calloc( 1, sizeof(animation_swarm_state_t) );

	if(!state)
		return NULL;

	animation_set_default_fps( parent, ANIMATION_SWARM_DEFAULT_FPS );
	animation_set_name( parent, ANIMATION_SWARM_NAME );
	animation_set_state( parent, (void*) state );

	return parent;
}


static void animation_swarm_destroy( animation_t * this )
{
	free( animation_get_state( this ) );
}


static void animation_swarm_initialize( animation_t * this )
{
	int ncols = 0;
	int nrows = 0;
	int i = 0;
	frame_t * frm = animation_get_frame(this);
	animation_swarm_state_t * state = animation_get_state(this);

	srand(time(NULL));

	frame_get_dimensions( frm, &ncols, &nrows );

	state->swarm.xpos = ncols / 2;
	state->swarm.ypos = nrows / 2;

	for( i = 0; i < ANIMATION_SWARM_BEES_COUNT; i++ )
	{
		state->swarm.bees[i].x = (rand() % ncols);
		state->swarm.bees[i].y = (rand() % nrows);
		state->swarm.bees[i].theta = ( (M_PI * 2) * ( (double) rand() / RAND_MAX ) );
		state->swarm.bees[i].speed = ANIMATION_SWARM_MIN_SPEED;
		state->swarm.bees[i].mode = mode_attack;
	}
}


static void animation_swarm_finish( animation_t * parent )
{
}


static void animation_swarm_next_frame( animation_t * this )
{
	int col = 0;
	int row = 0;
	int ncols = 0;
	int nrows = 0;
	frame_t * frm = animation_get_frame( this );
	console_t * con = animation_get_console( this );

	ncols = frame_get_cols_count( frm );
	nrows = frame_get_rows_count( frm );

	for( row = 0; row < nrows; row++ )
	{
		for( col = 0; col < ncols; col++ )
		{

		}
	}

	console_add_line( con, "swarm frame" );
}


static double random_value( double val )
{
	return val * ((double) rand() / RAND_MAX );
}


static double distance( int xa, int ya, int xb, int yb )
{
	int a = (xb - xa);
	int b = (yb - ya);

	return ( sqrt( (a * a) + (b * b) ) );
}


static void update( animation_t * this, double dt, int x, int y )
{
	double s = 0.0;
	double phi = 0.0;
	double accel = 0.0;
	double turn = 0.0;
	double k = 0.0;

	animation_swarm_state_t * state = animation_get_state(this);

	s = distance( state->swarm.xpos, state->y, x, y );

	switch( state->mode )
	{
		case mode_attack :
		{
			( (M_PI * 2) * ( (double) rand() / RAND_MAX ) )

			if( random_value(s) < random_value(ANIMATION_SWARM_INNER_RANGE) )
				mode = mode_evade;

			break;
		}

		case mode_evade :
		{
			if( random_value(s) > random_value(ANIMATION_SWARM_OUTER_RANGE) )
				mode = mode_attack;

			break;
		}
	}

	phi = atan2( y - this->y, x - this->x ) - theta;

	switch( mode )
	{
		case mode_attack :
		{
			accel = cos(phi) * ANIMATION_SWARM_MAX_ACCEL;
			turn = sin(phi) * ANIMATION_SWARM_MAX_TURN;

			break;
		}

		case mode_evade :
		{
			accel = sin(phi) * ANIMATION_SWARM_MAX_ACCEL;
			turn = cos(phi) * ANIMATION_SWARM_MAX_TURN;

			break;
		}
	}

	if( accel > 0 )
	{
		k = (ANIMATION_SWARM_MAX_SPEED - speed) / (ANIMATION_SWARM_MAX_SPEED - ANIMATION_SWARM_MIN_SPEED);
	}
	else
	{
		k = (speed - ANIMATION_SWARM_MIN_SPEED) / (ANIMATION_SWARM_MAX_SPEED - ANIMATION_SWARM_MIN_SPEED);
	}

	speed += k * accel;

	theta = fmod( (theta + turn * dt), (M_PI * 2) );

	x += (int)(speed * dt * cos(theta));
	y += (int)(speed * dt * sin(theta));
}

/* $Id: animation_swarm.c 405 2016-05-13 11:03:53Z tiago.ventura $ */
