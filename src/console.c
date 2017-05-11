/*!
	\file console.c
	\brief Console Object Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "console.h"


#define CONSOLE_MAX_LINE_LEN  (100)


/*!
	\brief Represents a Console Object
*/
struct console_s
{
	int index;
	int color;
	int bgcolor;
	int nlines;
	char ** text;
};


console_t * console_get_instance( void )
{
	return console_create( 0 );
}


console_t * console_create( int nlines )
{
	static console_t * con = NULL;
	int i = 0;

	if(con)
		return con;

	if(nlines <= 0)
		return NULL;

	con = (console_t*) calloc( 1, sizeof(console_t) );

	if( !con )
		return NULL;

	con->text = (char**) calloc( nlines, sizeof(char*) );

	if( !con->text )
	{
		console_destroy( con );
		return NULL;
	}

	for( i = 0; i < nlines; i++ )
	{
		con->text[i] = (char*) calloc( CONSOLE_MAX_LINE_LEN, sizeof(char) );

		if( !con->text[i] )
		{
			console_destroy( con );
			return NULL;
		}
	}

	con->nlines = nlines;

	return con;
}


void console_destroy( console_t * this )
{
	int i = 0;

	if( this->text )
	{
		for( i = 0; i < this->nlines; i++ )
			if( this->text[i] )
				free( this->text[i] );

		free( this->text );
	}

	free( this );
}


const char * console_get_line( console_t * this, int idx )
{
	if( (idx < 0) || (idx >= this->nlines) )
		return NULL;

	return this->text[ idx ];
}


void console_add_line( console_t * this, const char * fmt, ... )
{
	int i = 0;
	va_list args;
	char msg[ CONSOLE_MAX_LINE_LEN + 1 ] = {0};
	char * paux = NULL;

	if(!this)
		return;

	va_start( args, fmt );
	vsnprintf( msg, CONSOLE_MAX_LINE_LEN, fmt, args );
	va_end(args);

	paux = this->text[0];

	for( i = 0; i < this->nlines - 1; i++ )
		this->text[i] = this->text[ i + 1 ];

	this->text[ this->nlines - 1 ] = paux;

	strncpy( paux, msg, CONSOLE_MAX_LINE_LEN );
}


void console_clear( console_t * this )
{
	int i = 0;

	for( i = 0; i < this->nlines; i++ )
		memset( this->text[i], 0, CONSOLE_MAX_LINE_LEN );
}


const char * console_get_first_line( console_t * this )
{
	this->index = 0;
	return console_get_line( this, this->index );
}


const char * console_get_next_line( console_t * this )
{
	this->index++;
	return console_get_line( this, this->index );
}


int console_get_lines_count( console_t * this )
{
	return this->nlines;
}


int console_get_text_color( console_t * this )
{
	return this->color;
}


void console_set_text_color( console_t * this, int color )
{
	this->color = color;
}


int console_get_text_bgcolor( console_t * this )
{
	return this->bgcolor;
}


void console_set_text_bgcolor( console_t * this, int bgcolor )
{
	this->bgcolor = bgcolor;
}

/* $Id$ */
