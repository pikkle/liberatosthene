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

        /* create server stream */
        if ( ( le_server.sv_stream = le_stream_create( le_path, le_server.sv_scfg, le_server.sv_tcfg ) )._status != LE_ERROR_SUCCESS ) {

            /* send message */
            return( le_server._status = le_server.sv_stream._status, le_server );

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

        /* delete server stream */
        le_stream_delete( & le_server->sv_stream );

        /* delete structure */
        ( * le_server ) = le_delete;

    }

/*
    source - server methods
 */

    le_void_t le_server( le_server_t * const le_server ) {

        /* client socket variables */
        le_sock_t le_client = _LE_SOCK_NULL;

        /* client address variables */
        struct sockaddr_in le_addr = LE_ADDRIN_C;

        /* client address variables */
        socklen_t le_len = sizeof( struct sockaddr_in );

        /* server connexions */
        for ( ; ; ) {

            /* waiting client connexions */
            if ( ( le_client = accept( le_server->sv_sock, ( struct sockaddr * ) ( & le_addr ), & le_len ) ) != _LE_SOCK_NULL ) {

                /* switch on handshake */
                switch ( le_client_switch( le_client ) ) {

                    /* system injection */
                    case ( LE_MODE_IMOD ) : {

                        /* send authorisation */
                        if ( le_client_authorise( le_client, LE_MODE_IATH ) == LE_ERROR_SUCCESS ) {

                            /* connection to system injection */
                            le_server_inject_client( le_server, le_client );

                        }

                    } break;

                    /* system query */
                    case ( LE_MODE_RMOD ) : {

                        /* send authorisation */
                        if ( le_client_authorise( le_client, LE_MODE_RATH ) == LE_ERROR_SUCCESS ) {

                            /* connection to system query */
                            le_server_reduce_client( le_server, le_client );

                        }

                    } break;

                    /* system query */
                    case ( LE_MODE_QMOD ) : {

                        /* send authorisation */
                        if ( le_client_authorise( le_client, LE_MODE_QATH ) == LE_ERROR_SUCCESS ) {

                            /* connection to system query */
                            le_server_query_client( le_server, le_client );

                        }

                    } break;

                    /* system configuration */
                    case ( LE_MODE_CMOD ) : {

                        /* send authorisation */
                        if ( le_client_authorise( le_client, LE_MODE_CATH ) == LE_ERROR_SUCCESS ) {

                            /* connection to system */
                            le_server_config_client( le_server, le_client );

                        }

                    } break;

                }

                /* close client socket */
                close( le_client );

            }

        }

    }

/*
    source - client methods - injection
 */

    le_void_t le_server_inject_client( le_server_t * const le_server, le_sock_t const le_client ) {

        /* time variables */
        le_time_t le_time = _LE_TIME_NULL;

        /* stream variables */
        le_size_t le_stream = _LE_SIZE_NULL;

        /* array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* read time */
        if ( read( le_client, & le_time, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) {

            /* abort injection */
            return;

        }

        /* read client array */
        le_array_io_read( & le_array, le_client );

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
        if ( ( le_stream = le_stream_get_strict( & le_server->sv_stream, le_time, LE_STREAM_WRITE ) ) != _LE_SIZE_NULL ) {

            /* inject array */
            le_server_inject( le_server, & le_array, le_stream );

        }

        /* delete array */
        le_array_delete( & le_array );

    }

    le_void_t le_server_inject( le_server_t * const le_server, le_array_t const * const le_array, le_size_t const le_stream ) {

        /* array size variables */
        le_size_t le_size = le_array_get_size( le_array );

        /* depth variables */
        le_size_t le_parse = 0;
        le_size_t le_panex = 0;

        /* offset variables */
        le_size_t le_offset = 0;
        le_size_t le_offnex = 0;

        /* class variables */
        le_class_t le_class = LE_CLASS_C;

        /* address variables */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_server->sv_scfg - 1 );

        /* check consistency - abort injection */
        if ( le_size == 0 ) return;

        /* parsing array */
        for ( le_size_t le_index = 0; le_index < le_size; le_index += LE_ARRAY_SD ) {

            /* reset address digits */
            le_address_set_pose( & le_addr, le_array_sd_pose_a( le_array, le_index ) );

            /* reset depth variables */
            le_parse = 0;
            le_panex = 1;

            /* reset offset variables */
            le_offset = 0;
            le_offnex = 0;

            /* injection process */
            do {

                /* class importation */
                if ( le_class_io_read( & le_class, le_offnex, le_stream_get_file( & le_server->sv_stream, le_stream, le_parse ) ) == LE_ERROR_SUCCESS ) {

                    /* inject element in class */
                    le_class_set_push( & le_class, le_array_sd_data_a( le_array, le_index ) );

                } else {

                    /* initialise class with element */
                    le_class = le_class_create( le_array_sd_data_a( le_array, le_index ) );

                }

                /* retrieve daughter offset */
                le_offset = le_class_get_offset( & le_class, le_address_get_digit( & le_addr, le_parse ) );

                /* check daughter state */
                if ( ( le_offset == _LE_OFFS_NULL ) && ( ( le_panex ) != le_server->sv_scfg ) ) {

                    /* seek next scale eof */
                    fseek( le_stream_get_file( & le_server->sv_stream, le_stream, le_panex ), 0, SEEK_END );

                    /* assign eof offset */
                    le_offset = ftell( le_stream_get_file( & le_server->sv_stream, le_stream, le_panex ) );

                    /* insert offset in class */
                    le_class_set_offset( & le_class, le_address_get_digit( & le_addr, le_parse ), le_offset );

                }

                /* class exportation */
                le_class_io_write( & le_class, le_offnex, le_stream_get_file( & le_server->sv_stream, le_stream, le_parse ) );

            /* injection process condition */
            } while ( ( le_offnex = le_offset, ++ le_panex, ++ le_parse ) < le_server->sv_scfg );


        }

    }

/*
    source - client methods - reduction
 */

    le_void_t le_server_reduce_client( le_server_t * const le_server, le_sock_t const le_client ) {

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
        if ( ( le_address_get_mode( & le_addr ) & 0x01 ) != 0 ) {

            /* address reduction */
            le_stream_get_reduct( & le_server->sv_stream, & le_addr, 0 );

        }

        /* check address mode */
        if ( ( le_address_get_mode( & le_addr ) & 0x02 ) != 0 ) {

            /* address reduction */
            le_stream_get_reduct( & le_server->sv_stream, & le_addr, 1 );

        }

        /* write address */
        le_address_io_write( & le_addr, le_client );

    }

/*
    source - client methods - query
 */

    le_void_t le_server_query_client( le_server_t * const le_server, le_sock_t const le_client ) {

        /* array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* address variables */
        le_address_t le_addr = LE_ADDRESS_C;
        le_address_t le_enum = LE_ADDRESS_C;

        /* stream variables */
        le_size_t le_stream = _LE_SIZE_NULL;

        /* address variables */
        le_size_t le_mode = 0;
        le_size_t le_size = 0;
        le_size_t le_span = 0;

        /* offset variables */
        le_size_t le_offset = 0;

        /* read query address */
        le_address_io_read( & le_addr, le_client );

        /* extract address parameters */
        le_mode = le_address_get_mode( & le_addr );
        le_size = le_address_get_size( & le_addr );
        le_span = le_address_get_span( & le_addr ) + le_size;

        /* check consistency - abort query */
        if ( le_span >= le_server->sv_scfg ) return;

        /* check mode */
        if ( ( le_mode & 0x01 ) != 0 ) {

            /* create and check stream */
            if ( ( le_stream = le_stream_get_reduct( & le_server->sv_stream, & le_addr, 0 ) ) != _LE_SIZE_NULL ) {

                /* copy structure */
                le_enum = le_addr;

                /* retrieve class offset */
                if ( ( le_offset = le_stream_io_offset( & le_server->sv_stream, le_stream, & le_enum ) ) != _LE_OFFS_NULL ) {

                    /* gather daughters representative */
                    le_stream_io_gather( & le_server->sv_stream, le_stream, & le_enum, le_offset, le_size, le_span, & le_array );

                }

            }

        }

        /* check address mode */
        if ( ( le_mode & 0x02 ) != 0 ) {

            /* create and check stream */
            if ( ( le_stream = le_stream_get_reduct( & le_server->sv_stream, & le_addr, 1 ) ) != _LE_SIZE_NULL ) {

                /* copy structure */
                le_enum = le_addr;

                /* retrieve class offset */
                if ( ( le_offset = le_stream_io_offset( & le_server->sv_stream, le_stream, & le_enum ) ) != _LE_OFFS_NULL ) {

                    /* gather daughters representative */
                    le_stream_io_gather( & le_server->sv_stream, le_stream, & le_enum, le_offset, le_size, le_span, & le_array );

                }

            }

        }

        /* write query address on socket */
        le_address_io_write( & le_addr, le_client );

        /* purge array to socket */
        le_array_io_write( & le_array, le_client );

        /* unallocate array memory */
        le_array_delete( & le_array );

    }

/*
    source - client methods - server configuration
 */

    le_enum_t le_server_config_client( le_server_t const * const le_server, le_sock_t const le_client ) {

        /* returned value variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* server configuration array variables */
        le_array_t le_array = le_server_config( le_server );

        /* write array on socket */
        le_return = le_array_io_write( & le_array, le_client );

        /* delete array */
        le_array_delete( & le_array );

        /* send message */
        return( le_return );

    }

    le_array_t le_server_config( le_server_t const * const le_server ) {

        /* returned structure variables */
        le_array_t le_array = LE_ARRAY_C;

        /* compose configuration array */
        le_array_map_dt( & le_array, le_server->sv_scfg, le_server->sv_tcfg );

        /* return structure */
        return( le_array );

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

