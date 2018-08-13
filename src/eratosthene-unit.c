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

    # include "eratosthene-unit.h"

/*
    source - constructor/destructor methods
 */

    le_unit_t le_unit_create( le_char_t * const le_root, le_size_t const le_scfg, le_time_t const le_time, le_enum_t const le_mode ) {

        /* created structure variable */
        le_unit_t le_unit = LE_UNIT_C_TIME( le_time );

        /* parsing variable */
        le_size_t le_parse = 0;

        /* unit path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* file path variable */
        le_char_t le_file[_LE_USE_PATH] = { 0 };

        /* assign storage path */
        le_unit.un_root = le_root;

        /* assign spatial configuration */
        le_unit.un_scfg = le_scfg;

        /* compose directory path */
        sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/1", le_unit.un_root, le_time );

        /* check directory state */
        if ( le_get_exist( le_path ) == _LE_FALSE ) {

            /* check unit mode */
            if ( le_mode == LE_UNIT_READ ) {

                /* send message */
                return( le_set_status( le_unit, LE_ERROR_IO_READ ) );

            } else {

                /* create directory */
                if ( mkdir( ( char * ) le_path, 0755 ) != 0 ) {

                    /* send message */
                    return( le_set_status( le_unit, LE_ERROR_IO_WRITE ) );

                }

            }

        } else {

            /* compose locker path */
            sprintf( ( char * ) le_file, "%s.lock", le_path );

            /* check unit lock */
            if ( le_get_exist( le_file ) == _LE_TRUE ) {

                /* send message */
                return( le_set_status( le_unit, LE_ERROR_IO_ACCESS ) );

            }

        }

        /* parsing unit scales */
        while ( le_parse < le_unit.un_scfg ) {

            /* compose scale stream path */
            sprintf( ( char * ) le_file, "%s/%03" _LE_SIZE_P ".bin", le_path, le_parse );

            /* create scale stream */
            if ( ( le_unit.un_pile[le_parse] = fopen( ( char * ) le_file, le_unit_mode( le_mode ) ) ) == NULL ) {

                /* interrupt stream creation */
                le_parse = le_unit.un_scfg + 1;

            /* update index */
            } else { le_parse ++; }

        }

        /* check stream creation status */
        if ( le_parse != le_unit.un_scfg ) {

            /* delete unit structure */
            le_unit_delete( & le_unit );

            /* send message */
            return( le_set_status( le_unit, LE_ERROR_IO_ACCESS ) );

        }

        /* return create structure */
        return( le_unit );

    }

    le_void_t le_unit_delete( le_unit_t * const le_unit ) {

        /* deleted structure variable */
        le_unit_t le_delete = LE_UNIT_C;

        /* parsing scale stream */
        for ( le_size_t le_parse = 0; le_parse < _LE_USE_DEPTH; le_parse ++ ) {

            /* check stream state */
            if ( le_unit->un_pile[le_parse] != NULL ) {

                /* delete stream */
                fclose( le_unit->un_pile[le_parse] );

            }

        }

        /* delete structure */
        ( * le_unit ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_file_t le_unit_get_stream( le_unit_t const * const le_unit, le_size_t const le_scale ) {

        /* return unit stream */
        return( le_unit->un_pile[le_scale] );

    }

    le_time_t le_unit_get_time( le_unit_t const * const le_unit ) {

        /* return unit time */
        return( le_unit->un_time );

    }

    le_enum_t le_unit_get_prior( le_unit_t const * const le_unit, le_unit_t const * const le_candidate ) {

        /* compare unit time */
        if ( le_candidate->un_time < le_unit->un_time ) {

            /* send result */
            return( _LE_TRUE );

        } else {

            /* send result */
            return( _LE_FALSE );

        }

    }

/*
    source - mutator methods
 */

    le_enum_t le_unit_set_lock( le_unit_t * const le_unit, le_enum_t const le_state ) {

        /* stream variable */
        le_file_t le_lock = NULL;

        /* stream path variable */
        le_char_t le_path[_LE_USE_PATH] = { 0 };

        /* compose locker path */
        sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P ".lock", le_unit->un_root, le_unit->un_time );

        /* check state value */
        if ( le_state == LE_UNIT_LOCK ) {

            /* check lock path */
            if ( le_get_exist( le_path ) == _LE_TRUE ) {

                /* send message */
                return( _LE_FALSE );

            } else {

                /* create lock stream - create file */
                le_lock = fopen( ( char * ) le_path, "w" );

                /* delete lock stream */
                fclose( le_lock );

                /* send message */
                return( _LE_TRUE );

            }

        } else {

            /* check lock path */
            if ( le_get_exist( le_path ) == _LE_FALSE ) {

                /* send message */
                return( _LE_FALSE );

            } else {

                /* remove lock */
                remove( ( char * ) le_path );

                /* send message */
                return( _LE_TRUE );

            }

        }

    }

    le_void_t le_unit_set_optimise( le_unit_t * const le_unit ) {

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
            if ( le_unit->un_pile[le_parse] != NULL ) {

                /* compose dual stream path */
                sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/1/%03" _LE_SIZE_P ".dual", le_unit->un_root, le_unit->un_time, le_parse );

                /* create dual stream */
                le_dual[le_parse] = fopen( ( char * ) le_path, le_unit_mode( LE_UNIT_WRITE ) );

                /* update index */
                le_parse ++;

            /* abort duplication */
            } else { le_parse = _LE_USE_DEPTH; }

        }

        /* optimise unit storage */
        le_unit_io_arrange( le_unit, le_dual, 0, le_head, 0 );

        /* parsing scale */
        while ( ( -- le_parse ) >= 0 ) {

            /* check stream state */
            if ( le_unit->un_pile[le_parse] != NULL ) {

                /* replace stream by dual stream */
                fclose( le_unit->un_pile[le_parse] ), le_unit->un_pile[le_parse] = le_dual[le_parse];

                /* compose dual stream path */
                sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/1/%03" _LE_SIZE_P ".dual", le_unit->un_root, le_unit->un_time, le_parse );

                /* compose stream path */
                sprintf( ( char * ) le_prev, "%s/%" _LE_TIME_P "/1/%03" _LE_SIZE_P ".bin", le_unit->un_root, le_unit->un_time, le_parse );

                /* overwrite stream with dual stream */
                rename( ( char * ) le_path, ( char * ) le_prev );

            }

        }

    }

/*
    source - i/o methods
 */

    le_void_t le_unit_io_arrange( le_unit_t * const le_unit, le_file_t const * const le_dual, le_size_t const le_offset, le_size_t * const le_head, le_size_t const le_scale ) {

        /* offset variable */
        le_size_t le_enum = _LE_OFFS_NULL;

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* read class - unit */
        le_class_io_read( & le_class, le_offset, le_unit->un_pile[le_scale] );

        /* parsing class offsets */
        for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

            /* retrieve offset */
            if ( ( le_enum = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                /* update class offset */
                le_class_set_offset( & le_class, le_digit, le_head[le_scale + 1] );

                /* recursive optimisation process */
                le_unit_io_arrange( le_unit, le_dual, le_enum, le_head, le_scale + 1 );

            }

        }

        /* write class - dual */
        le_class_io_write( & le_class, le_head[le_scale], le_dual[le_scale] );

        /* update head array */
        le_head[le_scale] += LE_CLASS_ARRAY;

    }

    le_size_t le_unit_io_offset( le_unit_t * const le_unit, le_address_t * const le_addr ) {

        /* returned value variables */
        le_size_t le_offset = 0;

        /* parsing variables */
        le_size_t le_parse = 0;

        /* address size variables */
        le_size_t le_size = le_address_get_size( le_addr );

        /* follow offsets */
        do {

            /* read class offset */
            le_offset = le_class_io_offset( le_offset, le_address_get_digit( le_addr, le_parse ), le_unit_get_stream( le_unit, le_parse ) );

        /* update index and condition */
        } while ( ( ( ++ le_parse ) < le_size ) && ( le_offset != _LE_OFFS_NULL ) );

        /* return offset */
        return( le_offset );

    }

    le_void_t le_unit_io_inject_beta( le_unit_t * const le_unit, le_char_t const * const le_file ) {

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* address variable */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_unit->un_scfg - 1 );

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

        /* create stream */
        le_stream = fopen( ( char * ) le_file, "rb" );

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
                if ( le_class_io_read( & le_class, le_current, le_unit_get_stream( le_unit, le_scale ) ) == LE_ERROR_SUCCESS ) {

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
                if ( ( le_forward == _LE_OFFS_NULL ) && ( le_ahead != le_unit->un_scfg ) ) {

                    /* seek next scale eof */
                    fseek( le_unit_get_stream( le_unit, le_ahead ), 0, SEEK_END );

                    /* compute and check injection offset */
                    if ( ( le_forward = ftell( le_unit_get_stream( le_unit, le_ahead ) ) ) < _LE_OFFS_NULL ) {

                        /* insert offset in class */
                        le_class_set_offset( & le_class, le_digit, le_forward );

                    }

                }

                /* class exportation */
                le_class_io_write( & le_class, le_current, le_unit_get_stream( le_unit, le_scale ) );

            /* injection process condition */
            } while ( ( ( le_current = le_forward, ++ le_ahead, ++ le_scale ) < le_unit->un_scfg ) && ( le_forward < _LE_OFFS_NULL ) );

        }        

    }

    le_void_t le_unit_io_inject_multivertex( le_unit_t * const le_unit, le_char_t const * const le_file ) {

        /* buffer variable */
        le_byte_t le_buffer[5][LE_UV3_RECORD];

        /* buffer pointer variable */
        le_real_t * le_uv3p[5];

        /* head variable */
        le_size_t le_head = 0;

        /* stack variable */
        le_size_t le_push = 0;

        /* reading variable */
        le_size_t le_read = 0;

        /* amplitude variable */
        le_real_t le_wide = 0.0;

        /* distance variable */
        le_real_t le_dist = 0.0;

        /* stream variable */
        le_file_t le_stream = NULL;

        /* stream variable */
        le_file_t le_export = NULL;

        /* path variable */
        le_char_t le_path[_LE_USE_PATH];

        /* length variable */
        le_size_t le_length = 0;

        /* pointer variable */
        le_size_t le_depth = 0;

        /* address variable */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_unit->un_scfg );

        /* assign buffer pointer */
        le_uv3p[0] = ( le_real_t * ) le_buffer[0];
        le_uv3p[1] = ( le_real_t * ) le_buffer[1];
        le_uv3p[2] = ( le_real_t * ) le_buffer[2];
        le_uv3p[3] = ( le_real_t * ) le_buffer[3];
        le_uv3p[4] = ( le_real_t * ) le_buffer[4];

        /* compose path */
        sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/2/", le_unit->un_root, le_unit->un_time );

        /* compute length */
        le_length = strlen( ( char * ) le_path );

        /* create stream */
        le_stream = fopen( ( char * ) le_file, "rb" );

        /* parsing stream */
        while ( ( le_read = fread( le_buffer[le_head], sizeof( le_byte_t ), LE_UV3_RECORD, le_stream ) ) == LE_UV3_RECORD ) {

            /* check head value */
            if ( le_head == 0 ) {

                /* push primitive type */
                le_push = le_buffer[le_head][LE_UV3_POSE];

            }

            /* check stack */
            if ( ( ++ le_head ) == le_push ) {

                /* reset center */
                le_uv3p[3][0] = 0.0;
                le_uv3p[3][1] = 0.0;
                le_uv3p[3][2] = 0.0;

                /* reset amplitude */
                le_wide = 0.0;

                /* parsing vertex */
                for ( le_size_t le_parse = 0; le_parse < le_push; le_parse ++ ) {

                    /* center accumulation */
                    le_uv3p[3][0] += le_uv3p[le_parse][0];
                    le_uv3p[3][1] += le_uv3p[le_parse][1];
                    le_uv3p[3][2] += le_uv3p[le_parse][2];

                    /* check index */
                    if ( le_parse > 0 ) {

                        /* compute distance */
                        le_dist = le_uv3p[le_parse][2] - le_uv3p[0][2];

                        /* search for maximum */
                        if ( le_dist > le_wide ) {

                            /* update amplitude */
                            le_wide = le_dist;

                        }

                    }

                }

                /* compute center */
                le_uv3p[3][0] /= ( le_real_t ) le_push;
                le_uv3p[3][1] /= ( le_real_t ) le_push;
                le_uv3p[3][2] /= ( le_real_t ) le_push;

                /* compute address */
                le_address_set_pose( & le_addr, le_uv3p[3] );

                /* push depth */
                le_depth = le_length;

                /* parsing scales */
                for ( le_size_t le_parse = 0; le_parse < le_unit->un_scfg; le_parse ++ ) {

                    /* update path */
                    //le_path[le_depth++] = '/';
                    //le_path[le_depth++] = le_address_get_digit( & le_addr, le_parse ) + 48;
                    //le_path[le_depth] = '\0';
                    le_path[le_depth++] = le_address_get_digit( & le_addr, le_parse ) + 48;
                    le_path[le_depth] = '\0';

                    /* create directory */
                    //mkdir( ( char * ) le_path, 0755 );

                    le_real_t le_condition = LE_ADDRESS_RAN_H / pow( 2.0, le_parse );

                    le_export = fopen( ( char * ) le_path, "ab" );

                    /* check condition */
                    //if ( ( le_wide > le_condition ) || ( le_parse == ( le_unit->un_scfg - 1 ) ) ) {

                        /* update path */
                        //le_path[le_depth++] = '/';
                        //le_path[le_depth++] = 'd';
                        //le_path[le_depth] = '\0';

                        //fprintf( stderr, "%s\n", le_path );

                        fwrite( le_buffer[0], sizeof( le_byte_t ), LE_UV3_RECORD, le_export );
                        fwrite( le_buffer[1], sizeof( le_byte_t ), LE_UV3_RECORD, le_export );
                        fwrite( le_buffer[2], sizeof( le_byte_t ), LE_UV3_RECORD, le_export );

                        /* update path */
                        //le_path[le_depth -= 2] = '\0';

                    if ( le_parse == ( le_unit->un_scfg - 1 ) ) {

                        le_parse = le_unit->un_scfg;

                    }

                    fclose( le_export );

                }

                /* reset head */
                le_head = 0;

            }

        }

        /* close stream */
        fclose( le_stream );

    }

    le_void_t le_unit_io_inject( le_unit_t * const le_unit, le_array_t const * const le_array ) {

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* address variable */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_unit->un_scfg - 1 );

        /* digit variable */
        le_size_t le_digit = 0;

        /* scale variable */
        le_size_t le_scale = 0;
        le_size_t le_ahead = 0;

        /* offset variable */
        le_size_t le_forward = 0;
        le_size_t le_current = 0;

        /* array size variable */
        le_size_t le_size = le_array_get_size( le_array );

        /* parsing array */
        for ( le_size_t le_index = 0; le_index < le_size; le_index += LE_ARRAY_UV3 ) {

            /* primitive filtering */
            if ( ( * le_array_mac_type( le_array, le_index ) ) != LE_UV3_POINT ) continue;

            /* compute address digits */
            le_address_set_pose( & le_addr, le_array_mac_pose( le_array, le_index ) );

            /* reset scale */
            le_scale = 0;
            le_ahead = 1;

            /* reset offset */
            le_forward = 0;
            le_current = 0;

            /* injection process */
            do {

                /* class importation */
                if ( le_class_io_read( & le_class, le_current, le_unit_get_stream( le_unit, le_scale ) ) == LE_ERROR_SUCCESS ) {

                    /* inject element in class */
                    le_class_set_push( & le_class, le_array_mac_data( le_array, le_index ) );

                } else {

                    /* initialise class with element */
                    le_class = le_class_create( le_array_mac_data( le_array, le_index ) );

                }

                /* extract address digit */
                le_digit = le_address_get_digit( & le_addr, le_scale );

                /* retrieve daughter offset */
                le_forward = le_class_get_offset( & le_class, le_digit );

                /* check daughter state */
                if ( ( le_forward == _LE_OFFS_NULL ) && ( le_ahead != le_unit->un_scfg ) ) {

                    /* seek next scale eof */
                    fseek( le_unit_get_stream( le_unit, le_ahead ), 0, SEEK_END );

                    /* compute and check injection offset */
                    if ( ( le_forward = ftell( le_unit_get_stream( le_unit, le_ahead ) ) ) < _LE_OFFS_NULL ) {

                        /* insert offset in class */
                        le_class_set_offset( & le_class, le_digit, le_forward );

                    }

                }

                /* class exportation */
                le_class_io_write( & le_class, le_current, le_unit_get_stream( le_unit, le_scale ) );

            /* injection process condition */
            } while ( ( ( le_current = le_forward, ++ le_ahead, ++ le_scale ) < le_unit->un_scfg ) && ( le_forward < _LE_OFFS_NULL ) );

        }

    }

    le_void_t le_unit_io_gather_multivertex( le_unit_t * const le_unit, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        le_char_t le_path[_LE_USE_PATH] = { 0 };

        le_size_t le_length = 0;

        le_file_t le_stream = NULL;

        le_size_t le_size = 0;
        le_size_t le_file = 0;

        sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/2/", le_unit->un_root, le_unit->un_time );

        le_length = strlen( ( char * ) le_path );

        for ( le_size_t le_index = 0; le_index < le_parse; le_index ++ ) {

            le_path[le_length + le_index] = le_address_get_digit( le_addr, le_index ) + 48;

        }

        if ( le_get_exist( le_path ) == _LE_TRUE ) {

            le_stream = fopen( ( char * ) le_path, "rb" );

            fseek( le_stream, 0, SEEK_END );

            if ( ( le_file = ftell( le_stream ) ) > 0 ) {

                le_size = le_array_get_size( le_array );

                le_array_set( le_array, le_file );

                fseek( le_stream, 0, SEEK_SET );

                fread( le_array_get_byte( le_array ) + le_size, sizeof( le_byte_t ), le_file, le_stream );

            }

            fclose( le_stream );

            if ( le_parse < le_span ) {

                for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                    le_address_set_digit( le_addr, le_parse, le_digit );

                    le_unit_io_gather_multivertex( le_unit, le_addr, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

    le_void_t le_unit_io_gather( le_unit_t * const le_unit, le_address_t * const le_addr, le_size_t le_offset, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variable */
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
            le_class_io_data( le_offset, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unit, le_parse ) );

        } else {

            /* read class */
            le_class_io_readf( & le_class, le_offset, le_unit_get_stream( le_unit, le_parse ) );

            /* enumerate daughter classes */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* extract class offset */
                if ( ( le_offset = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_unit_io_gather( le_unit, le_addr, le_offset, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

    le_void_t le_unit_io_parallel( le_unit_t * const le_unia, le_unit_t * const le_unib, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t le_offseta, le_size_t le_offsetb, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

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
                if ( le_offseta != _LE_OFFS_NULL ) {

                    /* push element data */
                    le_class_io_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );

                } else {

                    /* push element data */
                    le_class_io_data( le_offsetb, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unib, le_parse ) );

                }

            } else if ( le_mode == 4 ) {

                /* logical and operator */
                if ( ( le_offseta != _LE_OFFS_NULL ) && ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UV3 );

                    /* push element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element type */
                    ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                    /* push element data */
                    le_class_io_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );

                }

            } else if ( le_mode == 5 ) {

                /* logical xor operator */
                if ( ( le_offseta != _LE_OFFS_NULL ) && ( le_offsetb == _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UV3 );

                    /* push element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element type */
                    ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                    /* push element data */
                    le_class_io_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );

                } else if ( ( le_offseta == _LE_OFFS_NULL ) && ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UV3 );

                    /* push element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element type */
                    ( * le_array_mac_ltype( le_array ) ) = LE_UV3_POINT;

                    /* push element data */
                    le_class_io_data( le_offsetb, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unib, le_parse ) );

                }

            }

        } else {

            /* check offset */
            if ( le_offseta != _LE_OFFS_NULL ) {

                /* read class */
                le_msga = le_class_io_readf( & le_classa, le_offseta, le_unit_get_stream( le_unia, le_parse ) );

            }

            /* check offset */
            if ( le_offsetb != _LE_OFFS_NULL ) {

                /* read class */
                le_msgb = le_class_io_readf( & le_classb, le_offsetb, le_unit_get_stream( le_unib, le_parse ) );

            }

            /* enumerate daughter classes */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* check message */
                if ( le_msga == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    le_offseta = le_class_get_offset( & le_classa, le_digit );

                /* reset offset */
                } else { le_offseta = _LE_OFFS_NULL; }

                /* check message */
                if ( le_msgb == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    le_offsetb = le_class_get_offset( & le_classb, le_digit );

                /* reset offset */
                } else { le_offsetb = _LE_OFFS_NULL; }

                /* check offsets */
                if ( ( le_offseta != _LE_OFFS_NULL ) || ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_unit_io_parallel( le_unia, le_unib, le_addr, le_mode, le_offseta, le_offsetb, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

