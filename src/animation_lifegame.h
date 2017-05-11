/*!
	\file animation_lifegame.h
	\brief Conway's Game of Life Animation Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __ANIMATION_LIFEGAME_H__
#define __ANIMATION_LIFEGAME_H__


#include "animation.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
	\brief Concrete Animation: Conway's Game of Life
	\return
*/
animation_implementation_t * animation_lifegame_get_implementation( void );

#ifdef __cplusplus
}
#endif


#endif /* __ANIMATION_LIFEGAME_H__ */

/* $Id: animation_lifegame.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
