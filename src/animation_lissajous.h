/*!
	\file animation_lissajous.h
	\brief Lissajous Animation Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#ifndef __ANIMATION_LISSAJOUS_H__
#define __ANIMATION_LISSAJOUS_H__

#include "frame.h"
#include "animation.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
	\brief Concrete Animation: Lissajous
	\return Lissajous Animation Implementation
*/
animation_implementation_t * animation_lissajous_get_implementation( void );

#ifdef __cplusplus
}
#endif

#endif /* __ANIMATION_LISSAJOUS_H__ */

/* $Id: animation_lissajous.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
