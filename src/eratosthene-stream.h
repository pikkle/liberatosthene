/*
 *  liberatosthene - eratosthene indexation server
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016 EPFL CDH DHLAB
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

    /*! \file   eratosthene-stream.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Stream module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_STREAM__
    # define __LE_STREAM__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - internal includes
 */

    # include "eratosthene.h"
    # include "eratosthene-address.h"
    # include "eratosthene-class.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_STREAM_C          { NULL, 0, 0, 0, NULL, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    # define LE_STREAM_UNIT_C     { _LE_TIME_NULL, { NULL } }

    /* define stream mode */
    # define LE_STREAM_READ       ( 0 )
    # define LE_STREAM_WRITE      ( 1 )

    /* define path model */
    # define LE_STREAM_TIME       ( "%s/%" _LE_TIME_P )
    # define LE_STREAM_FILE       ( "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin" )

/*
    header - preprocessor macros
 */

    /* define file mode macro */
    # define le_stream_mode(m)    ( ( m ) == LE_STREAM_READ ? "r+" : "w+" )

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct le_stream_unit_struct {

        le_time_t   su_time;
        le_file_t   su_file[_LE_USE_DEPTH];

    } le_stream_unit_t, le_unit_t;

    typedef struct le_stream_struct {

        le_char_t * sr_root;

        le_size_t   sr_scfg;
        le_time_t   sr_tcfg;

        le_size_t   sr_size;
        le_unit_t * sr_strm;

    le_enum_t _status; } le_stream_t;

/*
    header - function prototypes
 */

    le_stream_t le_stream_create( le_char_t * const le_path, le_size_t const le_scfg, le_time_t const le_tcfg );

    le_void_t le_stream_delete( le_stream_t * const le_stream );

    le_size_t le_stream_get_strict( le_stream_t * const le_stream, le_time_t const le_time, le_enum_t const le_mode );

    le_size_t le_stream_get_reduct( le_stream_t const * const le_stream, le_address_t * const le_addr, le_size_t const le_addrt );

    le_time_t le_stream_get_time( le_stream_t const * const le_stream, le_size_t const le_unit );

    le_file_t le_stream_get_file( le_stream_t const * const le_stream, le_size_t const le_unit, le_size_t const le_file );

    le_size_t le_stream_set( le_stream_t * const le_stream, le_time_t le_time, le_enum_t const le_mode );

    le_size_t le_stream_io_offset( le_stream_t const * const le_stream, le_size_t const le_unit, le_address_t const * const le_addr );

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

