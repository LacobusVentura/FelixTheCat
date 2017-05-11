/*!
	\file animation_fire.c
	\brief Fire Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <time.h>

#include "frame.h"
#include "palette.h"
#include "animation.h"
#include "animation_fire.h"

#define ANIMATION_FIRE_NAME          "Fire"
#define ANIMATION_FIRE_DEFAULT_FPS   (10)

/* Private Structures */
struct animation_fire_state_s
{
	frame_t * prevfrm;
};

/* Private Types */
typedef struct animation_fire_state_s animation_fire_state_t;


/* Private Prototypes */
static animation_t * animation_fire_create( animation_t * this );
static void animation_fire_destroy( animation_t * this );
static void animation_fire_initialize( animation_t * this );
static void animation_fire_finish( animation_t * this );
static void animation_fire_next_frame( animation_t * this );


/* Implementation */
animation_implementation_t * animation_fire_get_implementation( void )
{
	static animation_implementation_t impl;

	impl.create = animation_fire_create;
	impl.destroy = animation_fire_destroy;
	impl.initialize = animation_fire_initialize;
	impl.finish = animation_fire_finish;
	impl.first_frame = animation_fire_next_frame;
	impl.next_frame = animation_fire_next_frame;
	impl.previous_frame = animation_fire_next_frame;

	return &impl;
}


static animation_t * animation_fire_create( animation_t * parent )
{
	animation_fire_state_t * state = NULL;

	srand(time(NULL));

	state = (animation_fire_state_t*) calloc( 1, sizeof(animation_fire_state_t) );

	if(!state)
		return NULL;

	state->prevfrm = NULL;

	animation_set_default_fps( parent, ANIMATION_FIRE_DEFAULT_FPS );
	animation_set_name( parent, ANIMATION_FIRE_NAME );
	animation_set_state( parent, (void*) state );

	return parent;
}


static void animation_fire_destroy( animation_t * this )
{
	free( animation_get_state( this ) );
}


static void animation_fire_initialize( animation_t * this )
{
	animation_fire_state_t * state = animation_get_state(this);
	palette_t * pal = animation_get_palette(this);
	frame_t * frm = animation_get_frame(this);
	int i = 0;

	srand(time(NULL));

	state->prevfrm = frame_duplicate( frm );

	frame_set_border_mode( state->prevfrm, frame_border_toroidal );

	/* Red Fire Palette */
	for( i = 0; i < 64; i++ )
	{
		palette_set_color( pal, i, i * 4, 0,  0 );
		palette_set_color( pal, i + 64, 255,  i * 4, 0 );
		palette_set_color( pal, i + 128, 255,  255, i * 4 );
		palette_set_color( pal, i + 192, 255, 255, 255 );
	}
}


static void animation_fire_finish( animation_t * this )
{
	animation_fire_state_t * state = animation_get_state(this);

	frame_destroy( state->prevfrm );
	state->prevfrm = NULL;
}


static void animation_fire_next_frame( animation_t * this )
{
	int col = 0;
	int row = 0;
	int ncols = 0;
	int nrows = 0;
	int xnew = 0;
	frame_point_t pt;
	int sum = 0;
	frame_t * frm = animation_get_frame( this );
	animation_fire_state_t * state = animation_get_state(this);
	console_t * con = animation_get_console( this );

	frame_set_border_mode( frm, frame_border_toroidal );

	frame_get_dimensions( state->prevfrm, &ncols, &nrows );

	/* Fire Coal */
	for( col = 0; col < ncols; col++ )
	{
		pt.color = (rand() % 224) + 32;
		pt.bgcolor = pt.color;
		pt.chr = ' ';

		frame_set_point( state->prevfrm, col, nrows - 1, &pt );

		pt.color = (rand() % 224) + 32;
		pt.bgcolor = pt.color;
		pt.chr = ' ';

		frame_set_point( state->prevfrm, col, nrows - 2, &pt );
	}

	for( row = 0; row < nrows; row++ )
	{
		for( col = 0; col < ncols; col++ )
		{
			sum = 0;

			frame_get_point( state->prevfrm, col, row, &pt );
			sum += pt.color;

			frame_get_point( state->prevfrm, col - 1, row + 1, &pt );
			sum += pt.color;

			frame_get_point( state->prevfrm, col, row + 1, &pt );
			sum += pt.color;

			frame_get_point( state->prevfrm, col + 1, row + 1, &pt );
			sum += pt.color;

			xnew = ( sum  / 4 );

			if( xnew > 0 )
				xnew--;

			frame_make_point( &pt, 0, xnew, xnew, ' ' );
			frame_set_point( frm, col, row - 1, &pt );
		}
	}

	frame_copy( state->prevfrm, frm );

	console_add_line( con, "burning!" );
}

/* $Id: animation_fire.c 304 2015-08-08 00:57:58Z tiago.ventura $ */
