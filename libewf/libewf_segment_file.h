/*
 * Segment file reading/writing functions
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

#if !defined( _LIBEWF_SEGMENT_FILE_H )
#define _LIBEWF_SEGMENT_FILE_H

#include <common.h>
#include <types.h>

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "libewf_chunk_data.h"
#include "libewf_chunk_table.h"
#include "libewf_hash_sections.h"
#include "libewf_io_handle.h"
#include "libewf_libbfio.h"
#include "libewf_libcdata.h"
#include "libewf_libcerror.h"
#include "libewf_libfvalue.h"
#include "libewf_libfcache.h"
#include "libewf_libmfdata.h"
#include "libewf_media_values.h"
#include "libewf_section.h"
#include "libewf_single_files.h"

#include "ewf_data.h"
#include "ewf_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const uint8_t ewf1_dvf_file_signature[ 8 ];
extern const uint8_t ewf1_evf_file_signature[ 8 ];
extern const uint8_t ewf1_lvf_file_signature[ 8 ];
extern const uint8_t ewf2_evf_file_signature[ 8 ];
extern const uint8_t ewf2_lef_file_signature[ 8 ];

typedef struct libewf_segment_file libewf_segment_file_t;

struct libewf_segment_file
{
	/* The segment file type
	 */
	uint8_t type;

	/* The major version number
	 */
	uint8_t major_version;

	/* The minor version number
	 */
	uint8_t minor_version;

	/* The segment number
	 */
	uint32_t segment_number;

	/* The set identifier
	 */
	uint8_t set_identifier[ 16 ];

	/* The compression method
	 */
	uint16_t compression_method;

	/* The last section offset
	 */
	off64_t last_section_offset;

        /* The list of all the sections
         */
        libcdata_list_t *section_list;

	/* The number of chunks
	 */
	uint64_t number_of_chunks;

	/* Flags
	 */
	uint8_t flags;
};

int libewf_segment_file_initialize(
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error );

int libewf_segment_file_free(
     libewf_segment_file_t **segment_file,
     libcerror_error_t **error );

int libewf_segment_file_clone(
     libewf_segment_file_t **destination_segment_file,
     libewf_segment_file_t *source_segment_file,
     libcerror_error_t **error );

ssize_t libewf_segment_file_read_file_header(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_file_header(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libcerror_error_t **error );

int libewf_segment_file_read(
     intptr_t *io_handle,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     libmfdata_file_t *file,
     libfcache_cache_t *cache,
     uint8_t read_flags,
     libcerror_error_t **error );

ssize_t libewf_segment_file_read_table_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libewf_chunk_table_t *chunk_table,
         libmfdata_list_t *chunk_table_list,
         libcerror_error_t **error );

ssize_t libewf_segment_file_read_table2_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_chunk_table_t *chunk_table,
         libmfdata_list_t *chunk_table_list,
         libcerror_error_t **error );

ssize_t libewf_segment_file_read_volume_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libewf_media_values_t *media_values,
         libcerror_error_t **error );

ssize_t libewf_segment_file_read_delta_chunk_section(
         libewf_segment_file_t *segment_file,
         libewf_section_t *section,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libmfdata_list_t *chunk_table_list,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_device_information_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint8_t **device_information,
         size_t *device_information_size,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_case_data_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint8_t **case_data,
         size_t *case_data_size,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_header_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         int8_t compression_level,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_header2_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_xheader_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_header_sections_t *header_sections,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_header_sections(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_last_section(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         int last_segment_file,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_start(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         uint8_t **case_data,
         size_t *case_data_size,
         uint8_t **device_information,
         size_t *device_information_size,
         ewf_data_t **data_section,
         libewf_media_values_t *media_values,
         libfvalue_table_t *header_values,
         time_t timestamp,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_chunks_section_start(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         uint8_t *table_section_data,
         size_t table_section_data_size,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_table_entries,
         uint64_t number_of_chunks_written,
         uint32_t chunks_per_section,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_chunks_section_final(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         uint8_t *table_section_data,
         size_t table_section_data_size,
         uint8_t *table_entries_data,
         size_t table_entries_data_size,
         uint32_t number_of_table_entries,
         off64_t chunks_section_offset,
         size64_t chunks_section_size,
         uint32_t chunks_section_padding_size,
         uint64_t number_of_chunks_written,
         uint32_t section_number_of_chunks,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_chunk_data(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         libmfdata_list_t *chunk_table_list,
         int chunk_index,
         libewf_chunk_data_t *chunk_data,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_delta_chunk(
         libewf_segment_file_t *segment_file,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libmfdata_list_t *chunk_table_list,
         int chunk_index,
         libewf_chunk_data_t *chunk_data,
	 uint8_t no_section_append,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_hash_sections(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libcerror_error_t **error );

ssize_t libewf_segment_file_write_close(
         libewf_segment_file_t *segment_file,
         libewf_io_handle_t *io_handle,
         libbfio_pool_t *file_io_pool,
         int file_io_pool_entry,
         off64_t section_offset,
         uint64_t number_of_chunks_written_to_segment_file,
         int last_segment_file,
         libewf_hash_sections_t *hash_sections,
         libfvalue_table_t *hash_values,
         libewf_media_values_t *media_values,
         libcdata_array_t *sessions,
         libcdata_array_t *tracks,
         libcdata_range_list_t *acquiry_errors,
         ewf_data_t **data_section,
         libcerror_error_t **error );

int libewf_segment_file_write_sections_correction(
     libewf_segment_file_t *segment_file,
     libewf_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     uint64_t number_of_chunks_written_to_segment_file,
     int last_segment_file,
     libewf_media_values_t *media_values,
     libfvalue_table_t *header_values,
     time_t timestamp,
     libfvalue_table_t *hash_values,
     libewf_hash_sections_t *hash_sections,
     libcdata_array_t *sessions,
     libcdata_array_t *tracks,
     libcdata_range_list_t *acquiry_errors,
     uint8_t **case_data,
     size_t *case_data_size,
     uint8_t **device_information,
     size_t *device_information_size,
     ewf_data_t **data_section,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

