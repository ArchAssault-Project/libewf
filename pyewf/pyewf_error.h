/*
 * Error functions
 *
 * Copyright (c) 2008-2013, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYEWF_ERROR_H )
#define _PYEWF_ERROR_H

#include <common.h>
#include <types.h>

#include "pyewf_libcerror.h"
#include "pyewf_python.h"

#define PYEWF_ERROR_FORMAT_STRING_SIZE		128
#define PYEWF_ERROR_STRING_SIZE			512

#if defined( __cplusplus )
extern "C" {
#endif

void pyewf_error_raise(
      PyObject *exception_object,
      const char *format_string,
      const char *function,
      libcerror_error_t *error );

#if defined( __cplusplus )
}
#endif

#endif
