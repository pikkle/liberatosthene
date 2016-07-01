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
        
        /* Create socket - send message */
        if ( ( le_socket = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) return( _LE_SOCK_NULL );

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

        /* Check socket - close socket */
        if ( le_socket != _LE_SOCK_NULL ) close( le_socket );

        /* Return null socket */
        return( _LE_SOCK_NULL );

    }

/*
    source - handshake and authorisation methods
 */

    le_hand_t le_server_handshake( le_sock_t const le_socket ) {

        /* Handshake buffer variables */
        le_hand_t le_buffer = _LE_HAND_NULL;

        /* Read handshake - send message */
        if ( read( le_socket, & le_buffer, sizeof( le_hand_t ) ) != sizeof( le_hand_t ) ) return( _LE_HAND_NULL );

        /* Return received handshake */
        return( le_buffer );

    }

    le_enum_t le_server_authorise( le_sock_t const le_socket, le_enum_t const le_auth ) {

        /* Handshake buffer variables */
        le_hand_t le_buffer = le_auth;

        /* Write authorisation on socket - send message */
        if ( write( le_socket, & le_buffer, sizeof( le_hand_t ) ) != sizeof( le_hand_t ) ) return( LE_ERROR_IO_WRITE );

        /* Send authorisation */
        return( le_auth == _LE_HAND_NULL ? LE_ERROR_AUTH : LE_ERROR_SUCCESS );

    }

/*
    source - server methods
 */

    le_void_t le_server( le_sock_t const le_socket, le_system_t * const le_system ) {

        /* Client socket variables */
        le_sock_t le_client = _LE_SOCK_NULL;

        /* Client handshake variables */
        le_hand_t le_hand = _LE_HAND_NULL;

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
                    switch ( ( le_hand = le_server_handshake( le_client ) ) & 0xff ) {

                        /* Injection mode */
                        case ( LE_NETWORK_MODE_IMOD ) : {

                            /* Send authorisation */
                            if ( le_server_authorise( le_client, le_system_set_format( le_system, le_hand ) ) == LE_ERROR_SUCCESS ) {

                                /* Connection to system injection */
                                le_server_system_inject( le_client, le_system );

                            }

                        } break;

                        /* Query mode */
                        case ( LE_NETWORK_MODE_QMOD ) : {

                            /* Send authorisation */
                            if ( le_server_authorise( le_client, le_system_set_format( le_system, le_hand ) ) == LE_ERROR_SUCCESS ) {

                                /* Connection to system query */
                                le_server_system_query( le_client, le_system );

                            }

                        } break;

                        /* System times array mode */
                        case ( LE_NETWORK_MODE_AMOD ) : {

                            /* Send authorisation */
                            if ( le_server_authorise( le_client, le_system_set_format( le_system, le_hand ) ) == LE_ERROR_SUCCESS ) {

                                /* Connection to system times */
                                le_server_system_times( le_client, le_system );

                            }

                        } break;

                        /* System spatial configuration mode */
                        case ( LE_NETWORK_MODE_SMOD ) : {

                            /* Send authorisation */
                            if ( le_server_authorise( le_client, le_system_set_format( le_system, le_hand ) ) == LE_ERROR_SUCCESS ) {

                                /* Connection to system configuration */
                                le_server_system_sparam( le_client, le_system );

                            }

                        } break;

                        /* System temporal configuration mode */
                        case ( LE_NETWORK_MODE_TMOD ) : {

                            /* Send authorisation */
                            if ( le_server_authorise( le_client, le_system_set_format( le_system, le_hand ) ) == LE_ERROR_SUCCESS ) {

                                /* Connection to system configuration */
                                le_server_system_tparam( le_client, le_system );

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
    source - system methods
 */

    le_void_t le_server_system_inject( le_sock_t const le_socket, le_system_t * const le_system ) {

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Socket i/o count variables */
        le_size_t le_count = 0;
        le_size_t le_round = 0;

        /* Socket i/o reading variables */
        le_size_t le_retry  = 0;
        le_size_t le_bridge = 0;

        /* Socket i/o circular buffer variables */
        le_byte_t le_buffer[LE_NETWORK_SB_STRM] = LE_NETWORK_C;

        /* Buffer array variables */
        le_real_t * le_sp = NULL;
        le_time_t * le_tp = NULL;
        le_data_t * le_dp = NULL;

        /* Injection streaming loop */
        while ( le_retry < _LE_USE_RETRY ) {

            /* Read streaming bloc */
            if ( ( le_count = read( le_socket, le_buffer + le_bridge, _LE_USE_MTU ) + le_bridge ) >= LE_ARRAY_64S_LEN ) {

                /* Compute available records */
                le_round = le_count - ( le_count % LE_ARRAY_64S_LEN );

                /* Parsing received streaming bloc */
                for ( le_parse = 0; le_parse < le_round; le_parse += LE_ARRAY_64S_LEN ) {

                    /* Compute buffer pointers */
                    le_sp = ( le_real_t * ) ( le_buffer + le_parse );
                    le_tp = ( le_time_t * ) ( le_sp + 3 );
                    le_dp = ( le_data_t * ) ( le_tp + 1 );

                    /* Inject received element */
                    le_system_inject( le_system, le_sp, * le_tp, le_dp );

                }

                /* Compute bridge value */
                if ( ( le_bridge = le_count % LE_ARRAY_64S_LEN ) != 0 ) {

                    /* Apply circular condition */
                    memcpy( le_buffer, le_buffer + le_count - le_bridge, le_bridge );

                }                

            /* Update retry flag */
            le_retry = 0; } else { le_retry ++; }

        }

    }

    le_enum_t le_server_system_query( le_sock_t const le_socket, le_system_t * const le_system ) {

        /* Message variables */
        le_enum_t le_message = LE_ERROR_SUCCESS;

        /* Array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Address variables */
        le_address_t le_address = LE_ADDRESS_C;

        /* Read and decompose query */
        le_address_io_read( & le_address, le_socket );

        /* Send system query */
        le_array = le_system_query( le_system, & le_address );

        /* Write array to socket */
        le_message = le_array_io_write( & le_array, le_socket );

        /* Unallocate array memory */
        le_array_delete( & le_array );

        /* Send message */
        return( le_message );

    }

    le_enum_t le_server_system_times( le_sock_t const le_socket, le_system_t const * const le_system ) {

        /* Returned value variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* Array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Gather system time equivalences classes */
        le_array = le_system_times( le_system );

        /* Write array on socket */
        le_return = le_array_io_write( & le_array, le_socket );

        /* Delete array */
        le_array_delete( & le_array );

        /* Send message */
        return( le_return );

    }

    le_enum_t le_server_system_sparam( le_sock_t const le_socket, le_system_t const * const le_system ) {

        /* Space discretisation i/o variables */
        le_size_t le_sparam = le_system_get_sparam( le_system );

        /* Write configuration - send messasge */
        if ( write( le_socket, & le_sparam, sizeof( le_size_t ) ) != sizeof( le_size_t ) ) return( LE_ERROR_IO_WRITE );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_enum_t le_server_system_tparam( le_sock_t const le_socket, le_system_t const * const le_system ) {

        /* Time discretisation i/o variables */
        le_time_t le_tparam = le_system_get_tparam( le_system );

        /* Write configuration - send message */
        if ( write( le_socket, & le_tparam, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) return( LE_ERROR_IO_WRITE );

        /* Send messsage */
        return( LE_ERROR_SUCCESS );

    }

