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

    le_size_t le_stream_get_reduct( le_stream_t const * const le_stream, le_address_t * const le_addr, le_size_t const le_addrt ) {

        /* returned value variables */
        le_size_t le_index = _LE_SIZE_NULL;

        /* reduction variables */
        le_diff_t le_ledge = 0;
        le_diff_t le_uedge = 0;

        /* address time variables */
        le_time_t le_time = le_address_get_time( le_addr, le_addrt ) / le_stream->sr_tcfg;

        /* intervalle boundary */
        while ( ( le_uedge < le_stream->sr_size ) && ( le_time >= le_stream->sr_strm[le_uedge].su_time ) ) {

            /* update boundary */
            le_uedge ++;

        }

        /* check reduction requirement */
        if ( le_time == le_stream->sr_strm[le_uedge].su_time ) {

            /* return index */
            return( le_uedge );

        }

        /* intervalle boundary */
        le_ledge = le_uedge - 1;

        /* reduction procedure */
        while ( ( le_ledge >= 0 ) || ( le_uedge < le_stream->sr_size ) ) {

            /* case analysis */
            if ( le_ledge >= 0 ) {

                /* case analysis */
                if ( le_uedge < le_stream->sr_size ) {

                    /* select nearest unit */
                    if ( ( le_time - le_stream->sr_strm[le_ledge].su_time ) < ( le_stream->sr_strm[le_uedge].su_time - le_time ) ) {

                        /* assign index */
                        le_index = ( le_ledge -- );

                    } else {

                        /* assign index */
                        le_index = ( le_uedge ++ );

                    }

                } else {

                    /* assign index */
                    le_index = ( le_ledge -- );

                }

            } else {

                /* assign index */
                le_index = ( le_uedge ++ );

            }

            /* check cell state */
            if ( le_stream_io_offset( le_stream, le_index, le_addr ) != _LE_OFFS_NULL ) {

                /* return index */
                return( le_index );

            }

        }

        /* return index */
        return( _LE_SIZE_NULL );

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

/*
    source - i/o methods
 */

    le_size_t le_stream_io_offset( le_stream_t const * const le_stream, le_size_t const le_unit, le_address_t const * const le_addr ) {

        /* returned value variables */
        le_size_t le_offset = 0;

        /* parsing variables */
        le_size_t le_parse = 0;

        /* class variables */
        le_class_t le_class = LE_CLASS_C;

        /* address variables */
        le_size_t le_size = le_address_get_size( le_addr );

        /* following offsets */
        while ( ( le_parse < le_size ) && ( le_offset != _LE_OFFS_NULL ) ) {

            /* read class */
            if ( le_class_io_read( & le_class, le_offset, le_stream->sr_strm[le_unit].su_file[le_parse] ) == LE_ERROR_SUCCESS ) {

                /* retrieve daughter offset */
                le_offset = le_class_get_offset( & le_class, le_address_get_digit( le_addr, le_parse ) );

            } else {

                /* return offset */
                return( _LE_OFFS_NULL );

            }

            /* update follower */
            le_parse ++;

        }

        /* return offset */
        return( le_offset );

    }

