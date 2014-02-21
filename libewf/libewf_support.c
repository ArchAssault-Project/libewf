/*
 * Support functions
 *
 * Copyright (c) 2006-2013, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "libewf_definitions.h"
#include "libewf_filename.h"
#include "libewf_error.h"
#include "libewf_libbfio.h"
#include "libewf_libcerror.h"
#include "libewf_libclocale.h"
#include "libewf_libcnotify.h"
#include "libewf_libcstring.h"
#include "libewf_segment_file.h"
#include "libewf_support.h"

#include "ewf_definitions.h"

#if !defined( HAVE_LOCAL_LIBEWF )

/* Returns the library version as a string
 */
const char *libewf_get_version(
             void )
{
	return( (const char *) LIBEWF_VERSION_STRING );
}

/* Returns the access flags for reading
 */
int libewf_get_access_flags_read(
     void )
{
	return( (int) LIBEWF_ACCESS_FLAG_READ );
}

/* Returns the access flags for reading and writing
 */
int libewf_get_access_flags_read_write(
     void )
{
	return( (int) ( LIBEWF_ACCESS_FLAG_READ | LIBEWF_ACCESS_FLAG_WRITE ) );
}

/* Returns the access flags for writing
 */
int libewf_get_access_flags_write(
     void )
{
	return( (int) LIBEWF_ACCESS_FLAG_WRITE );
}

/* Returns the access flags for resume writing
 */
int libewf_get_access_flags_write_resume(
     void )
{
	return( (int) LIBEWF_ACCESS_FLAG_WRITE | LIBEWF_ACCESS_FLAG_RESUME );
}

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libewf_get_codepage(
     int *codepage,
     libcerror_error_t **error )
{
	static char *function = "libewf_get_codepage";

	if( libclocale_codepage_get(
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve codepage.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
int libewf_set_codepage(
     int codepage,
     libcerror_error_t **error )
{
	static char *function = "libewf_set_codepage";

	if( libclocale_codepage_set(
	     codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set codepage.",
		 function );

		return( -1 );
	}
	return( 1 );
}

#endif /* !defined( HAVE_LOCAL_LIBEWF ) */

/* Determines if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
int libewf_check_file_signature(
     const char *filename,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_check_file_signature";
	size_t filename_length           = 0;
	int result                       = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filename_length = libcstring_narrow_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		goto on_error;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	result = libewf_check_file_signature_file_io_handle(
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to check file signature using a file handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Determines if a file is an EWF file (check for the EWF file signature)
 * Returns 1 if true, 0 if not or -1 on error
 */
#if defined( HAVE_WIDE_CHARACTER_TYPE )
int libewf_check_file_signature_wide(
     const wchar_t *filename,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libewf_check_file_signature_wide";
	size_t filename_length           = 0;
	int result                       = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filename_length = libcstring_wide_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		goto on_error;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	result = libewf_check_file_signature_file_io_handle(
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to check file signature using a file handle.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}
#endif

/* Determines if a file is an EWF file (check for the EWF file signature) using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
int libewf_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	uint8_t signature[ 8 ];

	static char *function      = "libewf_check_file_signature_file_io_handle";
	ssize_t read_count         = 0;
	int file_io_handle_is_open = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     LIBBFIO_OPEN_READ,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file.",
			 function );

			return( -1 );
		}
	}
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: 0.",
		 function );

		if( file_io_handle_is_open == 0 )
		{
			libbfio_handle_close(
			 file_io_handle,
			 error );
		}
		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              signature,
	              8,
	              error );

	if( read_count != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read signature.",
		 function );

		libbfio_handle_close(
		 file_io_handle,
		 NULL );

		return( -1 );
	}
	if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_close(
		     file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file.",
			 function );

			return( -1 );
		}
	}
	/* The number of EWF segment files will be the largest
	 */
	if( memory_compare(
	     ewf1_evf_file_signature,
	     signature,
	     8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          ewf1_lvf_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          ewf2_evf_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          ewf2_lef_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	else if( memory_compare(
	          ewf1_dvf_file_signature,
	          signature,
	          8 ) == 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Globs the segment files according to the EWF naming schema
 * if format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns 1 if successful or -1 on error
 */
int libewf_glob(
     const char *filename,
     size_t filename_length,
     uint8_t format,
     char **filenames[],
     int *number_of_filenames,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	char *segment_filename           = NULL;
	void *reallocation               = NULL;
	static char *function            = "libewf_glob";
	size_t additional_length         = 0;
	size_t segment_extention_length  = 0;
	size_t segment_filename_index    = 0;
	size_t segment_filename_length   = 0;
	uint8_t segment_file_type        = 0;
	int result                       = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( filename_length == 0 )
	 || ( filename_length > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename length.",
		 function );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_UNKNOWN )
	 && ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK_IMAGER )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_V2_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_UNKNOWN )
	{
		if( ( filename_length > 4 )
		 && ( filename[ filename_length - 4 ] == '.' ) )
		{
			if( filename[ filename_length - 3 ] == 'E' )
			{
				format = LIBEWF_FORMAT_ENCASE5;
			}
			else if( filename[ filename_length - 3 ] == 'e' )
			{
				format = LIBEWF_FORMAT_EWF;
			}
			else if( filename[ filename_length - 3 ] == 'L' )
			{
				format = LIBEWF_FORMAT_LOGICAL_ENCASE5;
			}
			else if( filename[ filename_length - 3 ] == 's' )
			{
				format = LIBEWF_FORMAT_SMART;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid filename - unsupported extension: %s.",
				 function,
				 &( filename[ filename_length - 4 ] ) );

				return( -1 );
			}
			segment_extention_length = 4;
		}
		else if( ( filename_length > 5 )
		      && ( filename[ filename_length - 5 ] == '.' ) )
		{
			if( filename[ filename_length - 4 ] == 'E' )
			{
				format = LIBEWF_FORMAT_V2_ENCASE7;
			}
			else if( filename[ filename_length - 4 ] == 'L' )
			{
				format = LIBEWF_FORMAT_V2_LOGICAL_ENCASE7;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid filename - unsupported extension: %s.",
				 function,
				 &( filename[ filename_length - 5 ] ) );

				return( -1 );
			}
			if( filename[ filename_length - 3 ] != 'x' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid filename - unsupported extension: %s.",
				 function,
				 &( filename[ filename_length - 5 ] ) );

				return( -1 );
			}
			segment_extention_length = 5;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - missing extension.",
			 function );

			return( -1 );
		}
	}
	else
	{
		additional_length = 4;
	}
	if( segment_file_type == 0 )
	{
		if( ( format == LIBEWF_FORMAT_LOGICAL_ENCASE5 )
		 || ( format == LIBEWF_FORMAT_LOGICAL_ENCASE6 )
		 || ( format == LIBEWF_FORMAT_LOGICAL_ENCASE7 ) )
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL;
		}
		else if( format == LIBEWF_FORMAT_SMART )
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
		}
		else if( format == LIBEWF_FORMAT_V2_ENCASE7 )
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF2;
		}
		else if( format == LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 )
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL;
		}
		else
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF1;
		}
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	*number_of_filenames = 0;

	while( *number_of_filenames < (int) UINT16_MAX )
	{
		segment_filename_length = filename_length + additional_length;

		segment_filename = (char *) memory_allocate(
			                     sizeof( char ) * ( segment_filename_length + 1 ) );

		if( segment_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment filename.",
			 function );

			goto on_error;
		}
		if( libcstring_narrow_string_copy(
		     segment_filename,
		     filename,
		     filename_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy filename.",
			 function );

			goto on_error;
		}
		if( additional_length == 0 )
		{
			segment_filename_index = segment_filename_length - segment_extention_length;
		}
		else
		{
			segment_filename_index = filename_length;
		}
		segment_filename[ segment_filename_index++ ] = '.';

		if( libewf_filename_set_extension(
		     segment_filename,
		     segment_filename_length + 1,
		     &segment_filename_index,
		     (uint32_t) ( *number_of_filenames + 1 ),
		     (uint32_t) UINT16_MAX,
		     segment_file_type,
		     format,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set extension.",
			 function );

			goto on_error;
		}
		/* The libewf_filename_set_extension also adds the end-of-string character */

		if( libbfio_file_set_name(
		     file_io_handle,
		     segment_filename,
		     segment_filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name in file IO handle.",
			 function );

			goto on_error;
		}
		result = libbfio_handle_exists(
		          file_io_handle,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to test if file exists.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		*number_of_filenames += 1;

		reallocation = memory_reallocate(
		                *filenames,
		                sizeof( char * ) * *number_of_filenames );

		if( reallocation == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize filenames.",
			 function );

			goto on_error;
		}
		*filenames = (char **) reallocation;

		( *filenames )[ *number_of_filenames - 1 ] = segment_filename;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( segment_filename != NULL )
	{
		memory_free(
		 segment_filename );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Frees the globbed filenames
 * Returns 1 if successful or -1 on error
 */
int libewf_glob_free(
     char *filenames[],
     int number_of_filenames,
     libcerror_error_t **error )
{
	static char *function = "libewf_glob_free";
	int filename_iterator = 0;

	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of filenames value less than zero.",
		 function );

		return( -1 );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );
	}
	memory_free(
	 filenames );

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Globs the segment files according to the EWF naming schema
 * if format is known the filename should contain the base of the filename
 * otherwise the function will try to determine the format based on the extension
 * Returns 1 if successful or -1 on error
 */
int libewf_glob_wide(
     const wchar_t *filename,
     size_t filename_length,
     uint8_t format,
     wchar_t **filenames[],
     int *number_of_filenames,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	wchar_t *segment_filename        = NULL;
	void *reallocation               = NULL;
	static char *function            = "libewf_glob_wide";
	size_t additional_length         = 0;
	size_t segment_extention_length  = 0;
	size_t segment_filename_index    = 0;
	size_t segment_filename_length   = 0;
	int result                       = 0;
	uint8_t segment_file_type        = 0;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( filename_length == 0 )
	 || ( filename_length > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid filename length.",
		 function );

		return( -1 );
	}
	if( ( format != LIBEWF_FORMAT_UNKNOWN )
	 && ( format != LIBEWF_FORMAT_ENCASE1 )
	 && ( format != LIBEWF_FORMAT_ENCASE2 )
	 && ( format != LIBEWF_FORMAT_ENCASE3 )
	 && ( format != LIBEWF_FORMAT_ENCASE4 )
	 && ( format != LIBEWF_FORMAT_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LINEN5 )
	 && ( format != LIBEWF_FORMAT_LINEN6 )
	 && ( format != LIBEWF_FORMAT_SMART )
	 && ( format != LIBEWF_FORMAT_FTK_IMAGER )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE5 )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE6 )
	 && ( format != LIBEWF_FORMAT_LOGICAL_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_V2_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 )
	 && ( format != LIBEWF_FORMAT_EWF )
	 && ( format != LIBEWF_FORMAT_EWFX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format.",
		 function );

		return( -1 );
	}
	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( format == LIBEWF_FORMAT_UNKNOWN )
	{
		if( ( filename_length > 4 )
		 && ( filename[ filename_length - 4 ] == (wchar_t) '.' ) )
		{
			if( filename[ filename_length - 3 ] == (wchar_t) 'E' )
			{
				format = LIBEWF_FORMAT_ENCASE5;
			}
			else if( filename[ filename_length - 3 ] == (wchar_t) 'e' )
			{
				format = LIBEWF_FORMAT_EWF;
			}
			else if( filename[ filename_length - 3 ] == (wchar_t) 'L' )
			{
				format = LIBEWF_FORMAT_LOGICAL_ENCASE5;
			}
			else if( filename[ filename_length - 3 ] == (wchar_t) 's' )
			{
				format = LIBEWF_FORMAT_SMART;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid filename - unsupported extension: %ls.",
				 function,
				 &( filename[ filename_length - 4 ] ) );

				return( -1 );
			}
			segment_extention_length = 4;
		}
		else if( ( filename_length > 5 )
		      && ( filename[ filename_length - 5 ] == (wchar_t) '.' ) )
		{
			if( filename[ filename_length - 4 ] == (wchar_t) 'E' )
			{
				format = LIBEWF_FORMAT_V2_ENCASE7;
			}
			else if( filename[ filename_length - 4 ] == (wchar_t) 'L' )
			{
				format = LIBEWF_FORMAT_V2_LOGICAL_ENCASE7;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid filename - unsupported extension: %ls.",
				 function,
				 &( filename[ filename_length - 5 ] ) );

				return( -1 );
			}
			if( filename[ filename_length - 3 ] != (wchar_t) 'x' )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: invalid filename - unsupported extension: %ls.",
				 function,
				 &( filename[ filename_length - 5 ] ) );

				return( -1 );
			}
			segment_extention_length = 5;
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid filename - missing extension.",
			 function );

			return( -1 );
		}
	}
	else
	{
		additional_length = 4;
	}
	if( segment_file_type == 0 )
	{
		if( ( format == LIBEWF_FORMAT_LOGICAL_ENCASE5 )
		 || ( format == LIBEWF_FORMAT_LOGICAL_ENCASE6 )
		 || ( format == LIBEWF_FORMAT_LOGICAL_ENCASE7 ) )
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF1_LOGICAL;
		}
		else if( format == LIBEWF_FORMAT_SMART )
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF1_SMART;
		}
		else if( format == LIBEWF_FORMAT_V2_ENCASE7 )
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF2;
		}
		else if( format == LIBEWF_FORMAT_V2_LOGICAL_ENCASE7 )
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF2_LOGICAL;
		}
		else
		{
			segment_file_type = LIBEWF_SEGMENT_FILE_TYPE_EWF1;
		}
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	*number_of_filenames = 0;

	while( *number_of_filenames < (int) UINT16_MAX )
	{
		segment_filename_length = filename_length + additional_length;

		segment_filename = (wchar_t *) memory_allocate(
			                        sizeof( wchar_t ) * ( segment_filename_length + 1 ) );

		if( segment_filename == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create segment filename.",
			 function );

			goto on_error;
		}
		if( libcstring_wide_string_copy(
		     segment_filename,
		     filename,
		     filename_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy filename.",
			 function );

			goto on_error;
		}
		if( additional_length == 0 )
		{
			segment_filename_index = segment_filename_length - segment_extention_length;
		}
		else
		{
			segment_filename_index = filename_length;
		}
		segment_filename[ segment_filename_index++ ] = (wchar_t) '.';

		if( libewf_filename_set_extension_wide(
		     segment_filename,
		     segment_filename_length + 1,
		     &segment_filename_index,
		     (uint32_t) ( *number_of_filenames + 1 ),
		     (uint32_t) UINT16_MAX,
		     segment_file_type,
		     format,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set extension.",
			 function );

			goto on_error;
		}
		/* The libewf_filename_set_extension_wide also adds the end-of-string character */

		if( libbfio_file_set_name_wide(
		     file_io_handle,
		     segment_filename,
		     segment_filename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set name in file IO handle.",
			 function );

			goto on_error;
		}
		result = libbfio_handle_exists(
		          file_io_handle,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_GENERIC,
			 "%s: unable to test if file exists.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			memory_free(
			 segment_filename );

			break;
		}
		*number_of_filenames += 1;

		reallocation = memory_reallocate(
		                *filenames,
		                sizeof( wchar_t * ) * *number_of_filenames );

		if( reallocation == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize filenames.",
			 function );

			goto on_error;
		}
		*filenames = (wchar_t **) reallocation;

		( *filenames )[ *number_of_filenames - 1 ] = segment_filename;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( segment_filename != NULL )
	{
		memory_free(
		 segment_filename );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Frees the globbed wide filenames
 * Returns 1 if successful or -1 on error
 */
int libewf_glob_wide_free(
     wchar_t *filenames[],
     int number_of_filenames,
     libcerror_error_t **error )
{
	static char *function = "libewf_glob_wide_free";
	int filename_iterator = 0;

	if( filenames == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filenames.",
		 function );

		return( -1 );
	}
	if( number_of_filenames < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid number of filenames value less than zero.",
		 function );

		return( -1 );
	}
	for( filename_iterator = 0;
	     filename_iterator < number_of_filenames;
	     filename_iterator++ )
	{
		memory_free(
		 filenames[ filename_iterator ] );
	}
	memory_free(
	 filenames );

	return( 1 );
}

#endif

