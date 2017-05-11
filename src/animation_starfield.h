/*!
	\file animation_starfield.h
	\brief Starfield Effect Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __ANIMATION_STARFIELD_H__
#define __ANIMATION_STARFIELD_H__


#include "animation.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
	\brief Concrete Animation: Starfield
	\return
*/
animation_implementation_t * animation_starfield_get_implementation( void );


#ifdef __cplusplus
}
#endif


#endif /* __ANIMATION_STARFIELD_H__ */

/* $Id: animation_starfield.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
