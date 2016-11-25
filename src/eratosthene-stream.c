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

    # include "eratosthene-stream.h"

/*
    source - constructor/destructor methods
 */

    le_stream_t le_stream_create( le_char_t const * const le_root, le_time_t le_time, le_size_t const le_scfg, le_time_t const le_tcfg, le_enum_t const le_mode ) {

        /* created structure variables */
        le_stream_t le_stream = LE_STREAM_C_SIZE( le_scfg );

        /* access path string variables */
        le_char_t * le_path[_LE_USE_STRING] = { 0 };

        /* check consistency */
        if ( le_time == _LE_TIME_NULL ) {

            /* send message */
            return( le_stream._status = LE_ERROR_TIME, le_stream );

        }

        /* compute time equivalence class */
        le_time /= le_tcfg;

        /* check stream mode */
        if ( le_mode == LE_STREAM_READ ) {

            /* create stream handles */
            for ( le_size_t le_parse = 0; le_parse < le_stream.sr_size; le_parse ++ ) {

                /* compose handle path */
                sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_root, le_time, le_parse );

                /* create random access handles - r+ */
                if ( ( le_stream.sr_file[le_parse] = fopen( ( char * ) le_path, "r+" ) ) == NULL ) {

                    /* send message */
                    return( le_stream._status = LE_ERROR_IO_READ, le_stream );

                }

            }

        } else {

            /* compose directory name */
            sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P, le_root, le_time );

            /* create directory and check result */
            if ( mkdir( ( char * ) le_path, 0755 ) == 0 ) {

                /* create stream handles */
                for ( le_size_t le_parse = 0; le_parse < le_stream.sr_size; le_parse ++ ) {

                    /* compose handle path */
                    sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_root, le_time, le_parse );

                    /* create random access handle - w+ */
                    if ( ( le_stream.sr_file[le_parse] = fopen( ( char * ) le_path, "w+" ) ) == NULL ) {

                        /* send message */
                        return( le_stream._status = LE_ERROR_IO_WRITE, le_stream );
                    }

                }

            } else {

                /* create stream handles */
                for ( le_size_t le_parse = 0; le_parse < le_stream.sr_size; le_parse ++ ) {

                    /* compose handle path */
                    sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_root, le_time, le_parse );

                    /* create random access handle - r+ */
                    if ( ( le_stream.sr_file[le_parse] = fopen( ( char * ) le_path, "r+" ) ) == NULL ) {

                        /* send message */
                        return( le_stream._status = LE_ERROR_IO_READ, le_stream );

                    }

                }

            }

        }

        /* return create structure */
        return( le_stream );

    }

    le_void_t le_stream_delete( le_stream_t * const le_stream ) {

        /* deleted structure variables */
        le_stream_t le_delete = LE_STREAM_C;

        /* parsing stream handles */
        for ( le_size_t le_parse = 0; le_parse < le_stream->sr_size; le_parse ++ ) {

            /* check stream handle */
            if ( le_stream->sr_file[le_parse] != NULL ) {

                /* close handle */
                fclose( le_stream->sr_file[le_parse] );

            }

        }

        /* delete structure */
        ( * le_stream ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_file_t le_stream_get( le_stream_t const * const le_stream, le_size_t const le_index ) {

        /* return pointed stream handle */
        return( le_stream->sr_file[le_index] );

    }

