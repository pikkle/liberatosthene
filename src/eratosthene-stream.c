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

    le_stream_t le_stream_create( le_char_t * const le_path, le_size_t const le_scfg, le_time_t const le_tcfg ) {

        /* created structure variables */
        le_stream_t le_stream = LE_STREAM_C;

        /* directory variables */
        DIR * le_enum = NULL;

        /* entity variables */
        struct dirent * le_file = NULL;

        /* time variables */
        le_time_t le_time = _LE_TIME_NULL;

        /* check consistency */
        if ( le_path == NULL ) {

            /* send message */
            return( le_stream._status = LE_ERROR_IO_ACCESS, le_stream );

        }

        /* assign server configuration */
        le_stream.sr_scfg = le_scfg;
        le_stream.sr_tcfg = le_tcfg;

        /* assign server path */
        le_stream.sr_root = le_path;

        /* open and check directory */
        if ( ( le_enum = opendir( ( char * ) le_path ) ) == NULL ) {

            /* send message */
            return( le_stream._status = LE_ERROR_IO_READ, le_stream );

        }

        /* entities enumeration */
        while ( ( ( le_file = readdir( le_enum ) ) != NULL ) && ( le_stream._status == LE_ERROR_SUCCESS ) ) {

            /* check consistency */
            if ( ( le_file->d_type == DT_DIR ) && ( le_file->d_name[0] != '.' ) ) {

                /* extract directory time */
                le_time = le_time_str( le_file->d_name ) * le_tcfg;

                /* set stream unit */
                if ( le_stream_set( & le_stream, le_time, LE_STREAM_READ ) == _LE_SIZE_NULL ) {

                    /* push message */
                    le_stream._status = LE_ERROR_IO_ACCESS;

                }

            }

        }

        /* close directory */
        closedir( le_enum );

        /* return created structure */
        return( le_stream );

    }

    le_void_t le_stream_delete( le_stream_t * const le_stream ) {

        /* deleted structure variables */
        le_stream_t le_delete = LE_STREAM_C;

        /* check stack */
        if ( le_stream->sr_size > 0 ) {

            /* parsing stream units */
            for ( le_size_t le_parse = 0; le_parse < le_stream->sr_size; le_parse ++ ) {

                /* parsing unit files */
                for ( le_size_t le_index = 0; le_index < le_stream->sr_scfg; le_index ++ ) {

                    /* check state */
                    if ( le_stream->sr_strm[le_parse].su_file[le_index] != NULL ) {

                        /* delete file */
                        fclose( le_stream->sr_strm[le_parse].su_file[le_index] );

                    }

                }

            }

            /* release memory */
            free( le_stream->sr_strm );

        }

        /* delete structure */
        ( * le_stream ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_size_t le_stream_get_strict( le_stream_t * const le_stream, le_time_t le_time, le_enum_t const le_mode ) {

        /* compute equivalent time */
        le_time /= le_stream->sr_tcfg;

        /* parsing units */
        for ( le_size_t le_parse = 0; le_parse < le_stream->sr_size; le_parse ++ ) {

            /* check matching time */
            if ( le_time == le_stream->sr_strm[le_parse].su_time ) {

                /* return index */
                return( le_parse );

            }

        }

        /* check mode */
        if ( le_mode == LE_STREAM_WRITE ) {

            /* create and insert - return index */
            return( le_stream_set( le_stream, le_time * le_stream->sr_tcfg, le_mode ) );

        } else {

            /* return index */
            return( _LE_SIZE_NULL );

        }

    }

    le_size_t le_stream_get_reduct( le_stream_t const * const le_stream, le_time_t le_time ) {

        /* returned value variables */
        le_size_t le_index = _LE_SIZE_NULL;

        /* reduction variables */
        le_time_t le_dtime = _LE_TIME_MAX;
        le_time_t le_stime = _LE_TIME_NULL;

        /* compute equivalent time */
        le_time /= le_stream->sr_tcfg;

        /* parsing server stack */
        for ( le_size_t le_parse = 0; le_parse < le_stream->sr_size; le_parse ++ ) {

            /* compute and check distance */
            if ( ( le_stime = le_time_abs( le_stream->sr_strm[le_parse].su_time - le_time ) ) < le_dtime ) {

                /* update distance */
                le_dtime = le_stime;

                /* update index */
                le_index = le_parse;

            }

        }

        /* return index */
        return( le_index );

    }

    le_time_t le_stream_get_time( le_stream_t const * const le_stream, le_size_t const le_unit ) {

        /* return unit time */
        return( le_stream->sr_strm[le_unit].su_time * le_stream->sr_tcfg );

    }

    le_file_t le_stream_get_file( le_stream_t const * const le_stream, le_size_t const le_unit, le_size_t const le_file ) {

        /* return unit file */
        return( le_stream->sr_strm[le_unit].su_file[le_file] );

    }

/*
    source - mutator methods
 */

    le_size_t le_stream_set( le_stream_t * const le_stream, le_time_t le_time, le_enum_t const le_mode ) {

        /* memory variables */
        le_void_t * le_swap = NULL;

        /* insertion variables */
        le_size_t le_parse = 0;

        /* path variables */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

        /* compute equivalent time */
        le_time /= le_stream->sr_tcfg;

        /* check mode */
        if ( le_mode == LE_STREAM_WRITE ) {

            /* compose path */
            sprintf( ( char * ) le_path, LE_STREAM_TIME, le_stream->sr_root, le_time );

            /* create and check directory */
            if ( mkdir( ( char * ) le_path, 0755 ) != 0 ) {

                /* send message */
                return( _LE_SIZE_NULL );

            }

        }

        /* update stack memory */
        le_swap = realloc( le_stream->sr_strm, ( le_stream->sr_size + 1 ) * sizeof( le_unit_t ) );

        /* check consistency */
        if ( le_swap == NULL ) {

            /* send message */
            return( _LE_SIZE_NULL );

        }

        /* update stack memory */
        le_stream->sr_strm = ( le_unit_t * ) le_swap;

        /* intialise position search */
        le_parse = le_stream->sr_size;

        /* search insertion position */
        while ( ( le_parse > 0 ) && ( le_time < le_stream->sr_strm[le_parse-1].su_time ) ) {

            /* shift elements */
            le_stream->sr_strm[le_parse] = le_stream->sr_strm[le_parse-1];

            /* update index */
            le_parse --;

        }

        /* assign unit time */
        le_stream->sr_strm[le_parse].su_time = le_time;

        /* parsing unit */
        for ( le_size_t le_index = 0; le_index < le_stream->sr_scfg; le_index ++ ) {

            /* compose path */
            sprintf( ( char * ) le_path, LE_STREAM_FILE, le_stream->sr_root, le_time, le_index );

            /* create file */
            le_stream->sr_strm[le_parse].su_file[le_index] = fopen( ( char * ) le_path, le_stream_mode( le_mode ) );

            /* check consistency */
            if ( le_stream->sr_strm[le_parse].su_file[le_index] == NULL ) {

                /* send message */
                return( _LE_SIZE_NULL );

            }

        }

        /* validate stack update */
        le_stream->sr_size ++;

        /* return insertion index */
        return( le_parse );

    }

