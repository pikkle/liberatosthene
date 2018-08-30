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

        /* compute path */
        sprintf( ( char * ) le_door.dr_path, "%s/%" _LE_TIME_P, le_root, le_time / le_tcfg );

        /* compute path length */
        le_door.dr_plen = strlen( ( char * ) le_door.dr_path );

        /* compute poly-vertex path */
        sprintf( ( char * ) le_door.dr_mpth, "%s/2", le_door.dr_path );

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

        /* create mono-vertex stream */
        while ( ( le_parse < le_door.dr_scfg ) && ( le_get_status( le_door ) == LE_ERROR_SUCCESS ) ) {

            /* compose path */
            sprintf( ( char * ) le_path, "%s/1/%02" _LE_SIZE_P, le_door.dr_path, le_parse );

            /* create stream */
            if ( ( le_door.dr_sacc[le_parse] = fopen( ( char * ) le_path, le_door_mode( le_mode ) ) ) == NULL ) {

                /* delete mono-vertex stream */
                while ( ( -- le_parse ) >= 0 ) {

                    /* close stream */
                    fclose( le_door.dr_sacc[le_parse] );

                }

                /* return created structure */
                return( le_set_status( le_door, LE_ERROR_IO_READ ) );

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

            /* delete stream */
            fclose( le_door->dr_sacc[le_parse] );

        }

        /* delete structure */
        ( * le_door ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_time_t le_door_get_reduced( le_door_t const * const le_door ) {

        /* return door reduced time */
        return( le_door->dr_time );

    }

    le_enum_t le_door_get_nearest( le_door_t const * const le_prev, le_door_t const * const le_next, le_time_t const le_reduced ) {

        /* differential variable */
        le_time_t le_pdiff = _LE_TIME_NULL;

        /* differential variable */
        le_time_t le_ndiff = _LE_TIME_NULL;

        /* check link */
        if ( le_prev->dr_prev == NULL ) {

            /* send message */
            return( LE_DOOR_NEXT );

        }

        /* check link */
        if ( le_next->dr_next == NULL ) {

            /* send message */
            return( LE_DOOR_PREV );

        }

        /* compute differential */
        le_pdiff = ( ( le_door_t * ) le_prev->dr_prev )->dr_time - le_reduced;

        /* compute differential */
        le_ndiff = ( ( le_door_t * ) le_next->dr_next )->dr_time - le_reduced;

        /* compare differential */
        if ( le_time_abs( le_pdiff ) < le_time_abs( le_ndiff ) ) {

            /* send message */
            return( LE_DOOR_PREV );

        } else {

            /* send message */
            return( LE_DOOR_NEXT );

        }

    }

    le_enum_t le_door_get_mono( le_door_t const * const le_door ) {

        /* return detection result */
        return( ( le_door->dr_soff != _LE_OFFS_NULL ) ? _LE_TRUE : _LE_FALSE );

    }

    le_enum_t le_door_get_poly( le_door_t const * const le_door ) {

        /* return detection result */
        return( ( le_door->dr_mlen != 0 ) ? _LE_TRUE : _LE_FALSE );

    }

    le_enum_t le_door_get_equal( le_door_t const * const le_door, le_time_t const le_time ) {

        /* compare time and send message */
        return( ( le_door->dr_time == ( le_time / le_door->dr_tcfg ) ) ? _LE_TRUE : _LE_FALSE );

    }

    le_enum_t le_door_get_anterior( le_door_t const * const le_door, le_door_t const * const le_candidate ) {

        /* compare time and send message */
        return( ( le_candidate->dr_time < le_door->dr_time ) ? _LE_TRUE : _LE_FALSE );

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

    le_enum_t le_door_set_state( le_door_t const * const le_door, le_enum_t const le_state ) {

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* stream variable */
        le_file_t le_stream = NULL;

        /* compose path */
        sprintf( ( char * ) le_path, "%s/lock", le_door->dr_path );

        /* check target state */
        if ( le_state == LE_DOOR_LOCK ) {

            /* check current state */
            if ( le_get_exist( le_path ) == _LE_TRUE ) {

                /* send message */
                return( _LE_FALSE );

            } else {

                /* create and check stream */
                if ( ( le_stream = fopen( ( char * ) le_path, "w" ) ) == NULL ) {

                    /* send message */
                    return( _LE_FALSE );

                } else {

                    /* delete stream */
                    fclose( le_stream );

                    /* send message */
                    return( _LE_TRUE );

                }

            }

        } else {

            /* check current state */
            if ( le_get_exist( le_path ) == _LE_FALSE ) {

                /* send message */
                return( _LE_FALSE );

            } else {

                /* update state */
                if ( remove( ( char * ) le_path ) != 0 ) {

                    /* send message */
                    return( _LE_FALSE );

                } else {

                    /* send message */
                    return( _LE_TRUE );

                }

            }

        }

    }

    le_void_t le_door_set_insert( le_door_t * const le_door, le_door_t * const le_prev, le_door_t * const le_next ) {

        /* update chain link */
        le_door->dr_prev = ( le_void_t * ) le_prev;

        /* update chain link */
        le_door->dr_next = ( le_void_t * ) le_next;

        /* check pointer */
        if ( le_prev != NULL ) {

            /* update chain link */
            le_prev->dr_next = ( le_void_t * ) le_door;

        }

        /* check pointer */
        if ( le_next != NULL ) {

            /* update chain link */
            le_next->dr_prev = ( le_void_t * ) le_door;

        }

    }

/*
    source - i/o methods
 */

    le_enum_t le_door_io_each_inject_filter( le_door_t const * const le_door, le_array_t const * const le_array ) {

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* stream variable */
        le_file_t le_mstream = NULL;

        /* stream variable */
        le_file_t le_pstream = NULL;

        /* message variable */
        le_enum_t le_message = LE_ERROR_SUCCESS;

        /* array pointer variable */
        le_byte_t * le_head = NULL;

        /* array pointer variable */
        le_byte_t * le_edge = NULL;

        /* compose path */
        sprintf( ( char * ) le_path, "%s/0/1", le_door->dr_path );

        /* create and check stream */
        if ( ( le_mstream = fopen( ( char * ) le_path, "ab" ) ) == NULL ) {

            /* push message */
            le_message = LE_ERROR_IO_WRITE;

        } else {

            /* compose path */
            sprintf( ( char * ) le_path, "%s/0/2", le_door->dr_path );

            /* create and check stream */
            if ( ( le_pstream = fopen( ( char * ) le_path, "ab" ) ) == NULL ) {

                /* push message */
                le_message = LE_ERROR_IO_WRITE;

            } else {

                /* compute array pointer */
                le_edge = ( le_head = le_array_get_byte( le_array ) ) + le_array_get_size( le_array );

                /* parsing socket-array */
                while ( ( le_head < le_edge ) && ( le_message == LE_ERROR_SUCCESS ) ) {

                    /* primitive filtering */
                    if ( * ( le_head + LE_ARRAY_DATA_POSE ) == LE_UV3_POINT ) {

                        /* export primitive */
                        if ( fwrite( le_head, sizeof( le_byte_t ), LE_ARRAY_DATA, le_mstream ) != LE_ARRAY_DATA ) {

                            /* push message */
                            le_message = LE_ERROR_IO_WRITE;

                        }

                    } else {

                        /* export primitive */
                        if ( fwrite( le_head, sizeof( le_byte_t ), LE_ARRAY_DATA, le_pstream ) != LE_ARRAY_DATA ) {

                            /* push message */
                            le_message = LE_ERROR_IO_WRITE;

                        }

                    }

                    /* update head */
                    le_head += LE_ARRAY_DATA;

                }

                /* delete stream */
                fclose( le_pstream );

            }

            /* delete stream */
            fclose( le_mstream );

        }

        /* send message */
        return( le_message );

    }

    le_enum_t le_door_io_mono_inject( le_door_t const * const le_door ) {

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* address variable */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_door->dr_scfg - 1 );

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* digit variable */
        le_size_t le_digit = 0;

        /* scale variable */
        le_size_t le_scale = 0;

        /* scale variable */
        le_size_t le_ahead = 0;

        /* offset variable */
        le_size_t le_forward = 0;

        /* offset variable */
        le_size_t le_current = 0;

        /* reading variable */
        le_size_t le_read = 0;

        /* buffer variable */
        le_byte_t * le_buffer = NULL;

        /* array pointer variable */
        le_byte_t * le_head = NULL;

        /* array pointer variable */
        le_byte_t * le_edge = NULL;

        /* stream variable */
        le_file_t le_stream = NULL;

        /* message variable */
        le_enum_t le_message = LE_ERROR_SUCCESS;

        /* allocate buffer memory */
        if ( ( le_buffer = ( le_byte_t * ) malloc( LE_UV3_CHUNK * LE_ARRAY_DATA ) ) == NULL ) {

            /* push message */
            le_message = LE_ERROR_MEMORY;

        } else {

            /* compose path */
            sprintf( ( char * ) le_path, "%s/0/1", le_door->dr_path );

            /* create and check stream */
            if ( ( le_stream = fopen( ( char * ) le_path, "rb" ) ) == NULL ) {

                /* push message */
                le_message = LE_ERROR_IO_READ;

            } else {

                /* stream reading */
                while ( ( le_read = fread( le_buffer, sizeof( le_byte_t ), LE_UV3_CHUNK * LE_ARRAY_DATA, le_stream ) ) > 0 ) {

                    /* compute buffer pointer */
                    le_edge = ( le_head = le_buffer ) + le_read;

                    /* parsing buffer */
                    while ( le_head < le_edge ) {

                        /* compute address digits */
                        le_address_set_pose( & le_addr, ( le_real_t * ) le_head );

                        /* update head */
                        le_head += LE_ARRAY_DATA;

                        /* reset scale */
                        le_scale = 0;

                        /* reset scale */
                        le_ahead = 1;

                        /* reset offset */
                        le_forward = 0;

                        /* reset offset */
                        le_current = 0;

                        /* injection */
                        do {

                            /* class importation */
                            if ( le_class_io_read( & le_class, le_current, * ( le_door->dr_sacc + le_scale ) ) == LE_ERROR_SUCCESS ) {

                                /* inject element in class */
                                le_class_set_push( & le_class, ( le_data_t * ) ( le_head + LE_ARRAY_DATA_POSE + LE_ARRAY_DATA_TYPE ) );

                            } else {

                                /* initialise class with element */
                                le_class = le_class_create( ( le_data_t * ) ( le_head + LE_ARRAY_DATA_POSE + LE_ARRAY_DATA_TYPE ) );

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
                                if ( ( le_forward = ftell( * ( le_door->dr_sacc + le_ahead ) ) ) < _LE_OFFS_NULL ) {

                                    /* insert offset in class */
                                    le_class_set_offset( & le_class, le_digit, le_forward );

                                }

                            }

                            /* class exportation */
                            le_class_io_write( & le_class, le_current, * ( le_door->dr_sacc +le_scale ) );

                        /* injection process condition */
                        } while ( ( ( le_current = le_forward, ++ le_ahead, ++ le_scale ) < le_door->dr_scfg ) && ( le_forward < _LE_OFFS_NULL ) );

                    }

                }

                /* delete stream */
                fclose( le_stream );

            }

            /* release buffer memory */
            free( le_buffer );

        }

        /* send message */
        return( le_message );

    }

    le_enum_t le_door_io_poly_inject( le_door_t * const le_door ) {

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* stream variable */
        le_file_t le_stream = NULL;

        /* stack variable */
        le_size_t le_stack = 0;

        /* reading variable */
        le_size_t le_read = 0;

        /* buffer variable */
        le_byte_t * le_cache = NULL;

        /* buffer variable */
        le_byte_t * le_buffer = NULL;

        /* buffer pointer variable */
        le_byte_t * le_head = NULL;

        /* buffer pointer variable */
        le_byte_t * le_edge = NULL;

        /* message variable */
        le_enum_t le_message = LE_ERROR_SUCCESS;

        /* compose path */
        sprintf( ( char * ) le_path, "%s/0/2", le_door->dr_path );

        /* allocate buffer memory */
        if ( ( le_cache = ( le_byte_t * ) malloc( 3 * LE_ARRAY_DATA * sizeof( le_byte_t ) ) ) == NULL ) {

            /* push message */
            le_message = LE_ERROR_MEMORY;

        } else {

            /* allocate buffer memory */
            if ( ( le_buffer = ( le_byte_t * ) malloc( LE_UV3_CHUNK * LE_ARRAY_DATA * sizeof( le_byte_t ) ) ) == NULL ) {

                /* push message */
                le_message = LE_ERROR_MEMORY;

            } else {

                /* create and check stream */
                if ( ( le_stream = fopen( ( char * ) le_path, "rb" ) ) == NULL ) {

                    /* push message */
                    le_message = LE_ERROR_IO_READ;

                } else {

                    /* stream reading */
                    while ( ( ( le_read = fread( le_buffer, sizeof( le_byte_t ), LE_UV3_CHUNK * LE_ARRAY_DATA , le_stream ) ) > 0 ) && ( le_message == LE_ERROR_SUCCESS ) ) {

                        /* compute buffer pointer */
                        le_edge = ( le_head = le_buffer ) + le_read;

                        /* parsing chunk */
                        while ( ( le_head < le_edge ) && ( le_message == LE_ERROR_SUCCESS ) ) {

                            /* cache record */
                            memcpy( le_cache + ( le_stack * LE_ARRAY_DATA ), le_head, LE_ARRAY_DATA );

                            /* check stack state */
                            if ( ( ++ le_stack ) == * ( le_head + LE_ARRAY_DATA_POSE ) ) {

                                /* *** */
                                le_message = le_door_io_poly_inject_vertex( le_door, le_cache, le_stack );

                                /* clear stack */
                                le_stack = 0;

                            }

                            /* update head */
                            le_head += LE_ARRAY_DATA;

                        }

                    }

                    /* delete stream */
                    fclose( le_stream );

                }

                /* release buffer memory */
                free( le_buffer );

            }

            /* release buffer memory */
            free( le_cache );

        }

        /* send message */
        return( le_message );

    }

    le_enum_t le_door_io_poly_inject_vertex( le_door_t * const le_door, le_byte_t const * const le_cache, le_size_t const le_vertex ) {

        /* address variable */
        le_address_t le_stack[4];

        /* buffer pointer variable */
        le_byte_t * le_head = ( le_byte_t * ) le_cache;

        /* vector variable */
        le_real_t le_push[3];

        /* vector variable */
        le_real_t le_mean[3] = { 0.0 };

        /* stream variable */
        le_file_t le_stream = NULL;

        /* digit variable */
        le_byte_t le_digit = 0;

        /* parsing variable */
        le_size_t le_path = le_door->dr_plen + 2;

        /* parsing variable */
        le_size_t le_parse = 0;

        /* parsing variable */
        le_size_t le_index = 0;

        /* flag variable */
        le_enum_t le_flag = _LE_FALSE;

        /* message variable */
        le_enum_t le_message = LE_ERROR_SUCCESS;

        /* parsing vertex stack */
        for ( le_size_t le_parse = 0; le_parse < le_vertex; le_parse ++ ) {

            /* push coordinates and accumulation */
            le_mean[0] += ( le_push[0] = ( ( le_real_t * ) le_head )[0] );
            le_mean[1] += ( le_push[1] = ( ( le_real_t * ) le_head )[1] );
            le_mean[2] += ( le_push[2] = ( ( le_real_t * ) le_head )[2] );

            /* udpate address size */
            le_address_set_size( le_stack + le_parse, le_door->dr_scfg );

            /* compute address digit */
            le_address_set_pose( le_stack + le_parse, le_push );

            /* update head */
            le_head += LE_ARRAY_DATA;

        }

        /* compute vertex centroid */
        le_mean[0] /= le_vertex;
        le_mean[1] /= le_vertex;
        le_mean[2] /= le_vertex;

        /* update address size */
        le_address_set_size( le_stack + 3, le_door->dr_scfg );

        /* compute address digit */
        le_address_set_pose( le_stack + 3, le_mean );

        /* parsing spatial scale */
        while ( ( le_parse < le_door->dr_scfg ) && ( le_message == LE_ERROR_SUCCESS ) ) {

            /* check hash condition */
            if ( ( le_parse % 4 ) == 0 ) {

                /* push path termination */
                le_door->dr_mpth[le_path] = '\0';

                /* check directory */
                if ( le_get_exist( le_door->dr_mpth ) == _LE_FALSE ) {

                    /* create directory */
                    if ( mkdir( ( char * ) le_door->dr_mpth, 0755 ) != 0 ) {

                        /* push message */
                        le_message = LE_ERROR_IO_WRITE;

                    }

                }

                /* push hash switch */
                le_door->dr_mpth[le_path++] = '/';

            }

            /* check message */
            if ( le_message == LE_ERROR_SUCCESS ) {

                /* push address digit */
                le_door->dr_mpth[le_path++] = ( le_digit = le_address_get_digit( le_stack + 3, le_parse ) ) + 48;

                /* push file suffix */
                le_door->dr_mpth[le_path++] = '_';

                /* push path termination */
                le_door->dr_mpth[le_path--] = '\0';

                /* create and check stream */
                if ( ( le_stream = fopen( ( char * ) le_door->dr_mpth, "ab" ) ) == NULL ) {

                    /* push message */
                    le_message = LE_ERROR_IO_WRITE;

                } else {

                    /* check flag and scale boundary */
                    if ( ( le_flag == _LE_FALSE ) && ( ( le_parse + 1 ) < le_door->dr_scfg ) ) {

                        /* reset parser */
                        le_index = 0;

                        /* parsing vertex */
                        while ( ( le_index < le_vertex ) && ( le_flag == _LE_FALSE ) ) {

                            /* compare digit */
                            if ( le_digit != le_address_get_digit( le_stack + le_index, le_parse ) ) {

                                /* update flag */
                                le_flag = _LE_TRUE;

                            /* update index */
                            } else { le_index ++; }

                        }

                    } else {

                        /* export cache */
                        if ( fwrite( le_cache, sizeof( le_byte_t ) * LE_ARRAY_DATA, le_vertex, le_stream ) != le_vertex ) {

                            /* push message */
                            le_message = LE_ERROR_IO_WRITE;

                        }

                    }

                    /* delete stream */
                    fclose( le_stream );

                }

            }

            /* update parser */
            le_parse ++;

        }

        /* send message */
        return( le_message );

    }

    le_enum_t le_door_io_each_inject_clean( le_door_t const * const le_door ) {

        /* path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* compose path */
        sprintf( ( char * ) le_path, "%s/0/1", le_door->dr_path );

        /* check container */
        if ( le_get_exist( le_path ) == _LE_TRUE ) {

            /* remove file */
            if ( remove( ( char * ) le_path ) != 0 ) {

                /* send message */
                return( LE_ERROR_IO_WRITE );

            }

        }

        /* compose path */
        sprintf( ( char * ) le_path, "%s/0/2", le_door->dr_path );

        /* check container */
        if ( le_get_exist( le_path ) == _LE_TRUE ) {

            /* remove file */
            if ( remove( ( char * ) le_path ) != 0 ) {

                /* send message */
                return( LE_ERROR_IO_WRITE );

            }

        }

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_door_io_mono_optimise( le_door_t * const le_door ) {

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
                sprintf( ( char * ) le_path, "%s/1/%02" _LE_SIZE_P "_", le_door->dr_path, le_parse );

                /* create dual stream */
                le_dual[le_parse] = fopen( ( char * ) le_path, le_door_mode( LE_DOOR_WRITE ) );

                /* update index */
                le_parse ++;

            /* abort duplication */
            } else { le_parse = _LE_USE_DEPTH; }

        }

        /* optimise unit storage */
        le_door_io_mono_optimise_rewrite( le_door, le_dual, 0, le_head, 0 );

        /* parsing scale */
        while ( ( -- le_parse ) >= 0 ) {

            /* check stream state */
            if ( le_door->dr_sacc[le_parse] != NULL ) {

                /* replace stream by dual stream */
                fclose( le_door->dr_sacc[le_parse] ), le_door->dr_sacc[le_parse] = le_dual[le_parse];

                /* flush stream */
                fflush( le_door->dr_sacc[le_parse] );

                /* compose dual stream path */
                sprintf( ( char * ) le_path, "%s/1/%02" _LE_SIZE_P "_", le_door->dr_path, le_parse );

                /* compose stream path */
                sprintf( ( char * ) le_prev, "%s/1/%02" _LE_SIZE_P, le_door->dr_path, le_parse );

                /* overwrite stream with dual stream */
                rename( ( char * ) le_path, ( char * ) le_prev );

            }

        }

    }

    le_void_t le_door_io_mono_optimise_rewrite( le_door_t * const le_door, le_file_t const * const le_dual, le_size_t const le_offset, le_size_t * const le_head, le_size_t const le_scale ) {

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
                le_door_io_mono_optimise_rewrite( le_door, le_dual, le_enum, le_head, le_scale + 1 );

            }

        }

        /* write class - dual */
        le_class_io_write( & le_class, le_head[le_scale], le_dual[le_scale] );

        /* update head array */
        le_head[le_scale] += LE_CLASS_ARRAY;

    }

    le_enum_t le_door_io_mono_detect( le_door_t * const le_door, le_address_t const * const le_addr ) {

        /* parsing variables */
        le_size_t le_parse = 0;

        /* size variables */
        le_size_t le_size = le_address_get_size( le_addr );

        /* reset offset */
        le_door->dr_soff = 0;

        /* follow offset */
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

    le_enum_t le_door_io_poly_detect( le_door_t * const le_door, le_address_t const * const le_addr ) {

        /* parsing variable */
        le_size_t le_parse = le_door->dr_plen + 2;

        /* size variable */
        le_size_t le_size = le_address_get_size( le_addr );

        /* path composition */
        for ( le_size_t le_index = 0; le_index < le_size; le_index ++ ) {

            /* path hash switch */
            if ( ( le_index % 4 ) == 0 ) {

                /* push hash directory */
                le_door->dr_mpth[le_parse++] = '/';

            }

            /* push digit */
            le_door->dr_mpth[le_parse++] = le_address_get_digit( le_addr, le_index ) + 48;

        }

        /* push file switch */
        le_door->dr_mpth[le_parse ++] = '_';

        /* push string termination */
        le_door->dr_mpth[le_parse] = '\0';

        /* class detection */
        if ( le_get_exist( le_door->dr_mpth ) == _LE_TRUE ) {

            /* update path length */
            le_door->dr_mlen = le_parse - 2;

            /* detection message */
            return( _LE_TRUE );

        } else {

            /* update path length */
            le_door->dr_mlen = 0;

            /* detection message */
            return( _LE_FALSE );

        }

    }

    le_void_t le_door_io_mono_gather( le_door_t * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* enumeration boundary */
        if ( le_parse == le_span ) {

            /* update array size */
            le_array_set( le_array, LE_ARRAY_DATA );

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
                    le_door_io_mono_gather( le_door, le_addr, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

    le_void_t le_door_io_poly_gather( le_door_t * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* size variable */
        le_size_t le_size = 0;

        /* size variable */
        le_size_t le_file = 0;

        /* hash variable */
        le_size_t le_hash = ( le_parse - 1 ) % 4;

        /* stream variable */
        le_file_t le_stream = NULL;

        /* check hash condition */
        if ( le_hash == 0 ) {

            /* push hash switch */
            le_door->dr_mpth[le_door->dr_mlen++] = '/';

        }

        /* push address digit */
        le_door->dr_mpth[le_door->dr_mlen++] = le_address_get_digit( le_addr, le_parse - 1 ) + 48;

        /* push file suffix */
        le_door->dr_mpth[le_door->dr_mlen++] = '_';

        /* push path termination */
        le_door->dr_mpth[le_door->dr_mlen--] = '\0';

        /* detect poly-class */
        if ( le_get_exist( le_door->dr_mpth ) == _LE_TRUE ) {

            /* enumeration boundary */
            if ( le_parse < le_span ) {

                /* enumerate daughter poly-class */
                for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_door_io_poly_gather( le_door, le_addr, le_parse + 1, le_span, le_array );

                }

            } else {

                /* create stream */
                le_stream = fopen( ( char * ) le_door->dr_mpth, "rb" );

                /* stream offset */
                fseek( le_stream, 0, SEEK_END );

                /* retrieve and check stream size */
                if ( ( le_file = ftell( le_stream ) ) > 0 ) {

                    /* push socket-array size */
                    le_size = le_array_get_size( le_array );

                    /* update socket-array size */
                    le_array_set( le_array, le_file );

                    /* stream offset */
                    fseek( le_stream, 0, SEEK_SET );

                    /* read stream content */
                    if ( fread( le_array_get_byte( le_array ) + le_size, sizeof( le_byte_t ), le_file, le_stream ) != le_file ) {

                        /* restore socket-array size */
                        le_array_set_size( le_array, le_size );

                    }

                }

                /* delete stream */
                fclose( le_stream );

            }

        }

        /* pop file suffix */
        le_door->dr_mlen --;

        /* check hash condition */
        if ( le_hash == 0 ) {

            /* pop hash swicth */
            le_door->dr_mlen --;

        }

    }

    le_void_t le_door_io_mono_parallel( le_door_t * const le_pdoor, le_door_t * const le_sdoor, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variable */
        le_class_t le_pclass = LE_CLASS_C;

        /* class variable */
        le_class_t le_sclass = LE_CLASS_C;

        /* message variable */
        le_enum_t le_pmessage = LE_ERROR_IO_READ;

        /* message variable */
        le_enum_t le_smessage = LE_ERROR_IO_READ;

        /* enumeration boundary */
        if ( le_parse == le_span ) {

            /* switch on mode */
            if ( le_mode == LE_ADDRESS_OR ) {

                /* update array size */
                le_array_set( le_array, LE_ARRAY_DATA );

                /* push element position */
                le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                /* push element type */
                ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                /* logical or operator */
                if ( le_pdoor->dr_soff != _LE_OFFS_NULL ) {

                    /* push element data */
                    le_class_io_data( le_pdoor->dr_soff, le_array_mac_ldata( le_array ), * ( le_pdoor->dr_sacc + le_parse ) );

                } else {

                    /* push element data */
                    le_class_io_data( le_sdoor->dr_soff, le_array_mac_ldata( le_array ), * ( le_sdoor->dr_sacc + le_parse ) );

                }

            } else if ( le_mode == LE_ADDRESS_AND ) {

                /* logical and operator */
                if ( ( le_pdoor->dr_soff != _LE_OFFS_NULL ) && ( le_sdoor->dr_soff != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_DATA );

                    /* push element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element type */
                    ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                    /* push element data */
                    le_class_io_data( le_pdoor->dr_soff, le_array_mac_ldata( le_array ), * ( le_pdoor->dr_sacc + le_parse ) );

                }

            } else if ( le_mode == LE_ADDRESS_XOR ) {

                /* logical xor operator */
                if ( ( le_pdoor->dr_soff != _LE_OFFS_NULL ) && ( le_sdoor->dr_soff == _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_DATA );

                    /* push element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element type */
                    ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                    /* push element data */
                    le_class_io_data( le_pdoor->dr_soff, le_array_mac_ldata( le_array ), * ( le_pdoor->dr_sacc + le_parse ) );

                } else if ( ( le_pdoor->dr_soff == _LE_OFFS_NULL ) && ( le_sdoor->dr_soff != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_DATA );

                    /* push element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element type */
                    ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                    /* push element data */
                    le_class_io_data( le_sdoor->dr_soff, le_array_mac_ldata( le_array ), * ( le_sdoor->dr_sacc + le_parse ) );

                }

            }

        } else {

            /* check offset */
            if ( le_pdoor->dr_soff != _LE_OFFS_NULL ) {

                /* read class */
                le_pmessage = le_class_io_readf( & le_pclass, le_pdoor->dr_soff, * ( le_pdoor->dr_sacc + le_parse ) );

            }

            /* check offset */
            if ( le_sdoor->dr_soff != _LE_OFFS_NULL ) {

                /* read class */
                le_smessage = le_class_io_readf( & le_sclass, le_sdoor->dr_soff, * ( le_sdoor->dr_sacc + le_parse ) );

            }

            /* enumerate daughter classes */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* check message */
                if ( le_pmessage == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    le_pdoor->dr_soff = le_class_get_offset( & le_pclass, le_digit );

                /* reset offset */
                } else { le_pdoor->dr_soff = _LE_OFFS_NULL; }

                /* check message */
                if ( le_smessage == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    le_sdoor->dr_soff = le_class_get_offset( & le_sclass, le_digit );

                /* reset offset */
                } else { le_sdoor->dr_soff = _LE_OFFS_NULL; }

                /* check offsets */
                if ( ( le_pdoor->dr_soff != _LE_OFFS_NULL ) || ( le_sdoor->dr_soff != _LE_OFFS_NULL ) ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_door_io_mono_parallel( le_pdoor, le_sdoor, le_addr, le_mode, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

