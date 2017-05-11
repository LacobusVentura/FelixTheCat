/*!
	\file palette.h
	\brief Palette Object Interface
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/


#ifndef __PALETTE_H__
#define __PALETTE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/*!
	\brief Palette Object Type Definition
*/
typedef struct palette_s palette_t;


/*!
	\brief Palette Object Contructor
	\return Palette Object
*/
palette_t * palette_create( void );


/*!
	\brief Palette Object Destructor
	\param this Palette Object to Destroy
*/
void palette_destroy( palette_t * this );


/*!
	\brief Copy a Palette Object
	\param dst Destination Palette
	\param src Source Palette
*/
void palette_copy( palette_t * dst, palette_t * src );


/*!
	\brief Clears a Palette Object
	\param this
*/
void palette_clear( palette_t * this );


/*!
	\brief
	\param this Palette Object
	\return
*/
int palette_get_color_count( palette_t * pal );


/*!
	\brief Set Palette Object Color Element
	\param this Palette Object
	\param idx Color Element Index
	\param red Red Component
	\param green Green Component
	\param blue Blue Component
*/
void palette_set_color( palette_t * this, int idx, uint8_t red, uint8_t green, uint8_t blue );


/*!
	\brief Get Palette Object Color Element
	\param this Palette Object
	\param idx Color Element Index
	\param red Red Component
	\param green Green Component
	\param blue Blue Component
*/
void palette_get_color( palette_t * this, int idx, uint8_t * red, uint8_t * green, uint8_t * blue );


/*!
	\brief Set Palette Object Default Colors
	\param this Palette Object
*/
void palette_set_default( palette_t * this );


#ifdef __cplusplus
}
#endif


#endif /* __PALETTE_H__ */

/* $Id: palette.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
