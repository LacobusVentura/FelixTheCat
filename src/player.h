/*!
	\file player.h
	\brief Animation Player Object Interface (Abstract/Singleton)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "frame.h"
#include "filter.h"
#include "animation.h"
#include "console.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
	\brief Player Object Type Definition
*/
typedef struct player_s player_t;


/*!
	\brief Player Object Implmentation Type Definition
*/
typedef struct player_implementation_s player_implementation_t;


/*!
	\brief Player Object State Type Definition
*/
typedef enum player_state_e player_state_t;


typedef enum player_screen_format_e player_screen_format_t;


enum player_screen_format_e
{
	player_screen_format_undefined,
	player_screen_format_text,
	player_screen_format_graphic
};

/*!
	\brief Enumerate the player possible states
*/
enum player_state_e
{
	unitialized,
	stopped,
	playing,
	paused,
};


/*!
	\brief Represents the Pure Virtual Methods to Implement
*/
struct player_implementation_s
{
	player_t * (*create)( player_t * );
	void (*destroy)( player_t * );
	int (*screen_initialize)( player_t * );
	void (*screen_finish)(  player_t * );
	void (*set_palette) ( player_t *, palette_t * );
	void (*render_frame)( player_t *, frame_t * );
	void (*refresh_console)( player_t * );
};

/*!
	\brief
	\return
*/
player_t * player_get_instance( void );


/*!
	\brief
	\param impl
	\return
*/
player_t * player_create( player_implementation_t * impl );


/*!
	\brief
	\param this
*/
void player_destroy( player_t * this );


/*!
	\brief
	\param this
	\return Zero on success and non-zero on failure
*/
int player_screen_initialize( player_t * this );


/*!
	\brief
	\param this
*/
void player_screen_finish( player_t * this );


/*!
	\brief
	\param this
*/
void player_play( player_t * this );


/*!
	\brief
	\param this
*/
void player_pause( player_t * this );


/*!
	\brief
	\param this
*/
void player_stop( player_t * this );


/*!
	\brief
	\param this
	\return
*/
player_state_t player_get_state( player_t * this );


/*!
	\brief
	\param this
	\param anim
	\return
*/
void player_set_animation( player_t * this, animation_t * anim );


/*!
	\brief
	\param this
	\return
*/
animation_t * player_get_animation( player_t * this );


/*!
	\brief
	\param this
	\return
*/
int player_get_screen_cols_count( player_t * this );


/*!
	\brief
	\param this
	\param ncols
*/
void player_set_screen_cols_count( player_t * this, int ncols );


/*!
	\brief
	\param this
	\return
*/
int player_get_screen_rows_count( player_t * this );


/*!
	\brief
	\param this
	\param nrows
*/
void player_set_screen_rows_count( player_t * this, int nrows );


/*!
	\brief
	\param this
	\param ncols
*/
int player_get_real_cols_count( player_t * this );


/*!
	\brief
	\param this
	\param nrows
*/
int player_get_real_rows_count( player_t * this );

/*!
	\brief
	\param this
	\param nrows
*/
void player_set_real_cols_count( player_t * this, int nrows );


/*!
	\brief
	\param this
	\param nrows
*/
void player_set_real_rows_count( player_t * this, int nrows );


/*!
	\brief
	\param this
	\param ncols
	\param nrows
*/
void player_set_screen_dimensions( player_t * this, int ncols, int nrows );


/*!
	\brief
	\param this
	\param ncols
	\param nrows
*/
void player_get_screen_dimensions( player_t * this, int * ncols, int * nrows );



/*!
	\brief
	\param this
	\param ncols
	\param nrows
*/
void player_set_real_dimensions( player_t * this, int ncols, int nrows );


/*!
	\brief
	\param this
	\param ncols
	\param nrows
*/
void player_get_real_dimensions( player_t * this, int * ncols, int * nrows );


/*!
	\brief
	\param this
	\return
*/
player_screen_format_t player_get_screen_format( player_t * this );


/*!
	\brief
	\param this
	\param fmt
*/
void player_set_screen_format( player_t * this, player_screen_format_t fmt );


/*!
	\brief
	\param this
	\return
*/
double player_get_fps( player_t * this );


/*!
	\brief
	\param this
	\param fps
*/
void player_set_fps( player_t * this, double fps );


/*!
	\brief
	\param this
	\return
*/
filter_t * player_get_filter( player_t * this );


/*!
	\brief
	\param this
	\param data
*/
void player_set_filter( player_t * this, filter_t * filter );


/*!
	\brief
	\param this
	\return
*/
void * player_get_data( player_t * this );


/*!
	\brief
	\param this
	\param data
*/
void player_set_data( player_t * this, void * data );


/*!
	\brief
	\param this
	\return
*/
double player_get_real_fps( player_t * this );


void player_set_console( player_t * this, console_t * con );


console_t * player_get_console( player_t * this );


void player_get_console_position( player_t * this, int * col, int * row );

void player_get_console_dimension( player_t * this, int * ncols, int * nrows );

void player_set_console_position( player_t * this, int col, int row );

void player_set_console_dimension( player_t * this, int ncols, int nrows );

void player_set_description( player_t * this, const char * desc );

const char * player_get_description( player_t * this );

#ifdef __cplusplus
}
#endif

#endif /* __PLAYER_H__ */

/* $Id: player.h 302 2015-08-06 17:04:55Z tiago.ventura $ */
