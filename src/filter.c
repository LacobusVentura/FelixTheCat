/*!
	\file filter.c
	\brief Linear Spatial Filter Object Implementation (Abstract)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#include <stdlib.h>

#include "frame.h"
#include "filter.h"


/*!
	\brief Represents a Filter Object
*/
struct filter_s
{
	filter_implementation_t * impl;
	void * data;
};


filter_t * filter_create( filter_implementation_t * impl )
{
	filter_t * flt = NULL;

	if( !impl )
		return NULL;

	flt = (filter_t*) calloc( 1, sizeof(filter_t) );

	if(!flt)
		return NULL;

	flt->impl = impl;
	flt->data = NULL;

	flt->impl->create( flt );

	return flt;
}


void filter_destroy( filter_t * this )
{
	this->impl->destroy( this );
	free( this );
}


void * filter_get_data( filter_t * this )
{
	return this->data;
}


void filter_set_data( filter_t * this, void * data )
{
	this->data = data;
}


void filter_get_filterd_point( filter_t * this, frame_point_t * pt, frame_t * frm, int col, int row )
{
	this->impl->get_filtered_point( this, pt, frm, col, row );
}


void filter_frame( filter_t * this, frame_t * frm )
{
	frame_t * frmaux = NULL;
	frame_point_t pt;
	int row = 0;
	int col = 0;
	int nrows = 0;
	int ncols = 0;


	frmaux = frame_duplicate( frm );

	if(!frmaux)
		return;

	frame_get_dimensions( frmaux, &ncols, &nrows );

	for( col = 0; col < ncols; col++ )
	{
		for( row = 0; row < nrows; row++ )
		{
			this->impl->get_filtered_point( this, &pt, frm, col, row );

			frame_set_point( frmaux, col, row, &pt );
		}
	}

	frame_copy( frm, frmaux );

	frame_destroy( frmaux );

	return;
}

/* $Id: filter.c 304 2015-08-08 00:57:58Z tiago.ventura $ */
