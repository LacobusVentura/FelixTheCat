/*!
	\file animation_matrix.c
	\brief Starfield Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>

#include "frame.h"
#include "animation.h"
#include "animation_matrix.h"


#define ANIMATION_MATRIX_NAME          "Matrix"
#define ANIMATION_MATRIX_DEFAULT_FPS   (2)


struct animation_matrix_state_s
{
	int x;
};

typedef struct animation_matrix_state_s animation_matrix_state_t;


static animation_t * animation_matrix_create( animation_t * parent );
static void animation_matrix_destroy( animation_t * this );
static void animation_matrix_next_frame( animation_t * this );
static void animation_matrix_initialize( animation_t * this );
static void animation_matrix_finish( animation_t * this );


animation_implementation_t * animation_matrix_get_implementation( void )
{
	static animation_implementation_t impl;

	impl.create = animation_matrix_create;
	impl.destroy = animation_matrix_destroy;
	impl.initialize = animation_matrix_initialize;
	impl.finish = animation_matrix_finish;
	impl.first_frame = animation_matrix_next_frame;
	impl.next_frame = animation_matrix_next_frame;
	impl.previous_frame = animation_matrix_next_frame;

	return &impl;
}


static animation_t * animation_matrix_create( animation_t * parent )
{
	animation_matrix_state_t * state = NULL;

	state = calloc( 1, sizeof(animation_matrix_state_t) );

	if(!state)
		return NULL;

	animation_set_default_fps( parent, ANIMATION_MATRIX_DEFAULT_FPS );
	animation_set_name( parent, ANIMATION_MATRIX_NAME );
	animation_set_state( parent, (void*) state );

	return parent;
}


static void animation_matrix_destroy( animation_t * this )
{
	free( animation_get_state( this ) );
}


static void animation_matrix_initialize( animation_t * this )
{
}


static void animation_matrix_finish( animation_t * parent )
{
}


static void animation_matrix_next_frame( animation_t * this )
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

	console_add_line( con, "frame" );
}

/* $Id: animation_matrix.c 300 2015-07-31 04:49:42Z tiago.ventura $ */
