/*!
	\file common.h
	\brief Common Definitions
	\author Tiago Ventura (tiago.ventura@gmail.com)
*/

#ifndef __COMMON_H__
#define __COMMON_H__


#ifdef __cplusplus
extern "C" {
#endif


/*! \brief Extracts the sign of x, returns -1 for negative numbers and +1 for positive numbers */
#define sgn(x)     ((x<0)?-1:((x>0)?1:0))


#ifndef BOOL
/*! \brief Defines a boolean type */
typedef int BOOL;
#endif


#ifndef TRUE
/*! \brief Represents a true state */
#define TRUE (1)
#endif


#ifndef FALSE
/*! \brief Represents a false state */
#define FALSE (0)
#endif


#ifndef sizeof_static_array
/*! \brief Calculate the elements count of an static array */
#define sizeof_static_array(_sa)   ( sizeof(_sa) / sizeof(_sa[0]) )
#endif


#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H__ */

/* $Id: common.h 293 2015-07-28 05:24:22Z tiago.ventura $ */
