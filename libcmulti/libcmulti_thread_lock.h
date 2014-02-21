/*
 * Thread lock functions
 *
 * Copyright (C) 2012-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCMULTI_THREAD_LOCK_H )
#define _LIBCMULTI_THREAD_LOCK_H

#include <common.h>
#include <types.h>

#if defined( HAVE_PTHREAD_H )
#include <pthread.h>
#endif

#include "libcmulti_libcerror.h"
#include "libcmulti_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcmulti_internal_thread_lock libcmulti_internal_thread_lock_t;

struct libcmulti_internal_thread_lock
{
#if defined( WINAPI )
	/* The critical section
	 */
	CRITICAL_SECTION critical_section;

#elif defined( HAVE_PTHREAD_H )
	/* The mutex
	 */
	pthread_mutex_t mutex;

#else
	/* Dummy
	 */
	int dummy;
#endif
};

int libcmulti_thread_lock_initialize(
     libcmulti_thread_lock_t **lock,
     libcerror_error_t **error );

int libcmulti_thread_lock_free(
     libcmulti_thread_lock_t **lock,
     libcerror_error_t **error );

int libcmulti_thread_lock_grab(
     libcmulti_thread_lock_t *lock,
     libcerror_error_t **error );

int libcmulti_thread_lock_release(
     libcmulti_thread_lock_t *lock,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

