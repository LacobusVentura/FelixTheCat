/*!
	\file filter_noise.h
	\brief Noise Filter Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __FILTER_NOISE_H__
#define __FILTER_NOISE_H__


#include "filter.h"

#ifdef __cplusplus
extern "C" {
#endif


/*!
	\brief
	\return
*/
filter_implementation_t * filter_noise_get_implementation( void );


/*!
	\brief
	\return
*/
void filter_noise_set_dispersion( filter_t * this, int dispersion );


/*!
	\brief
	\return
*/
int filter_noise_get_dispersion( filter_t * this );


#ifdef __cplusplus
}
#endif


#endif /* __FILTER_NOISE_H__ */

/* $Id: filter_noise.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
