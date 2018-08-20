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

    # include "eratosthene-door.h"

/*
    source - constructor/destructor methods
 */

    le_door_t le_door_create( le_char_t const * const le_root, le_size_t const le_scfg, le_time_t const le_tcfg, le_time_t const le_time, le_enum_t const le_mode ) {

        /* created structure variable */
        le_door_t le_door = LE_DOOR_C_SCT( le_scfg, le_tcfg, le_time / le_tcfg );

        /* parsing variable */
        le_size_t le_parse = 0;

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* create path */
        sprintf( ( char * ) le_door.dr_path, "%s/%" _LE_TIME_P, le_root, le_time / le_tcfg );

        /* compute path length */
        le_door.dr_plen = strlen( ( char * ) le_door.dr_path );

        /* check unit state */
        if ( le_get_exist( le_door.dr_path ) == _LE_FALSE ) {

            /* check door mode */
            if ( le_mode == LE_DOOR_READ ) {

                /* send message */
                return( le_set_status( le_door, LE_ERROR_IO_READ ) );

            } else {

                /* create directory */
                if ( mkdir( ( char * ) le_door.dr_path, 0755 ) != 0 ) {

                    /* send message */
                    return( le_set_status( le_door, LE_ERROR_IO_WRITE ) );

                }

                /* compose path */
                sprintf( ( char * ) le_path, "%s/0", le_door.dr_path );

                /* create directory */
                if ( mkdir( ( char * ) le_path, 0755 ) != 0 ) {

                    /* send message */
                    return( le_set_status( le_door, LE_ERROR_IO_WRITE ) );

                }

                /* compose path */
                sprintf( ( char * ) le_path, "%s/1", le_door.dr_path );

                /* create directory */
                if ( mkdir( ( char * ) le_path, 0755 ) != 0 ) {

                    /* send message */
                    return( le_set_status( le_door, LE_ERROR_IO_WRITE ) );

                }

                /* compose path */
                sprintf( ( char * ) le_path, "%s/2", le_door.dr_path );

                /* create directory */
                if ( mkdir( ( char * ) le_path, 0755 ) != 0 ) {

                    /* send message */
                    return( le_set_status( le_door, LE_ERROR_IO_WRITE ) );

                }

            }

        }

        // debug // remaining streams on error

        /* create monovertex stream */
        while ( ( le_parse < le_door.dr_scfg ) && ( le_get_status( le_door ) == LE_ERROR_SUCCESS ) ) {

            /* compose path */
            sprintf( ( char *) le_path, "%s/1/%02" _LE_SIZE_P, le_door.dr_path, le_parse );

            /* create stream */
            if ( ( le_door.dr_sacc[le_parse] = fopen( ( char * ) le_path, le_door_mode( le_mode ) ) ) == NULL ) {

                /* update status */
                le_door._status = LE_ERROR_IO_READ;

            /* update index */
            } else { le_parse ++; }

        }

        /* return created structure */
        return( le_door );

    }

    le_void_t le_door_delete( le_door_t * const le_door ) {

        /* deleted structure variable */
        le_door_t le_delete = LE_DOOR_C;

        /* parsing monovertex stream */
        for ( le_size_t le_parse = 0; le_parse < le_door->dr_scfg; le_parse ++ ) {

            /* check stream state */
            if ( le_door->dr_sacc[le_parse] != NULL ) {

                /* delete stream */
                fclose( le_door->dr_sacc[le_parse] );

            }

        }

        /* delete structure */
        ( * le_door ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_enum_t le_door_get_inject( le_door_t const * const le_door ) {

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* create path */
        sprintf( ( char * ) le_path, "%s/0/1", le_door->dr_path );

        /* check injection */
        if ( le_get_exist( le_path ) == _LE_TRUE ) {

            /* send message */
            return( _LE_TRUE );

        }

        /* compose path */
        sprintf( ( char * ) le_path, "%s/0/2", le_door->dr_path );

        /* check injection */
        if ( le_get_exist( le_path ) == _LE_TRUE ) {

            /* send message */
            return( _LE_TRUE );

        }

        /* send message */
        return( _LE_FALSE );

    }

    le_enum_t le_door_get_switch( le_door_t const * const le_prev, le_door_t const * const le_next, le_time_t const le_reduced ) {

        /* differential variable */
        le_time_t le_pdiff = 0;

        /* differential variable */
        le_time_t le_ndiff = 0;

        /* check link */
        if ( le_prev->dr_prev == NULL ) {

            /* send message */
            return( _LE_FALSE );

        } else {

            /* check link */
            if ( le_next->dr_next == NULL ) {

                /* send message */
                return( _LE_TRUE );

            } else {

                /* compute differential */
                le_pdiff = le_time_abs( ( ( le_door_t * ) le_prev->dr_prev )->dr_time - le_reduced );

                /* compute differential */
                le_ndiff = le_time_abs( ( ( le_door_t * ) le_next->dr_next )->dr_time - le_reduced );

                /* compare differential */
                if ( le_pdiff < le_ndiff ) {

                    /* send message */
                    return( _LE_TRUE );

                } else {

                    /* send message */
                    return( _LE_FALSE );

                }

            }

        }

    }

    le_size_t le_door_get_offset( le_door_t const * const le_door ) {

        /* return mono-vertex offset */
        return( le_door->dr_soff );

    }

    le_time_t le_door_get_interval( le_door_t const * const le_door, le_time_t const le_time ) {

        /* compute and return interval */
        return( le_time - ( le_door->dr_time * le_door->dr_tcfg ) );

    }

    le_enum_t le_door_get_equal( le_door_t const * const le_door, le_time_t const le_time ) {

        /* compare time */
        if ( le_door->dr_time == ( le_time / le_door->dr_tcfg ) ) {

            /* send message */
            return( _LE_TRUE );

        } else {

            /* send message */
            return( _LE_FALSE );

        }

    }

    le_enum_t le_door_get_anterior( le_door_t const * const le_door, le_door_t const * const le_candidate ) {

        /* compare time */
        if ( le_candidate->dr_time < le_door->dr_time ) {

            /* send message */
            return( _LE_TRUE );

        } else {

            /* send message */
            return( _LE_FALSE );

        }

    }

    le_door_t * le_door_get_prev( le_door_t const * const le_door ) {

        /* send door link */
        return( ( le_door_t * ) le_door->dr_prev );

    }

    le_door_t * le_door_get_next( le_door_t const * const le_door ) {

        /* send door link */
        return( ( le_door_t * ) le_door->dr_next );

    }

/*
    source - mutator methods
 */

    le_void_t le_door_set_insert( le_door_t * const le_door, le_door_t * const le_prev, le_door_t * const le_next ) {

        /* update link */
        le_door->dr_prev = ( le_void_t * ) le_prev;
        le_door->dr_next = ( le_void_t * ) le_next;

        /* check pointer */
        if ( le_prev != NULL ) {

            /* update link */
            le_prev->dr_next = ( le_void_t * ) le_door;

        }

        /* check pointer */
        if ( le_next != NULL ) {

            /* update link */
            le_next->dr_prev = ( le_void_t * ) le_door;

        }

    }

/*
    source - i/o methods
 */

    le_enum_t le_door_io_detect_monovertex( le_door_t * const le_door, le_address_t const * const le_addr ) {

        /* parsing variables */
        le_size_t le_parse = 0;

        /* size variables */
        le_size_t le_size = le_address_get_size( le_addr );

        /* reset offset */
        le_door->dr_soff = 0;

        /* follow offsets */
        do {

            /* read class offset */
            le_door->dr_soff = le_class_io_offset( le_door->dr_soff, le_address_get_digit( le_addr, le_parse ), * ( le_door->dr_sacc + le_parse ) );

            /* read and check class offset */
            if ( le_door->dr_soff == _LE_OFFS_NULL ) {

                /* send message */  
                return( _LE_FALSE );

            }

        /* update index and condition */
        } while ( ( ++ le_parse ) < le_size );

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_door_io_detect_multivertex( le_door_t * const le_door, le_address_t const * const le_addr ) {

        /* parsing variable */
        le_size_t le_parse = le_door->dr_plen;

        /* size variable */
        le_size_t le_size = le_address_get_size( le_addr );

        /* compose mulit-path */
        strcpy( ( char * ) le_door->dr_mpth, ( char * ) le_door->dr_path );

        /* update path */
        le_door->dr_mpth[le_parse++] = '/';
        le_door->dr_mpth[le_parse++] = '2';
        le_door->dr_mpth[le_parse++] = '/';

        /* detection path composition */
        for ( le_size_t le_index = 0; le_index < le_size; le_index ++ ) {

            /* update path */
            le_door->dr_mpth[le_parse++] = le_address_get_digit( le_addr, le_index ) + 48;

        }

        /* update path */
        le_door->dr_mpth[le_parse] = '\0';

        /* return detection result */
        return( le_get_exist( le_door->dr_mpth ) );

    }

    le_enum_t le_door_io_inject_filter( le_door_t const * const le_door, le_array_t const * const le_array ) {

        /* path variable */
        le_char_t le_vpath[_LE_USE_PATH] = { 0 };

        /* path variable */
        le_char_t le_mpath[_LE_USE_PATH] = { 0 };

        /* stream variable */
        le_file_t le_vstream = NULL;

        /* stream variable */
        le_file_t le_mstream = NULL;

        /* array pointer variable */
        le_byte_t * le_head = NULL;

        /* array pointer variable */
        le_byte_t * le_edge = NULL;

        /* message variable */
        le_enum_t le_message = _LE_TRUE;

        /* compose path */
        sprintf( ( char * ) le_vpath, "%s/0/1", le_door->dr_path );

        /* compose path */
        sprintf( ( char * ) le_mpath, "%s/0/2", le_door->dr_path );

        /* check unit state */
        if ( le_get_exist( le_vpath ) == _LE_TRUE ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* check unit state */
        if ( le_get_exist( le_mpath ) == _LE_TRUE ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* create stream */
        if ( ( le_vstream = fopen( ( char * ) le_vpath, "wb" ) ) == NULL ) {

            /* push message */
            le_message = _LE_FALSE;

        } else {

            /* create stream */
            if ( ( le_mstream = fopen( ( char * ) le_mpath, "wb" ) ) == NULL ) {

                /* send message */
                le_message = _LE_FALSE;

            } else {

                /* compute array pointer */
                le_edge = ( le_head = le_array_get_byte( le_array ) ) + le_array_get_size( le_array );

                /* parsing array */
                while ( ( le_head < le_edge ) && ( le_message == _LE_TRUE ) ) {

                    /* filter on primitive type */
                    if ( le_head[LE_UV3_POSE] == LE_UV3_POINT ) {

                        /* export primitive to stream */
                        if ( fwrite( le_head, sizeof( le_byte_t ), LE_UV3_RECORD, le_vstream ) != LE_UV3_RECORD ) {

                            /* push message */
                            le_message = _LE_FALSE;

                        }

                    } else {

                        /* export primitive to stream */
                        if ( fwrite( le_head, sizeof( le_byte_t ), LE_UV3_RECORD, le_mstream ) != LE_UV3_RECORD ) {

                            /* push message */
                            le_message = _LE_FALSE;

                        }

                    }

                    /* update head */
                    le_head += LE_UV3_RECORD;

                }

                /* delete stream */
                fclose( le_mstream );

            }

            /* delete stream */
            fclose( le_vstream );

        }

        /* send message */
        return( le_message );        

    }

    le_enum_t le_door_io_inject_monovertex( le_door_t * const le_door ) {

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* address variable */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_door->dr_scfg - 1 );

        /* digit variable */
        le_size_t le_digit = 0;

        /* scale variable */
        le_size_t le_scale = 0;
        le_size_t le_ahead = 0;

        /* offset variable */
        le_size_t le_forward = 0;
        le_size_t le_current = 0;

        /* reading variable */
        le_size_t le_read = 0;

        /* buffer variable */
        le_byte_t le_buffer[LE_UV3_RECORD];

        /* stream variable */
        le_file_t le_stream = NULL;

        /* compose path */
        sprintf( ( char * ) le_path, "%s/0/1", le_door->dr_path );

        /* create stream */
        if ( ( le_stream = fopen( ( char * ) le_path, "rb" ) ) == NULL ) {

            /* send message */
            return( LE_ERROR_IO_READ );

        }

        /* parsing stream */
        while ( ( le_read = fread( le_buffer, sizeof( le_byte_t ), LE_UV3_RECORD, le_stream ) ) == LE_UV3_RECORD ) {

            /* compute address digits */
            le_address_set_pose( & le_addr, ( le_real_t * ) le_buffer );

            /* reset scale */
            le_scale = 0;
            le_ahead = 1;

            /* reset offset */
            le_forward = 0;
            le_current = 0;

            /* injection process */
            do {

                /* class importation */
                if ( le_class_io_read( & le_class, le_current, le_door->dr_sacc[le_scale] ) == LE_ERROR_SUCCESS ) {

                    /* inject element in class */
                    le_class_set_push( & le_class, ( le_data_t * ) ( le_buffer + LE_UV3_POSE + LE_UV3_TYPE ) );

                } else {

                    /* initialise class with element */
                    le_class = le_class_create( ( le_data_t * ) ( le_buffer + LE_UV3_POSE + LE_UV3_TYPE ) );

                }

                /* extract address digit */
                le_digit = le_address_get_digit( & le_addr, le_scale );

                /* retrieve daughter offset */
                le_forward = le_class_get_offset( & le_class, le_digit );

                /* check daughter state */
                if ( ( le_forward == _LE_OFFS_NULL ) && ( le_ahead != le_door->dr_scfg ) ) {

                    /* seek next scale eof */
                    fseek( le_door->dr_sacc[le_ahead], 0, SEEK_END );

                    /* compute and check injection offset */
                    if ( ( le_forward = ftell( le_door->dr_sacc[le_ahead] ) ) < _LE_OFFS_NULL ) {

                        /* insert offset in class */
                        le_class_set_offset( & le_class, le_digit, le_forward );

                    }

                }

                /* class exportation */
                le_class_io_write( & le_class, le_current, le_door->dr_sacc[le_scale] );

            /* injection process condition */
            } while ( ( ( le_current = le_forward, ++ le_ahead, ++ le_scale ) < le_door->dr_scfg ) && ( le_forward < _LE_OFFS_NULL ) );

        }

        /* delete stream */
        fclose( le_stream );

        /* send message */
        return( LE_ERROR_SUCCESS );       

    }

    le_enum_t le_door_io_inject_multivertex( le_door_t const * const le_door ) {

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* address variable */
        le_address_t le_main = LE_ADDRESS_C_SIZE( le_door->dr_scfg );

        /* address variable */
        le_address_t le_addr[LE_UV3_TRIANGLE];

        /* buffer variable */
        le_byte_t le_buffer[LE_UV3_TRIANGLE][LE_UV3_RECORD];

        /* coordinates variable */
        le_real_t le_vector[6];

        /* buffer pointer variable */
        le_real_t * le_pose = NULL;

        /* buffer index variable */
        le_size_t le_head = 0;

        /* stack variable */
        le_size_t le_push = 0;

        /* index variable */
        le_size_t le_index = 0;

        /* length variable */
        le_size_t le_length = 0;

        /* length variable */
        le_size_t le_build = 0;

        /* digit variable */
        le_byte_t le_digit = 0;

        /* injection variable */
        le_enum_t le_flag = _LE_FALSE;

        /* stream variable */
        le_file_t le_stream = NULL;

        /* stream variable */
        le_file_t le_inject = NULL;

        /* compose path */
        sprintf( ( char * ) le_path, "%s/0/2", le_door->dr_path );

        /* create stream */
        if ( ( le_stream = fopen( ( char * ) le_path, "rb" ) ) == NULL ) {

            /* send message */
            return( LE_ERROR_IO_READ );

        }

        /* compose path */
        sprintf( ( char * ) le_path, "%s/2/", le_door->dr_path );

        /* compute path length */
        le_length = strlen( ( char * ) le_path );

        /* parsing address stack */
        for ( le_size_t le_parse = 0; le_parse < LE_UV3_TRIANGLE; le_parse ++ ) {

            /* initialise address */
            le_address_set_size( le_addr + le_parse, le_door->dr_scfg );

        }

        /* parsing stream */
        while ( fread( le_buffer[le_head], sizeof( le_byte_t ), LE_UV3_RECORD, le_stream ) == LE_UV3_RECORD ) {

            /* compute buffer pointer */
            le_pose = ( le_real_t * ) le_buffer[le_head];

            /* derivate vertex coordinates */
            le_vector[3] = le_pose[0];
            le_vector[4] = le_pose[1];
            le_vector[5] = le_pose[2];

            /* check head state */
            if ( le_head == 0 ) {

                /* update stack */
                le_push = le_buffer[le_head][LE_UV3_POSE];

                /* center computation */
                le_vector[0] = le_vector[3];
                le_vector[1] = le_vector[4];
                le_vector[2] = le_vector[5];

            } else {

                /* center computation */
                le_vector[0] += le_vector[3];
                le_vector[1] += le_vector[4];
                le_vector[2] += le_vector[5];

            }

            /* compose address */
            le_address_set_pose( le_addr + le_head, le_vector + 3 );

            /* update and check head */
            if ( ( ++ le_head ) == le_push ) {

                /* center computation */
                le_vector[0] /= le_push;
                le_vector[1] /= le_push;
                le_vector[2] /= le_push;

                /* compute address */
                le_address_set_pose( & le_main, le_vector );

                /* reset building index */
                le_build = le_length;

                /* reset injection flag */
                le_flag = _LE_FALSE;

                /* parsing scales */
                for ( le_size_t le_parse = 0; le_parse < le_door->dr_scfg; le_parse ++ ) {

                    /* update path */
                    le_path[le_build++] = ( le_digit = le_address_get_digit( & le_main, le_parse ) ) + 48;

                    /* validate path */
                    le_path[le_build  ] = '\0';

                    /* create and check injection stream */
                    if ( ( le_inject = fopen( ( char * ) le_path, "ab" ) ) != NULL ) {

                        /* injection flag state */
                        if ( ( le_flag == _LE_FALSE ) && ( ( le_parse + 1 ) < le_door->dr_scfg ) ) {

                            /* reset parser */
                            le_index = 0;

                            /* parsing address stack */
                            while ( ( le_index < le_push ) && ( le_flag == _LE_FALSE ) ) {

                                /* digit comparison */
                                if ( le_digit != le_address_get_digit( le_addr + le_index, le_parse ) ) {

                                    /* update flag */
                                    le_flag = _LE_TRUE;

                                /* update index */
                                } else { le_index ++; }

                            }

                        } else {

                            /* export primitive */
                            fwrite( le_buffer[0], sizeof( le_byte_t ), le_push * LE_UV3_RECORD, le_inject );

                        }

                        /* delete injection stream */
                        fclose( le_inject );

                    }

                }

                /* reset head */
                le_head = 0;

            }

        }

        /* delete stream */
        fclose( le_stream );

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_door_io_inject_clean( le_door_t const * const le_door ) {

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* compose path */
        sprintf( ( char * ) le_path, "%s/0/1", le_door->dr_path );

        /* check container */
        if ( le_get_exist( le_path ) == _LE_TRUE ) {

            /* remove file */
            remove( ( char * ) le_path );

        }

        /* compose path */
        sprintf( ( char * ) le_path, "%s/0/2", le_door->dr_path );

        /* check container */
        if ( le_get_exist( le_path ) == _LE_TRUE ) {

            /* remove file */
            remove( ( char * ) le_path );

        }

    }

    le_void_t le_door_io_optimise_monovertex( le_door_t * const le_door ) {

        /* string variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* string variable */
        le_char_t le_prev[_LE_USE_PATH] = { 0 };

        /* dual stream variable */
        le_file_t le_dual[_LE_USE_DEPTH] = { NULL };

        /* offset array variable */
        le_size_t le_head[_LE_USE_DEPTH] = { 0 };

        /* parsing variable */
        le_size_t le_parse = 0;

        /* parsing scales */
        while ( le_parse < _LE_USE_DEPTH ) {

            /* check stream state */
            if ( le_door->dr_sacc[le_parse] != NULL ) {

                /* compose dual stream path */
                sprintf( ( char * ) le_path, "%s/1/%02" _LE_SIZE_P ".dual", le_door->dr_path, le_parse );

                /* create dual stream */
                le_dual[le_parse] = fopen( ( char * ) le_path, le_door_mode( LE_DOOR_WRITE ) );

                /* update index */
                le_parse ++;

            /* abort duplication */
            } else { le_parse = _LE_USE_DEPTH; }

        }

        /* optimise unit storage */
        le_door_io_optimise_proc_monovertex( le_door, le_dual, 0, le_head, 0 );

        /* parsing scale */
        while ( ( -- le_parse ) >= 0 ) {

            /* check stream state */
            if ( le_door->dr_sacc[le_parse] != NULL ) {

                /* replace stream by dual stream */
                fclose( le_door->dr_sacc[le_parse] ), le_door->dr_sacc[le_parse] = le_dual[le_parse];

                /* flush stream */
                fflush( le_door->dr_sacc[le_parse] );

                /* compose dual stream path */
                sprintf( ( char * ) le_path, "%s/1/%02" _LE_SIZE_P ".dual", le_door->dr_path, le_parse );

                /* compose stream path */
                sprintf( ( char * ) le_prev, "%s/1/%02" _LE_SIZE_P, le_door->dr_path, le_parse );

                /* overwrite stream with dual stream */
                rename( ( char * ) le_path, ( char * ) le_prev );

            }

        }

    }

    le_void_t le_door_io_optimise_proc_monovertex( le_door_t * const le_door, le_file_t const * const le_dual, le_size_t const le_offset, le_size_t * const le_head, le_size_t const le_scale ) {

        /* offset variable */
        le_size_t le_enum = _LE_OFFS_NULL;

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* read class - unit */
        le_class_io_read( & le_class, le_offset, le_door->dr_sacc[le_scale] );

        /* parsing class offsets */
        for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

            /* retrieve offset */
            if ( ( le_enum = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                /* update class offset */
                le_class_set_offset( & le_class, le_digit, le_head[le_scale + 1] );

                /* recursive optimisation process */
                le_door_io_optimise_proc_monovertex( le_door, le_dual, le_enum, le_head, le_scale + 1 );

            }

        }

        /* write class - dual */
        le_class_io_write( & le_class, le_head[le_scale], le_dual[le_scale] );

        /* update head array */
        le_head[le_scale] += LE_CLASS_ARRAY;

    }

    le_void_t le_door_io_gather_monovertex( le_door_t * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variable */ // move to door structure
        le_class_t le_class = LE_CLASS_C;

        /* enumeration boundary */
        if ( le_parse == le_span ) {

            /* update array size */
            le_array_set( le_array, LE_ARRAY_UV3 );

            /* push element position */
            le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

            /* push element type */
            ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

            /* push element data */
            le_class_io_data( le_door->dr_soff, le_array_mac_ldata( le_array ), * ( le_door->dr_sacc + le_parse ) );

        } else {

            /* read class */
            le_class_io_readf( & le_class, le_door->dr_soff, * ( le_door->dr_sacc + le_parse ) );

            /* enumerate daughter classes */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* extract class offset */
                if ( ( le_door->dr_soff = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_door_io_gather_monovertex( le_door, le_addr, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

    le_void_t le_door_io_gather_multivertex( le_door_t const * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        le_char_t le_path[_LE_USE_PATH] = { 0 };

        le_size_t le_length = 0;

        le_file_t le_stream = NULL;

        le_size_t le_size = 0;
        le_size_t le_file = 0;

        sprintf( ( char * ) le_path, "%s/2/", le_door->dr_path );

        le_length = strlen( ( char * ) le_path );

        for ( le_size_t le_index = 0; le_index < le_parse; le_index ++ ) {

            le_path[le_length + le_index] = le_address_get_digit( le_addr, le_index ) + 48;

        }

        if ( le_get_exist( le_path ) == _LE_TRUE ) {

            if ( le_parse < le_span ) {

                for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                    le_address_set_digit( le_addr, le_parse, le_digit );

                    le_door_io_gather_multivertex( le_door, le_addr, le_parse + 1, le_span, le_array );

                }

            } else {

                le_stream = fopen( ( char * ) le_path, "rb" );

                fseek( le_stream, 0, SEEK_END );

                if ( ( le_file = ftell( le_stream ) ) > 0 ) {

                    le_size = le_array_get_size( le_array );

                    le_array_set( le_array, le_file );

                    fseek( le_stream, 0, SEEK_SET );

                    fread( le_array_get_byte( le_array ) + le_size, sizeof( le_byte_t ), le_file, le_stream );

                }

                fclose( le_stream );

            }

        }

    }

    le_void_t le_door_io_parallel_monovertex( le_door_t * const le_unia, le_door_t * const le_unib, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variable */
        le_class_t le_classa = LE_CLASS_C;
        le_class_t le_classb = LE_CLASS_C;

        /* message variable */
        le_enum_t le_msga = LE_ERROR_IO_READ;
        le_enum_t le_msgb = LE_ERROR_IO_READ;

        /* enumeration boundary */
        if ( le_parse == le_span ) {

            /* switch on mode */
            if ( le_mode == 3 ) {

                /* update array size */
                le_array_set( le_array, LE_ARRAY_UV3 );

                /* push element position */
                le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                /* push element type */
                ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                /* logical or operator */
                //if ( le_offseta != _LE_OFFS_NULL ) {
                if ( le_unia->dr_soff != _LE_OFFS_NULL ) {

                    /* push element data */
                    //le_class_io_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );
                    le_class_io_data( le_unia->dr_soff, le_array_mac_ldata( le_array ), * ( le_unia->dr_sacc + le_parse ) );

                } else {

                    /* push element data */
                    //le_class_io_data( le_offsetb, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unib, le_parse ) );
                    le_class_io_data( le_unib->dr_soff, le_array_mac_ldata( le_array ), * ( le_unib->dr_sacc + le_parse ) );

                }

            } else if ( le_mode == 4 ) {

                /* logical and operator */
                //if ( ( le_offseta != _LE_OFFS_NULL ) && ( le_offsetb != _LE_OFFS_NULL ) ) {
                if ( ( le_unia->dr_soff != _LE_OFFS_NULL ) && ( le_unib->dr_soff != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UV3 );

                    /* push element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element type */
                    ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                    /* push element data */
                    //le_class_io_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );
                    le_class_io_data( le_unia->dr_soff, le_array_mac_ldata( le_array ), * ( le_unia->dr_sacc + le_parse ) );

                }

            } else if ( le_mode == 5 ) {

                /* logical xor operator */
                //if ( ( le_offseta != _LE_OFFS_NULL ) && ( le_offsetb == _LE_OFFS_NULL ) ) {
                if ( ( le_unia->dr_soff != _LE_OFFS_NULL ) && ( le_unib->dr_soff == _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UV3 );

                    /* push element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element type */
                    ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                    /* push element data */
                    //le_class_io_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );
                    le_class_io_data( le_unia->dr_soff, le_array_mac_ldata( le_array ), * ( le_unia->dr_sacc + le_parse ) );

                //} else if ( ( le_offseta == _LE_OFFS_NULL ) && ( le_offsetb != _LE_OFFS_NULL ) ) {
                } else if ( ( le_unia->dr_soff == _LE_OFFS_NULL ) && ( le_unib->dr_soff != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UV3 );

                    /* push element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element type */
                    ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                    /* push element data */
                    //le_class_io_data( le_offsetb, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unib, le_parse ) );
                    le_class_io_data( le_unib->dr_soff, le_array_mac_ldata( le_array ), * ( le_unib->dr_sacc + le_parse ) );

                }

            }

        } else {

            /* check offset */
            //if ( le_offseta != _LE_OFFS_NULL ) {
            if ( le_unia->dr_soff != _LE_OFFS_NULL ) {

                /* read class */
                //le_msga = le_class_io_readf( & le_classa, le_offseta, le_unit_get_stream( le_unia, le_parse ) );
                le_msga = le_class_io_readf( & le_classa, le_unia->dr_soff, * ( le_unia->dr_sacc + le_parse ) );

            }

            /* check offset */
            //if ( le_offsetb != _LE_OFFS_NULL ) {
            if ( le_unib->dr_soff != _LE_OFFS_NULL ) {

                /* read class */
                //le_msgb = le_class_io_readf( & le_classb, le_offsetb, le_unit_get_stream( le_unib, le_parse ) );
                le_msgb = le_class_io_readf( & le_classb, le_unib->dr_soff, * ( le_unib->dr_sacc + le_parse ) );

            }

            /* enumerate daughter classes */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* check message */
                if ( le_msga == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    //le_offseta = le_class_get_offset( & le_classa, le_digit );
                    le_unia->dr_soff = le_class_get_offset( & le_classa, le_digit );

                /* reset offset */
                //} else { le_offseta = _LE_OFFS_NULL; }
                } else { le_unia->dr_soff = _LE_OFFS_NULL; }

                /* check message */
                if ( le_msgb == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    //le_offsetb = le_class_get_offset( & le_classb, le_digit );
                    le_unib->dr_soff = le_class_get_offset( & le_classb, le_digit );

                /* reset offset */
                //} else { le_offsetb = _LE_OFFS_NULL; }
                } else { le_unib->dr_soff = _LE_OFFS_NULL; }

                /* check offsets */
                //if ( ( le_offseta != _LE_OFFS_NULL ) || ( le_offsetb != _LE_OFFS_NULL ) ) {
                if ( ( le_unia->dr_soff != _LE_OFFS_NULL ) || ( le_unib->dr_soff != _LE_OFFS_NULL ) ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_door_io_parallel_monovertex( le_unia, le_unib, le_addr, le_mode, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

