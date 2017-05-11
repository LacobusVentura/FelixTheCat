/*!
	\file animation_tvstatic.h
	\brief TV Static Animation Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __ANIMATION_TVSTATIC_H__
#define __ANIMATION_TVSTATIC_H__

#include "animation.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
	\brief Concrete Animation: TV Static
	\return
*/
animation_implementation_t * animation_tvstatic_get_implementation( void );


#ifdef __cplusplus
}
#endif


#endif /* __ANIMATION_TVSTATIC_H__ */

/* $Id: animation_tvstatic.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
