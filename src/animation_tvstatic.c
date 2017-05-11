/*!
	\file animation_tvstatic.c
	\brief TV Static Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <time.h>

#include "frame.h"
#include "palette.h"
#include "animation.h"
#include "animation_tvstatic.h"


#define ANIMATION_TVSTATIC_NAME          "TVStatic"
#define ANIMATION_TVSTATIC_COLOR_COUNT   (256)
#define ANIMATION_TVSTATIC_DEFAULT_FPS   (10)

/* Private Types */
typedef struct animation_tvstatic_state_s animation_tvstatic_state_t;


/* Private Prototypes */
static animation_t * animation_tvstatic_create( animation_t * this );
static void animation_tvstatic_destroy( animation_t * this );
static void animation_tvstatic_initialize( animation_t * this );
static void animation_tvstatic_finish( animation_t * this );
static void animation_tvstatic_next_frame( animation_t * this );


/* Implementation */
animation_implementation_t * animation_tvstatic_get_implementation( void )
{
	static animation_implementation_t impl;

	impl.create = animation_tvstatic_create;
	impl.destroy = animation_tvstatic_destroy;
	impl.initialize = animation_tvstatic_initialize;
	impl.finish = animation_tvstatic_finish;
	impl.first_frame = animation_tvstatic_next_frame;
	impl.next_frame = animation_tvstatic_next_frame;
	impl.previous_frame = animation_tvstatic_next_frame;

	return &impl;
}


static animation_t * animation_tvstatic_create( animation_t * parent )
{
	srand(time(NULL));

	animation_set_default_fps( parent, ANIMATION_TVSTATIC_DEFAULT_FPS );
	animation_set_name( parent, ANIMATION_TVSTATIC_NAME );

	return parent;
}


static void animation_tvstatic_destroy( animation_t * this )
{
}


static void animation_tvstatic_initialize( animation_t * this )
{
	int i = 0;
	palette_t * pal = animation_get_palette( this );

	/* Grayscale Palette */
	for( i = 0; i < ANIMATION_TVSTATIC_COLOR_COUNT; i++ )
		palette_set_color( pal, i, i, i, i );
}


static void animation_tvstatic_finish( animation_t * this )
{
}


static void animation_tvstatic_next_frame( animation_t * this )
{
	int col = 0;
	int row = 0;
	int ncols = 0;
	int nrows = 0;
	int color = 0;
	frame_point_t pt;
	frame_t * frm = animation_get_frame( this );
	console_t * con = animation_get_console( this );

	frame_get_dimensions( frm, &ncols, &nrows );

	for( row = 0; row < nrows; row++ )
	{
		for( col = 0; col < ncols; col++ )
		{
			color = rand() % ANIMATION_TVSTATIC_COLOR_COUNT;

			frame_make_point( &pt, color, color, color, ' ' );
			frame_set_point( frm, col, row, &pt );
		}
	}

	console_add_line( con, "array=%dx%d / colors=%d", ncols, nrows, ANIMATION_TVSTATIC_COLOR_COUNT );
}

/* $Id: animation_tvstatic.c 300 2015-07-31 04:49:42Z tiago.ventura $ */
