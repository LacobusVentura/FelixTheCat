/*!
	\file animation_fernfractal.h
	\brief Barnsley Fern Fractal Animation Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __ANIMATION_FERNFRACTAL_H__
#define __ANIMATION_FERNFRACTAL_H__


#include "animation.h"


#ifdef __cplusplus
extern "C" {
#endif

/*!
	\brief Concrete Animation: Fern Fractal
	\return
*/
animation_implementation_t * animation_fern_fractal_get_implementation( void );


#ifdef __cplusplus
}
#endif


#endif /* __ANIMATION_FERNFRACTAL_H__ */

/* $Id: animation_fernfractal.h 280 2015-07-28 01:33:05Z tiago.ventura $ */
