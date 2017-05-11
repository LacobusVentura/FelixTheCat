/*!
	\file animation.c
	\brief Animation Object Implementation (Abstract)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "frame.h"
#include "palette.h"
#include "animation.h"
#include "console.h"


#define ANIMATION_MSG_MAX_LEN   (256)
#define ANIMATION_NAME_MAX_LEN  (32)

/*!
	\brief Represents an Animation Object
*/
struct animation_s
{
	char name[ ANIMATION_NAME_MAX_LEN + 1 ];
	char message[ ANIMATION_MSG_MAX_LEN + 1 ];
	palette_t * palette;
	frame_t * frame;
	void * state;
	int frame_sequence;
	animation_implementation_t * impl;
	int ncols;
	int nrows;
	int default_fps;
	console_t * console;
};


animation_t * animation_create( animation_implementation_t * impl )
{
	animation_t * anim = NULL;

	anim = (animation_t*) calloc( 1, sizeof(animation_t) );

	if(!anim)
		return NULL;

	anim->impl = impl;

	anim->impl->create( anim );

	return anim;
}


void animation_destroy( animation_t * this )
{
	this->impl->destroy( this );
	free( this );
}


void animation_initialize( animation_t * this, int ncols, int nrows )
{
	this->ncols = ncols;
	this->nrows = nrows;
	this->frame_sequence = 0;

	this->frame = frame_create( ncols, nrows );
	this->palette = palette_create();

	this->impl->initialize( this );
}


void animation_finish( animation_t * this )
{
	this->impl->finish( this );

	palette_destroy( this->palette );
	frame_destroy( this->frame );
}


void animation_first_frame( animation_t * this )
{
	this->impl->first_frame( this );
	this->frame_sequence = 1;
}


void animation_next_frame( animation_t * this )
{
	this->impl->next_frame( this );
	this->frame_sequence++;
}


void animation_previous_frame( animation_t * this )
{
	this->impl->previous_frame( this );
	this->frame_sequence--;
}


int animation_get_default_fps( animation_t * this )
{
	return this->default_fps;
}


void animation_set_default_fps( animation_t * this, int fps )
{
	this->default_fps = fps;
}


palette_t * animation_get_palette( animation_t * this )
{
	return this->palette;
}


frame_t * animation_get_frame( animation_t * this )
{
	return this->frame;
}


void animation_set_state( animation_t * this, void * state )
{
	this->state = state;
}


void * animation_get_state( animation_t * this )
{
	return this->state;
}


int animation_get_cols_count( animation_t * this )
{
	return this->ncols;
}


int animation_get_rows_count( animation_t * this )
{
	return this->nrows;
}


void animation_reinitialize( animation_t * this )
{
	this->impl->finish( this );
	this->impl->initialize( this );
}


console_t * animation_get_console( animation_t * this )
{
	return this->console;
}


void animation_set_console( animation_t * this, console_t * con )
{
	this->console = con;
}


const char * animation_get_name( animation_t * this )
{
	return this->name;
}


void animation_set_name( animation_t * this, const char * name )
{
	strncpy( this->name, name, ANIMATION_NAME_MAX_LEN );
}


int animation_get_frame_sequence( animation_t * this )
{
	return this->frame_sequence;
}

/* $Id: animation.c 300 2015-07-31 04:49:42Z tiago.ventura $ */
