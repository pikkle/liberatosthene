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

        /* initialise boxes */
        for ( le_size_t le_parse = 0; le_parse < _LE_USE_PENDING; le_parse ++ ) {

            /* assign null socket */
            le_boxes[le_parse].bx_sock = _LE_SOCK_NULL;

            /* assign server structure */
            le_boxes[le_parse].bx_srvp = ( le_void_t * ) le_server;

        }

        /* client connexion handler */
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

        /* thread box variables */
        le_box_t * le_box = ( le_box_t * ) le_box_;

        /* server structure variables */
        le_server_t * le_server = ( le_server_t * ) le_box->bx_srvp;

        /* client state variables */
        le_enum_t le_active = _LE_TRUE;

        /* authentication flag variables */
        le_enum_t le_unlock = _LE_FALSE;

        /* stream structure variables */
        le_stream_t le_stream = LE_STREAM_C;

        /* socket-array stack variables */
        le_array_t le_stack[_LE_USE_ARRAY];

        /* initialise socket-array stack */
        for ( le_size_t le_parse = 0; le_parse < _LE_USE_ARRAY; le_parse ++ ) {

            /* initialise socket-array */
            le_stack[le_parse] = le_array_create();

        }

        /* create stream structure */
        le_stream = le_stream_create( le_server->sv_path, le_server->sv_scfg, le_server->sv_tcfg );

        /* client management */
        while ( le_active == _LE_TRUE ) {

            /* waiting client socket-array */
            switch( le_array_io_read( le_stack, le_box->bx_sock ) ) {

                /* client/server authentication */
                case ( LE_MODE_AUTH ) : {

                    /* check authentication lock */
                    if ( le_unlock == _LE_FALSE ) {

                        /* mode management */
                        le_active = le_server_io_agreement( le_server, & le_stream, le_stack, le_box->bx_sock );

                        /* update authentication lock */
                        le_unlock = le_active;

                    /* resume client connection */
                    } else { le_active = _LE_FALSE; }

                } break;

                /* client/server resiliation */
                case ( LE_MODE_RESI ) : {

                    /* check authentication lock */
                    if ( le_unlock == _LE_TRUE ) {

                        /* mode management */
                        le_active = le_server_io_resiliate( le_server, & le_stream, le_stack, le_box->bx_sock );

                    /* resume client connection */
                    } else { le_active = _LE_FALSE; }

                } break;

                /* client/server query */
                case ( LE_MODE_QUER ) : {

                    /* check authentication lock */
                    if ( le_unlock == _LE_TRUE ) {

                        /* mode management */
                        le_active = le_server_io_query( le_server, & le_stream, le_stack, le_box->bx_sock );

                    /* resume client connection */
                    } else { le_active = _LE_FALSE; }

                } break;

                /* client/server injection */
                case ( LE_MODE_INJE ) : {

                    /* check authentication lock */
                    if ( le_unlock == _LE_TRUE ) {

                        /* mode management */
                        le_active = le_server_io_inject( le_server, & le_stream, le_stack, le_box->bx_sock );

                    /* resume client connection */
                    } else { le_active = _LE_FALSE; }

                } break;

                /* client/server unexpected mode */
                default : {

                    /* resume client connection */
                    le_active = _LE_FALSE;

                } break;

            };

        }

        /* delete stream structure */
        le_stream_delete( & le_stream );

        /* close client socket */
        close( le_box->bx_sock );

        /* release thread */
        return( le_box->bx_sock = _LE_SOCK_NULL, NULL );

    }

/*
    source - i/o methods
 */

    le_enum_t le_server_io_agreement( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* agreement value variables */
        le_size_t le_agree = LE_AGRT_NULL;

        /* serialisation variables */
        le_size_t le_head = 0;

        /* serialise authentication */
        le_array_serial( le_stack, & le_agree, sizeof( le_size_t ), 0, _LE_GET );

        /* check authentication */
        if ( le_agree == LE_AGRT_QUER ) {

            /* update agreement value */
            le_agree = LE_AGRT_AUTH;

        }

        /* check additionnal consistency */
        if ( le_array_get_size( le_stack ) != sizeof( le_size_t ) ) {

            /* update agreement value */
            le_agree = LE_AGRT_NULL;

        }

        /* check stream state */
        if ( le_stream->_status != LE_ERROR_SUCCESS ) {

            /* update agreement value */
            le_agree = LE_AGRT_NULL;

        }

        /* resize socket-array */
        le_array_set_size( le_stack, LE_ARRAY_AUTH );

        /* serialise authentication */
        le_head = le_array_serial( le_stack, & le_agree, sizeof( le_size_t ), le_head, _LE_SET );

        /* serialise confirguration */
        le_head = le_array_serial( le_stack, & le_server->sv_scfg, sizeof( le_size_t ), le_head, _LE_SET );
        le_head = le_array_serial( le_stack, & le_server->sv_tcfg, sizeof( le_time_t ), le_head, _LE_SET );

        /* send socket-array */
        le_array_io_write( le_stack, LE_MODE_AUTH, le_socket );

        /* send message */
        return( le_agree == LE_AGRT_AUTH ? _LE_TRUE : _LE_FALSE );

    }

    le_enum_t le_server_io_resiliate( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* send message */
        return( _LE_FALSE );

    }

    le_enum_t le_server_io_inject( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* time value variables */
        le_time_t le_time = _LE_TIME_NULL;

        /* stream index variables */
        le_size_t le_index = _LE_SIZE_NULL;

        /* serialise time */
        le_array_serial( le_stack, & le_time, sizeof( le_time_t ), 0, _LE_GET );

        /* read data array */
        if ( le_array_io_read( le_stack, le_socket ) != LE_MODE_INJE ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* create stream index */
        if ( ( le_index = le_stream_get_strict( le_stream, le_time, LE_STREAM_WRITE ) ) == _LE_SIZE_NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* decode data array */
        le_array_uf3_decode( le_stack, le_stack + 1 );

        /* inject array */
        le_stream_io_inject( le_stream, le_index, le_stack + 1 );

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_server_io_query( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* address variables */
        le_address_t le_addr = LE_ADDRESS_C;

        /* query-pack variables */
        le_size_t le_parse = 0;
        le_size_t le_qpack = le_array_get_size( le_stack );

        /* address parameter variables */
        le_size_t le_mode = 0;
        le_size_t le_size = 0;
        le_size_t le_span = 0;

        /* offset variables */
        le_size_t le_ofsa = _LE_OFFS_NULL;
        le_size_t le_ofsb = _LE_OFFS_NULL;

        /* stream variables */
        le_size_t le_stra = _LE_SIZE_NULL;
        le_size_t le_strb = _LE_SIZE_NULL;

        /* reading query-pack */
        while ( ( le_parse = le_address_serial( & le_addr, le_stack, le_parse, _LE_GET ) ) <= le_qpack ) {

            /* retrieve address parameters */
            le_size = le_address_get_size( & le_addr );
            le_span = le_address_get_span( & le_addr ) + le_size;

            /* retrieve address mode */
            le_mode = le_address_get_mode( & le_addr );

            /* update array size */
            le_array_set_size( le_stack + 1, 0 );

            /* switch on address mode */
            if ( le_mode == 1 ) {

                /* reduce address */
                if ( ( le_stra = le_stream_get_reduct( le_stream, & le_addr, 0, & le_ofsa ) ) != _LE_SIZE_NULL ) {

                    /* gathering data */
                    le_stream_io_gather( le_stream, le_stra, & le_addr, le_ofsa, le_size, le_span, le_stack + 1 );

                }

            } else
            if ( le_mode == 2 ) {

                /* reduce address */
                if ( ( le_strb = le_stream_get_reduct( le_stream, & le_addr, 1, & le_ofsb ) ) != _LE_SIZE_NULL ) {

                    /* gathering data */
                    le_stream_io_gather( le_stream, le_strb, & le_addr, le_ofsb, le_size, le_span, le_stack + 1 );

                }

            } else {

                /* reduce address */
                le_stra = le_stream_get_reduct( le_stream, & le_addr, 0, & le_ofsa );
                le_strb = le_stream_get_reduct( le_stream, & le_addr, 1, & le_ofsb );

                /* check reductions */
                if ( ( le_stra != _LE_SIZE_NULL ) || ( le_strb != _LE_SIZE_NULL ) ) {

                    /* gathering data */
                    le_stream_io_parallel( le_stream, le_stra, le_strb, & le_addr, le_ofsa, le_ofsb, le_size, le_span, le_stack + 1 );

                }

            }

            /* encode socket-array */
            le_array_uf3_encode( le_stack + 1, le_stack + 2 );

            /* write socket-array */
            le_array_io_write( le_stack + 2, LE_MODE_QUER, le_socket );

        }

        /* send message */
        return( _LE_TRUE );

    }

