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

    le_sock_t le_client_create( le_char_t const * const le_ip, le_sock_t const le_port ) {

        /* address variables */
        struct sockaddr_in le_addr = LE_ADDRIN_C_PORT( le_port );

        /* returned value variables */
        le_sock_t le_socket = _LE_SOCK_NULL;

        /* check consistency - send message */
        if ( le_ip == NULL ) return( _LE_SOCK_NULL );

        /* convert address */
        if ( inet_pton( AF_INET, ( char * ) le_ip, & le_addr.sin_addr ) <= 0 ) {

            /* send message */
            return( _LE_SOCK_NULL );

        }

        /* create socket */
        if ( ( le_socket = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) {

            /* send message */
            return( _LE_SOCK_NULL );

        }

        /* sockets connection */
        if ( connect( le_socket, ( struct sockaddr * ) & le_addr, sizeof( struct sockaddr_in ) ) == _LE_SOCK_NULL ) {

            /* close socket */
            close( le_socket );

            /* send message */
            return( _LE_SOCK_NULL );

        }

        /* return connected socket */
        return( le_socket );

    }

    le_sock_t le_client_delete( le_sock_t const le_socket ) {

        /* check socket */
        if ( le_socket != _LE_SOCK_NULL ) {

            /* close socket */
            close( le_socket );

        }

        /* return null socket */
        return( _LE_SOCK_NULL );

    }

/*
    source - handshake methods
 */

    le_enum_t le_client_handshake( le_sock_t const le_socket, le_enum_t const le_mode ) {

        /* socket i/o buffer variables */
        le_byte_t er_buffer = le_mode;

        /* write handshake */
        if ( write( le_socket, & er_buffer, sizeof( le_byte_t ) ) != sizeof( le_byte_t ) ) {

            /* send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* read handshake */
        if ( read ( le_socket, & er_buffer, sizeof( le_byte_t ) ) != sizeof( le_byte_t ) ) {

            /* send message */
            return( LE_ERROR_IO_READ );

        }

        /* check server authorisation - send message */
        return( ( er_buffer & 0x7f ) == le_mode ? LE_ERROR_SUCCESS : LE_ERROR_AUTH );

    }

    le_byte_t le_client_switch( le_sock_t const le_socket ) {

        /* socket i/o buffer variables */
        le_byte_t le_buffer = LE_MODE_NULL;

        /* read handshake */
        if ( read( le_socket, & le_buffer, sizeof( le_byte_t ) ) != sizeof( le_byte_t ) ) {

            /* send message */
            return( LE_MODE_NULL );

        }

        /* return received handshake */
        return( le_buffer );

    }

    le_enum_t le_client_authorise( le_sock_t const le_socket, le_byte_t const le_auth ) {

        /* write authorisation */
        if ( write( le_socket, & le_auth, sizeof( le_byte_t ) ) != sizeof( le_byte_t ) ) {

            /* send message */
            return( LE_ERROR_IO_WRITE );

        }

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - common methods
 */

    le_enum_t le_client_array( le_char_t const * const le_ip, le_sock_t const le_port, le_enum_t const le_mode, le_array_t * const le_array ) {

        /* returned value variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* socket variables */
        le_sock_t le_socket = _LE_SOCK_NULL;

        /* create connection to server */
        if ( ( le_socket = le_client_create( le_ip, le_port ) ) == _LE_SOCK_NULL ) {

            /* send message */
            return( LE_ERROR_IO_SOCKET );

        }

        /* server/client handshake */
        if ( ( le_return = le_client_handshake( le_socket, le_mode ) ) == LE_ERROR_SUCCESS ) {

            /* read array from server */
            le_return = le_array_io_read( le_array, le_socket );

        }

        /* delete connection to server */
        le_socket = le_client_delete( le_socket );

        /* Send message */
        return( le_return );

    }

