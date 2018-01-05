/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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
        le_stream_t le_stream = LE_STREAM_I( le_path, le_scfg, le_tcfg );

        /* directory variables */
        DIR * le_enum = NULL;

        /* entity variables */
        struct dirent * le_file = NULL;

        /* time variables */
        le_time_t le_time = _LE_TIME_NULL;

        /* check consistency */
        if ( le_stream.sr_root == NULL ) {

            /* send message */
            return( le_stream._status = LE_ERROR_IO_ACCESS, le_stream );

        }

        /* open and check directory */
        if ( ( le_enum = opendir( ( char * ) le_stream.sr_root ) ) == NULL ) {

            /* send message */
            return( le_stream._status = LE_ERROR_IO_READ, le_stream );

        }

        /* entities enumeration */
        while ( ( ( le_file = readdir( le_enum ) ) != NULL ) && ( le_stream._status == LE_ERROR_SUCCESS ) ) {

            /* check consistency */
            if ( ( le_file->d_type == DT_DIR ) && ( le_file->d_name[0] != '.' ) ) {

                /* extract directory time */
                le_time = le_time_str( le_file->d_name ) * le_stream.sr_tcfg;

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

                    /* check stream state */
                    if ( le_stream->sr_strm[le_parse].su_file[le_index] != NULL ) {

                        /* delete stream */
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

    le_size_t le_stream_get_strict( le_stream_t * const le_stream, le_time_t const le_time, le_enum_t const le_mode ) {

        /* time variables */
        le_time_t le_reduce = le_time / le_stream->sr_tcfg;

        /* parsing units */
        for ( le_size_t le_parse = 0; le_parse < le_stream->sr_size; le_parse ++ ) {

            /* check matching time */
            if ( le_reduce == le_stream->sr_strm[le_parse].su_time ) {

                /* return index */
                return( le_parse );

            }

        }

        /* check access mode */
        if ( le_mode == LE_STREAM_WRITE ) {

            /* trigger unit creation - return index */
            return( le_stream_set( le_stream, le_time, le_mode ) );

        /* return index */
        } else { return( _LE_SIZE_NULL ); }

    }

    le_size_t le_stream_get_reduct( le_stream_t const * const le_stream, le_address_t * const le_addr, le_size_t const le_addrt, le_size_t * const le_option ) {

        /* returned value variables */
        le_size_t le_index = _LE_SIZE_NULL;

        /* reduction variables */
        le_size_t le_ledge = 0;
        le_size_t le_uedge = 0;

        /* offset variables */
        le_size_t le_offset = _LE_OFFS_NULL;

        /* address time variables */
        le_time_t le_time = le_address_get_time( le_addr, le_addrt );

        /* check consistency */
        if ( le_time == _LE_TIME_NULL ) {

            /* return optional offset */
            if ( le_option != NULL ) ( * le_option ) = le_offset;

            /* return index */
            return( _LE_SIZE_NULL );

        }

        /* compute reduced time */
        le_time /= le_stream->sr_tcfg;

        /* intervalle boundary - update boundary */
        while ( ( le_uedge < le_stream->sr_size ) && ( le_time >= le_stream->sr_strm[le_uedge].su_time ) ) le_uedge ++;

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
                        le_index = le_ledge --;

                    } else {

                        /* assign index */
                        le_index = le_uedge ++;

                    }

                } else {

                    /* assign index */
                    le_index = le_ledge --;

                }

            } else {

                /* assign index */
                le_index = le_uedge ++;

            }

            /* check cell state */
            if ( ( le_offset = le_stream_io_offset( le_stream, le_index, le_addr ) ) != _LE_OFFS_NULL ) {

                /* temporal distance limitation */
                if ( le_time_abs( ( le_time - le_stream->sr_strm[le_index].su_time ) * le_stream->sr_tcfg ) < 1576800000 ) {

                    /* reduce address time */
                    le_address_set_time( le_addr, le_addrt, le_stream->sr_strm[le_index].su_time * le_stream->sr_tcfg );

                    /* return optional offset */
                    if ( le_option != NULL ) ( * le_option ) = le_offset;

                    /* return index */
                    return( le_index );

                } else {

                    /* reduce address time */
                    le_address_set_time( le_addr, le_addrt, _LE_TIME_NULL );

                    /* return optional offset */
                    if ( le_option != NULL ) ( * le_option ) = _LE_OFFS_NULL;

                    /* return index */
                    return( _LE_SIZE_NULL );

                }

            }

        }

        /* reduce address time */
        le_address_set_time( le_addr, le_addrt, _LE_TIME_NULL );

        /* return optional offset */
        if ( le_option != NULL ) ( * le_option ) = _LE_OFFS_NULL;

        /* return index */
        return( _LE_SIZE_NULL );

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

    le_void_t le_stream_io_inject( le_stream_t const * const le_stream, le_size_t const le_unit, le_array_t const * const le_array ) {

        /* class variables */
        le_class_t le_class = LE_CLASS_C;

        /* address variables */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_stream->sr_scfg - 1 );

        /* depth variables */
        le_size_t le_parse = 0;
        le_size_t le_panex = 0;

        /* offset variables */
        le_size_t le_offset = 0;
        le_size_t le_offnex = 0;

        /* array size variables */
        le_size_t le_size = le_array_get_size( le_array );

        /* address size variables */
        le_size_t le_span = le_address_get_size( & le_addr ) + 1;

        /* check consistency - abort injection */
        if ( le_size == 0 ) return;

        /* parsing array */
        for ( le_size_t le_index = 0; le_index < le_size; le_index += LE_ARRAY_UF3 ) {

            /* reset address digits */
            le_address_set_pose( & le_addr, le_array_sd_pose_a( le_array, le_index ) );

            /* reset depth variables */
            le_parse = 0;
            le_panex = 1;

            /* reset offset variables */
            le_offset = 0;
            le_offnex = 0;

            /* injection process */
            do {

                /* class importation */
                if ( le_class_io_read( & le_class, le_offnex, le_stream->sr_strm[le_unit].su_file[le_parse] ) == LE_ERROR_SUCCESS ) {

                    /* inject element in class */
                    le_class_set_push( & le_class, le_array_sd_data_a( le_array, le_index ) );

                } else {

                    /* initialise class with element */
                    le_class = le_class_create( le_array_sd_data_a( le_array, le_index ) );

                }

                /* retrieve daughter offset */
                le_offset = le_class_get_offset( & le_class, le_address_get_digit( & le_addr, le_parse ) );

                /* check daughter state */
                if ( ( le_offset == _LE_OFFS_NULL ) && ( ( le_panex ) != le_span ) ) {

                    /* seek next scale eof */
                    fseek( le_stream->sr_strm[le_unit].su_file[le_panex], 0, SEEK_END );

                    /* assign eof offset */
                    le_offset = ftell( le_stream->sr_strm[le_unit].su_file[le_panex] );

                    /* insert offset in class */
                    le_class_set_offset( & le_class, le_address_get_digit( & le_addr, le_parse ), le_offset );

                }

                /* class exportation */
                le_class_io_write( & le_class, le_offnex, le_stream->sr_strm[le_unit].su_file[le_parse] );

            /* injection process condition */
            } while ( ( le_offnex = le_offset, ++ le_panex, ++ le_parse ) < le_span );

        }

    }

    le_size_t le_stream_io_offset( le_stream_t const * const le_stream, le_size_t const le_unit, le_address_t const * const le_addr ) {

        /* returned value variables */
        le_size_t le_offset = 0;

        /* parsing variables */
        le_size_t le_parse = 0;

        /* class variables */
        le_class_t le_class = LE_CLASS_C;

        /* address size variables */
        le_size_t le_size = le_address_get_size( le_addr );

        /* following offsets */
        while ( ( le_parse < le_size ) && ( le_offset != _LE_OFFS_NULL ) ) {

            /* read class */
            if ( le_class_io_read( & le_class, le_offset, le_stream->sr_strm[le_unit].su_file[le_parse] ) == LE_ERROR_SUCCESS ) {

                /* retrieve daughter offset */
                le_offset = le_class_get_offset( & le_class, le_address_get_digit( le_addr, le_parse ) );

                /* update follower */
                le_parse ++;

            } else {

                /* return offset */
                return( _LE_OFFS_NULL );

            }

        }

        /* return offset */
        return( le_offset );

    }

    le_void_t le_stream_io_gather( le_stream_t const * const le_stream, le_size_t const le_unit, le_address_t * const le_addr, le_size_t le_offset, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variables */
        le_class_t le_class = LE_CLASS_C;

        /* read class */
        if ( le_class_io_read( & le_class, le_offset, le_stream->sr_strm[le_unit].su_file[le_parse] ) == LE_ERROR_SUCCESS ) {

            /* check query span */
            if ( le_parse == le_span ) {

                /* pepare array for insertion */
                le_array_set( le_array, LE_ARRAY_UF3 );

                /* insert position coordinates */
                le_address_get_pose_( le_addr, le_parse, ( le_real_t * ) le_array_sd_pose_al( le_array ) );

                /* insert color components */
                le_class_get_data( & le_class, ( le_data_t * ) le_array_sd_data_al( le_array ) );

            } else {

                /* daughters classes enumeration */
                for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                    /* retreive daughter class offset */
                    if ( ( le_offset = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                        /* update address index digit */
                        le_address_set_digit( le_addr, le_parse, le_digit );

                        /* recursive query */
                        le_stream_io_gather( le_stream, le_unit, le_addr, le_offset, le_parse + 1, le_span, le_array );

                    }

                }

            }

        }

    }

    le_void_t le_stream_io_parallel( le_stream_t const * const le_stream, le_size_t const le_unia, le_size_t const le_unib, le_address_t * const le_addr, le_size_t le_offseta, le_size_t le_offsetb, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variables */
        le_class_t le_classa = LE_CLASS_C;
        le_class_t le_classb = LE_CLASS_C;

        /* status variables */
        le_enum_t le_returna = LE_ERROR_IO_READ;
        le_enum_t le_returnb = LE_ERROR_IO_READ;

        /* check offset */
        if ( le_offseta != _LE_OFFS_NULL ) {

            /* read stream class */
            le_returna = le_class_io_read( & le_classa, le_offseta, le_stream->sr_strm[le_unia].su_file[le_parse] );

        }

        /* check offset */
        if ( le_offsetb != _LE_OFFS_NULL ) {

            /* read stream class */
            le_returnb = le_class_io_read( & le_classb, le_offsetb, le_stream->sr_strm[le_unib].su_file[le_parse] );

        }

        /* check gathering */
        if ( le_parse == le_span ) {

            /* check address mode */
            if ( le_address_get_mode( le_addr ) ==  3 ) {

                /* logical or operator */
                if ( le_returna == LE_ERROR_SUCCESS ) {

                    /* pepare array for insertion */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* insert position coordinates */
                    le_address_get_pose_( le_addr, le_parse, ( le_real_t * ) le_array_sd_pose_al( le_array ) );

                    /* insert color components */
                    le_class_get_data( & le_classa, ( le_data_t * ) le_array_sd_data_al( le_array ) );

                } else {

                    /* pepare array for insertion */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* insert position coordinates */
                    le_address_get_pose_( le_addr, le_parse, ( le_real_t * ) le_array_sd_pose_al( le_array ) );

                    /* insert color components */
                    le_class_get_data( & le_classb, ( le_data_t * ) le_array_sd_data_al( le_array ) );

                }

            } else if ( le_address_get_mode( le_addr ) == 4 ) {

                /* logical and operator */
                if ( ( le_returna == LE_ERROR_SUCCESS ) && ( le_returnb == LE_ERROR_SUCCESS ) ) {

                    /* pepare array for insertion */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* insert position coordinates */
                    le_address_get_pose_( le_addr, le_parse, ( le_real_t * ) le_array_sd_pose_al( le_array ) );

                    /* insert color components */
                    le_class_get_data( & le_classa, ( le_data_t * ) le_array_sd_data_al( le_array ) );

                }

            } else {

                /* logical xor operator */
                if ( ( le_returna == LE_ERROR_SUCCESS ) && ( le_returnb != LE_ERROR_SUCCESS ) ) {

                    /* pepare array for insertion */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* insert position coordinates */
                    le_address_get_pose_( le_addr, le_parse, ( le_real_t * ) le_array_sd_pose_al( le_array ) );

                    /* insert color components */
                    le_class_get_data( & le_classa, ( le_data_t * ) le_array_sd_data_al( le_array ) );

                } else if ( ( le_returna != LE_ERROR_SUCCESS ) && ( le_returnb == LE_ERROR_SUCCESS ) ) {

                    /* pepare array for insertion */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* insert position coordinates */
                    le_address_get_pose_( le_addr, le_parse, ( le_real_t * ) le_array_sd_pose_al( le_array ) );

                    /* insert color components */
                    le_class_get_data( & le_classb, ( le_data_t * ) le_array_sd_data_al( le_array ) );

                }

            }

        } else {

            /* daughters classes enumeration */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* check status value */
                if ( le_returna == LE_ERROR_SUCCESS ) {

                    /* retreive daughter class offset */
                    le_offseta = le_class_get_offset( & le_classa, le_digit );

                /* assign null offset */
                } else { le_offseta = _LE_OFFS_NULL; }

                /* check status value */
                if ( le_returnb == LE_ERROR_SUCCESS ) {

                    /* retreive daughter class offset */
                    le_offsetb = le_class_get_offset( & le_classb, le_digit );

                /* assign null offset */
                } else { le_offsetb = _LE_OFFS_NULL; }

                /* gathering condition */
                if ( ( le_offseta != _LE_OFFS_NULL ) || ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update address index digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive gathering */
                    le_stream_io_parallel( le_stream, le_unia, le_unib, le_addr, le_offseta, le_offsetb, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

