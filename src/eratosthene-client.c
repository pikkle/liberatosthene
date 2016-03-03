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

    # include "eratosthene-client.h"

/*
    source - constructor/destructor methods
 */

    le_sock_t le_client_create( le_char_t const * const le_ip, le_size_t const le_port ) {

        /* Socket variables */
        le_sock_t le_socket = LE_NETWORK_NULL;

        /* Address variables */
        struct sockaddr_in le_addr = LE_SOCKADDR_IN_C_PORT( le_port );

        /* Convert address */
        if ( inet_pton( AF_INET, ( char * ) le_ip, & le_addr.sin_addr ) <= 0 ) {

            /* Send message */
            return( LE_NETWORK_NULL );

        }

        /* Create socket */
        if ( ( le_socket = socket( AF_INET, SOCK_STREAM, 0 ) ) == LE_NETWORK_NULL ) {

            /* Send message */
            return( LE_NETWORK_NULL );

        }

        /* Sockets connection */
        if ( connect( le_socket, ( struct sockaddr * ) & le_addr, sizeof( le_addr ) ) == LE_NETWORK_NULL ) {

            /* Close socket */
            close( le_socket );

            /* Send message */
            return( LE_NETWORK_NULL );

        }

        /* Return connected socket */
        return( le_socket );

    }

    le_void_t le_client_delete( le_sock_t const le_socket ) {

        /* Check socket */
        if ( le_socket != LE_NETWORK_NULL ) {

            /* Close socket */
            close( le_socket );

        }

    }

/*
    source - handshaked methods
 */

    le_enum_t le_client_handshake_mode( le_sock_t const le_socket, le_enum_t le_mode ) {

        /* Handshake buffer variables */
        le_byte_t le_buffer = ( le_byte_t ) le_mode;

        /* Write handshake */
        if ( write( le_socket, & le_buffer, sizeof( le_byte_t ) ) == sizeof( le_byte_t ) ) {

            /* Wait handshake authorisation */
            if ( read( le_socket, & le_buffer, sizeof( le_byte_t ) ) == sizeof( le_byte_t ) ) {

                /* Check authorisation */
                if ( ( le_buffer & 0x7F ) == le_mode ) {

                    /* Send message */
                    return( LE_ERROR_SUCCESS );

                } else {

                    /* Send message */
                    return( LE_ERROR_AUTH );

                }

            } else {

                /* Send message */
                return( LE_ERROR_SOCKET );

            }

        } else {

            /* Send message */
            return( LE_ERROR_SOCKET );

        }

    }

