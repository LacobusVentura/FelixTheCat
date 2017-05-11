/*!
	\file animation_fire.h
	\brief Fire Animation Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __ANIMATION_FIRE_H__
#define __ANIMATION_FIRE_H__


#include "animation.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
	\brief Concrete Animation: Fire
	\return
*/
animation_implementation_t * animation_fire_get_implementation( void );


#ifdef __cplusplus
}
#endif


#endif /* __ANIMATION_FIRE_H__ */

/* $Id: animation_fire.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
