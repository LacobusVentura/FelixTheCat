/*!
	\file animation_lifegame.c
	\brief Conway's Game of Life Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <time.h>

#include "frame.h"
#include "animation.h"
#include "animation_lifegame.h"


#define ANIMATION_LIFEGAME_NAME          "LifeGame"
#define ANIMATION_LIFEGAME_DEFAULT_FPS   (10)


struct animation_lifegame_state_s
{
	frame_t * prevfrm;
	int population;
	int generation;
};

typedef struct animation_lifegame_state_s animation_lifegame_state_t;


static animation_t * animation_lifegame_create( animation_t * parent );
static void animation_lifegame_destroy( animation_t * this );
static void animation_lifegame_next_frame( animation_t * this );
static void animation_lifegame_initialize( animation_t * this );
static void animation_lifegame_finish( animation_t * this );


animation_implementation_t * animation_lifegame_get_implementation( void )
{
	static animation_implementation_t impl;

	impl.create = animation_lifegame_create;
	impl.destroy = animation_lifegame_destroy;
	impl.initialize = animation_lifegame_initialize;
	impl.finish = animation_lifegame_finish;
	impl.first_frame = animation_lifegame_next_frame;
	impl.next_frame = animation_lifegame_next_frame;
	impl.previous_frame = animation_lifegame_next_frame;

	return &impl;
}


static animation_t * animation_lifegame_create( animation_t * parent )
{
	animation_lifegame_state_t * state = NULL;

	state = calloc( 1, sizeof(animation_lifegame_state_t) );

	if(!state)
		return NULL;

	state->prevfrm = NULL;

	animation_set_default_fps( parent, ANIMATION_LIFEGAME_DEFAULT_FPS );
	animation_set_name( parent, ANIMATION_LIFEGAME_NAME );
	animation_set_state( parent, (void*) state );

	return parent;
}


static void animation_lifegame_destroy( animation_t * this )
{
	free( animation_get_state( this ) );
}


static void animation_lifegame_initialize( animation_t * this )
{
	int col = 0;
	int row = 0;
	int ncols = 0;
	int nrows = 0;
	int alive = 0;
	frame_point_t pt;
	frame_t * frm = animation_get_frame(this);
	animation_lifegame_state_t * state = animation_get_state(this);

	srand(time(NULL));

	state->prevfrm = frame_duplicate( frm );

	frame_set_border_mode( state->prevfrm, frame_border_toroidal );

	frame_get_dimensions( state->prevfrm, &ncols, &nrows );

	state->population = 0;
	state->generation = 0;

	frame_clear( state->prevfrm );

	/* Random Initial "previous" Frame */
	for( row = 0; row < nrows; row++ )
	{
		for( col = 0; col < ncols; col++ )
		{
			alive = ( (rand() % 100) < 25 ) ? 1 : 0;

			frame_make_point( &pt, alive, 0, (alive) ? 2 : 7, '*' );

			frame_set_point( state->prevfrm, col, row, &pt );

			if(alive)
				state->population++;
		}
	}
}


static void animation_lifegame_finish( animation_t * parent )
{
	animation_lifegame_state_t * state = animation_get_state(parent);

	frame_destroy( state->prevfrm );
	state->prevfrm = NULL;
}


static void animation_lifegame_next_frame( animation_t * this )
{
	int col = 0;
	int row = 0;
	int ncols = 0;
	int nrows = 0;
	int neighbours = 0;
	int population = 0;
	int alive = 0;
	frame_point_t pt;
	frame_t * frm = animation_get_frame( this );
	animation_lifegame_state_t * state = animation_get_state(this);
	console_t * con = animation_get_console(this);

	frame_clear( frm );

	frame_set_border_mode( frm, frame_get_border_mode( state->prevfrm ) );

	frame_get_dimensions( state->prevfrm, &ncols, &nrows );

	for( row = 0; row < nrows; row++ )
	{
		for( col = 0; col < ncols; col++ )
		{
			neighbours = 0;
			alive = 0;

			frame_get_point( state->prevfrm, col - 1, row + 1, &pt );

			if(pt.value)
				neighbours++;

			frame_get_point( state->prevfrm, col, row + 1, &pt );

			if(pt.value)
				neighbours++;

			frame_get_point( state->prevfrm, col + 1, row + 1, &pt );

			if(pt.value)
				neighbours++;

			frame_get_point( state->prevfrm, col - 1, row, &pt );

			if(pt.value)
				neighbours++;

			frame_get_point( state->prevfrm, col + 1, row, &pt );

			if(pt.value)
				neighbours++;

			frame_get_point( state->prevfrm, col - 1, row - 1, &pt );

			if(pt.value)
				neighbours++;

			frame_get_point( state->prevfrm, col, row - 1, &pt );

			if(pt.value)
				neighbours++;

			frame_get_point( state->prevfrm, col + 1, row - 1, &pt );

			if(pt.value)
				neighbours++;

			frame_get_point( state->prevfrm, col, row, &pt );

			if( pt.value )
			{
				if( ( neighbours == 2 ) || ( neighbours == 3 ) )
					alive = 1;
			}
			else
			{
				if( neighbours == 3 )
					alive = 1;
			}

			if( alive )
			{
				pt.value = 1;
				pt.color = 10;
				pt.bgcolor = 0;
				pt.chr = '*';

				frame_set_point( frm, col, row, &pt );

				population++;
			}
		}
	}

	state->population = population;
	state->generation++;
	frame_copy( state->prevfrm, frm );

	console_add_line( con, "%dx%d / generation=%d / alive=%d / dead=%d / total=%d", ncols, nrows, state->generation, state->population, (ncols * nrows) - state->population, (ncols * nrows) );
}

/* $Id: animation_lifegame.c 300 2015-07-31 04:49:42Z tiago.ventura $ */
