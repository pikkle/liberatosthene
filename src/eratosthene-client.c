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

        /* Address variables */
        struct sockaddr_in le_addr = LE_SOCKADDR_IN_C_PORT( le_port );

        /* Returned value variables */
        le_sock_t le_socket = _LE_SOCK_NULL;

        /* Convert address - send message */
        if ( inet_pton( AF_INET, ( char * ) le_ip, & le_addr.sin_addr ) <= 0 ) return( _LE_SOCK_NULL );

        /* Create socket - send message */
        if ( ( le_socket = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) return( _LE_SOCK_NULL );

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

        /* Check socket - close socket */
        if ( le_socket != _LE_SOCK_NULL ) close( le_socket );

        /* Return null socket */
        return( _LE_SOCK_NULL );

    }

/*
    source - handshake and authorisation methods
 */

    le_enum_t le_client_handshake( le_sock_t const le_socket, le_enum_t le_mode, le_enum_t le_format ) {

        /* Handshake buffer variables */
        le_hand_t le_buffer = ( le_hand_t ) le_mode | ( le_hand_t ) le_format << 8;

        /* Check consistency - send message */
        if ( le_socket == _LE_SOCK_NULL ) return( LE_ERROR_IO_SOCKET );

        /* Write handshake - send message */
        if ( write( le_socket, & le_buffer, sizeof( le_hand_t ) ) != sizeof( le_hand_t ) ) return( LE_ERROR_IO_WRITE );

        /* Wait handshake authorisation - send message */
        if ( read( le_socket, & le_buffer, sizeof( le_hand_t ) ) != sizeof( le_hand_t ) ) return( LE_ERROR_IO_READ );

        /* Check authorisation - send message */
        return( ( ( le_buffer & 0x7f ) == le_mode ) ? LE_ERROR_SUCCESS : LE_ERROR_AUTH );

    }

/*
    source - system methods
 */

    le_enum_t le_client_system_query_array( le_sock_t const le_socket, le_array_t * const le_array, le_char_t const * const le_query ) {

        /* Query buffer variables */
        le_byte_t le_buffer[LE_NETWORK_SB_ADDR] = LE_NETWORK_C;

        /* Check consistency - send message */
        if ( le_socket == _LE_SOCK_NULL ) return( LE_ERROR_IO_SOCKET );

        /* Copy query string to i/o buffer */
        strcpy( ( char * ) le_buffer, ( char * ) le_query );

        /* Send query to socket - send message */
        if ( write( le_socket, le_buffer, LE_NETWORK_SB_ADDR ) != LE_NETWORK_SB_ADDR ) return( LE_ERROR_IO_WRITE );

        /* Read array from socket */
        return( le_array_io_read( le_array, le_socket ) );

    }

    le_size_t le_client_system_sparam( le_sock_t const le_socket ) {

        /* Space discretisation i/o variables */
        le_size_t le_sparam = _LE_SIZE_NULL;

        /* Read configuration - send message */
        if ( read( le_socket, & le_sparam, sizeof( le_size_t ) ) != sizeof( le_size_t ) ) return( _LE_SIZE_NULL );

        /* Return configuration */
        return( le_sparam );

    }

    le_time_t le_client_system_tparam( le_sock_t const le_socket ) {

        /* Time discretisation i/o variables */
        le_time_t le_tparam = _LE_TIME_NULL;

        /* Read configuration - send message */
        if ( read( le_socket, & le_tparam, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) return( _LE_TIME_NULL );

        /* Return configuration */
        return( le_tparam );

    }

