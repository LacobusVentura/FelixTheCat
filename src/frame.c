/*!
	\file frame.c
	\brief Frame Object Implementation
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "frame.h"


/*!
	\brief Represents a Frame Object
*/
struct frame_s
{
	int ncols;
	int nrows;
	frame_border_mode_t border;
	frame_point_t ** buf;
};


frame_t * frame_create( int ncols, int nrows )
{
	frame_t * frm = NULL;
	int i = 0;


	frm = (frame_t*) calloc( 1, sizeof(frame_t) );

	if( !frm )
		return NULL;

	frm->ncols = ncols;
	frm->nrows = nrows;
	frm->border = frame_border_zero_padded;

	frm->buf = (frame_point_t**) calloc( nrows, sizeof(frame_point_t*) );

	if( !frm->buf )
	{
		frame_destroy( frm );
		return NULL;
	}

	for( i = 0; i < nrows; i++ )
	{
		frm->buf[i] = (frame_point_t*) calloc( ncols, sizeof(frame_point_t) );

		if( !frm->buf[i] )
		{
			frame_destroy( frm );
			return NULL;
		}
	}

	return frm;
}


void frame_destroy( frame_t * frm )
{
	int i = 0;

	if( frm->buf )
	{
		for( i = 0; i < frm->nrows; i++ )
			if( frm->buf[i] )
				free( frm->buf[i] );

		free( frm->buf );
	}

	free( frm );
}


void frame_copy( frame_t * dst, frame_t * src )
{
	int i = 0;

	for( i = 0; i < src->nrows; i++ )
		memcpy( dst->buf[i], src->buf[i], src->ncols * sizeof(frame_point_t) );
}


frame_t * frame_duplicate( frame_t * frm )
{
	frame_t * new = NULL;

	new = frame_create( frm->ncols, frm->nrows );

	if(!new)
		return NULL;

	frame_copy( new, frm );

	return new;
}


void frame_clear( frame_t * this )
{
	int i = 0;

	for( i = 0; i < this->nrows; i++ )
		memset( this->buf[i], 0, this->ncols * sizeof(frame_point_t) );
}


void frame_fill( frame_t * this, frame_point_t * pt )
{
	int row = 0;
	int col = 0;

	for( row = 0; row < this->nrows; row++ )
		for( col = 0; col < this->ncols; col++ )
			memcpy( &this->buf[row][col], pt, sizeof(frame_point_t) );
}


void frame_set_border_mode( frame_t * this, frame_border_mode_t mode )
{
	this->border = mode;
}


frame_border_mode_t frame_get_border_mode( frame_t * this )
{
	return this->border;
}


int frame_get_cols_count( frame_t * this )
{
	return this->ncols;
}


int frame_get_rows_count( frame_t * this )
{
	return this->nrows;
}


void frame_get_dimensions( frame_t * this, int * ncols, int * nrows )
{
	*ncols = this->ncols;
	*nrows = this->nrows;
}


void frame_set_point( frame_t * this, int col, int row, frame_point_t * pt )
{
	if( (this->border == frame_border_zero_padded) || (this->border == frame_border_extended) )
	{
		if( (row < 0) || (col < 0) || (row >= this->nrows) || (col >= this->ncols) )
			return;

		memcpy( &(this->buf[row][col]), pt, sizeof(frame_point_t) );
	}
	else if( this->border == frame_border_toroidal )
	{
		if( row < 0 )
			row = this->nrows + (row % this->nrows);

		if( row >= this->nrows )
			row = row % this->nrows;

		if( col < 0 )
			col = this->ncols + (col % this->ncols);

		if( col >= this->ncols )
			col = col % this->ncols;

		memcpy( &(this->buf[row][col]), pt, sizeof(frame_point_t) );
	}
}


void frame_get_point( frame_t * this, int col, int row, frame_point_t * pt )
{
	if( this->border == frame_border_zero_padded )
	{
		if( (row < 0) || (col < 0) || (col >= this->ncols) || (row >= this->nrows) )
		{
			memset( pt, 0, sizeof(frame_point_t) );
			return;
		}

		memcpy( pt, &(this->buf[row][col]), sizeof(frame_point_t) );

	}
	else if( this->border == frame_border_extended )
	{
		if( row < 0 )
			row = 0;

		if( row >= this->nrows )
			row = this->nrows - 1;

		if( col < 0 )
			col = 0;

		if( col >= this->ncols )
			col = this->ncols - 1;

		memcpy( pt, &(this->buf[row][col]), sizeof(frame_point_t) );
	}
	else if( this->border == frame_border_toroidal )
	{
		if( row < 0 )
			row = this->nrows + (row % this->nrows);

		if( row >= this->nrows )
			row = row % this->nrows;

		if( col < 0 )
			col = this->ncols + (col % this->ncols);

		if( col >= this->ncols )
			col = col % this->ncols;

		memcpy( pt, &(this->buf[row][col]), sizeof(frame_point_t) );
	}
}


void frame_draw_line( frame_t * this, int col1, int row1, int col2, int row2, frame_point_t * pt )
{
	int i = 0;
	int dx = 0;
	int dy = 0;
	int sdx = 0;
	int sdy = 0;
	int dxabs = 0;
	int dyabs = 0;
	int x = 0;
	int y = 0;
	int px = 0;
	int py = 0;

	/* Bresenham's line-drawing algorithm */

	dx = col2 - col1;
	dy = row2 - row1;

	dxabs = abs(dx);
	dyabs = abs(dy);

	sdx = sgn(dx);
	sdy = sgn(dy);

	x = dyabs >> 1;
	y = dxabs >> 1;

	px = col1;
	py = row1;

	if( dxabs >= dyabs ) /* the line is more horizontal than vertical */
	{
		for( i = 0; i < dxabs; i++ )
		{
			y += dyabs;

			if( y >= dxabs )
			{
				y -= dxabs;
				py += sdy;
			}

			px += sdx;

			frame_set_point( this, px, py, pt );
		}
	}
	else /* the line is more vertical than horizontal */
	{
		for( i = 0; i < dyabs; i++ )
		{
			x += dxabs;

			if( x >= dyabs )
			{
				x -= dyabs;
				px += sdx;
			}

			py += sdy;

			frame_set_point( this, px, py, pt );
		}
	}
}


void frame_draw_rect( frame_t * this, int left, int top, int right, int bottom, frame_point_t * pt )
{
	int i = 0;
	int aux = 0;

	if( top > bottom )
	{
		aux = top;
		top = bottom;
		bottom = aux;
	}

	if( left > right )
	{
		aux = left;
		left = right;
		right = aux;
	}

	for( i = left; i <= right; i++ )
	{
		frame_set_point( this, i, top, pt );
		frame_set_point( this, i, bottom, pt );
	}

	for( i = top; i <= bottom; i++ )
	{
		frame_set_point( this, left, i, pt );
		frame_set_point( this, right, i, pt );
	}
}


void frame_draw_circle( frame_t * this, int col, int row, int radius, frame_point_t * pt )
{
	int xoff = radius;
	int yoff = 0;
	int d = 1 - xoff;

	/* Midpoint circle algorithm */

	while( xoff >= yoff )
	{
		frame_set_point( this, col + xoff, row + yoff, pt );
		frame_set_point( this, col + yoff, row + xoff, pt );
		frame_set_point( this, col - xoff, row + yoff, pt );
		frame_set_point( this, col - yoff, row + xoff, pt );
		frame_set_point( this, col - xoff, row - yoff, pt );
		frame_set_point( this, col - yoff, row - xoff, pt );
		frame_set_point( this, col + xoff, row - yoff, pt );
		frame_set_point( this, col + yoff, row - xoff, pt );

		yoff++;

		if( d <= 0 )
		{
			d += (2 * yoff) + 1;
		}
		else
		{
			xoff--;
			d += (2 * (yoff - xoff)) + 1;
		}
	}
}

void frame_draw_ellipse( frame_t * this, int col, int row, int xr, int yr, frame_point_t * pt )
{
	int x = 0;
	int y = 0;
	int xchange = 0;
	int ychange = 0;
	int ellipse_error = 0;
	int two_a_square = 0;
	int two_b_square = 0;
	int stopx = 0;
	int stopy = 0;

	/* A Fast Bresenham Type Algorithm For Drawing Ellipses - by John Kennedy */

	two_a_square = 2 * xr * xr;
	two_b_square = 2 * yr * yr;

	x = xr;

	xchange = yr * yr * ( 1 - (2 * xr) );
	ychange = xr * xr;

	stopx = two_b_square * xr;

	while( stopx >= stopy )
	{
		frame_set_point( this, col + x, row + y, pt );
		frame_set_point( this, col - x, row + y, pt );
		frame_set_point( this, col - x, row - y, pt );
		frame_set_point( this, col + x, row - y, pt );

		y++;

		stopy += two_a_square;
		ellipse_error += ychange;
		ychange += two_a_square;

		if( ( (2 * ellipse_error) + xchange ) > 0 )
		{
			x--;

			stopx -= two_b_square;
			ellipse_error += xchange;
			xchange += two_b_square;
		}
	};

	x = 0;
	y = yr;

	xchange = yr * yr;
	ychange = xr * xr * ( 1 - (2 * yr) );

	ellipse_error = 0;

	stopx = 0;
	stopy = two_a_square * yr;

	while( stopx <= stopy )
	{
		frame_set_point( this, col + x, row + y, pt );
		frame_set_point( this, col - x, row + y, pt );
		frame_set_point( this, col - x, row - y, pt );
		frame_set_point( this, col + x, row - y, pt );

		x++;

		stopx += two_b_square;
		ellipse_error += xchange;
		xchange += two_b_square;

		if( ( (2 * ellipse_error) + ychange ) > 0 )
		{
			y--;

			stopy -= two_a_square;
			ellipse_error += ychange;
			ychange += two_a_square;
		}
	}
}


inline frame_point_t * frame_make_point( frame_point_t * pt, int value, int bgcolor, int color, char chr )
{
	pt->value = value;
	pt->bgcolor = bgcolor;
	pt->color = color;
	pt->chr = chr;

	return pt;
}

/* $Id: frame.c 304 2015-08-08 00:57:58Z tiago.ventura $ */
