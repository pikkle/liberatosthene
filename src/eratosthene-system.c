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
 *
 */

    # include "eratosthene-system.h"

/*
    source - handle methods
 */

    le_enum_t le_system_create( le_system_t * const le_system, le_size_t const le_sdisc, le_time_t const le_tdisc, le_char_t const * const le_root ) {

        /* Check consistency */
        if ( le_sdisc >= LE_DEPTH_MAX ) {

            /* Send message */
            return( LE_ERROR_SYS_DEPTH );

        }

        /* Assign system discretisation */
        le_system->sm_sdisc = le_sdisc;
        le_system->sm_tdisc = le_tdisc;

        /* Assign initial scale stream stack */
        le_system->sm_scale = NULL;

        /* Assign system root path */
        strcpy( ( char * ) le_system->sm_root, ( char * ) le_root );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_system_delete( le_system_t * const le_system ) {

        /* Delete scale stream stack */
        le_system_close( le_system );

    }

/*
    source - injection methods
 */

    le_enum_t le_system_inject( le_system_t * const le_system, le_real_t * const le_pose, le_data_t const * const le_data, le_time_t const le_time ) {

        /* Address variables */
        le_byte_t le_addr[LE_DEPTH_MAX+1] = { 0 };

        /* Injection depth variables */
        le_size_t le_depth = 0;

        /* Offset tracker variables */
        le_size_t le_offset = 0;
        le_size_t le_offnex = 0;

        /* Class tracker variables */
        le_class_t le_class = LE_CLASS_C;

        /* Returned value variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* System scale stream management */
        if ( ( le_return = le_system_open( le_system, le_time ) ) != LE_ERROR_SUCCESS ) {

            /* Send message */
            return( le_return );

        }

        /* Compute address */
        le_address( le_pose, le_addr, le_system->sm_sdisc );

        /* Injection process */
        do {

            /* Memorise offset */
            le_offnex = le_offset;

            /* Class importation */
            if ( le_class_io_read( & le_class, le_offnex, le_system->sm_scale[le_depth] ) == LE_ERROR_SUCCESS ) {

                /* Inject element in class */
                le_class_set_inject( & le_class, le_data );

            } else {

                /* Initialise class with element */
                le_class_set_init( & le_class, le_data );

            }

            /* Retrieve daughter offset */
            le_offset = le_class_get_offset( & le_class, le_addr[le_depth] );

            /* Check daughter state */
            if ( ( le_offset == LE_CLASS_NULL ) && ( le_depth < ( le_system->sm_sdisc - 1 ) ) ) {

                /* Seek next scale eof */
                fseek( le_system->sm_scale[le_depth+1], 0, SEEK_END );

                /* Assign eof offset */
                le_offset = ftell( le_system->sm_scale[le_depth+1] );

                /* Insert offset in class */
                le_class_set_offset( & le_class, le_addr[le_depth], le_offset );

            }

            /* Class exportation */
            le_class_io_write( & le_class, le_offnex, le_system->sm_scale[le_depth] );

        /* Injection process condition */
        } while ( ( ++ le_depth ) < le_system->sm_sdisc );

        /* Send message */
        return( le_return );

    }

/*
    source - i/o methods
 */

    le_enum_t le_system_open( le_system_t * const le_system, le_time_t const le_time ) {

        /* Persistent time variables */
        static le_time_t le_tflag = 0;

        /* Path variables */
        le_char_t le_path[256] = { 0 };

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Check necessities */
        if ( ( ( le_time / le_system->sm_tdisc ) == le_tflag ) && ( le_system->sm_scale != NULL ) ) {

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
            le_tflag = le_time / le_system->sm_tdisc;

            /* Create scales streams */
            for( le_parse = 0; le_parse < le_system->sm_sdisc; le_parse ++ ) {

                /* Check scale stream */
                if ( le_system->sm_scale[le_parse] != NULL ) {

                    /* Delete scale stream */
                    fclose( le_system->sm_scale[le_parse] );

                }

                /* Create t-class scale path */
                sprintf( ( char * ) le_path, "%s/%" _LE_P_TIME "/scale-%03" _LE_P_SIZE ".bin", le_system->sm_root, le_tflag, le_parse );

                /* Create scale stream - r+ read/write */
                if ( ( le_system->sm_scale[le_parse] = fopen( ( char * ) le_path, "r+" ) ) == NULL ) {

                    /* Create scale stream - w+ read/write */
                    if ( ( le_system->sm_scale[le_parse] = fopen( ( char * ) le_path, "w+" ) ) == NULL ) {

                        /* Send message */
                        return( LE_ERROR_IO_ACCESS );

                    }

                }

            }

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );            

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
    
