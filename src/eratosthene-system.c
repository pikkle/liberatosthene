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
    source - i/o methods
 */

    le_enum_t le_system_open( le_system_t * const le_system, le_time_t const le_time, le_char_t const * const le_root ) {

        /* Path variables */
        le_char_t le_path[256] = { 0 };

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Check necessities */
        if ( ( le_time / le_system->sm_terl ) == le_system->sm_time ) {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        } else {

            /* Check stream stack */
            if ( le_system->sm_scale == NULL ) {

                /* Allocate stack memory */
                if ( ( le_system->sm_scale = malloc( sizeof( FILE * ) * le_system->sm_depth ) ) == NULL ) {

                    /* Send message */
                    return( LE_ERROR_MEMORY );

                }

                /* Initialise stack */
                for ( le_parse = 0; le_parse < le_system->sm_depth; le_parse ++ ) {

                    /* Invalidate pointer */
                    le_system->sm_scale[le_parse] = NULL;

                }

            }

            /* Update system time */
            le_system->sm_time = le_time / le_system->sm_terl;

            /* Create scales streams */
            for( le_parse = 0; le_parse < le_system->sm_depth; le_parse ++ ) {

                /* Check scale stream */
                if ( le_system->sm_scale[le_parse] != NULL ) {

                    /* Delete scale stream */
                    fclose( le_system->sm_scale[le_parse] );

                }

                /* Create t-class scale path */
                sprintf( ( char * ) le_path, "%s/%" _LE_P_TIME "/scale-%03" _LE_P_SIZE ".bin", le_root, le_system->sm_time, le_parse );

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
            for( ; le_parse < le_system->sm_depth; le_parse ++ ) {

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
    
