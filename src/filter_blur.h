/*!
	\file filter_blur.h
	\brief Blur Filter Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __FILTER_BLUR_H__
#define __FILTER_BLUR_H__


#include "filter.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
	\brief
	\return
*/
filter_implementation_t * filter_blur_get_implementation( void );


#ifdef __cplusplus
}
#endif


#endif /* __FILTER_BLUR_H__ */

/* $Id: filter_blur.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
