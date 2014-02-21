/*
 * The internal type definitions
 *
 * Copyright (c) 2012-2013, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBCMULTI_TYPES_H )
#define _LIBCMULTI_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBCMULTI for local use of libcstring
 * The definitions in <libcstring/types.h> are copied here
 * for local use of libcstring
 */
#if defined( HAVE_LOCAL_LIBCMULTI )

/* The following type definitions hide internal data structures
 */

#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libcmulti_thread_lock {}	libcmulti_thread_lock_t;

#else
typedef intptr_t libcmulti_thread_lock_t;

#endif /* defined( HAVE_LOCAL_LIBCMULTI ) */

#endif

#endif

