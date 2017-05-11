/*!
	\file console.h
	\brief Console Object Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#ifndef __CONSOLE_H__
#define __CONSOLE_H__


typedef struct console_s console_t;


console_t * console_get_instance( void );
console_t * console_create( int nlines );
void console_destroy( console_t * this );
const char * console_get_line( console_t * this, int idx );
void console_add_line( console_t * this, const char * fmt, ... );
void console_clear( console_t * this );
const char * console_get_first_line( console_t * this );
const char * console_get_next_line( console_t * this );
int console_get_lines_count( console_t * this );
int console_get_text_color( console_t * this );
void console_set_text_color( console_t * this, int color );
int console_get_text_bgcolor( console_t * this );
void console_set_text_bgcolor( console_t * this, int bgcolor );

#endif /* __CONSOLE_H__ */

/* $Id$ */
