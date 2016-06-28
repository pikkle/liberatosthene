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

    # include "eratosthene-client.h"

/*
    source - constructor/destructor methods
 */

    le_sock_t le_client_create( le_char_t const * const le_ip, le_size_t const le_port ) {

        /* Socket variables */
        le_sock_t le_socket = _LE_SOCK_NULL;

        /* Address variables */
        struct sockaddr_in le_addr = LE_SOCKADDR_IN_C_PORT( le_port );

        /* Convert address */
        if ( inet_pton( AF_INET, ( char * ) le_ip, & le_addr.sin_addr ) <= 0 ) {

            /* Send message */
            return( _LE_SOCK_NULL );

        }

        /* Create socket */
        if ( ( le_socket = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) {

            /* Send message */
            return( _LE_SOCK_NULL );

        }

        /* Sockets connection */
        if ( connect( le_socket, ( struct sockaddr * ) & le_addr, sizeof( le_addr ) ) == _LE_SOCK_NULL ) {

            /* Close socket */
            close( le_socket );

            /* Send message */
            return( _LE_SOCK_NULL );

        }

        /* Return connected socket */
        return( le_socket );

    }

    le_sock_t le_client_delete( le_sock_t const le_socket ) {

        /* Check socket */
        if ( le_socket != _LE_SOCK_NULL ) {

            /* Close socket */
            close( le_socket );

        }

        /* Return null socket */
        return( _LE_SOCK_NULL );

    }

/*
    source - handshake methods
 */

    le_enum_t le_client_handshake_mode( le_sock_t const le_socket, le_enum_t le_mode ) {

        /* Handshake buffer variables */
        le_byte_t le_buffer = ( le_byte_t ) le_mode;

        /* Check consistency */
        if ( le_socket == _LE_SOCK_NULL ) {

            /* Send message */
            return( LE_ERROR_IO_SOCKET );

        }

        /* Write handshake */
        if ( write( le_socket, & le_buffer, sizeof( le_byte_t ) ) != sizeof( le_byte_t ) ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* Wait handshake authorisation */
        if ( read( le_socket, & le_buffer, sizeof( le_byte_t ) ) != sizeof( le_byte_t ) ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Check authorisation */
        if ( ( le_buffer & 0x7F ) != le_mode ) {

            /* Send message */
            return( LE_ERROR_AUTH );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

/*
    source - connection methods
 */

    le_size_t le_client_system_sdisc( le_sock_t const le_socket ) {

        /* Space discretisation i/o variables */
        le_size_t le_sdisc = _LE_SIZE_NULL;

        /* Read configuration */
        if ( read( le_socket, & le_sdisc, sizeof( le_size_t ) ) != sizeof( le_size_t ) ) {

            /* Return configuration */
            return( _LE_SIZE_NULL );

        } else {

            /* Return configuration */
            return( le_sdisc );

        }

    }

    le_time_t le_client_system_tdisc( le_sock_t const le_socket ) {

        /* Time discretisation i/o variables */
        le_time_t le_tdisc = _LE_TIME_NULL;

        /* Read configuration */
        if ( read( le_socket, & le_tdisc, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) {

            /* Return null configuration */
            return( _LE_TIME_NULL );

        } else {

            /* Return read configuration */
            return( le_tdisc );
            
        }

    }

    le_enum_t le_client_system_array( le_sock_t const le_socket, le_array_t * const le_array, le_char_t const * const le_query ) {

        /* Query buffer variables */
        le_byte_t le_buffer[LE_NETWORK_SB_ADDR] = LE_NETWORK_C;

        /* Check consistency */
        if ( le_socket == _LE_SOCK_NULL ) {

            /* Send message */
            return( LE_ERROR_IO_SOCKET );

        }

        /* Copy query string to i/o buffer */
        strcpy( ( char * ) le_buffer, ( char * ) le_query );

        /* Send query to socket */
        if ( write( le_socket, le_buffer, LE_NETWORK_SB_ADDR ) != LE_NETWORK_SB_ADDR ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* Read array from socket */
        return( le_array_io_read( le_array, le_socket ) );

    }

