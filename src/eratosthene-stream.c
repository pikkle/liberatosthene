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

    le_stream_t le_stream_create( le_char_t const * const le_root, le_size_t const le_scfg, le_time_t const le_tcfg ) {

        /* created structure variables */
        le_stream_t le_stream = LE_STREAM_C;

        /* entity variables */
        struct dirent * le_ent = NULL;

        /* directory variables */
        DIR * le_dir = NULL;

        /* insertion variables */
        le_diff_t le_parse = 0;

        /* time variables */
        le_time_t le_time = _LE_TIME_NULL;

        /* memory allocation variables */
        le_byte_t * le_swap = NULL;

        /* path variables */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

        /* assign stack configuration */
        le_stream.sr_scfg = le_scfg;
        le_stream.sr_tcfg = le_tcfg;

        /* assign root path */
        strcpy( ( char * ) le_stream.sr_root, ( char * ) le_root );

        /* open and check directory */
        if ( ( le_dir = opendir( ( char * ) le_root ) ) != NULL ) {

            /* entities enumeration */
            while ( ( ( le_ent = readdir( le_dir ) ) != NULL ) && ( le_stream._status == LE_ERROR_SUCCESS ) ) {

                /* check entity consistency */
                if ( ( le_ent->d_type == DT_DIR ) && ( le_ent->d_name[0] != '.' ) ) {

                    /* update stack memory */
                    if ( ( le_swap = realloc( le_stream.sr_strm, ( le_stream.sr_size + 1 ) * sizeof( le_unit_t ) ) ) == NULL ) {

                        /* send message */
                        le_stream._status = LE_ERROR_MEMORY;

                    } else {

                        /* update stack memory */
                        le_stream.sr_strm = ( le_unit_t * ) le_swap;

                        /* retriev directory time */
                        le_time = le_time_str( le_ent->d_name );

                        /* search insertion position */
                        le_parse = 0; while ( le_parse < le_stream.sr_size ) {

                            /* check position */
                            if ( le_stream.sr_strm[le_parse].su_time <= le_time ) le_parse ++; else break;

                        }

                        /* tail element shift */
                        for ( le_diff_t le_shift = le_stream.sr_size; le_shift > le_parse; le_shift -- ) {

                            /* shift element */
                            le_stream.sr_strm[le_shift] = le_stream.sr_strm[le_shift - 1];

                        }

                        /* assign inserted element time */
                        le_stream.sr_strm[le_parse].su_time = le_time;

                        /* create stream handles */
                        for ( le_size_t le_index = 0; le_index < le_stream.sr_scfg; le_index ++ ) {

                            /* compose handle path */
                            sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_stream.sr_root, le_stream.sr_strm[le_parse].su_time, le_index );

                            /* create handle */
                            if ( ( le_stream.sr_strm[le_parse].su_file[le_index] = fopen( ( char * ) le_path, "r+" ) ) == NULL ) {

                                /* send message */
                                le_stream._status = LE_ERROR_IO_READ;

                            }

                        }

                        /* update stack size */
                        le_stream.sr_size ++;

                    }

                }

            }

            /* close directory */
            closedir( le_dir );

        }

        /* DEBUG */
        //for ( le_size_t le_dg = 0; le_dg < le_stream.sr_size; le_dg ++ ) fprintf( stderr, "unit %" _LE_SIZE_P " with %" _LE_TIME_P "\n", le_dg, le_stream.sr_strm[le_dg].su_time );
        /* DEBUG */

        /* return created structure */
        return( le_stream );

    }

    le_void_t le_stream_delete( le_stream_t * const le_stream ) {

        /* deleted structure variables */
        le_stream_t le_delete = LE_STREAM_C;

        /* check unit stack */
        if ( le_stream->sr_size > 0 ) {

            /* parsing stream stack */
            for ( le_size_t le_parse = 0; le_parse < le_stream->sr_size; le_parse ++ ) {

                /* parsing stream */
                for ( le_size_t le_index = 0; le_index < le_stream->sr_scfg; le_index ++ ) {

                    /* check handle state */
                    if ( le_stream->sr_strm[le_parse].su_file[le_index] != NULL ) {

                        /* delete handle */
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

    le_size_t le_stream_get( le_stream_t * const le_stream, le_time_t le_time, le_enum_t const le_mode ) {

        /* equivalent time */
        le_time /= le_stream->sr_tcfg;

        /* parsing stream times */
        for ( le_size_t le_parse = 0; le_parse < le_stream->sr_size; le_parse ++ ) {

            /* check matching time */
            if ( le_time == le_stream->sr_strm[le_parse].su_time ) {

                /* send index */
                return( le_parse );

            }

        }

        /* returned stream index */
        return( _LE_SIZE_NULL );

    }

    le_file_t le_stream_get_handle( le_stream_t const * const le_stream, le_size_t const le_time, le_size_t const le_scale ) {

        /* return stream handle */
        return( le_stream->sr_strm[le_time].su_file[le_scale] );

    }

    le_time_t le_stream_get_reduced( le_stream_t const * const le_stream, le_time_t le_time ) {

        /* equivalent time */
        le_time /= le_stream->sr_tcfg;

        /* reduction variables */
        le_time_t le_rtime = _LE_TIME_NULL;
        le_time_t le_dtime = _LE_TIME_MAX;
        le_time_t le_stime = _LE_TIME_NULL;

        /* parsing server stack */
        for ( le_size_t le_parse = 0; le_parse < le_stream->sr_size; le_parse ++ ) {

            /* compute and check distance */
            if ( ( le_stime = le_time_abs( le_stream->sr_strm[le_parse].su_time - le_time ) ) < le_dtime ) {

                /* update distance */
                le_dtime = le_stime;

                /* update reduced time */
                le_rtime = le_stream->sr_strm[le_parse].su_time;

            }

        }

        /* return reduced time */
        return( le_rtime != _LE_TIME_NULL ? le_rtime * le_stream->sr_tcfg : le_rtime );

    }

