/*!
	\file animation_swarm.h
	\brief Matrix Digital Rain Effect Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __ANIMATION_SWARM_H__
#define __ANIMATION_SWARM_H__


#include "animation.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
	\brief Concrete Animation: Swarm
	\return
*/
animation_implementation_t * animation_swarm_get_implementation( void );

#ifdef __cplusplus
}
#endif


#endif /* __ANIMATION_SWARM_H__ */

/* $Id: animation_swarm.h 312 2015-09-02 20:17:56Z tiago.ventura $ */
