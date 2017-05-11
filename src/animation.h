/*!
	\file animation.h
	\brief Animation Object Interface (Abstract)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __ANIMATION_H__
#define __ANIMATION_H__


#include "frame.h"
#include "palette.h"
#include "console.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
	\brief Animation Object Type Definition
*/
typedef struct animation_s animation_t;


/*!
	\brief Implementation of an Animation Object Type Definition
*/
typedef struct animation_implementation_s animation_implementation_t;


/*!
	\brief Represents an Implementation of an Animation
*/
struct animation_implementation_s
{
	animation_t * (*create) (animation_t *);
	void (*destroy) (animation_t *);
	void (*initialize) (animation_t *);
	void (*finish) (animation_t *);
	void (*first_frame) (animation_t *);
	void (*next_frame) (animation_t *);
	void (*previous_frame) (animation_t *);
};


/*!
	\brief Creates an Animation Object (Virtual)
	\param impl Implementation
*/
animation_t * animation_create( animation_implementation_t * impl );


/*!
	\brief Destroy an Animation Object (Virtual)
	\param this Animation Object
*/
void animation_destroy( animation_t * this );


/*!
	\brief Initialize an Animation Object (Virtual)
	\param this Animation Object
*/
void animation_initialize( animation_t * this, int ncols, int nrows );


/*!
	\brief Finish an Animation Object (Virtual)
	\param this Animation Object
*/
void animation_finish( animation_t * this );


/*!
	\brief Reinitialize an Animation Object
	\param this Animation Object
*/
void animation_reinitialize( animation_t * this );


/*!
	\brief Get the first Frame of an Animation (Virtual)
	\param this Animation Object
	\param pfrm Frame Object
*/
void animation_first_frame( animation_t * this );


/*!
	\brief Get the next Frame of an Animation (Virtual)
	\param this Animation Object
	\param pfrm Frame Object
*/
void animation_next_frame( animation_t * this );


/*!
	\brief Get the Previous Frame of an Animation (Virtual)
	\param this Animation Object
	\param pfrm Frame Object
*/
void animation_previous_frame( animation_t * this );


/*!
	\brief Get the Anaimation's Default Frame Rate (Virtual)
	\param this Animation Object
	\param pfrm Frame Object
*/
int animation_get_default_fps( animation_t * this );


/*!
	\brief Get actual frame
	\param this Animation Object
	\return Palette Object
*/
frame_t * animation_get_frame( animation_t * this );


/*!
	\brief Get animation default fps
	\param this Animation Object
	\return
*/
int animation_get_default_fps( animation_t * this );


/*!
	\brief Set animation default fps
	\param this Animation Object
	\param fps
*/
void animation_set_default_fps( animation_t * this, int fps );


/*!
	\brief Get actual palette
	\param this Animation Object
	\return Palette Object
*/
palette_t * animation_get_palette( animation_t * this );


/*!
	\brief Set actual state object
	\param this Animation Object
*/
void animation_set_state( animation_t * this, void * state );


/*!
	\brief Get actual state object
	\param this Animation Object
	\return State Object
*/
void * animation_get_state( animation_t * this );


/*!
	\brief
	\param this Animation Object
	\return
*/
int animation_get_cols_count( animation_t * this );


/*!
	\brief
	\param this Animation Object
	\return
*/
int animation_get_rows_count( animation_t * this );

/*!
	\brief
	\param this Animation Object
	\return
*/
const char * animation_get_name( animation_t * this );


/*!
	\brief
	\param this Animation Object
	\return
*/
void animation_set_name( animation_t * this, const char * name );

/*!
	\brief
	\param this Animation Object
	\return
*/
int animation_get_frame_sequence( animation_t * this );


console_t * animation_get_console( animation_t * this );


void animation_set_console( animation_t * this, console_t * con );

#ifdef __cplusplus
}
#endif

#endif /* __ANIMATION_H__ */

/* $Id: animation.h 300 2015-07-31 04:49:42Z tiago.ventura $ */
