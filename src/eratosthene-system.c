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

    # include "eratosthene-system.h"

/*
    source - constructor/destructor methods
 */

    le_system_t le_system_create( le_char_t const * const le_root ) {

        /* Returned structure variables */
        le_system_t le_system = LE_SYSTEM_C;

        /* Stream variables */
        FILE * le_stream = NULL;

        /* Open configuration stream */
        if ( ( le_stream = fopen( strcat( strcat( ( char * ) le_system.sm_path, ( char * ) le_root ), "/system" ), "r" ) ) == NULL ) {

            /* Send message - return created structure */
            le_system._status = LE_ERROR_IO_ACCESS; return( le_system );

        }

        /* Check configuration reading */
        if ( fscanf( le_stream, "%" _LE_SIZE_S " %" _LE_TIME_S, & le_system.sm_area, & le_system.sm_time ) != 2 ) {

            /* Close configuration stream */
            fclose( le_stream );

            /* Send message - return created structure */
            le_system._status = LE_ERROR_IO_READ; return( le_system );

        }

        /* Close configuration stream */
        fclose( le_stream );

        /* Check consistency */
        if ( ( le_system.sm_area <= 0 ) || ( le_system.sm_area >= _LE_USE_DEPTH ) ) {

            /* Send message - return created structure */
            le_system._status = LE_ERROR_DEPTH; return( le_system );

        }

        /* Check consistency */
        if ( le_system.sm_time <= 0 ) {

            /* Send message - return created structure */
            le_system._status = LE_ERROR_TIME; return( le_system );

        }

        /* Assign provided root path */
        strcpy( ( char * ) le_system.sm_path, ( char * ) le_root );

        /* Return created structure */
        return( le_system );

    }

    le_system_t le_system_delete( le_system_t * const le_system ) {

        /* Returned structure variables */
        le_system_t le_return = LE_SYSTEM_C;

        /* Delete streams stack */
        le_system_io_close( le_system );

        /* Return deleted structure */
        return( le_return );

    }

/*
    source - injection methods
 */

    le_void_t le_system_inject( le_system_t * const le_system, le_array_sf_t const * const le_access ) {

        /* Address variables */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_system->sm_area - 1 );

        /* Class variables */
        le_class_t le_class = LE_CLASS_C;

        /* Depth variables */
        le_size_t le_parse = 0;
        le_size_t le_panex = 1;

        /* Offset variables */
        le_size_t le_offset = 0;
        le_size_t le_offnex = 0;

        /* System scale stream management - send message */
        if ( le_system_io_open( le_system, le_access->as_time[0] ) != LE_ERROR_SUCCESS ) return;

        /* Compute address index */
        le_address_set_pose( & le_addr, le_access->as_pose );

        /* Injection process */
        do {

            /* Class importation */
            if ( le_class_io_read( & le_class, le_offnex, le_system->sm_stream[le_parse] ) == LE_ERROR_SUCCESS ) {

                /* Inject element in class */
                le_class_set_push( & le_class, le_access->as_data );

            } else {

                /* Initialise class with element */
                le_class = le_class_create( le_access->as_data );

            }

            /* Retrieve daughter offset */
            le_offset = le_class_get_offset( & le_class, le_address_get_digit( & le_addr, le_parse ) );

            /* Check daughter state */
            if ( ( le_offset == _LE_OFFS_NULL ) && ( ( le_panex ) != le_system->sm_area ) ) {

                /* Seek next scale eof */
                fseek( le_system->sm_stream[le_panex], 0, SEEK_END );

                /* Assign eof offset */
                le_offset = ftell( le_system->sm_stream[le_panex] );

                /* Insert offset in class */
                le_class_set_offset( & le_class, le_address_get_digit( & le_addr, le_parse ), le_offset );

            }

            /* Class exportation */
            le_class_io_write( & le_class, le_offnex, le_system->sm_stream[le_parse] );

        /* Injection process condition */
        } while ( ( le_offnex = le_offset, ++ le_panex, ++ le_parse ) < le_system->sm_area );

    }

/*
    source - query methods
 */

    le_void_t le_system_query( le_system_t * const le_system, le_address_t * const le_addr, le_array_t * const le_array, le_size_t const le_parse, le_size_t le_offset, le_sock_t const le_socket ) {

        /* Class variables */
        le_class_t le_class = LE_CLASS_C;

        /* Address size variables */
        le_size_t le_size = le_address_get_size( le_addr );

        /* Query initialisation */
        if ( le_parse == 0 ) {

            /* Check consistency */
            if ( ( le_size + le_address_get_depth( le_addr ) ) >= le_system->sm_area ) return;

            /* System scale stream management */
            if ( le_system_io_open( le_system, le_address_get_time( le_addr ) ) != LE_ERROR_SUCCESS ) return;

        }

        /* Read class */
        if ( le_class_io_read( & le_class, le_offset, le_system->sm_stream[le_parse] ) == LE_ERROR_SUCCESS ) {

            /* Query process mode */
            if ( le_parse < le_size ) {

                /* Retreive daughter class offset */
                if ( ( le_offset = le_class_get_offset( & le_class, le_address_get_digit( le_addr, le_parse ) ) ) != _LE_OFFS_NULL ) {

                    /* Recursive query */
                    le_system_query( le_system, le_addr, le_array, le_parse + 1, le_offset, le_socket );

                }

            } else if ( le_parse == le_size + le_address_get_depth( le_addr ) ) {

                /* Position array variables */
                le_real_t le_pose[3] = { 0.0 };

                /* Correct address size */
                le_address_set_size( le_addr, le_parse );

                /* Retreive class representative */
                le_address_get_pose( le_addr, le_pose );

                /* Restore address size */
                le_address_set_size( le_addr, le_size );

                /* Inject gathered element in array */
                le_array_set_pushrf( le_array, le_pose, le_class_get_data( & le_class ) );

                /* Query array streaming */
                le_array_io_stream( le_array, le_socket, 0 );

            } else {

                /* Class daughters enumeration */
                for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                    /* Update address index digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* Retreive daughter class offset */
                    if ( ( le_offset = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                        /* Recursive query */
                        le_system_query( le_system, le_addr, le_array, le_parse + 1, le_offset, le_socket );

                    }

                }

            }

        }

    }

/*
    source - temporal methods
 */

    le_array_t le_system_times( le_system_t const * const le_system ) {

        /* Time buffer variables */
        le_time_t le_buffer = _LE_TIME_NULL;

        /* Returned structure variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Directory variables */
        DIR * le_directory = NULL;

        /* Directory entity variables */
        struct dirent * le_entity = NULL;

        /* Create directory handle - send message */
        if ( ( le_directory = opendir( ( char * ) le_system->sm_path ) ) == NULL ) return( le_array );

        /* Directory entity enumeration */
        while ( ( le_entity = readdir( le_directory ) ) != NULL ) {

            /* Check entity type */
            if ( ( le_entity->d_type == DT_DIR ) && ( le_entity->d_name[0] != '.' ) ) {

                /* Convert directory name */
                le_buffer = le_system->sm_time * strtoull( le_entity->d_name, NULL, 10 );

                /* Push buffer in array */
                le_array_set_pushtf( & le_array, le_buffer );

            }

        }

        /* Delete directory handle */
        closedir( le_directory );

        /* Return structure */
        return( le_array );

    }

/*
    source - configuration methods
 */

    le_array_t le_system_config( le_system_t const * const le_system ) {

        /* Returned structure variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Compose configuration array */
        le_array_set_pushcf( & le_array, le_system->sm_area, le_system->sm_time );

        /* Return structure */
        return( le_array );

    }

/*
    source - i/o methods
 */

    le_enum_t le_system_io_open( le_system_t * const le_system, le_time_t const le_time ) {

        /* Temporal class variables */
        static le_time_t le_flag = _LE_TIME_NULL;

        /* Path variables */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

        /* Check necessities - send message */
        if ( ( le_time / le_system->sm_time ) == le_flag ) return( LE_ERROR_SUCCESS );

        /* Update temporal class */
        le_flag = le_time / le_system->sm_time;

        /* Compose temporal class directoy */
        sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P, le_system->sm_path, le_flag );

        /* Create directory */
        mkdir( ( char * ) le_path, 0777 );

        /* Create scale streams */
        for ( le_size_t le_parse = 0; le_parse < le_system->sm_area; le_parse ++ ) {

            /* Check stream state */
            if ( le_system->sm_stream[le_parse] != NULL ) fclose( le_system->sm_stream[le_parse] );

            /* Create scale path */
            sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_system->sm_path, le_flag, le_parse );

            /* Create scale stream (r+) */
            if ( ( le_system->sm_stream[le_parse] = fopen( ( char * ) le_path, "r+" ) ) == NULL ) {

                /* Create scale stream (w+) - send message */
                if ( ( le_system->sm_stream[le_parse] = fopen( ( char * ) le_path, "w+" ) ) == NULL ) return( LE_ERROR_IO_ACCESS );

            }

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_system_io_flush( le_system_t * const le_system ) {
        
        /* Parsing streams */
        for ( le_size_t le_parse = 0; le_parse < le_system->sm_area; le_parse ++ ) {

            /* Check stream state - flush stream */
            if ( le_system->sm_stream[le_parse] != NULL ) fflush( le_system->sm_stream[le_parse] );

        }

    }

    le_void_t le_system_io_close( le_system_t * const le_system ) {

        /* Parsing scales streams */
        for( le_size_t le_parse = 0; le_parse < le_system->sm_area; le_parse ++ ) {

            /* Check stream */
            if ( le_system->sm_stream[le_parse] != NULL ) {

                /* Close scale stream */
                fclose( le_system->sm_stream[le_parse] );

                /* Invalidate pointer */
                le_system->sm_stream[le_parse] = NULL;

            }

        }

    }
    
