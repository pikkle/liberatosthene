/*
 *  liberatosthene - geodetic system
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

    le_enum_t le_system_create( le_system_t * const le_system, le_char_t const * const le_root ) {

        /* Stream variables */
        FILE * le_stream = NULL;

        /* Construct structure */
        le_cons( le_system_t, le_system, LE_SYSTEM_C );

        /* Copy provided root path */
        strcpy( ( char * ) le_system->sm_root, ( char * ) le_root );

        /* Open configuration stream */
        if ( ( le_stream = fopen( strcat( ( char * ) le_system->sm_root, "/system" ), "r" ) ) == NULL ) {

            /* Send message */
            return( LE_ERROR_IO_ACCESS );

        }

        /* Check configuration reading */
        if ( fscanf( le_stream, "%" _LE_SIZE_S " %" _LE_TIME_S, & ( le_system->sm_sdisc ), & ( le_system->sm_tdisc ) ) != 2 ) {

            /* Close configuration stream */
            fclose( le_stream );

            /* Send message */
            return( LE_ERROR_IO_READ );

        } else {

            /* Close configuration stream */
            fclose( le_stream );

        }

        /* Check consistency */
        if ( ( le_system->sm_sdisc <= 0 ) || ( le_system->sm_sdisc >= _LE_USE_DEPTH ) ) {

            /* Send message */
            return( LE_ERROR_DEPTH );

        }

        /* Check consistency */
        if ( le_system->sm_tdisc <= 0 ) {

            /* Send message */
            return( LE_ERROR_TIME );

        } else {

            /* Initialise streams stack */
            le_system->sm_scale = NULL;

            /* Assign provided root path */
            strcpy( ( char * ) le_system->sm_root, ( char * ) le_root );

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_void_t le_system_delete( le_system_t * const le_system ) {

        /* Delete streams stack */
        le_system_close( le_system );

    }

/*
    source - accessor methods
 */

    le_size_t le_system_get_sdisc( le_system_t const * const le_system ) {

        /* Return system space discretisation */
        return( le_system->sm_sdisc );

    }

    le_time_t le_system_get_tdisc( le_system_t const * const le_system ) {

        /* Return system time discretisation */
        return( le_system->sm_tdisc );

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
        //le_address_t le_addr = LE_ADDRESS_C_SIZE( le_system->sm_sdisc - 2 );
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_system->sm_sdisc - 1 );

        /* System scale stream management */
        if ( ( le_return = le_system_open( le_system, le_time ) ) != LE_ERROR_SUCCESS ) {

            /* Send message */
            return( le_return );

        }

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
                le_class_set_init( & le_class, le_data );

            }

            /* Retrieve daughter offset */
            le_offset = le_class_get_offset( & le_class, le_address_get_digit( & le_addr, le_depth ) );

            /* Check daughter state */
            if ( ( le_offset == _LE_SIZE_NULL ) && ( le_depth < ( le_system->sm_sdisc - 1 ) ) ) {

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
        } while ( ( ++ le_depth ) < le_system->sm_sdisc );

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

        /* Check consistency */
        //if ( ( le_address_get_size( le_addr ) + le_address_get_depth( le_addr ) + 1 ) >= le_system->sm_sdisc ) {
        if ( ( le_address_get_size( le_addr ) + le_address_get_depth( le_addr ) ) >= le_system->sm_sdisc ) {

            /* Return element array */
            return( le_return );

        }

        /* System scale stream management */
        if ( le_system_open( le_system, le_address_get_time( le_addr ) ) != LE_ERROR_SUCCESS ) {

            /* Return element array */
            return( le_return );

        }

        /* Query class search */
        do {

            /* Class importation */
            if ( le_class_io_read( & le_class, le_offset, le_system->sm_scale[le_depth] ) != LE_ERROR_SUCCESS ) {

                /* Return element array */
                return( le_return );

            } else {

                /* Extract daughter offset */
                le_offset = le_class.cs_addr[le_address_get_digit( le_addr, le_depth )];

            }

        /* Query class search condition */
        } while ( ( ( ++ le_depth ) <= le_address_get_size( le_addr ) ) && ( le_offset != _LE_SIZE_NULL ) );

        /* Check query class search */
        if ( ( -- le_depth ) == le_address_get_size( le_addr ) ) {

            /* Gathering process */
            le_system_gather( le_system, & le_return, le_addr, & le_class, le_depth, le_depth + le_address_get_depth( le_addr ) );

        }

        /* Return element array */
        return( le_return );
           
    }

    le_void_t le_system_gather( le_system_t * const le_system, le_array_t * const le_array, le_address_t * const le_addr, le_class_t * const le_class, le_size_t const le_head, le_size_t const le_target ) {

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
                if ( ( le_offset = le_class_get_offset( le_class, le_parse ) ) != _LE_SIZE_NULL ) {

                    /* Set address size */  
                    le_address_set_size( le_addr, le_head + 1 );

                    /* Set address digit */
                    le_address_set_digit( le_addr, le_head, le_parse );

                    /* Read daughter */
                    le_class_io_read( & le_clnex, le_offset, le_system->sm_scale[le_head+1] );

                    /* Recursive gathering process */
                    le_system_gather( le_system, le_array, le_addr, & le_clnex, le_head + 1, le_target );

                }

            }

        } else {

            /* Retreive class representative */
            le_address_get_pose( le_addr, le_pose );

            /* Inject gathered element in array */
            le_array_set_push( le_array, le_pose, le_address_get_time( le_addr ), le_class_get_data( le_class ) );

        }

    }

/*
    source - i/o methods
 */

    le_enum_t le_system_open( le_system_t * const le_system, le_time_t const le_time ) {

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Path variables */
        le_char_t le_path[256] = { 0 };

        /* Persistent time variables */
        static le_time_t le_flag = _LE_TIME_MIN;

        /* Check necessities */
        if ( ( ( le_time / le_system->sm_tdisc ) == le_flag ) && ( le_system->sm_scale != NULL ) ) {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        } else {

            /* Check stream stack */
            if ( le_system->sm_scale == NULL ) {

                /* Allocate stack memory */
                if ( ( le_system->sm_scale = malloc( sizeof( FILE * ) * le_system->sm_sdisc ) ) == NULL ) {

                    /* Send message */
                    return( LE_ERROR_MEMORY );

                }

                /* Initialise stack */
                for ( le_parse = 0; le_parse < le_system->sm_sdisc; le_parse ++ ) {

                    /* Invalidate pointer */
                    le_system->sm_scale[le_parse] = NULL;

                }

            }

            /* Update presistent time */
            le_flag = le_time / le_system->sm_tdisc;

            /* Create time directory path */
            sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P, le_system->sm_root, le_flag );

            /* Create time directory */
            mkdir( ( char * ) le_path, 0777 );

            /* Create scales streams */
            for( le_parse = 0; le_parse < le_system->sm_sdisc; le_parse ++ ) {

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

    }

    le_void_t le_system_flush( le_system_t * const le_system ) {

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Parsing stream */
        for ( ; le_parse < le_system->sm_sdisc; le_parse ++ ) {

            /* Check stream state */
            if ( le_system->sm_scale[le_parse] != NULL ) {

                /* Flush stream */
                fflush( le_system->sm_scale[le_parse] );

            }

        }

    }

    le_void_t le_system_close( le_system_t * const le_system ) {

        /* Parser variables */
        le_size_t le_parse = 0;

        /* Check structure */
        if ( le_system->sm_scale != NULL ) {

            /* Parsing scales streams */
            for( ; le_parse < le_system->sm_sdisc; le_parse ++ ) {

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
    
