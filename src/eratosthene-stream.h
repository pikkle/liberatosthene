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

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_STREAM_C         { 0, { NULL } }
    # define LE_STREAM_C_SIZE(s) { s, { NULL } }

    /* define stream mode */
    # define LE_STREAM_READ      ( 0 )
    # define LE_STREAM_WRITE     ( 1 )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    typedef struct le_stream_struct {

        le_size_t sr_size;
        le_file_t sr_file[_LE_USE_DEPTH];

    le_enum_t _status; } le_stream_t;

/*
    header - function prototypes
 */

    le_stream_t le_stream_create( le_char_t const * const le_root, le_time_t le_time, le_size_t const le_scfg, le_time_t const le_tcfg, le_enum_t const le_mode );

    le_void_t le_stream_delete( le_stream_t * const le_stream );

    le_file_t le_stream_get( le_stream_t const * const le_stream, le_size_t const le_index );

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

