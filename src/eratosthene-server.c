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

    # include "eratosthene-server.h"

/*
    source - common methods
 */

    le_sock_t le_server_client_open( le_char_t const * const le_hostname, le_sock_t const le_port ) {

        /* Host entity variables */
        struct hostent * le_host = NULL;

        /* Host address variables */
        struct sockaddr_in le_addr;

        /* Socket variables */
        le_sock_t le_socket = LE_SERVER_NULL;

        /* Check socket */
        if ( ( le_socket = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {

            /* Send message */
            return( LE_SERVER_NULL );

        }

        /* Resovle hostname */
        if ( ( le_host = gethostbyname( ( char * ) le_hostname ) ) == NULL ) {

            /* Close socket */
            close( le_socket );

            /* Send message */
            return( LE_SERVER_NULL );

        }

        /* Create address structure */
        le_addr.sin_family = AF_INET;
        le_addr.sin_port   = htons( le_port );

        /* Copy IP address */
        memcpy( & le_addr.sin_addr, le_host->h_addr_list[0], le_host->h_length );

        /* Create connection */
        if ( connect( le_socket, ( struct sockaddr * ) & le_addr, sizeof( le_addr ) ) < 0 ) {

            /* Close socket */
            close( le_socket );

            /* Send message */
            return( LE_SERVER_NULL );

        }

        /* Return created socket */
        return( le_socket );

    }

    le_void_t le_server_client_close( le_sock_t const le_socket ) {

        /* Close socket */
        close( le_socket );

    }

/*
    source - processing methods
 */

    le_void_t le_server_inject( le_system_t * const le_system, int const le_client ) {

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Socket i/o count variables */
        le_size_t le_count = 0;

        /* Socket i/o pointers variables */
        le_real_t * le_pose = NULL;
        le_data_t * le_data = NULL;
        le_time_t * le_time = NULL;

        /* Socket i/o buffer variables */
        le_byte_t le_buffer[LE_SERVER_DGMAX] = { 0 };

        /* Read incoming elements */
        while ( ( le_count = read( le_client, le_buffer, LE_SERVER_DGMAX ) ) > 0 ) {

            /* Inject element in system */
            for ( le_parse = 0; le_parse < le_count; le_parse += LE_SERVER_ELEN ) {

                /* Compute i/o pointers */
                le_pose = ( le_real_t * ) ( le_buffer + le_parse );
                le_time = ( le_time_t * ) ( le_pose + 3 );
                le_data = ( le_data_t * ) ( le_time + 1 );

                /* Element injection */
                le_system_inject( le_system, le_pose, * le_time, le_data );

            }

        }

        /* Force stream cache export */
        le_system_flush( le_system );

    }

    le_enum_t le_server_query( le_system_t * const le_system, int const le_client ) {

        /* Query structure variables */
        le_array_t le_query = LE_ARRAY_C;

        /* Socket i/o count variables */
        le_size_t le_count = 0;

        /* Socket i/o buffer variables */
        le_byte_t le_buffer = 0;

        /* Parsing variables */
        le_size_t le_parse = 0;

        /* Query depth variables */
        le_size_t le_depth = 0;

        /* Query time variables */
        le_time_t le_time = 0;

        /* Query address variables */
        le_address_t le_address = LE_ADDRESS_C;

        /* Read query time */
        if ( ( le_count = read( le_client, & le_time, sizeof( le_time_t ) ) ) != sizeof( le_time_t ) ) {

            /* Send message */
            return( LE_ERROR_QUERY );

        }

        /* Read query address length */
        if ( ( le_count = read( le_client, & le_buffer, sizeof( le_byte_t ) ) ) != sizeof( le_byte_t ) ) {

            /* Send message */
            return( LE_ERROR_QUERY );

        }

        /* Assign address length */
        le_address_set_size( & le_address, ( le_size_t ) le_buffer );

        /* Read query address digits */
        if ( ( le_count = read( le_client, le_address_get_digits( & le_address ), _LE_USE_DEPTH ) ) != _LE_USE_DEPTH ) {

            /* Send message */
            return( LE_ERROR_QUERY );

        }

        /* Read query depth */
        if ( ( le_count = read( le_client, & le_depth, sizeof( le_depth ) ) ) != sizeof( le_size_t ) ) {

            /* Send message */
            return( LE_ERROR_QUERY );

        }

        /* Perform query */
        le_query = le_system_query( le_system, le_time, & le_address, le_depth );

        /* Check query results */
        if ( le_array_get_size( & le_query ) > 0 ) {

            /* Parsing query results */
            for ( le_parse = 0; le_parse < le_array_get_size( & le_query ); le_parse += LE_SERVER_DGMAX ) {

                /* Compute segment size */
                le_count = ( le_parse + LE_SERVER_DGMAX ) >= le_array_get_size( & le_query ) ? LE_SERVER_DGMAX - ( ( le_parse + LE_SERVER_DGMAX ) - le_array_get_size( & le_query ) ) : LE_SERVER_DGMAX;

                /* Send segment to socket */
                if ( write( le_client, le_array_get_byte( & le_query ) + le_parse, le_count ) != le_count ) {

                    /* Warning */
                    fprintf( stderr, "DEBUG : warning on socket write\n" );

                }

            }

            /* Delete query array */
            LE_ARRAY_D( & le_query ); 

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - server
 */

    le_enum_t le_server( le_system_t * const le_system ) {

        /* Socket handles variables */
        int le_server = -1;
        int le_client = -1;

        /* Socket length variables */
        socklen_t le_slen = 0;

        /* TCP/IP buffer variables */
        le_byte_t le_buffer = 0;

        /* TCP/IP address variables */
        struct sockaddr_in le_saddr;
        struct sockaddr_in le_caddr;

        /* Create server socket */
        if ( ( le_server = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {

            /* Send message */
            return( LE_ERROR_SOCKET );

        }

        /* Address configuration */
        le_saddr.sin_family = AF_INET;
        le_saddr.sin_addr.s_addr = INADDR_ANY;
        le_saddr.sin_port = htons( LE_SERVER_PORT );

        /* Bind socket */
        if ( bind( le_server, ( struct sockaddr * ) & le_saddr, sizeof( le_saddr ) ) < 0 ) {

            /* Send message */
            return( LE_ERROR_BINDING );

        }
        
        /* TCP/IP listening configuration */
        listen( le_server, 5 );

        /* TCP/IP listening */
        while ( LE_SERVER_ANSWER == 42 ) {

            /* Waiting incomming connection */
            if ( ( le_client = accept( le_server, ( struct sockaddr * ) & le_caddr, & le_slen ) ) >= 0 ) {

                /* Secondary handshake */
                if ( read( le_client, & le_buffer, sizeof( le_buffer ) ) == sizeof( le_buffer ) ) {

                    /* Switch on connection type */
                    switch ( le_buffer ) {

                        /* Injection connection */
                        case ( LE_SERVER_CNINJ ) : {

                            /* Validate connection */
                            le_buffer = LE_SERVER_VALID;

                            /* Write secondary handshake answer */
                            if ( write( le_client, & le_buffer, sizeof( le_byte_t ) ) == sizeof( le_buffer ) ) {

                                /* Injection connection management */
                                le_server_inject( le_system, le_client );

                            }

                        } break;

                        /* Query connection */
                        case ( LE_SERVER_CNQRY ) : {

                            /* Validate connection */
                            le_buffer = LE_SERVER_VALID;

                            /* Write secondary handshake answer */
                            if ( write( le_client, & le_buffer, sizeof ( le_byte_t ) ) == sizeof( le_byte_t ) ) {

                                /* Query connection management */
                                le_server_query( le_system, le_client );

                            }

                        } break;

                    }

                }

                /* Close client socket */
                close( le_client );

            }            

        }

        /* Close server socket */
        close( le_server );

        /* Return null pointer */
        return( LE_ERROR_SUCCESS );

    }

