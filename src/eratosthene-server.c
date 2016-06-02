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

    # include "eratosthene-server.h"

/*
    source - constructor/destructor methods
 */

    le_sock_t le_server_create( le_size_t const le_port ) {

        /* Socket variables */
        le_sock_t le_socket = _LE_SOCK_NULL;

        /* Address variables */
        struct sockaddr_in le_addr = LE_SOCKADDR_IN_C_PORT( le_port );
        
        /* Create socket */
        if ( ( le_socket = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) {

            /* Send message */
            return( _LE_SOCK_NULL );

        }

        /* Assign address to socket */
        if ( bind( le_socket, ( struct sockaddr * ) ( & le_addr ), sizeof( struct sockaddr_in ) ) == _LE_SOCK_NULL ) {

            /* Close socket */
            close( le_socket );

            /* Send message */
            return( _LE_SOCK_NULL );

        }
        
        /* Make socket ready to accept connection */
        if ( listen( le_socket, _LE_USE_PENDING ) == _LE_SOCK_NULL ) {

            /* Close socket */
            close( le_socket );

            /* Send message */
            return( _LE_SOCK_NULL );

        }

        /* Return listening socket */
        return( le_socket );

    }

    le_sock_t le_server_delete( le_sock_t const le_socket ) {

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

    le_enum_t le_server_handshake_mode( le_sock_t const le_socket ) {

        /* Handshake buffer variable */
        le_byte_t le_buffer = LE_NETWORK_MODE_NULL;

        /* Read handshake */
        if ( read( le_socket, & le_buffer, sizeof( le_byte_t ) ) != sizeof( le_byte_t ) ) {

            /* Send message */
            return( LE_NETWORK_MODE_NULL );

        } else {

            /* Send received mode */
            return( ( le_enum_t ) le_buffer );

        }

    }

    le_enum_t le_server_handshake_auth( le_sock_t const le_socket, le_enum_t const le_auth ) {

        /* Handshake buffer variable */
        le_byte_t le_buffer = ( le_byte_t ) le_auth;

        /* Write handshake authorisation */
        if ( write( le_socket, & le_buffer, sizeof( le_byte_t ) ) != sizeof( le_byte_t ) ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

/*
    source - server methods
 */

    le_void_t le_server( le_sock_t const le_socket, le_system_t * const le_system ) {

        /* Client socket variables */
        le_sock_t le_client = _LE_SOCK_NULL;

        /* Client address variables */
        struct sockaddr_in le_addr = LE_SOCKADDR_IN_C;

        /* Client address length variables */
        socklen_t le_len = sizeof( struct sockaddr_in );

        /* Check socket */
        if ( le_socket != _LE_SOCK_NULL ) {

            /* Incoming connection handle */
            for ( ; ; ) {

                /* Accept incoming connection */
                if ( ( le_client = accept( le_socket, ( struct sockaddr * ) & le_addr, & le_len ) ) != _LE_SOCK_NULL ) {

                    /* Switch on handshake */
                    switch ( le_server_handshake_mode( le_client ) ) {

                        /* Injection mode */
                        case ( LE_NETWORK_MODE_IMOD ) : {

                            /* Send authorisation */
                            if ( le_server_handshake_auth( le_client, LE_NETWORK_MODE_IATH ) == LE_ERROR_SUCCESS ) {

                                /* Connection to system injection */
                                le_server_system_inject( le_client, le_system );

                            }

                        } break;

                        /* Query mode */
                        case ( LE_NETWORK_MODE_QMOD ) : {

                            /* Send authorisation */
                            if ( le_server_handshake_auth( le_client, LE_NETWORK_MODE_QATH ) == LE_ERROR_SUCCESS ) {

                                /* Connection to system query */
                                le_server_system_query( le_client, le_system );

                            }

                        } break;

                        /* System spatial configuration mode */
                        case ( LE_NETWORK_MODE_SMOD ) : {

                            /* Send authorisation */
                            if ( le_server_handshake_auth( le_client, LE_NETWORK_MODE_SATH ) == LE_ERROR_SUCCESS ) {

                                /* Connection to system configuration */
                                le_server_system_sdisc( le_client, le_system );

                            }

                        } break;

                        /* System temporal configuration mode */
                        case ( LE_NETWORK_MODE_TMOD ) : {

                            /* Send authorisation */
                            if ( le_server_handshake_auth( le_client, LE_NETWORK_MODE_TATH ) == LE_ERROR_SUCCESS ) {

                                /* Connection to system configuration */
                                le_server_system_tdisc( le_client, le_system );

                            }

                        } break;

                    }

                }

                /* Close client connection */
                close( le_client );

            }

        }

    }

/*
    source - connection methods
 */

    le_void_t le_server_system_inject( le_sock_t const le_socket, le_system_t * const le_system ) {

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Socket i/o count variables */
        le_size_t le_count = 0;

        /* Socket i/o buffer variables */
        le_byte_t le_buffer[LE_NETWORK_BUFFER_SYNC] = LE_NETWORK_BUFFER_C;

        /* Array pointer variables */
        le_real_t * le_ptrp = NULL;
        le_time_t * le_ptrt = NULL;
        le_data_t * le_ptrd = NULL;

        /* Reading incoming elements */
        while ( ( le_count = read( le_socket, le_buffer, LE_NETWORK_BUFFER_SYNC ) ) > 0 ) {

            /* Parsing received elements */
            for ( le_parse = 0; le_parse < le_count; le_parse += LE_ARRAY_LINE ) {

                /* Compute element pointers */
                le_ptrp = ( le_real_t * ) ( le_buffer + le_parse );
                le_ptrt = ( le_time_t * ) ( le_ptrp + 3 );
                le_ptrd = ( le_data_t * ) ( le_ptrt + 1 );

                /* Inject received element */
                le_system_inject( le_system, le_ptrp, * le_ptrt, le_ptrd );

            }

        }

    }

    le_enum_t le_server_system_query( le_sock_t const le_socket, le_system_t * const le_system ) {

        /* Message variables */
        le_enum_t le_message = LE_ERROR_SUCCESS;

        /* Array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Address variables */
        le_address_t le_address = LE_ADDRESS_C;

        /* Socket i/o buffer variables */
        le_byte_t le_buffer[LE_NETWORK_BUFFER_ADDR] = LE_NETWORK_BUFFER_C;

        /* Read query address */
        if ( read( le_socket, le_buffer, LE_NETWORK_BUFFER_ADDR ) != LE_NETWORK_BUFFER_ADDR ) {

            /* Send message */
            return( LE_ERROR_IO_READ );

        }

        /* Decompose address string */
        le_address_cvsa( & le_address, le_buffer );

        /* Send system query */
        le_array = le_system_query( le_system, & le_address );

        /* Write array to socket */
        le_message = le_array_io_write( & le_array, le_socket );

        /* Unallocate array memory */
        le_array_delete( & le_array );

        /* Send message */
        return( le_message );

    }

    le_enum_t le_server_system_sdisc( le_sock_t const le_socket, le_system_t const * const le_system ) {

        /* Space discretisation i/o variables */
        le_size_t le_sdisc = le_system_get_sdisc( le_system );

        /* Write configuration */
        if ( write( le_socket, & le_sdisc, sizeof( le_size_t ) ) != sizeof( le_size_t ) ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* Send message */
            return( LE_ERROR_SUCCESS );

        }

    }

    le_enum_t le_server_system_tdisc( le_sock_t const le_socket, le_system_t const * const le_system ) {

        /* Time discretisation i/o variables */
        le_time_t le_tdisc = le_system_get_tdisc( le_system );

        /* Write configuration */
        if ( write( le_socket, & le_tdisc, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) {

            /* Send message */
            return( LE_ERROR_IO_WRITE );

        } else {

            /* Send messsage */
            return( LE_ERROR_SUCCESS );

        }

    }

