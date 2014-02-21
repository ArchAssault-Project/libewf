/*
 * ISO 8859-14 codepage (Celtic) function
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

#include <common.h>
#include <types.h>

#include "libuna_codepage_iso_8859_14.h"

/* Extended ASCII to Unicode character lookup table for ISO 8859-14 codepage
 * Unknown are filled with the Unicode replacement character 0xfffd
 */
const uint16_t libuna_codepage_iso_8859_14_byte_stream_to_unicode_base_0xa0[ 96 ] = {
	0x00a0, 0x1e02, 0x1e03, 0x00a3, 0x010a, 0x010b, 0x1e0a, 0x00a7,
	0x1e80, 0x00a9, 0x1e82, 0x1e0b, 0x1ef2, 0x00ad, 0x00ae, 0x0178,
	0x1e1e, 0x1e1f, 0x0120, 0x0121, 0x1e40, 0x1e41, 0x00b6, 0x1e56,
	0x1e81, 0x1e57, 0x1e83, 0x1e60, 0x1ef3, 0x1e84, 0x1e85, 0x1e61,
	0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,
	0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
	0x0174, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x1e6a,
	0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x0176, 0x00df,
	0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,
	0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,
	0x0175, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x1e6b,
	0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x0177, 0x00ff
};

/* Unicode to ASCII character lookup table for ISO 8859-14 codepage
 * Unknown are filled with the ASCII replacement character 0x1a
 */
const uint8_t libuna_codepage_iso_8859_14_unicode_to_byte_stream_base_0x00c0[ 64 ] = {
	0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
	0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
	0x1a, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0x1a,
	0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0x1a, 0xdf,
	0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
	0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
	0x1a, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0x1a,
	0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0x1a, 0xff,
};

const uint8_t libuna_codepage_iso_8859_14_unicode_to_byte_stream_base_0x0170[ 8 ] = {
	0x1a, 0x1a, 0x1a, 0x1a, 0xd0, 0xf0, 0xde, 0xfe
};

const uint8_t libuna_codepage_iso_8859_14_unicode_to_byte_stream_base_0x1e80[ 8 ] = {
	0xa8, 0xb8, 0xaa, 0xba, 0xbd, 0xbe, 0x1a, 0x1a
};

