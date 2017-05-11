/*!
	\file filter_blur.c
	\brief Bluer Filter Object Implementation (Concrete)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#include <stdlib.h>

#include "frame.h"
#include "filter.h"
#include "filter_blur.h"


static filter_t * filter_blur_create( filter_t * parent );
static void filter_blur_destroy( filter_t * this );
static void filter_blur_get_filtered_point( filter_t * this, frame_point_t * pt, frame_t * frm, int col, int row );


filter_implementation_t * filter_blur_get_implementation( void )
{
	static filter_implementation_t impl;

	impl.create = filter_blur_create;
	impl.destroy = filter_blur_destroy;
	impl.get_filtered_point = filter_blur_get_filtered_point;

	return &impl;
}



static filter_t * filter_blur_create( filter_t * parent )
{
	return parent;
}


static void filter_blur_destroy( filter_t * this )
{
}


static void filter_blur_get_filtered_point( filter_t * this, frame_point_t * pt, frame_t * frm, int col, int row )
{
	int sum = 0;
	frame_point_t point;

	frame_get_point( frm, col + 1, row + 1, &point );
	sum += point.color;

	frame_get_point( frm, col + 1, row -1, &point );
	sum += point.color;

	frame_get_point( frm, col - 1, row + 1, &point );
	sum += point.color;

	frame_get_point( frm, col - 1, row - 1, &point );
	sum += point.color;

	frame_get_point( frm, col, row, &point );
	sum += point.color;

	pt->bgcolor = point.bgcolor;
	pt->chr = point.chr;
	pt->color = ( sum / 5 );
}

/* $Id: filter_blur.c 293 2015-07-28 05:24:22Z tiago.ventura $ */
