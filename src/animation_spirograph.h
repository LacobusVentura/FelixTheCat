/*!
	\file animation_spirograph.h
	\brief Spirograph Animation Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __ANIMATION_SPIROGRAPH_H__
#define __ANIMATION_SPIROGRAPH_H__


#include "animation.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
	\brief Concrete Animation: Spirograph
	\return
*/
animation_implementation_t * animation_spirograph_get_implementation( void );


#ifdef __cplusplus
}
#endif


#endif /* __ANIMATION_SPIROGRAPH_H__ */

/* $Id: animation_spirograph.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
