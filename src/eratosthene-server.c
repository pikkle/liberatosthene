/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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

    le_server_t le_server_create( le_char_t * const le_path, le_sock_t const le_port ) {

        /* created structure variables */
        le_server_t le_server = LE_SERVER_C;

        /* address variables */
        struct sockaddr_in le_addr = LE_ADDRIN_C_PORT( le_port );

        /* check consistency */
        if ( le_path == NULL ) {

            /* send message */
            return( le_server._status = LE_ERROR_IO_ACCESS, le_server );

        }

        /* assign server path */
        le_server.sv_path = le_path;

        /* create server configuration */
        if ( ( le_server._status = le_server_set_config( & le_server ) ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( le_server );

        }

        /* create server socket */
        if ( ( le_server.sv_sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) {

            /* send message */
            return( le_server._status = LE_ERROR_IO_SOCKET, le_server );

        } else {

            /* server socket address */
            if ( bind( le_server.sv_sock, ( struct sockaddr * ) ( & le_addr ), sizeof( struct sockaddr_in ) ) == _LE_SOCK_NULL ) {

                /* close server socket */
                close( le_server.sv_sock );

                /* send message */
                return( le_server._status = LE_ERROR_IO_BIND, le_server );

            }

            /* server socket state */
            if ( listen( le_server.sv_sock, _LE_USE_PENDING ) == _LE_SOCK_NULL ) {

                /* close server socket */
                close( le_server.sv_sock );

                /* send message */
                return( le_server._status = LE_ERROR_IO_LISTEN, le_server );

            }

        }

        /* return created structure */
        return( le_server );

    }

    le_void_t le_server_delete( le_server_t * const le_server ) {

        /* deleted structure variables */
        le_server_t le_delete = LE_SERVER_C;

        /* check socket */
        if ( le_server->sv_sock != _LE_SOCK_NULL ) {

            /* close socket */
            close( le_server->sv_sock );

        }

        /* delete structure */
        ( * le_server ) = le_delete;

    }

/*
    source - mutator methods
 */

    le_enum_t le_server_set_config( le_server_t * const le_server ) {

        /* server path variables */
        le_size_t le_plen = strlen( ( char * ) le_server->sv_path );

        /* stream variables */
        FILE * le_stream = NULL;

        /* open configuration stream */
        if ( ( le_stream = fopen( strcat( ( char * ) le_server->sv_path, "/system" ), "r" ) ) == NULL ) {

            /* send message */
            return( LE_ERROR_IO_ACCESS );

        }

        /* read configuration parameter */
        if ( fscanf( le_stream, "%" _LE_SIZE_S, & le_server->sv_scfg ) != 1 ) {

            /* close stream */
            fclose( le_stream );

            /* send message */
            return( LE_ERROR_IO_READ );

        }

        /* read configuration parameter */
        if ( fscanf( le_stream, "%" _LE_TIME_S, & le_server->sv_tcfg ) != 1 ) {

            /* close stream */
            fclose( le_stream );

            /* send message */
            return( LE_ERROR_IO_READ );

        }

        /* close stream */
        fclose( le_stream );

        /* check parameter consistency */
        if ( ( le_server->sv_scfg <= 0 ) || ( le_server->sv_scfg >= _LE_USE_DEPTH ) ) {

            /* send message */
            return( LE_ERROR_DEPTH );

        }

        /* check parameter consistency */
        if ( le_server->sv_tcfg <= 0 ) {

            /* send message */
            return( LE_ERROR_TIME );

        }

        /* remove configuration from path */
        le_server->sv_path[le_plen] = '\0';

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - i/o methods - principale
 */

    le_void_t le_server_io( le_server_t * const le_server ) {

        /* client address variables */
        struct sockaddr_in le_addr = LE_ADDRIN_C;

        /* client address variables */
        socklen_t le_len = sizeof( struct sockaddr_in );

        /* thread boxes variables */
        le_box_t le_boxes[_LE_USE_PENDING];

        /* parsing boxes */
        for ( le_size_t le_parse = 0; le_parse < _LE_USE_PENDING; le_parse ++ ) {

            /* assign null socket */
            le_boxes[le_parse].bx_sock = _LE_SOCK_NULL;

            /* assign server structure */
            le_boxes[le_parse].bx_srvp = ( le_void_t * ) le_server;

        }

        /* client connection handler */
        for ( le_size_t le_index = 0; le_index < _LE_USE_PENDING ; le_index = ( le_index + 1 ) % _LE_USE_PENDING ) {

            /* check box state */
            if ( le_boxes[le_index].bx_sock != _LE_SOCK_NULL ) continue;

            /* waiting clients connections */
            if ( ( le_boxes[le_index].bx_sock = accept( le_server->sv_sock, ( struct sockaddr * ) ( & le_addr ), & le_len ) ) != _LE_SOCK_NULL ) {

                /* create clients thread */
                pthread_create( & le_boxes[le_index].bx_proc, NULL, & le_server_io_client, ( le_void_t * ) & le_boxes[le_index] );

            }

        }

    }

    le_void_t * le_server_io_client( le_void_t * le_box_ ) {

        /* connection variables */
        le_enum_t le_active = _LE_TRUE;

        /* thread box variables */
        le_box_t * le_box = ( le_box_t * ) le_box_;

        /* server variables */
        le_server_t * le_server = le_box->bx_srvp;

        /* stream variables */
        le_stream_t le_stream = LE_STREAM_C;

        /* create stream structure */
        le_stream = le_stream_create( le_server->sv_path, le_server->sv_scfg, le_server->sv_tcfg );

        /* client connection handler */
        while ( le_active == _LE_TRUE ) {

        /* switch on handshake */
        switch ( le_client_switch( le_box->bx_sock ) ) {

                /* system connection */
                case ( LE_MODE_AMOD ) : {

                    /* check stream consistency */
                    if ( le_stream._status == LE_ERROR_SUCCESS ) {

                        /* send client authorisation */
                        le_client_authorise( le_box->bx_sock, LE_MODE_AMOD );

                    } else {

                        /* send client failure */
                        le_client_authorise( le_box->bx_sock, LE_MODE_NULL );

                        /* disconnect client */
                        le_active = _LE_FALSE;

                    }

                } break;

                /* system disconnection */
                case ( LE_MODE_BMOD ) : {

                    /* send client authorisation */
                    le_client_authorise( le_box->bx_sock, LE_MODE_BMOD );

                    /* disconnect client */
                    le_active = _LE_FALSE;

                } break;

                /* system injection */
                case ( LE_MODE_IMOD ) : {

                    /* send authorisation */
                    if ( le_client_authorise( le_box->bx_sock, LE_MODE_IMOD ) == LE_ERROR_SUCCESS ) {

                        /* connection to system injection */
                        le_server_io_inject( le_server, le_box->bx_sock, & le_stream );

                    }

                } break;

                /* system reduction */
                case ( LE_MODE_RMOD ) : {

                    /* send authorisation */
                    if ( le_client_authorise( le_box->bx_sock, LE_MODE_RMOD ) == LE_ERROR_SUCCESS ) {

                        /* connection to system query */
                        le_server_io_reduce( le_server, le_box->bx_sock, & le_stream );

                    }

                } break;

                /* system query */
                case ( LE_MODE_QMOD ) : {

                    /* send authorisation */
                    if ( le_client_authorise( le_box->bx_sock, LE_MODE_QMOD ) == LE_ERROR_SUCCESS ) {

                        /* connection to system query */
                        le_server_io_query( le_server, le_box->bx_sock, & le_stream );

                    }

                } break;

                /* system configuration */
                case ( LE_MODE_CMOD ) : {

                    /* send authorisation */
                    if ( le_client_authorise( le_box->bx_sock, LE_MODE_CMOD ) == LE_ERROR_SUCCESS ) {

                        /* connection to system */
                        le_server_io_config( le_server, le_box->bx_sock, & le_stream );

                    }

                } break;

            }

        }

        /* delete stream structure */
        le_stream_delete( & le_stream );

        /* close client socket */
        close( le_box->bx_sock );

        /* release thread box */
        return( le_box->bx_sock = _LE_SOCK_NULL, NULL );

    }

/*
    source - i/o methods
 */

    le_void_t le_server_io_inject( le_server_t * const le_server, le_sock_t const le_client, le_stream_t * const le_stream ) {

        /* array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* time variables */
        le_time_t le_time = _LE_TIME_NULL;

        /* stream variables */
        le_size_t le_index = _LE_SIZE_NULL;

        /* read time */
        if ( read( le_client, & le_time, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) {

            /* abort injection */
            return;

        }

        /* read client array */
        if ( le_array_io_read( & le_array, le_client ) != LE_ERROR_SUCCESS ) {

            /* abort injection */
            return;

        }

        /* check consistency - security check */
        if ( le_array_get_size( & le_array ) < LE_ARRAY_SD ) {

            /* abort injection */
            return;

        }

        /* check consistency - security check */
        if ( ( le_array_get_size( & le_array ) % LE_ARRAY_SD ) != 0 ) {

            /* abort injection */
            return;

        }

        /* create stream */
        if ( ( le_index = le_stream_get_strict( le_stream, le_time, LE_STREAM_WRITE ) ) != _LE_SIZE_NULL ) {

            /* inject array */
            le_stream_io_inject( le_stream, le_index, & le_array );

        }

        /* delete array */
        le_array_delete( & le_array );

    }

    le_void_t le_server_io_reduce( le_server_t * const le_server, le_sock_t const le_client, le_stream_t * const le_stream ) {

        /* address variables */
        le_address_t le_addr = LE_ADDRESS_C;

        /* read address */
        le_address_io_read( & le_addr, le_client );

        /* check consistency */
        if ( ( le_address_get_size( & le_addr ) + le_address_get_span( & le_addr ) ) >= le_server->sv_scfg ) {

            /* abort query */
            return;

        }

        /* check address mode */
        if ( le_address_get_mode( & le_addr ) != 2 ) {

            /* address reduction */
            le_stream_get_reduct( le_stream, & le_addr, 0, NULL );

        }

        /* check address mode */
        if ( le_address_get_mode( & le_addr ) != 1 ) {

            /* address reduction */
            le_stream_get_reduct( le_stream, & le_addr, 1, NULL );

        }

        /* write address */
        le_address_io_write( & le_addr, le_client );

    }

    le_void_t le_server_io_query( le_server_t const * const le_server, le_sock_t const le_client, le_stream_t * const le_stream ) {

        /* array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* address variables */
        le_address_t le_addr = LE_ADDRESS_C;

        /* address variables */
        le_size_t le_size = 0;
        le_size_t le_span = 0;

        /* offset variables */
        le_size_t le_offseta = _LE_OFFS_NULL;
        le_size_t le_offsetb = _LE_OFFS_NULL;

        /* stream variables */
        le_size_t le_streama = _LE_SIZE_NULL;
        le_size_t le_streamb = _LE_SIZE_NULL;

        /* read query address */
        le_address_io_read( & le_addr, le_client );

        /* extract address parameters */
        le_size = le_address_get_size( & le_addr );
        le_span = le_address_get_span( & le_addr ) + le_size;

        /* check consistency */
        if ( le_span >= le_server->sv_scfg ) {

            /* abort query */
            return;

        }

        /* check address mode */
        if ( le_address_get_mode( & le_addr ) == 1 ) {

            /* create and check stream */
            if ( ( le_streama = le_stream_get_reduct( le_stream, & le_addr, 0, & le_offseta ) ) != _LE_SIZE_NULL ) {

                /* gathering daughters representative */
                le_stream_io_gather( le_stream, le_streama, & le_addr, le_offseta, le_size, le_span, & le_array );

            }

        } else if ( le_address_get_mode( & le_addr ) == 2 ) {

            /* create and check stream */
            if ( ( le_streamb = le_stream_get_reduct( le_stream, & le_addr, 1, & le_offsetb ) ) != _LE_SIZE_NULL ) {

                /* gathering daughters representative */
                le_stream_io_gather( le_stream, le_streamb, & le_addr, le_offsetb, le_size, le_span, & le_array );

            }

        } else {

            /* create streams */
            le_streama = le_stream_get_reduct( le_stream, & le_addr, 0, & le_offseta );
            le_streamb = le_stream_get_reduct( le_stream, & le_addr, 1, & le_offsetb );

            /* check streams */
            if ( ( le_streama != _LE_SIZE_NULL ) || ( le_streamb != _LE_SIZE_NULL ) ) {

                /* gathering daughters representative */
                le_stream_io_parallel( le_stream, le_streama, le_streamb, & le_addr, le_offseta, le_offsetb, le_size, le_span, & le_array );

            }

        }

        /* write query address on socket */
        le_address_io_write( & le_addr, le_client );

        /* purge array to socket */
        le_array_io_write( & le_array, le_client );

        /* unallocate array memory */
        le_array_delete( & le_array );

    }

    le_void_t le_server_io_config( le_server_t const * const le_server, le_sock_t const le_client, le_stream_t * const le_stream ) {

        /* server configuration array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* compose configuration array */
        le_array_map_dt( & le_array, le_server->sv_scfg, le_server->sv_tcfg );

        /* write array on socket */
        le_array_io_write( & le_array, le_client );

        /* delete array */
        le_array_delete( & le_array );

    }

