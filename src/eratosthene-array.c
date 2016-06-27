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

    # include "eratosthene-array.h"

/*
    source - constructor/destructor methods
 */

    le_array_t le_array_create( le_void_t ) {

        /* Array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Return array */
        return( le_array );

    }

    le_void_t le_array_delete( le_array_t * const le_array ) {

        /* Check array state */
        if ( le_array->ar_vsze > 0 ) {

            /* Unallocate array memory */
            free( le_array->ar_byte );

            /* Reset array descriptors */
            le_array->ar_vsze = 0;
            le_array->ar_size = 0;

        }

    }

/*
    source - accessor methods
 */

    le_size_t le_array_get_size( le_array_t const * const le_array ) {

        /* Return array size */
        return( le_array->ar_size );

    }

    le_byte_t * le_array_get_byte( le_array_t const * const le_array ) {

        /* Return array bytes pointer */
        return( ( le_byte_t * ) le_array->ar_byte );

    }

/*
    source - mutator methods
 */

    le_enum_t le_array_set_memory( le_array_t * const le_array, le_size_t const le_length ) {

        /* Memory swapping variables */
        le_byte_t * le_swap = NULL;

        /* Check memory reallocation necessities */
        if ( ( le_array->ar_size + le_length ) >= le_array->ar_vsze ) {

            /* Update virtual size */
            le_array->ar_vsze += le_length * LE_ARRAY_STEP;

            /* Array memory allocation */
            if ( ( le_swap = ( le_byte_t * ) realloc( ( void * ) le_array->ar_byte, le_array->ar_vsze ) ) == NULL ) {

                /* Send message */
                return( LE_ERROR_MEMORY );

            }

            /* Swap memory pointers */
            le_array->ar_byte = le_swap;

        }

        /* Update array size */
        le_array->ar_size += le_length;

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_array_set_push( le_array_t * const le_array, le_enum_t const le_format, le_real_t const * const le_pose, le_time_t const le_time, le_data_t const * const le_data ) {

        /* Returned value variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* Switch on format */
        switch ( le_format ) {

            /* 64S16D array format */
            case ( LE_ARRAY_64S ) : {

                /* Array memory allocation */
                if ( ( le_return = le_array_set_memory( le_array, LE_ARRAY_64S_LEN ) ) == LE_ERROR_SUCCESS ) {

                    /* Array pointer variables */
                    le_real_t * le_sp = ( le_real_t * ) ( le_array->ar_byte + le_array->ar_size - LE_ARRAY_64S_LEN );
                    le_time_t * le_tp = ( le_time_t * ) ( le_sp + 3 );
                    le_data_t * le_dp = ( le_data_t * ) ( le_tp + 1 );

                    /* Assign array record elements */
                    le_sp[0] = le_pose[0];
                    le_sp[1] = le_pose[1];
                    le_sp[2] = le_pose[2];
                    le_tp[0] = le_time;
                    le_dp[0] = le_data[0];
                    le_dp[1] = le_data[1];
                    le_dp[2] = le_data[2];

                /* Send message */
                } return( le_return );

            } break;

            /* 64S16D array format */
            case ( LE_ARRAY_64R ) : {

                /* Array memory allocation */
                if ( ( le_return = le_array_set_memory( le_array, LE_ARRAY_64R_LEN ) ) == LE_ERROR_SUCCESS ) {

                    /* Array pointer variables */
                    le_real_t * le_sp = ( le_real_t * ) ( le_array->ar_byte + le_array->ar_size - LE_ARRAY_64R_LEN );
                    le_data_t * le_dp = ( le_data_t * ) ( le_sp + 3 );

                    /* Assign array record elements */
                    le_sp[0] = le_pose[0];
                    le_sp[1] = le_pose[1];
                    le_sp[2] = le_pose[2];
                    le_dp[0] = le_data[0];
                    le_dp[1] = le_data[1];
                    le_dp[2] = le_data[2];

                /* Send message */
                } return( le_return );

            } break;

        };

        /* Send message */
        return( LE_ERROR_FORMAT );

    }

/*
    source - i/o methods
 */

    le_enum_t le_array_io_write( le_array_t const * const le_array, le_sock_t const le_socket ) {

        /* Segmentation variables */
        le_size_t le_parse = 0;
        le_size_t le_count = 0;

        /* Virtual size variables */
        le_size_t le_vsize = le_array->ar_size;

        /* Check socket */
        if ( le_socket == _LE_SOCK_NULL ) {

            /* Send message */
            return( LE_ERROR_IO_SOCKET );

        }

        /* Writing array to socket */
        while ( le_parse < le_array->ar_size ) {

            /* Compute bloc size through virtual size */
            le_count = ( le_vsize < LE_NETWORK_BUFFER_SYNC ) ? le_vsize : LE_NETWORK_BUFFER_SYNC;

            /* Write bloc on socket */
            if ( write( le_socket, le_array->ar_byte + le_parse, le_count ) != le_count ) {

                /* Update writing status */
                return( LE_ERROR_IO_WRITE );

            }

            /* Update parser */
            le_parse += LE_NETWORK_BUFFER_SYNC;
            le_vsize -= LE_NETWORK_BUFFER_SYNC;

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

