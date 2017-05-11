/*!
	\file frame.h
	\brief Frame Object Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#ifndef __FRAME_H__
#define __FRAME_H__


#ifdef __cplusplus
extern "C" {
#endif

/*!
	\brief Represents a Frame Point type
*/
struct frame_point_s
{
	int color;
	int bgcolor;
	char chr;
	int value;
};

/*!
	\brief Define a Frame Point type
*/
typedef struct frame_point_s frame_point_t;


/*!
	\brief Define a Frame Object type (opaque)
*/
typedef struct frame_s frame_t;

/*!
	\brief Define a Frame Object Mode type
*/
typedef enum frame_border_mode_e frame_border_mode_t;

/*!
	\brief Enumerate Frame object modes
*/
enum frame_border_mode_e
{
	frame_border_zero_padded,  /*!< Black Border */
	frame_border_toroidal,     /*!< Toroidal Array / No Border */
	frame_border_extended      /*!< Extended Border */
};

/*!
	\brief Frame Object Constructor
	\param ncols
	\param nrows
	\return Frame Object
*/
frame_t * frame_create( int ncols, int nrows );

/*!
	\brief Frame Object Destructor
	\param this Frame Object to be destroyed
*/
void frame_destroy( frame_t * this );

/*!
	\brief Copy a Frame Object to Another
	\param dst Destination Frame Object
	\param src Source Frame Object
*/
void frame_copy( frame_t * dst, frame_t * src );

/*!
	\brief Duplicate a Frame Object
	\param this Frame Object to Duplicate
	\return Duplicated Frame Object
*/
frame_t * frame_duplicate( frame_t * this );

/*!
	\brief Clear a Frame Object
	\param this Frame Object
*/
void frame_clear( frame_t * this );

/*!
	\brief Fill a Frame Object
	\param this Frame Object
*/
void frame_fill( frame_t * this, frame_point_t * pt );

/*!
	\brief Set frame array border mode
	\param this Frame Object
	\param border Border mode
*/
void frame_set_border_mode( frame_t * this, frame_border_mode_t mode );

/*!
	\brief Get frame array border mode
	\param this Frame Object
	\return Current frame array mode
*/
frame_border_mode_t frame_get_border_mode( frame_t * this );

/*!
	\brief Get frame columns count
	\param this Frame Object
	\return  columns count
*/
int frame_get_cols_count( frame_t * this );

/*!
	\brief Get frame rows count
	\param this Frame Object
	\return rows count
*/
int frame_get_rows_count( frame_t * this );

/*!
	\brief Get frame dimensions
	\param this Frame Object
	\param ncols
	\param nrows
*/
void frame_get_dimensions( frame_t * this, int * ncols, int * nrows );

/*!
	\brief Set point
	\param this Frame Object
	\param col
	\param row
	\param val
*/
void frame_set_point( frame_t * this, int col, int row, frame_point_t * pt );

/*!
	\brief Get point
	\param this Frame Object
	\param col
	\param row
*/
void frame_get_point( frame_t * this, int col, int row, frame_point_t * pt );

/*!
	\brief Draw a Circle
	\param this Frame Object
	\param col
	\param row
	\param radius
	\param color
	\return
*/
void frame_draw_circle( frame_t * this, int col, int row, int radius, frame_point_t * pt );

/*!
	\brief Draw a Line
	\param this Frame Object
	\param col1
	\param row1
	\param col2
	\param row2
*/
void frame_draw_line( frame_t * this, int col1, int row1, int col2, int row2, frame_point_t * pt );

/*!
	\brief Draw a Rectangle
	\param this Frame Object
	\param left
	\param top
	\param right
	\param bottom
	\param color
*/
void frame_draw_rect( frame_t * this, int left, int top, int right, int bottom, frame_point_t * pt );

/*!
	\brief Draw an Ellipse
	\param this Frame Object
	\param col
	\param row
	\param xr
	\param yr
	\param color
*/
void frame_draw_ellipse( frame_t * this, int col, int row, int xr, int yr, frame_point_t * pt );

/*!
	\brief Make Point
	\returns
*/
frame_point_t * frame_make_point( frame_point_t * pt, int value, int bgcolor, int color, char chr );

#ifdef __cplusplus
}
#endif

#endif /* __FRAME_H__ */

/* $Id: frame.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
