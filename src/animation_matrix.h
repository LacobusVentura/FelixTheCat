/*!
	\file animation_matrix.h
	\brief Matrix Digital Rain Effect Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __ANIMATION_MATRIX_H__
#define __ANIMATION_MATRIX_H__


#include "animation.h"


#ifdef __cplusplus
extern "C" {
#endif


/*!
	\brief Concrete Animation: Matrix
	\return
*/
animation_implementation_t * animation_matrix_get_implementation( void );

#ifdef __cplusplus
}
#endif


#endif /* __ANIMATION_MATRIX_H__ */

/* $Id: animation_matrix.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
