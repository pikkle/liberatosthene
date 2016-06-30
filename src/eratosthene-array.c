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
            le_array->ar_vsze += LE_ARRAY_STEP;

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

            /* 64S array format */
            case ( LE_ARRAY_64S ) : {

                /* Array memory allocation */
                if ( ( le_return = le_array_set_memory( le_array, LE_ARRAY_64S_LEN ) ) == LE_ERROR_SUCCESS ) {

                    /* Array pointer variables */
                    le_real_t * le_sp = ( le_real_t * ) ( le_array->ar_byte + le_array->ar_size - LE_ARRAY_64S_LEN );
                    le_time_t * le_tp = ( le_time_t * ) ( le_sp + 3 );
                    le_data_t * le_dp = ( le_data_t * ) ( le_tp + 1 );

                    /* Record element - space */
                    le_sp[0] = le_pose[0];
                    le_sp[1] = le_pose[1];
                    le_sp[2] = le_pose[2];

                    /* Record element - time */
                    le_tp[0] = le_time;

                    /* Record element - information */
                    le_dp[0] = le_data[0];
                    le_dp[1] = le_data[1];
                    le_dp[2] = le_data[2];

                /* Send message */
                } return( le_return );

            } break;

            /* 64R array format */
            case ( LE_ARRAY_64R ) : {

                /* Array memory allocation */
                if ( ( le_return = le_array_set_memory( le_array, LE_ARRAY_64R_LEN ) ) == LE_ERROR_SUCCESS ) {

                    /* Array pointer variables */
                    le_real_t * le_sp = ( le_real_t * ) ( le_array->ar_byte + le_array->ar_size - LE_ARRAY_64R_LEN );
                    le_data_t * le_dp = ( le_data_t * ) ( le_sp + 3 );

                    /* Record element - space */
                    le_sp[0] = le_pose[0];
                    le_sp[1] = le_pose[1];
                    le_sp[2] = le_pose[2];

                    /* Record element - information */
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

        /* Array pointer variables */
        le_byte_t * le_lbloc = le_array->ar_byte;
        le_byte_t * le_hbloc = le_array->ar_byte + _LE_USE_MTU;
        le_byte_t * le_sbloc = le_array->ar_byte + le_array->ar_size;

        /* Segment size variables */
        le_size_t le_size = 0;

        /* Check consistency - send message */
        if ( le_socket == _LE_SOCK_NULL ) return( LE_ERROR_IO_SOCKET );

        /* Sending array over TCP/IP */
        while ( le_lbloc < le_sbloc ) {

            /* Compute bloc size */
            le_size = ( le_size_t ) ( le_hbloc > le_sbloc ? le_sbloc - le_lbloc : le_hbloc - le_lbloc );

            /* Send bloc to socket - Send message */
            if ( write( le_socket, le_lbloc, le_size ) != le_size ) return( LE_ERROR_IO_WRITE );

            /* Update pointers */
            le_lbloc += _LE_USE_MTU;
            le_hbloc += _LE_USE_MTU;

        }
            
        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_array_io_read( le_array_t * const le_array, le_sock_t const le_socket ) {

        /* Returned value variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* Socket i/o variables */
        le_size_t le_size = 0;
        le_size_t le_read = 0;

        /* Check consistency - send message */
        if ( le_socket == _LE_SOCK_NULL ) return( LE_ERROR_IO_SOCKET );

        /* Receiving array over TCP/IP */
        while ( le_read < 5 ) {

            /* Array memory allocation - Send message */
            if ( ( le_return = le_array_set_memory( le_array, _LE_USE_MTU ) ) != LE_ERROR_SUCCESS ) return( le_return );

            /* Array size management */
            le_array->ar_size -= _LE_USE_MTU;

            /* Read bloc on socket */
            if ( ( le_size = read( le_socket, le_array->ar_byte + le_array->ar_size, _LE_USE_MTU ) ) > 0 ) {

                /* Array size management */
                le_array->ar_size += le_size;

                /* Clear reading value */
                le_read = 0;

            /* Update reading value */
            } else { le_read ++; }

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

