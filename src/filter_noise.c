/*!
	\file filter_noise.c
	\brief Noise Filter Object Implementation (Concrete)
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#include <stdlib.h>

#include "frame.h"
#include "filter.h"
#include "filter_noise.h"


#define FILTER_NOISE_DEFAULT_DISPERSION_VALUE    (3)


struct filter_noise_params_s
{
	int dispersion;
};

typedef struct filter_noise_params_s filter_noise_params_t;


static filter_t * filter_noise_create( filter_t * parent );
static void filter_noise_destroy( filter_t * this );
static void filter_noise_get_filtered_point( filter_t * this, frame_point_t * pt, frame_t * frm, int col, int row );


filter_implementation_t * filter_noise_get_implementation( void )
{
	static filter_implementation_t impl;

	impl.create = filter_noise_create;
	impl.destroy = filter_noise_destroy;
	impl.get_filtered_point = filter_noise_get_filtered_point;

	return &impl;
}


static filter_t * filter_noise_create( filter_t * parent )
{
	filter_noise_params_t * params = NULL;

	params = (filter_noise_params_t *) calloc( 1, sizeof(filter_noise_params_t) );

	if(!params)
		return NULL;

	params->dispersion = FILTER_NOISE_DEFAULT_DISPERSION_VALUE;

	filter_set_data( parent, params );

	return parent;
}


static void filter_noise_destroy( filter_t * this )
{
	free( filter_get_data(this) );
}


void filter_noise_set_dispersion( filter_t * this, int dispersion )
{
	((filter_noise_params_t*)(filter_get_data(this)))->dispersion = dispersion;
}


int filter_noise_get_dispersion( filter_t * this )
{
	return ((filter_noise_params_t*)(filter_get_data(this)))->dispersion;
}


static void filter_noise_get_filtered_point( filter_t * this, frame_point_t * pt, frame_t * frm, int col, int row )
{
	int coff = 0;
	int roff = 0;

	int d = filter_noise_get_dispersion( this );

	roff = ( rand() % (d + 1) ) - (d / 2);
	coff = ( rand() % (d + 1) ) - (d / 2);

	frame_get_point( frm, col + coff, row + roff, pt );
}

/* $Id: filter_noise.c 293 2015-07-28 05:24:22Z tiago.ventura $ */
