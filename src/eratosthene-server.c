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

    le_server_t le_server_create( le_char_t * const le_root, le_sock_t const le_port ) {

        /* created structure variables */
        le_server_t le_server = LE_SERVER_C;

        /* address variables */
        struct sockaddr_in le_addr = LE_ADDRIN_C_PORT( le_port );

        /* assign server path - check consistency */
        if ( ( le_server.sv_root = le_root ) == NULL ) {

            /* send message */
            return( le_set_status( le_server, LE_ERROR_IO_ACCESS ) );

        }

        /* create server configuration */
        if ( ( le_server._status = le_server_set_config( & le_server ) ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( le_server );

        }

        /* create server socket */
        if ( ( le_server.sv_sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) {

            /* send message */
            return( le_set_status( le_server, LE_ERROR_IO_SOCKET ) );

        }

        /* server socket address */
        if ( bind( le_server.sv_sock, ( struct sockaddr * ) ( & le_addr ), sizeof( struct sockaddr_in ) ) == _LE_SOCK_NULL ) {

            /* close server socket */
            close( le_server.sv_sock );

            /* send message */
            return( le_set_status( le_server, LE_ERROR_IO_BIND ) );

        }

        /* server socket state */
        if ( listen( le_server.sv_sock, _LE_USE_PENDING ) == _LE_SOCK_NULL ) {

            /* close server socket */
            close( le_server.sv_sock );

            /* send message */
            return( le_set_status( le_server, LE_ERROR_IO_LISTEN ) );

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
    source - accessor methods
 */

    le_size_t le_server_get_thread( le_server_t * const le_server ) {

        /* parsing variable */
        le_size_t le_parse = 0;

        /* searching available thread */
        while ( ( ++ le_parse ) < _LE_USE_PENDING ) {

            /* check thread availability */
            if ( le_server_mac_pool( le_server, le_parse, LE_SERVER_ACTIVE ) == 0 ) {

                /* return thread index */
                return( le_parse );

            }

        }

        /* return thread index */
        return( le_parse );

    }

/*
    source - mutator methods
 */

    le_enum_t le_server_set_config( le_server_t * const le_server ) {

        /* stream variables */
        FILE * le_tree = NULL;

        /* string length variables */
        le_size_t le_plen = strlen( ( char * ) le_server->sv_root );

        /* open configuration stream */
        if ( ( le_tree = fopen( strcat( ( char * ) le_server->sv_root, "/system" ), "r" ) ) == NULL ) {

            /* send message */
            return( LE_ERROR_IO_ACCESS );

        }

        /* read configuration parameter */
        if ( fscanf( le_tree, "%" _LE_SIZE_S, & le_server->sv_scfg ) != 1 ) {

            /* close stream */
            fclose( le_tree );

            /* send message */
            return( LE_ERROR_IO_READ );

        }

        /* read configuration parameter */
        if ( fscanf( le_tree, "%" _LE_TIME_S, & le_server->sv_tcfg ) != 1 ) {

            /* close stream */
            fclose( le_tree );

            /* send message */
            return( LE_ERROR_IO_READ );

        }

        /* close stream */
        fclose( le_tree );

        /* check consistency */
        if ( ( le_server->sv_scfg <= 1 ) || ( le_server->sv_scfg >= _LE_USE_DEPTH ) ) {

            /* send message */
            return( LE_ERROR_DEPTH );

        }

        /* check consistency */
        if ( le_server->sv_tcfg <= 0 ) {

            /* send message */
            return( LE_ERROR_TIME );

        }

        /* restore server path */
        le_server->sv_root[le_plen] = '\0';

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_server_set_broadcast( le_server_t * const le_server, le_size_t const le_tid, le_byte_t const le_message ) {

        /* parsing variable */
        le_size_t le_parse = 0;

        /* parsing thread pool */
        while ( ( ++ le_parse ) < _LE_USE_PENDING ) {

            /* broadcast message */
            le_server_mac_set( le_server, le_parse, le_message );

        }

        /* reset thread self-message */
        le_server_mac_clear( le_server, le_tid, le_message );

    }

    le_enum_t le_server_set_tree( le_server_t * const le_server, le_size_t const le_tid, le_tree_t * const le_tree ) {

        /* check thread message */
        if ( le_server_mac_pool( le_server, le_tid, LE_SERVER_RELOAD ) == 0 ) {

            /* send message */
            return( _LE_TRUE );

        }

        /* delete tree structure */
        le_tree_delete( le_tree );

        /* create tree structure */
        if ( le_get_status( ( * le_tree ) = le_tree_create( le_server->sv_root, le_server->sv_scfg, le_server->sv_tcfg ) ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* reset thread message */
        le_server_mac_clear( le_server, le_tid, LE_SERVER_RELOAD );

        /* send message */
        return( _LE_TRUE );

    }

/*
    source - i/o methods
 */

    le_void_t le_server_io( le_server_t * const le_server ) {

        /* socket variable */
        le_sock_t le_socket = _LE_SOCK_NULL;

        /* process variable */
        le_proc_t le_thread;

        /* process variable */
        le_size_t le_tid = 0;

        /* principal server loop */
        while ( le_server_mac_pool( le_server, 0, LE_SERVER_ACTIVE ) != 0 ) {

            /* client connection */
            if ( ( le_socket = le_client_io_accept( le_server->sv_sock ) ) != _LE_SOCK_NULL ) {

                /* search available thread */
                if ( ( le_tid = le_server_get_thread( le_server ) ) < _LE_USE_PENDING ) {

                    /* client thread pack variable */
                    le_pack_t le_pack = { le_server, le_tid, le_socket, PTHREAD_MUTEX_INITIALIZER };

                    /* initialise thread pool */
                    le_server->sv_pool[le_tid] = LE_SERVER_RELOAD;

                    /* create client thread */
                    if ( pthread_create( & le_thread, NULL, le_server_io_client, ( void * ) ( & le_pack ) ) == 0 ) {

                        /* wait thread confirmation */
                        while ( le_tid != 0 ) {

                            /* thread critical region */
                            pthread_mutex_lock( & le_pack.p_m );

                            /* check thread confirmation */
                            if ( le_server_mac_pool( le_server, le_tid, LE_SERVER_ACTIVE ) != 0 ) {

                                /* thread confirmed */
                                le_tid = 0;    

                            }

                            /* thread critical region */
                            pthread_mutex_unlock( & le_pack.p_m );

                        }

                    /* refuse connection */
                    } else { close( le_socket ); }

                /* refuse connection */
                } else { close( le_socket ); }

            }

        }

    }

    le_void_t * le_server_io_client( le_void_t * le_pack ) {

        /* server variable */
        le_server_t * le_server = ( ( _pck ) le_pack )->p_s;

        /* process variable */
        le_size_t le_tid = ( ( _pck ) le_pack )->p_t;

        /* socket variable */
        le_sock_t le_socket = ( ( _pck ) le_pack )->p_c;

        /* socket-array variables */
        le_array_t le_stack[_LE_USE_ARRAY];

        /* stream variables */
        le_tree_t le_tree = LE_TREE_C;

        /* thread critical region */
        pthread_mutex_lock( & ( ( _pck ) le_pack )->p_m );

        /* thread activity confirmation */
        le_server_mac_set( le_server, le_tid, LE_SERVER_ACTIVE );

        /* thread critical region */
        pthread_mutex_unlock( & ( ( _pck ) le_pack )->p_m );

        /* create thread socket-array */
        le_array_mac_create( le_stack, _LE_USE_ARRAY );

        /* connection manager */
        while ( le_server_mac_pool( le_server, le_tid, LE_SERVER_ACTIVE ) != 0 ) {

            /* thread pooling */
            if ( le_server_set_tree( le_server, le_tid, & le_tree ) == _LE_TRUE ) {

                /* client socket-array */
                switch( le_array_io_read( le_stack, le_socket ) ) {

                    /* authentication */
                    case ( LE_MODE_AUTH ) : {

                        /* mode management - update state */
                        if ( le_server_io_auth( le_server, & le_tree, le_stack, le_socket ) != _LE_TRUE ) {

                            /* reset thread activity */
                            le_server_mac_clear( le_server, le_tid, LE_SERVER_ACTIVE );

                        }

                    } break;

                    /* inject */
                    case ( LE_MODE_INJE ) : {

                        /* mode management - update state */
                        if ( le_server_io_inject( le_server, & le_tree, le_stack, le_socket ) != _LE_TRUE ) {

                            /* reset thread activity */
                            le_server_mac_clear( le_server, le_tid, LE_SERVER_ACTIVE );

                        } else {

                            /* broadcast message */
                            le_server_set_broadcast( le_server, le_tid, LE_SERVER_RELOAD );

                        }

                    } break;

                    /* optimise */
                    case ( LE_MODE_OPTM ) : {

                        /* mode management - update state */
                        if ( le_server_io_optm( le_server, & le_tree, le_stack, le_socket ) != _LE_TRUE ) {

                            /* reset thread activity */
                            le_server_mac_clear( le_server, le_tid, LE_SERVER_ACTIVE );

                        } else {

                            /* broadcast message */
                            le_server_set_broadcast( le_server, le_tid, LE_SERVER_RELOAD );

                        }

                    } break;

                    /* query */
                    case ( LE_MODE_QUER ) : {

                        /* mode management - update state */
                        if ( le_server_io_query( le_server, & le_tree, le_stack, le_socket ) != _LE_TRUE ) {

                            /* reset thread activity */
                            le_server_mac_clear( le_server, le_tid, LE_SERVER_ACTIVE );

                        }

                    } break;

                    /* unexpected mode - update state */
                    default : {

                        /* reset thread activity */
                        le_server_mac_clear( le_server, le_tid, LE_SERVER_ACTIVE );

                    } break;

                };

            /* reset thread activity */
            } else { le_server_mac_clear( le_server, le_tid, LE_SERVER_ACTIVE ); }

        }

        /* delete thread stream */
        le_tree_delete( & le_tree );

        /* delete thread socket-array */
        le_array_mac_delete( le_stack, _LE_USE_ARRAY );

        /* close connection */
        close( le_socket );

        /* thread exit */
        return( NULL );

    }

    le_enum_t le_server_io_auth( le_server_t * const le_server, le_tree_t * const le_tree, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* serialisation variable */
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

    le_enum_t le_server_io_inject( le_server_t * const le_server, le_tree_t * const le_tree, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* time value variable */
        le_time_t le_time = _LE_TIME_NULL;

        /* unit variable */
        le_unit_t * le_unit = NULL;

        /* check consistency */
        if ( le_array_get_size( le_stack ) != LE_ARRAY_INJE ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* serialise time */
        le_array_serial( le_stack, & le_time, sizeof( le_time_t ), 0, _LE_GET );

        /* check consistency */
        if ( le_time == _LE_TIME_NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* read socket-array */
        if ( le_array_io_read( le_stack, le_socket ) != LE_MODE_INJE ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* check consistency */
        if ( ( le_array_get_size( le_stack ) % LE_ARRAY_UF3 ) != 0 ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* retrieve and check unit */
        if ( ( le_unit = le_tree_get_unit( le_tree, le_time, LE_UNIT_WRITE ) ) == NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* lock unit */
        le_unit_set_lock( le_unit, LE_UNIT_LOCK );

        /* inject socket-array */
        le_unit_io_inject( le_unit, le_stack );

        /* open unit */
        le_unit_set_lock( le_unit, LE_UNIT_OPEN );

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_server_io_optm( le_server_t * const le_server, le_tree_t * const le_tree, le_array_t * const le_stack, le_sock_t const le_socket ) {

        /* time value variable */
        le_time_t le_time = _LE_TIME_NULL;

        /* unit variable */
        le_unit_t * le_unit = NULL;

        /* check consistency */
        if ( le_array_get_size( le_stack ) != LE_ARRAY_OPTM ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* serialise time */
        le_array_serial( le_stack, & le_time, sizeof( le_time_t ), 0, _LE_GET );

        /* check consistency */
        if ( le_time == _LE_TIME_NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* retrieve and check unit */
        if ( ( le_unit = le_tree_get_unit( le_tree, le_time, LE_UNIT_READ ) ) == NULL ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* lock unit */
        le_unit_set_lock( le_unit, LE_UNIT_LOCK );

        /* optimise unit storage */
        le_unit_set_optimise( le_unit );

        /* open unit */
        le_unit_set_lock( le_unit, LE_UNIT_OPEN );

        /* send message */
        return( _LE_TRUE );

    }

    le_enum_t le_server_io_query( le_server_t * const le_server, le_tree_t * const le_tree, le_array_t * const le_stack, le_sock_t const le_socket ) {

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

        /* unit variable */
        le_unit_t * le_uia = NULL;
        le_unit_t * le_uib = NULL;

        /* check consistency */
        if ( ( le_length % LE_ARRAY_ADDR ) != 0 ) {

            /* send message */
            return( _LE_FALSE );

        }

        /* parsing socket-array */
        while ( ( le_parse = le_address_serial( & le_addr, le_stack, le_parse, _LE_GET ) ) <= le_length ) {

            /* retrieve address size */
            le_size = le_address_get_size( & le_addr );

            /* retrieve address span */
            le_depth = le_address_get_span( & le_addr ) + le_size;

            /* reset socket-array */
            le_array_set_size( le_stack + 1, 0 );

            /* check address mode */
            if ( ( le_mode = le_address_get_mode( & le_addr ) ) < 3 ) {

                /* reduce and check address */
                if ( ( le_uia = le_tree_get_query( le_tree, & le_addr, le_mode - 1, & le_offa ) ) != NULL ) {

                    /* gathering process */
                    le_unit_io_gather( le_uia, & le_addr, le_offa, le_size, le_depth, le_stack + 1 );

                }

            } else {

                /* reduce address */
                le_uia = le_tree_get_query( le_tree, & le_addr, 0, & le_offa );

                /* reduce address */
                le_uib = le_tree_get_query( le_tree, & le_addr, 1, & le_offb );

                /* check address */
                if ( ( le_uia != NULL ) || ( le_uib != NULL ) ) {

                    /* gathering process */
                    le_unit_io_parallel( le_uia, le_uib, & le_addr, le_mode, le_offa, le_offb, le_size, le_depth, le_stack + 1 );

                }

            }

            /* write socket-array */
            if ( le_array_io_write( le_stack + 1, LE_MODE_QUER, le_socket ) != LE_MODE_QUER ) {

                /* send message */
                return( _LE_FALSE );

            }

        }

        /* send message */
        return( _LE_TRUE );

    }

