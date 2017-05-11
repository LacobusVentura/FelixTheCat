/*!
	\file filter.h
	\brief Linear Spatial Filter Interface (Abstract)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#ifndef __FILTER_H__
#define __FILTER_H__

#include "frame.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct filter_implementation_s filter_implementation_t;

typedef struct filter_s filter_t;

/*!
	\brief Represents an Implementation of a Filter Object
*/
struct filter_implementation_s
{
	filter_t * (*create) (filter_t *);
	void (*destroy) (filter_t *);
	void (*get_filtered_point) ( filter_t*, frame_point_t*, frame_t*, int, int );
};


filter_t * filter_create( filter_implementation_t * impl );

void filter_destroy( filter_t * this );

void filter_frame( filter_t * this, frame_t * frm );

void * filter_get_data( filter_t * this );

void filter_set_data( filter_t * this, void * data );

#ifdef __cplusplus
}
#endif

#endif /* __FILTER_H__ */

/* $Id: filter.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
