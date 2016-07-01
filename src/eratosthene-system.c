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
        if ( ( le_stream = fopen( strcat( strcat( ( char * ) le_system.sm_root, ( char * ) le_root ), "/system" ), "r" ) ) == NULL ) {

            /* Send message */
            le_system._status = LE_ERROR_IO_ACCESS; return( le_system );

        }

        /* Check configuration reading */
        if ( fscanf( le_stream, "%" _LE_SIZE_S " %" _LE_TIME_S, & ( le_system.sm_sparam ), & ( le_system.sm_tparam ) ) != 2 ) {

            /* Close configuration stream */
            fclose( le_stream );

            /* Send message */
            le_system._status = LE_ERROR_IO_READ; return( le_system );

        }

        /* Check consistency */
        if ( ( le_system.sm_sparam <= 0 ) || ( le_system.sm_sparam >= _LE_USE_DEPTH ) ) {

            /* Send message */
            le_system._status = LE_ERROR_DEPTH; return( le_system );

        }

        /* Check consistency */
        if ( le_system.sm_tparam <= 0 ) {

            /* Send message */
            le_system._status = LE_ERROR_TIME; return( le_system );

        }

        /* Assign provided root path */
        strcpy( ( char * ) le_system.sm_root, ( char * ) le_root );

        /* Close configuration stream */
        fclose( le_stream );

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
    source - accessor methods
 */

    le_size_t le_system_get_sparam( le_system_t const * const le_system ) {

        /* Return system space discretisation */
        return( le_system->sm_sparam );

    }

    le_time_t le_system_get_tparam( le_system_t const * const le_system ) {

        /* Return system time discretisation */
        return( le_system->sm_tparam );

    }

/*
    source - mutator methods
 */

    le_hand_t le_system_set_format( le_system_t * const le_system, le_hand_t const le_hand ) {

        /* Switch on authorisation mode */
        switch ( le_hand & 0xff ) {

            /* Injection authorisation */
            case ( LE_NETWORK_MODE_IMOD ) : {

                /* Switch on format */
                switch ( le_hand >> 8 ) {

                    /* Transmission format */
                    case ( LE_ARRAY_64S ) : {

                        /* Change transmission mode */
                        le_system->sm_format = LE_ARRAY_64S;

                        /* Return autorisation */
                        return( LE_NETWORK_MODE_IATH );

                    } break;

                };

            } break;

            /* Query authorisation */
            case ( LE_NETWORK_MODE_QMOD ) : {

                /* Switch on format */
                switch ( le_hand >> 8 ) {

                    /* Transmission format */
                    case ( LE_ARRAY_64S ) : {

                        /* Change transmission mode */
                        le_system->sm_format = LE_ARRAY_64S;

                        /* Return authorisation */
                        return( LE_NETWORK_MODE_QATH );

                    } break;

                    /* Transmission format */
                    case ( LE_ARRAY_64R ) : {

                        /* Change transmission mode */
                        le_system->sm_format = LE_ARRAY_64R;

                        /* Return authorisation */
                        return( LE_NETWORK_MODE_QATH );

                    } break;

                };

            } break;

            /* System times array mode */
            case ( LE_NETWORK_MODE_AMOD ) : {

                /* Switch on format */
                switch ( le_hand >> 8 ) {

                    /* Transmission format */
                    case ( LE_ARRAY_64T ) : {

                        /* Transmission format */
                        le_system->sm_format = LE_ARRAY_64T;

                        /* Return authorisation */
                        return( LE_NETWORK_MODE_AATH );

                    } break;

                };

            } break;

            /* Spatial parameter query */
            case ( LE_NETWORK_MODE_SMOD ) : {

                /* Return authorisation */
                return( LE_NETWORK_MODE_SATH );

            } break;

            /* Temporal parameter query */
            case ( LE_NETWORK_MODE_TMOD ) : {

                /* Return authorisation */
                return( LE_NETWORK_MODE_TATH );

            } break;

        };

        /* Authorisation failure */
        return( _LE_HAND_NULL );

    }

/*
    source - injection methods
 */

    le_enum_t le_system_inject( le_system_t * const le_system, le_real_t * const le_pose, le_time_t const le_time, le_data_t const * const le_data ) {

        /* Returned status variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* Depth variables */
        le_size_t le_depth = 0;

        /* Offset variables */
        le_size_t le_offset = 0;
        le_size_t le_offnex = 0;

        /* Class variables */
        le_class_t le_class = LE_CLASS_C;

        /* Address variables */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_system->sm_sparam - 1 );

        /* System scale stream management - send message */
        if ( ( le_return = le_system_io_open( le_system, le_time ) ) != LE_ERROR_SUCCESS ) return( le_return );

        /* Compute address */
        le_address_set_pose( & le_addr, le_pose );

        /* Injection process */
        do {

            /* Memorise offset */
            le_offnex = le_offset;

            /* Class importation */
            if ( le_class_io_read( & le_class, le_offnex, le_system->sm_scale[le_depth] ) == LE_ERROR_SUCCESS ) {

                /* Inject element in class */
                le_class_set_push( & le_class, le_data );

            } else {

                /* Initialise class with element */
                le_class = le_class_create( le_data );

            }

            /* Retrieve daughter offset */
            le_offset = le_class_get_offset( & le_class, le_address_get_digit( & le_addr, le_depth ) );

            /* Check daughter state */
            if ( ( le_offset == _LE_OFFS_NULL ) && ( le_depth < ( le_system->sm_sparam - 1 ) ) ) {

                /* Seek next scale eof */
                fseek( le_system->sm_scale[le_depth+1], 0, SEEK_END );

                /* Assign eof offset */
                le_offset = ftell( le_system->sm_scale[le_depth+1] );

                /* Insert offset in class */
                le_class_set_offset( & le_class, le_address_get_digit( & le_addr, le_depth ), le_offset );

            }

            /* Class exportation */
            le_class_io_write( & le_class, le_offnex, le_system->sm_scale[le_depth] );

        /* Injection process condition */
        } while ( ( ++ le_depth ) < le_system->sm_sparam );

        /* Send message */
        return( le_return );

    }

/*
    source - query methods
 */

    le_array_t le_system_query( le_system_t * const le_system, le_address_t * const le_addr ) {

        /* Query depth variables */
        le_size_t le_depth = 0;

        /* Offset tracker variables */
        le_size_t le_offset = 0;

        /* Class tracker variables */
        le_class_t le_class = LE_CLASS_C;

        /* Returned array variables */
        le_array_t le_return = LE_ARRAY_C;

        /* Check consistency - send array */
        if ( ( le_address_get_size( le_addr ) + le_address_get_depth( le_addr ) ) >= le_system->sm_sparam ) return( le_return );

        /* System scale stream management - send array */
        if ( le_system_io_open( le_system, le_address_get_time( le_addr ) ) != LE_ERROR_SUCCESS ) return( le_return );

        /* Query class search */
        do {

            /* Class importation */
            if ( le_class_io_read( & le_class, le_offset, le_system->sm_scale[le_depth] ) != LE_ERROR_SUCCESS ) {

                /* Return element array */
                return( le_return );

            } else {

                /* Extract daughter offset */
                le_offset = le_class_get_offset( & le_class, le_address_get_digit( le_addr, le_depth ) );

            }

        /* Query class search condition */
        } while ( ( ( ++ le_depth ) <= le_address_get_size( le_addr ) ) && ( le_offset != _LE_OFFS_NULL ) );

        /* Check query class search */
        if ( ( -- le_depth ) == le_address_get_size( le_addr ) ) {

            /* Gathering process */
            le_system_query_gather( le_system, & le_return, le_addr, & le_class, le_depth, le_depth + le_address_get_depth( le_addr ) );

        }

        /* Return element array */
        return( le_return );
           
    }

    le_void_t le_system_query_gather( le_system_t * const le_system, le_array_t * const le_array, le_address_t * const le_addr, le_class_t * const le_class, le_size_t const le_head, le_size_t const le_target ) {

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Offset variables */
        le_size_t le_offset = 0;

        /* Spatial coordinates variables */
        le_real_t le_pose[3] = { 0.0 };

        /* Class variables */
        le_class_t le_clnex = LE_CLASS_C;

        /* Check head and target */
        if ( le_head < le_target ) {

            /* Parsing class daughter */
            for ( ; le_parse < 8; le_parse ++ ) {

                /* Check daughter */
                if ( ( le_offset = le_class_get_offset( le_class, le_parse ) ) != _LE_OFFS_NULL ) {

                    /* Set address size */  
                    le_address_set_size( le_addr, le_head + 1 );

                    /* Set address digit */
                    le_address_set_digit( le_addr, le_head, le_parse );

                    /* Read daughter */
                    le_class_io_read( & le_clnex, le_offset, le_system->sm_scale[le_head+1] );

                    /* Recursive gathering process */
                    le_system_query_gather( le_system, le_array, le_addr, & le_clnex, le_head + 1, le_target );

                }

            }

        } else {

            /* Retreive class representative */
            le_address_get_pose( le_addr, le_pose );

            /* Inject gathered element in array */
            le_array_set_push( le_array, le_system->sm_format, le_pose, le_address_get_time( le_addr ), le_class_get_data( le_class ) );

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
        if ( ( le_directory = opendir( ( char * ) le_system->sm_root ) ) == NULL ) return( le_array );

        /* Directory entity enumeration */
        while ( ( le_entity = readdir( le_directory ) ) != NULL ) {

            /* Check entity type */
            if ( le_entity->d_type == DT_DIR ) {

                /* Convert directory name */
                le_buffer = le_system->sm_tparam * strtoull( le_entity->d_name, NULL, 10 );

                /* Push buffer in array */
                le_array_set_push( & le_array, LE_ARRAY_64T, NULL, le_buffer, NULL );

            }

        }

        /* Delete directory handle */
        closedir( le_directory );

        /* Return structure */
        return( le_array );

    }

/*
    source - i/o methods
 */

    le_enum_t le_system_io_open( le_system_t * const le_system, le_time_t const le_time ) {

        /* Persistent time variables */
        static le_time_t le_flag = _LE_TIME_MIN;

        /* Path variables */
        le_char_t le_path[256] = { 0 };

        /* Check necessities - send message */
        if ( ( ( le_time / le_system->sm_tparam ) == le_flag ) && ( le_system->sm_scale != NULL ) ) return( LE_ERROR_SUCCESS );

        /* Check stream stack */
        if ( le_system->sm_scale == NULL ) {

            /* Allocate stack memory */
            if ( ( le_system->sm_scale = malloc( sizeof( FILE * ) * le_system->sm_sparam ) ) == NULL ) {

                /* Send message */
                return( LE_ERROR_MEMORY );

            }

            /* Initialise stack */
            for ( le_size_t le_parse = 0; le_parse < le_system->sm_sparam; le_parse ++ ) {

                /* Invalidate pointer */
                le_system->sm_scale[le_parse] = NULL;

            }

        }

        /* Update presistent time */
        le_flag = le_time / le_system->sm_tparam;

        /* Create time directory path */
        sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P, le_system->sm_root, le_flag );

        /* Create time directory */
        mkdir( ( char * ) le_path, 0777 );

        /* Create scales streams */
        for( le_size_t le_parse = 0; le_parse < le_system->sm_sparam; le_parse ++ ) {

            /* Check scale stream */
            if ( le_system->sm_scale[le_parse] != NULL ) {

                /* Delete scale stream */
                fclose( le_system->sm_scale[le_parse] );

            }

            /* Create scale path */
            sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_system->sm_root, le_flag, le_parse );

            /* Create scale stream - r+ read/write */
            if ( ( le_system->sm_scale[le_parse] = fopen( ( char * ) le_path, "r+" ) ) == NULL ) {

                /* Create scale stream - w+ read/write */
                if ( ( le_system->sm_scale[le_parse] = fopen( ( char * ) le_path, "w+" ) ) == NULL ) {

                    /* Send message */
                    return( LE_ERROR_IO_ACCESS );

                }

            }

            /* Handle file/directory permission */
            chmod( ( char * ) le_path, 0777 );

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_system_io_flush( le_system_t * const le_system ) {
        
        /* Parsing streams */
        for ( le_size_t le_parse = 0; le_parse < le_system->sm_sparam; le_parse ++ ) {

            /* Check stream state */
            if ( le_system->sm_scale[le_parse] != NULL ) {

                /* Flush stream */
                fflush( le_system->sm_scale[le_parse] );

            }

        }

    }

    le_void_t le_system_io_close( le_system_t * const le_system ) {

        /* Check structure */
        if ( le_system->sm_scale != NULL ) {

            /* Parsing scales streams */
            for( le_size_t le_parse = 0; le_parse < le_system->sm_sparam; le_parse ++ ) {

                /* Check stream */
                if ( le_system->sm_scale[le_parse] != NULL ) {

                    /* Close scale stream */
                    fclose( le_system->sm_scale[le_parse] );

                    /* Invalidate pointer */
                    le_system->sm_scale[le_parse] = NULL;

                }

            }

            /* Unallocate stream stack */
            free( le_system->sm_scale );

            /* Invalidate pointer */
            le_system->sm_scale = NULL;

        }

    }
    
