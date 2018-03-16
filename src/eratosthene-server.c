/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2018 DHLAB, EPFL
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

        /* assign server path - check consistency */
        if ( ( le_server.sv_path = le_path ) == NULL ) {

            /* send message */
            return( le_server._status = LE_ERROR_IO_ACCESS, le_server );

        }

        /* create server configuration */
        if ( ( le_server._status = le_server_set_config( & le_server ) ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( le_server );

        }

        /* create server socket */
        if ( ( le_server.sv_sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) {

            /* send message */
            return( le_server._status = LE_ERROR_IO_SOCKET, le_server );

        }

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

        /* stream variables */
        FILE * le_stream = NULL;

        /* string length variables */
        le_size_t le_plen = strlen( ( char * ) le_server->sv_path );

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

        /* check consistency */
        if ( ( le_server->sv_scfg <= 0 ) || ( le_server->sv_scfg >= _LE_USE_DEPTH ) ) {

            /* send message */
            return( LE_ERROR_DEPTH );

        }

        /* check consistency */
        if ( le_server->sv_tcfg <= 0 ) {

            /* send message */
            return( LE_ERROR_TIME );

        }

        /* restore server path */
        le_server->sv_path[le_plen] = '\0';

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - i/o methods
 */

    le_void_t le_server_io( le_server_t * const le_server ) {

        /* server client management */
        # pragma omp parallel num_threads( _LE_USE_PENDING )
        {

        /* socket variable */
        le_sock_t le_socket = _LE_SOCK_NULL;

        /* state variables */
        le_enum_t le_active = _LE_TRUE;

        /* stream variables */
        le_stream_t le_stream = LE_STREAM_C;

        /* socket-array variables */
        le_array_t le_stack[_LE_USE_ARRAY];

        /* create socket-array stack */
        le_array_stack_create( le_stack, _LE_USE_ARRAY );

        /* create client socket */
        while ( ( le_socket = le_client_accept( le_server->sv_sock ) ) != _LE_SOCK_NULL ) {

            /* create and check connection stream */
            if ( ( le_stream = le_stream_create( le_server->sv_path, le_server->sv_scfg, le_server->sv_tcfg ) )._status == LE_ERROR_SUCCESS ) {

                /* connection manager */
                while ( le_active == _LE_TRUE ) {

                    /* client socket-array */
                    switch( le_array_io_read( le_stack, le_socket ) ) {

                        /* authentication */
                        case ( LE_MODE_AUTH ) : {

                            /* mode management - update state */
                            le_active = le_server_io_auth( le_server, & le_stream, le_stack, le_socket );

                        } break;

                        /* injection */
                        case ( LE_MODE_INJE ) : {

                            /* mode management - update state */
                            le_active = le_server_io_inject( le_server, & le_stream, le_stack, le_socket );

                        } break;

                        /* query */
                        case ( LE_MODE_QUER ) : {

                            /* mode management - update state */
                            le_active = le_server_io_query( le_server, & le_stream, le_stack, le_socket );

                        } break;

                        /* unexpected mode - update state */
                        default : { le_active = _LE_FALSE; } break;

                    };

                }

                /* delete connection stream */
                le_stream_delete( & le_stream );

            }

            /* close client socket */
            close( le_socket );

            /* update state */
            le_active = _LE_TRUE;

        }

        /* delete socket-array stack */
        le_array_stack_delete( le_stack, _LE_USE_ARRAY );

        } /* openmp */

    }

/*
    source - i/o methods
 */

    le_enum_t le_server_io_auth( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* serailisation variable */
        le_size_t le_serial = 0;

        /* check consistency */
        if ( le_array_get_size( le_stack ) != 0 ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* update socket-array size */
        le_array_set_size( le_stack, LE_ARRAY_AUTH );

        /* serialise server configuration */
        le_serial = le_array_serial( le_stack, & le_server->sv_scfg, sizeof( le_size_t ), le_serial, _LE_SET );
        le_serial = le_array_serial( le_stack, & le_server->sv_tcfg, sizeof( le_time_t ), le_serial, _LE_SET );

        /* write socket-array */
        if ( le_array_io_write( le_stack, LE_MODE_AUTH, le_socket ) != LE_MODE_AUTH ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_server_io_inject( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* time value variable */
        le_time_t le_time = _LE_TIME_NULL;

        /* stream index variable */
        le_size_t le_index = _LE_SIZE_NULL;

        /* check consistency */
        if ( le_array_get_size( le_stack ) != sizeof( le_time_t ) ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* serialise time */
        le_array_serial( le_stack, & le_time, sizeof( le_time_t ), 0, _LE_GET );

        /* read socket-array */
        if ( le_array_io_read( le_stack, le_socket ) != LE_MODE_INJE ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* decode socket array */
        if ( le_array_uf3_decode( le_stack, le_stack + 1 ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( _LE_FALSE );

        }
        
        /* create and check stream index */
        if ( ( le_index = le_stream_get_strict( le_stream, le_time, LE_STREAM_WRITE ) ) == _LE_SIZE_NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* inject socket-array */
        le_stream_io_inject( le_stream, le_index, le_stack + 1 );

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_server_io_query( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* address variable */
        le_address_t le_addr = LE_ADDRESS_C;

        /* socket-array parsing variable */
        le_size_t le_parse = 0;

        /* socket-array size variable */
        le_size_t le_length = le_array_get_size( le_stack );

        /* address mode variable */
        le_byte_t le_mode = 0;

        /* address size variable */
        le_size_t le_size = 0;

        /* address depth variable */
        le_size_t le_depth = 0;

        /* offset variable */
        le_size_t le_offa = _LE_OFFS_NULL;
        le_size_t le_offb = _LE_OFFS_NULL;

        /* stream variable */
        le_size_t le_stra = _LE_SIZE_NULL;
        le_size_t le_strb = _LE_SIZE_NULL;

        /* check consistency */
        if ( ( le_length % LE_ARRAY_ADDR ) != 0 ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* parsing socket-array */
        while ( ( le_parse = le_address_serial( & le_addr, le_stack, le_parse, _LE_GET ) ) <= le_length ) {

            /* retrieve address mode */
            le_mode = le_address_get_mode( & le_addr );

            /* retrieve address size */
            le_size = le_address_get_size( & le_addr );

            /* retrieve address span */
            le_depth = le_address_get_span( & le_addr ) + le_size;

            /* reset socket-array */
            le_array_set_size( le_stack + 1, 0 );

            /* check address mode */
            if ( le_mode == 1 ) {

                /* reduce address time */
                le_stra = le_stream_get_reduct( le_stream, & le_addr, 0, & le_offa );

                /* reduce address time */
                if ( le_stra != _LE_SIZE_NULL ) {

                    /* gathering process */
                    le_stream_io_gather( le_stream, le_stra, & le_addr, le_offa, le_size, le_depth, le_stack + 1 );

                }

            } else if ( le_mode == 2 ) {

                /* reduce address time */
                le_strb = le_stream_get_reduct( le_stream, & le_addr, 1, & le_offb );

                /* check address reduction */
                if ( le_strb != _LE_SIZE_NULL ) {

                    /* gathering process */
                    le_stream_io_gather( le_stream, le_strb, & le_addr, le_offb, le_size, le_depth, le_stack + 1 );

                }

            } else {

                /* reduce address time */
                le_stra = le_stream_get_reduct( le_stream, & le_addr, 0, & le_offa );

                /* reduce address time */
                le_strb = le_stream_get_reduct( le_stream, & le_addr, 1, & le_offb );

                /* check address reduction */
                if ( ( le_stra != _LE_SIZE_NULL ) && ( le_strb != _LE_SIZE_NULL ) ) {

                    /* gathering process */
                    le_stream_io_parallel( le_stream, le_stra, le_strb, & le_addr, le_offa, le_offb, le_size, le_depth, le_stack + 1 );

                }

            }

            /* encode socket-array */
            if ( le_array_uf3_encode( le_stack + 1, le_stack + 2 ) != LE_ERROR_SUCCESS ) {

                /* send message */
                return( _LE_FALSE );

            }

            /* write socket-array */
            if ( le_array_io_write( le_stack + 2, LE_MODE_QUER, le_socket ) != LE_MODE_QUER ) {

                /* send message */
                return( _LE_FALSE );

            }

        }

        /* send message */
        return( _LE_TRUE );

    }

