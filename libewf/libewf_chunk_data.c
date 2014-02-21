/*
 * Chunk data functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libewf_checksum.h"
#include "libewf_chunk_data.h"
#include "libewf_compression.h"
#include "libewf_definitions.h"
#include "libewf_libcerror.h"
#include "libewf_libcnotify.h"
#include "libewf_types.h"

#include "ewf_definitions.h"

/* Initialize the chunk data
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_initialize(
     libewf_chunk_data_t **chunk_data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_initialize";

	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( *chunk_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid chunk data value already set.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	*chunk_data = memory_allocate_structure(
	               libewf_chunk_data_t );

	if( *chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create chunk data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *chunk_data,
	     0,
	     sizeof( libewf_chunk_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear chunk data.",
		 function );

		goto on_error;
	}
	if( data_size > 0 )
	{
		/* The allocated data size should be rounded to the next 16-byte increment
		 */
		if( ( data_size % 16 ) != 0 )
		{
			data_size += 16;
		}
		data_size = ( data_size / 16 ) * 16;

		( *chunk_data )->data = (uint8_t *) memory_allocate(
		                                     sizeof( uint8_t ) * data_size );

		if( ( *chunk_data )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		( *chunk_data )->allocated_data_size = data_size;
		( *chunk_data )->flags               = LIBEWF_CHUNK_DATA_ITEM_FLAG_MANAGED_DATA;
	}
	return( 1 );

on_error:
	if( *chunk_data != NULL )
	{
		memory_free(
		 *chunk_data );

		*chunk_data = NULL;
	}
	return( -1 );
}

/* Frees the chunk data including elements
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_free(
     libewf_chunk_data_t **chunk_data,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_free";

	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( *chunk_data != NULL )
	{
		if( ( ( *chunk_data )->flags & LIBEWF_CHUNK_DATA_ITEM_FLAG_MANAGED_DATA ) != 0 )
		{
			if( ( *chunk_data )->data != NULL )
			{
				memory_free(
				 ( *chunk_data )->data );
			}
		}
		if( ( *chunk_data )->compressed_data != NULL )
		{
			memory_free(
			 ( *chunk_data )->compressed_data );
		}
		memory_free(
		 *chunk_data );

		*chunk_data = NULL;
	}
	return( 1 );
}

/* Packs the chunk data
 * This function either adds the checksum or compresses the chunk data
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_pack(
     libewf_chunk_data_t *chunk_data,
     uint32_t chunk_size,
     uint16_t compression_method,
     int8_t compression_level,
     uint8_t compression_flags,
     const uint8_t *compressed_zero_byte_empty_block,
     size_t compressed_zero_byte_empty_block_size,
     uint8_t pack_flags,
     libcerror_error_t **error )
{
	static char *function   = "libewf_chunk_data_pack";
	uint64_t fill_pattern   = 0;
	uint32_t chunk_checksum = 0;
	int8_t chunk_io_flags   = 0;
	int result              = 0;

	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - missing data.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T > 4
	if( chunk_size == 0 )
#else
	if( ( chunk_size == 0 )
	 || ( (size_t) chunk_size > (size_t) SSIZE_MAX ) )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: chunk size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_PACKED ) == 0 )
	{
		if( ( ( compression_flags & LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
		 || ( ( compression_flags & LIBEWF_COMPRESS_FLAG_USE_PATTERN_FILL_COMPRESSION ) != 0 ) )
		{
			if( ( chunk_data->data_size % 8 ) == 0 )
			{
				result = libewf_chunk_data_check_for_64_bit_pattern_fill(
					  chunk_data->data,
					  chunk_data->data_size,
					  &fill_pattern,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine if chunk data contains a fill pattern.",
					 function );

					return( -1 );
				}
				else if( result != 0 )
				{
					if( ( compression_flags & LIBEWF_COMPRESS_FLAG_USE_PATTERN_FILL_COMPRESSION ) != 0 )
					{
						pack_flags &= ~( LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM );
						pack_flags |= LIBEWF_PACK_FLAG_FORCE_COMPRESSION;
						pack_flags |= LIBEWF_PACK_FLAG_USE_PATTERN_FILL_COMPRESSION;
					}
					else if( fill_pattern == 0 )
					{
						pack_flags &= ~( LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM );
						pack_flags |= LIBEWF_PACK_FLAG_FORCE_COMPRESSION;
						pack_flags |= LIBEWF_PACK_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
					}
				}
			}
			else if( ( compression_flags & LIBEWF_COMPRESS_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
			{
				result = libewf_chunk_data_check_for_empty_block(
					  chunk_data->data,
					  chunk_data->data_size,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine if chunk data is an empty block.",
					 function );

					return( -1 );
				}
				else if( result != 0 )
				{
					if( chunk_data->data[ 0 ] == 0 )
					{
						pack_flags &= ~( LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM );
						pack_flags |= LIBEWF_PACK_FLAG_FORCE_COMPRESSION;
						pack_flags |= LIBEWF_PACK_FLAG_USE_EMPTY_BLOCK_COMPRESSION;
					}
				}
			}
		}
		if( ( compression_level != LIBEWF_COMPRESSION_NONE )
		 || ( ( pack_flags & LIBEWF_PACK_FLAG_FORCE_COMPRESSION ) != 0 ) )
		{
			if( ( pack_flags & LIBEWF_PACK_FLAG_FORCE_COMPRESSION ) == 0 )
			{
				chunk_data->compressed_data_size = chunk_size;
			}
			else if( ( pack_flags & LIBEWF_PACK_FLAG_USE_PATTERN_FILL_COMPRESSION ) != 0 )
			{
				chunk_data->compressed_data_size = 8;
			}
			else if( ( pack_flags & LIBEWF_PACK_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
			{
				chunk_data->compressed_data_size = compressed_zero_byte_empty_block_size;

				if( ( compressed_zero_byte_empty_block_size % 16 ) != 0 )
				{
					chunk_data->compressed_data_size += 16 - ( compressed_zero_byte_empty_block_size % 16 );
				}
			}
			/* If the compression is forced and none of the other compression pack flags are set
			 * we are dealing with EWF-S01 allow it to have compressed chunks larger than the chunk size
			 * A factor 2 should suffice
			 */
			else
			{
				chunk_data->compressed_data_size = 2 * chunk_size;
			}
			chunk_data->compressed_data = (uint8_t *) memory_allocate(
			                                           sizeof( uint8_t ) * chunk_data->compressed_data_size );

			if( chunk_data->compressed_data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create compressed data.",
				 function );

				goto on_error;
			}
		}
		if( libewf_chunk_data_pack_buffer(
		     chunk_data->data,
		     chunk_data->allocated_data_size,
		     chunk_data->compressed_data,
		     &( chunk_data->compressed_data_offset ),
		     &( chunk_data->compressed_data_size ),
		     chunk_size,
		     chunk_data->data_size,
		     &( chunk_data->padding_size ),
		     compression_method,
		     compression_level,
		     &( chunk_data->range_flags ),
		     &chunk_checksum,
		     &chunk_io_flags,
		     compressed_zero_byte_empty_block,
		     compressed_zero_byte_empty_block_size,
		     pack_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to pack chunk buffer.",
			 function );

			goto on_error;
		}
		chunk_data->data_size = chunk_data->compressed_data_size;

		if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			if( ( chunk_data->flags & LIBEWF_CHUNK_DATA_ITEM_FLAG_MANAGED_DATA ) != 0 )
			{
				memory_free(
				 chunk_data->data );
			}
			chunk_data->data  = chunk_data->compressed_data;
			chunk_data->flags = LIBEWF_CHUNK_DATA_ITEM_FLAG_MANAGED_DATA;

			chunk_data->compressed_data      = NULL;
			chunk_data->compressed_data_size = 0;
		}
		chunk_data->range_flags |= LIBEWF_RANGE_FLAG_IS_PACKED;
	}
	return( 1 );

on_error:
	if( chunk_data->compressed_data != NULL )
	{
		memory_free(
		 chunk_data->compressed_data );

		chunk_data->compressed_data = NULL;
	}
	chunk_data->compressed_data_size = 0;

	return( -1 );
}

/* Packs a buffer containing the chunk data
 * This function either adds the checksum or compresses the chunk data
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_pack_buffer(
     uint8_t *data,
     size_t data_size,
     uint8_t *compressed_data,
     size_t *compressed_data_offset,
     size_t *compressed_data_size,
     uint32_t chunk_size,
     size_t chunk_data_size,
     size_t *chunk_padding_size,
     uint16_t compression_method,
     int8_t compression_level,
     uint32_t *range_flags,
     uint32_t *chunk_checksum,
     int8_t *chunk_io_flags,
     const uint8_t *compressed_zero_byte_empty_block,
     size_t compressed_zero_byte_empty_block_size,
     uint8_t pack_flags,
     libcerror_error_t **error )
{
	static char *function            = "libewf_chunk_data_pack_buffer";
	size_t safe_compressed_data_size = 0;
	int result                       = 0;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk size value out of bounds.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T <= 4
	if( (size_t) chunk_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: chunk size value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	if( chunk_padding_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk padding size.",
		 function );

		return( -1 );
	}
	if( range_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid range flags.",
		 function );

		return( -1 );
	}
	if( chunk_checksum == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk checksum.",
		 function );

		return( -1 );
	}
	if( chunk_io_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk IO flags.",
		 function );

		return( -1 );
	}
	/* Make sure range flags is cleared before usage
	 */
	*range_flags = 0;

	if( ( ( pack_flags & LIBEWF_PACK_FLAG_FORCE_COMPRESSION ) != 0 )
	 || ( compression_level != LIBEWF_COMPRESSION_NONE ) )
	{
		if( compressed_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid compressed data.",
			 function );

			return( -1 );
		}
		if( compressed_data == data )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: compressed data points to the same buffer as data.",
			 function );

			return( -1 );
		}
		if( compressed_data_offset == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid compressed data offset.",
			 function );

			return( -1 );
		}
		if( compressed_data_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid compressed data size.",
			 function );

			return( -1 );
		}
		if( *compressed_data_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid compressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( ( ( pack_flags & LIBEWF_PACK_FLAG_USE_PATTERN_FILL_COMPRESSION ) != 0 )
		 && ( chunk_data_size == (size_t) chunk_size ) )
		{
			if( *compressed_data_size < 8 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: compressed data size value out of bounds.",
				 function );

				return( -1 );
			}
			if( memory_copy(
			     compressed_data,
			     data,
			     8 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy fill pattern to compressed chunk buffer.",
				 function );

				return( -1 );
			}
			safe_compressed_data_size = compressed_zero_byte_empty_block_size;
		}
		else if( ( ( pack_flags & LIBEWF_PACK_FLAG_USE_EMPTY_BLOCK_COMPRESSION ) != 0 )
		      && ( chunk_data_size == (size_t) chunk_size )
		      && ( compressed_zero_byte_empty_block != NULL ) )
		{
			if( *compressed_data_size < compressed_zero_byte_empty_block_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: compressed data size value out of bounds.",
				 function );

				return( -1 );
			}
			if( memory_copy(
			     compressed_data,
			     compressed_zero_byte_empty_block,
			     compressed_zero_byte_empty_block_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy compressed zero byte empty block to compressed chunk buffer.",
				 function );

				return( -1 );
			}
			safe_compressed_data_size = compressed_zero_byte_empty_block_size;
		}
		else
		{
			/* If compression was forced but no compression level provided use the default
			 */
			if( compression_level == LIBEWF_COMPRESSION_NONE )
			{
				compression_level = LIBEWF_COMPRESSION_DEFAULT;
			}
			safe_compressed_data_size = *compressed_data_size;

/* TODO add a light weight entropy test */
			result = libewf_compress_data(
			          compressed_data,
			          &safe_compressed_data_size,
			          compression_method,
			          compression_level,
			          data,
			          chunk_data_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
				 "%s: unable to compress chunk data.",
				 function );

				return( -1 );
			}
			else if( result == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
		                if( libcnotify_verbose != 0 )
		                {
	        	                libcnotify_printf(
		                         "%s: required compressed data size: %zd.\n",
		                         function,
					 safe_compressed_data_size );
				}
#endif
				if( ( pack_flags & LIBEWF_PACK_FLAG_FORCE_COMPRESSION ) != 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
					 LIBCERROR_COMPRESSION_ERROR_COMPRESS_FAILED,
					 "%s: unable to compress chunk data - compression was forced but compressed data is too small.",
					 function );

					return( -1 );
				}
			}
		}
		if( ( ( pack_flags & LIBEWF_PACK_FLAG_FORCE_COMPRESSION ) != 0 )
		 || ( safe_compressed_data_size < chunk_data_size ) )
		{
			if( safe_compressed_data_size < 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: compressed data size value out of bounds.",
				 function );

				return( -1 );
			}
			*range_flags = LIBEWF_RANGE_FLAG_IS_COMPRESSED;

			if( ( pack_flags & LIBEWF_PACK_FLAG_USE_PATTERN_FILL_COMPRESSION ) != 0 )
			{
				*range_flags |= LIBEWF_RANGE_FLAG_USES_PATTERN_FILL;
			}
			else
			{
				if( compression_method == LIBEWF_COMPRESSION_METHOD_DEFLATE )
				{
					/* Deflate has its own checksum
					 */
					byte_stream_copy_to_uint32_little_endian(
					 &( compressed_data[ safe_compressed_data_size - 4 ] ),
					 *chunk_checksum );
				}
/* TODO bzip2 support */
#ifdef IGNORE
				else if( compression_method == LIBEWF_COMPRESSION_METHOD_BZIP2 )
				{
					/* Some parts of the bzip2 compressed block are not stored
					 */
					*compressed_data_offset    = 4;
					safe_compressed_data_size -= 4;
				}
#endif
				if( ( pack_flags & LIBEWF_PACK_FLAG_ADD_ALIGNMENT_PADDING ) != 0 )
				{
					*chunk_padding_size = safe_compressed_data_size % 16;

					if( *chunk_padding_size != 0 )
					{
						*chunk_padding_size = 16 - *chunk_padding_size;
					}
					if( ( safe_compressed_data_size + *chunk_padding_size ) > *compressed_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
						 "%s: invalid compressed data size value too small.",
						 function );

						return( -1 );
					}
					if( memory_set(
					     &( compressed_data[ safe_compressed_data_size ] ),
					     0,
					     *chunk_padding_size ) == NULL )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_MEMORY,
						 LIBCERROR_MEMORY_ERROR_SET_FAILED,
						 "%s: unable to clear alignment padding.",
						 function );

						return( -1 );
					}
				}
			}
			*compressed_data_size = safe_compressed_data_size;
		}
	}
	if( ( ( *range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) == 0 )
	 && ( ( pack_flags & LIBEWF_PACK_FLAG_CALCULATE_CHECKSUM ) != 0 ) )
	{
		if( libewf_checksum_calculate_adler32(
		     chunk_checksum,
		     data,
		     chunk_data_size,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate checksum.",
			 function );

			return( -1 );
		}
		if( ( chunk_data_size + 4 ) <= data_size )
		{
			byte_stream_copy_from_uint32_little_endian(
			 &( data[ chunk_data_size ] ),
			 *chunk_checksum );

			chunk_data_size += 4;
		}
		else
		{
			*chunk_io_flags = LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET;
		}
		*range_flags = LIBEWF_RANGE_FLAG_HAS_CHECKSUM;

		if( ( pack_flags & LIBEWF_PACK_FLAG_ADD_ALIGNMENT_PADDING ) != 0 )
		{
			*chunk_padding_size = chunk_data_size % 16;

			if( *chunk_padding_size != 0 )
			{
				*chunk_padding_size = 16 - *chunk_padding_size;
			}
			if( ( chunk_data_size + *chunk_padding_size ) > data_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
				 "%s: invalid data size value too small.",
				 function );

				return( -1 );
			}
			if( memory_set(
			     &( data[ chunk_data_size ] ),
			     0,
			     *chunk_padding_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear alignment padding.",
				 function );

				return( -1 );
			}
		}
		*compressed_data_size = chunk_data_size;
	}
	return( 1 );
}

/* Unpacks the chunk data
 * This function either validates the checksum or decompresses the chunk data
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_unpack(
     libewf_chunk_data_t *chunk_data,
     uint32_t chunk_size,
     uint16_t compression_method,
     libcerror_error_t **error )
{
	static char *function = "libewf_chunk_data_unpack";

	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid chunk data - missing data.",
		 function );

		return( -1 );
	}
#if SIZEOF_SIZE_T <= 4
	if( (size_t) chunk_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: chunk size value out of bounds.",
		 function );

		return( -1 );
	}
#endif
	if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_PACKED ) != 0 )
	{
		if( ( chunk_data->range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
		{
			if( chunk_data->compressed_data != NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
				 "%s: invalid chunk data - compressed data value already set.",
				 function );

				return( -1 );
			}
			chunk_data->compressed_data      = chunk_data->data;
			chunk_data->compressed_data_size = chunk_data->data_size;

			/* Reserve 4 bytes for the checksum
			 */
			chunk_data->allocated_data_size = (size_t) ( chunk_size + 4 );

			/* The allocated data size should be rounded to the next 16-byte increment
			 */
			if( ( chunk_data->allocated_data_size % 16 ) != 0 )
			{
				chunk_data->allocated_data_size += 16;
			}
			chunk_data->allocated_data_size = ( chunk_data->allocated_data_size / 16 ) * 16;

			chunk_data->data = (uint8_t *) memory_allocate(
			                                sizeof( uint8_t ) * chunk_data->allocated_data_size );

			if( chunk_data->data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create data.",
				 function );

				return( -1 );
			}
			chunk_data->data_size = (size_t) chunk_size;
		}
		if( libewf_chunk_data_unpack_buffer(
		     chunk_data->data,
		     &( chunk_data->data_size ),
		     chunk_data->compressed_data,
		     chunk_data->compressed_data_size,
		     chunk_size,
		     compression_method,
		     chunk_data->range_flags,
		     0,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to unpack chunk buffer.",
			 function );

#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				if( ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
			}
#endif
			libcerror_error_free(
			 error );

			chunk_data->range_flags |= LIBEWF_RANGE_FLAG_IS_CORRUPTED;
		}
		chunk_data->range_flags &= ~( LIBEWF_RANGE_FLAG_IS_PACKED );
	}
	return( 1 );
}

/* Unpacks a buffer containing the chunk data
 * This function either validates the checksum or decompresses the chunk data
 * Returns 1 if successful or -1 on error
 */
int libewf_chunk_data_unpack_buffer(
     uint8_t *data,
     size_t *data_size,
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint32_t chunk_size,
     uint16_t compression_method,
     uint32_t range_flags,
     uint32_t chunk_checksum,
     int8_t chunk_io_flags,
     libcerror_error_t **error )
{
	static char *function        = "libewf_chunk_data_unpack_buffer";
	uint32_t calculated_checksum = 0;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	if( *data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( chunk_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( range_flags & LIBEWF_RANGE_FLAG_IS_COMPRESSED ) != 0 )
	{
		if( compressed_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid compressed data.",
			 function );

			return( -1 );
		}
		if( compressed_data == data )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: compressed data points to the same buffer as data.",
			 function );

			return( -1 );
		}
		if( compressed_data_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid compressed data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		if( ( range_flags & LIBEWF_RANGE_FLAG_USES_PATTERN_FILL ) != 0 )
		{
			if( *data_size < (size_t) chunk_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid data size value out of bounds.",
				 function );

				return( -1 );
			}
			if( compressed_data_size < (size_t) 8 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid compressed data size value out of bounds.",
				 function );

				return( -1 );
			}
			*data_size = (size_t) chunk_size;

			switch( chunk_size % 8 )
			{
				case 7:
					data[ --chunk_size ] = compressed_data[ 7 ];
				case 6:
					data[ --chunk_size ] = compressed_data[ 6 ];
				case 5:
					data[ --chunk_size ] = compressed_data[ 5 ];
				case 4:
					data[ --chunk_size ] = compressed_data[ 4 ];
				case 3:
					data[ --chunk_size ] = compressed_data[ 3 ];
				case 2:
					data[ --chunk_size ] = compressed_data[ 2 ];
				case 1:
					data[ --chunk_size ] = compressed_data[ 1 ];
			}
			while( chunk_size > 0 )
			{
/* TODO make this memory aligned ? */
				data[ --chunk_size ] = compressed_data[ 7 ];
				data[ --chunk_size ] = compressed_data[ 6 ];
				data[ --chunk_size ] = compressed_data[ 5 ];
				data[ --chunk_size ] = compressed_data[ 4 ];
				data[ --chunk_size ] = compressed_data[ 3 ];
				data[ --chunk_size ] = compressed_data[ 2 ];
				data[ --chunk_size ] = compressed_data[ 1 ];
				data[ --chunk_size ] = compressed_data[ 0 ];
			}
		}
		else
		{
			if( libewf_decompress_data(
			     compressed_data,
			     compressed_data_size,
			     compression_method,
			     data,
			     data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
				 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
				 "%s: unable to decompress chunk data.",
				 function );

				return( -1 );
			}
		}
	}
	else if( ( range_flags & LIBEWF_RANGE_FLAG_HAS_CHECKSUM ) != 0 )
	{
		if( *data_size < 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: chunk data size value out of bounds.",
			 function );

			return( -1 );
		}
		*data_size -= 4;

		if( ( chunk_io_flags & LIBEWF_CHUNK_IO_FLAG_CHECKSUM_SET ) == 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( data[ *data_size ] ),
			 chunk_checksum );
		}
		if( libewf_checksum_calculate_adler32(
		     &calculated_checksum,
		     data,
		     *data_size,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to calculate checksum.",
			 function );

			return( -1 );
		}
		if( chunk_checksum != calculated_checksum )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: chunk data checksum does not match (stored: 0x%08" PRIx32 ", calculated: 0x%08" PRIx32 ").",
			 function,
			 chunk_checksum,
			 calculated_checksum );

			return( -1 );
		}
	}
	return( 1 );
}

/* Checks if a buffer containing the chunk data is filled with same value bytes (empty-block)
 * Returns 1 if a pattern was found, 0 if not or -1 on error
 */
int libewf_chunk_data_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libewf_aligned_t *aligned_data_index = NULL;
	libewf_aligned_t *aligned_data_start = NULL;
	uint8_t *data_index                  = NULL;
	uint8_t *data_start                  = NULL;
	static char *function                = "libewf_chunk_data_check_for_empty_block";

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	data_start = (uint8_t *) data;
	data_index = (uint8_t *) data + 1;
	data_size -= 1;

	/* Only optimize for data larger than the alignment
	 */
	if( data_size > ( 2 * sizeof( libewf_aligned_t ) ) )
	{
		/* Align the data start
		 */
		while( ( (intptr_t) data_start % sizeof( libewf_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_start += 1;
			data_index += 1;
			data_size  -= 1;
		}
		/* Align the data index
		 */
		while( ( (intptr_t) data_index % sizeof( libewf_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_index += 1;
			data_size  -= 1;
		}
		aligned_data_start = (libewf_aligned_t *) data_start;
		aligned_data_index = (libewf_aligned_t *) data_index;

		while( data_size > sizeof( libewf_aligned_t ) )
		{
			if( *aligned_data_start != *aligned_data_index )
			{
				return( 0 );
			}
			aligned_data_index += 1;
			data_size          -= sizeof( libewf_aligned_t );
		}
		data_index = (uint8_t *) aligned_data_index;
	}
	while( data_size != 0 )
	{
		if( *data_start != *data_index )
		{
			return( 0 );
		}
		data_index += 1;
		data_size  -= 1;
	}
	return( 1 );
}

/* Checks if a buffer containing the chunk data is filled with a 64-bit pattern
 * Returns 1 if a pattern was found, 0 if not or -1 on error
 */
int libewf_chunk_data_check_for_64_bit_pattern_fill(
     const uint8_t *data,
     size_t data_size,
     uint64_t *pattern,
     libcerror_error_t **error )
{
	libewf_aligned_t *aligned_data_index = NULL;
	libewf_aligned_t *aligned_data_start = NULL;
	uint8_t *data_index                  = NULL;
	uint8_t *data_start                  = NULL;
	static char *function                = "libewf_chunk_data_check_for_64_bit_pattern_fill";

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( pattern == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pattern.",
		 function );

		return( -1 );
	}
	if( ( data_size % 8 ) != 0 )
	{
		return( 0 );
	}
	data_start = (uint8_t *) data;
	data_index = (uint8_t *) data + 8;
	data_size -= 8;

	/* Only optimize for data larger than the alignment
	 */
	if( data_size > ( 2 * sizeof( libewf_aligned_t ) ) )
	{
		/* Align the data start
		 */
		while( ( (intptr_t) data_start % sizeof( libewf_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_start += 1;
			data_index += 1;
			data_size  -= 1;
		}
		/* Align the data index
		 */
		while( ( (intptr_t) data_index % sizeof( libewf_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_index += 1;
			data_size  -= 1;
		}
		aligned_data_start = (libewf_aligned_t *) data_start;
		aligned_data_index = (libewf_aligned_t *) data_index;

		while( data_size > sizeof( libewf_aligned_t ) )
		{
			if( *aligned_data_start != *aligned_data_index )
			{
				return( 0 );
			}
			aligned_data_start += 1;
			aligned_data_index += 1;
			data_size          -= sizeof( libewf_aligned_t );
		}
		data_start = (uint8_t *) aligned_data_start;
		data_index = (uint8_t *) aligned_data_index;
	}
	while( data_size != 0 )
	{
		if( *data_start != *data_index )
		{
			return( 0 );
		}
		data_start += 1;
		data_index += 1;
		data_size  -= 1;
	}
	byte_stream_copy_to_uint64_little_endian(
	 data,
	 *pattern );

	return( 1 );
}

