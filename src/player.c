/*!
	\file player.c
	\brief Animation Player Object Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "common.h"
#include "player.h"
#include "frame.h"
#include "animation.h"
#include "palette.h"
#include "filter.h"
#include "console.h"


#define PLAYER_TEXT_STATUS_MAX_LEN         (512)
#define PLAYER_DESCRIPTION_MAX_LEN         (64)

/*!
	\brief Represents a Player Object
*/
struct player_s
{
	double fps;
	double real_fps;
	player_state_t state;
	animation_t * anim;
	int screen_ncols;
	int screen_nrows;
	int real_ncols;
	int real_nrows;
	player_implementation_t * impl;
	filter_t * filter;
	void * data;
	player_screen_format_t screen_format;
	char description[ PLAYER_DESCRIPTION_MAX_LEN ];
	console_t * console;
	int console_col;
	int console_row;
	int console_ncols;
	int console_nrows;
};


static inline uint64_t player_timespec_diff( struct timespec * start, struct timespec * end );
static void player_time_delay( player_t * this, int64_t elapsed );
static void player_render_frame( player_t * this, frame_t * frm );
static void player_set_palette( player_t * this, palette_t * pal );
static const char * player_get_status_text( player_t * this );
static void player_refresh_console( player_t * this );


player_t * player_get_instance( void )
{
	return player_create( NULL );
}


player_t * player_create( player_implementation_t * impl )
{
	static player_t * singleton = NULL;

	if(singleton)
		return singleton;

	if(!impl)
		return NULL;

	singleton = (player_t*) calloc( 1, sizeof(player_t) );

	if(!singleton)
		return NULL;

	singleton->fps = 1.0;
	singleton->impl = impl;
	singleton->screen_format = player_screen_format_undefined;
	singleton->state = unitialized;

	singleton->impl->create( singleton );

	return singleton;
}


void player_destroy( player_t * this )
{
	this->impl->destroy( this );
	free( this );
}


int player_screen_initialize( player_t * this )
{
	int ret = 0;

	if( this->state != unitialized )
		return -1;

	ret = this->impl->screen_initialize( this );

	if( ret )
		return -1;

	this->state = stopped;

	return 0;
}


void player_screen_finish( player_t * this )
{
	if( this->state != stopped )
		return;

	this->impl->screen_finish( this );
	this->state = unitialized;
}


static void player_render_frame( player_t * this, frame_t * frm )
{
	if( !this->filter )
	{
		this->impl->render_frame( this, frm );
	}
	else
	{
		frame_t * frmaux = NULL;

		frmaux = frame_duplicate( frm );

		if(!frmaux)
			return;

		filter_frame( this->filter, frmaux );

		this->impl->render_frame( this, frmaux );

		frame_destroy( frmaux );
	}
}


static void player_set_palette( player_t * this, palette_t * pal )
{
	this->impl->set_palette( this, pal );
}


void player_refresh_console( player_t * this )
{
	this->impl->refresh_console( this );
}


void player_set_console( player_t * this, console_t * con )
{
	this->console = con;
}


console_t * player_get_console( player_t * this )
{
	return this->console;
}


player_state_t player_get_state( player_t * this )
{
	return this->state;
}


void player_set_animation( player_t * this, animation_t * anim )
{
	this->anim = anim;
	this->fps = animation_get_default_fps( anim );
}


animation_t * player_get_animation( player_t * this )
{
	return this->anim;
}


int player_get_screen_cols_count( player_t * this )
{
	return this->screen_ncols;
}


void player_set_screen_cols_count( player_t * this, int ncols )
{
	this->screen_ncols = ncols;
}


int player_get_screen_rows_count( player_t * this )
{
	return this->screen_nrows;
}


void player_set_screen_rows_count( player_t * this, int nrows )
{
	this->screen_nrows = nrows;
}


void player_get_screen_dimensions( player_t * this, int * ncols, int * nrows )
{
	*ncols = this->screen_ncols;
	*nrows = this->screen_nrows;
}


void player_set_screen_dimensions( player_t * this, int ncols, int nrows )
{
	this->screen_ncols = ncols;
	this->screen_nrows = nrows;
}


void player_get_real_dimensions( player_t * this, int * ncols, int * nrows )
{
	*ncols = this->real_ncols;
	*nrows = this->real_nrows;
}


void player_set_real_dimensions( player_t * this, int ncols, int nrows )
{
	this->real_ncols = ncols;
	this->real_nrows = nrows;
}

int player_get_real_cols_count( player_t * this )
{
	return this->real_ncols;
}


int player_get_real_rows_count( player_t * this )
{
	return this->real_nrows;
}


void player_set_real_cols_count( player_t * this, int ncols )
{
	this->real_ncols = ncols;
}


void player_set_real_rows_count( player_t * this, int nrows )
{
	this->real_nrows = nrows;
}

void player_get_console_position( player_t * this, int * col, int * row )
{
	*col = this->console_col;
	*row = this->console_row;
}


void player_get_console_dimension( player_t * this, int * ncols, int * nrows )
{
	*ncols = this->console_ncols;
	*nrows = this->console_nrows;
}


void player_set_console_position( player_t * this, int col, int row )
{
	this->console_col = col;
	this->console_row = row;
}


void player_set_console_dimension( player_t * this, int ncols, int nrows )
{
	this->console_ncols = ncols;
	this->console_nrows = nrows;
}


double player_get_fps( player_t * this )
{
	return this->fps;
}


void player_set_fps( player_t * this, double fps )
{
	this->fps = fps;
}


filter_t * player_get_filter( player_t * this )
{
	return this->filter;
}


void player_set_filter( player_t * this, filter_t * flt )
{
	this->filter = flt;
}


void * player_get_data( player_t * this )
{
	return this->data;
}


void player_set_data( player_t * this, void * data )
{
	this->data = data;
}


player_screen_format_t player_get_screen_format( player_t * this )
{
	return this->screen_format;
}


void player_set_screen_format( player_t * this, player_screen_format_t fmt )
{
	this->screen_format = fmt;
}


double player_get_real_fps( player_t * this )
{
	return this->real_fps;
}


void player_set_description( player_t * this, const char * desc )
{
	strncpy( this->description, desc, PLAYER_DESCRIPTION_MAX_LEN );
}


const char * player_get_description( player_t * this )
{
	return this->description;
}


static inline uint64_t player_timespec_diff( struct timespec * start, struct timespec * end )
{
	struct timespec temp;

	if( ( end->tv_nsec - start->tv_nsec ) < 0 )
	{
		temp.tv_sec = end->tv_sec - start->tv_sec - 1;
		temp.tv_nsec = 1000000000L + end->tv_nsec - start->tv_nsec;
	}
	else
	{
		temp.tv_sec = end->tv_sec - start->tv_sec;
		temp.tv_nsec = end->tv_nsec - start->tv_nsec;
	}

	return (temp.tv_sec * 1000000000L) + temp.tv_nsec;
}


static const char * player_get_status_text( player_t * this )
{
	static char text[ PLAYER_TEXT_STATUS_MAX_LEN + 1 ] = {0};
	animation_t * anim = player_get_animation(this);
	double real_fps = player_get_real_fps(this);
	double player_fps = player_get_fps(this);
	int synch = (real_fps < player_fps) ? 0 : 1;

	snprintf( text, PLAYER_TEXT_STATUS_MAX_LEN, "Player: seq=%d / fps=%0.1f / synch=%s", animation_get_frame_sequence( anim ), real_fps, (synch)?"ok":"error" );

	return text;
}


static void player_time_delay( player_t * this, int64_t elapsed )
{
	struct timespec frmdelay;
	int64_t frame_duration = 0;
	int64_t delay = 0;

	frame_duration = 1000000000L / fabs( this->fps );

	delay = frame_duration - elapsed;

	if( delay > 0L )
	{
		frmdelay.tv_sec = delay / 1000000000L;
		frmdelay.tv_nsec = delay % 1000000000L;

		nanosleep( &frmdelay, NULL );
	}

	if( elapsed > frame_duration )
	{
		this->real_fps = 1000000000L / (double) elapsed;
	}
	else
	{
		this->real_fps = 1000000000L / (double) frame_duration;
	}
}


void player_play( player_t * this )
{
	struct timespec start;
	struct timespec end;


	if( (this->state != stopped) && (this->state != paused) )
		return;

	this->state = playing;

	animation_initialize( this->anim, this->screen_ncols, this->screen_nrows );

	while( this->state == playing )
	{
		/* Stopwatch Started */
		clock_gettime( CLOCK_REALTIME, &start );

		/* Console Output */
		//console_clear( this->console );
		console_add_line( this->console, player_get_status_text( this ) );

		if( this->fps > 0.0 )
		{
			/* Animation Forward */
			animation_next_frame( this->anim );
		}
		else if( this->fps < 0.0 )
		{
			/* Animation Backward */
			animation_previous_frame( this->anim );
		}

		/* Set Palette */
		player_set_palette( this, animation_get_palette( this->anim ) );

		/* Render Frame */
		player_render_frame( this, animation_get_frame( this->anim ) );

		/* Refresh Console */
		player_refresh_console( this );

		/* Stopwatch Finished */
		clock_gettime( CLOCK_REALTIME, &end );

		/* Frame time delay */
		player_time_delay( this, player_timespec_diff( &start, &end ) );
	}

	animation_finish( this->anim );
}


void player_pause( player_t * this )
{
	if( this->state == playing )
		this->state = paused;
}


void player_stop( player_t * this )
{
	if( (this->state == playing) || (this->state == paused) )
		this->state = stopped;
}

/* $Id: player.c 302 2015-08-06 17:04:55Z tiago.ventura $ */
