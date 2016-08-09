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

    le_server_t le_server_create( le_size_t const le_port, le_char_t const * const le_path ) {

        /* Created structure variables */
        le_server_t le_server = LE_SERVER_C;

        /* Stream variables */
        FILE * le_stream = NULL;

        /* Address variables */
        struct sockaddr_in le_addr = LE_SOCKADDR_IN_C_PORT( le_port );

        /* Open configuration stream */
        if ( ( le_stream = fopen( strcat( strcat( ( char * ) le_server.sv_path, ( char * ) le_path ), "/system" ), "r" ) ) == NULL ) {

            /* Send message */
            return( le_server._status = LE_ERROR_IO_ACCESS, le_server );

        } else {

            /* Remove server path configuration suffix */
            le_server.sv_path[strlen( ( char * ) le_path )] = '\0';

            /* Check configuration reading */
            if ( fscanf( le_stream, "%" _LE_SIZE_S " %" _LE_TIME_S, & le_server.sv_area, & le_server.sv_time ) != 2 ) {

                /* Close configuration stream */
                fclose( le_stream );

                /* Send message */
                return( le_server._status = LE_ERROR_IO_READ, le_server );

            }

            /* Close configuration stream */
            fclose( le_stream );

            /* Check consistency */
            if ( ( le_server.sv_area <= 0 ) || ( le_server.sv_area >= _LE_USE_DEPTH ) ) {

                /* Send message */
                return( le_server._status = LE_ERROR_DEPTH, le_server );

            }

            /* Check consistency */
            if ( le_server.sv_time <= 0 ) {

                /* Send message */
                return( le_server._status = LE_ERROR_TIME, le_server );

            }

        }

        /* Create socket */
        if ( ( le_server.sv_sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) {

            /* Send message */
            return( le_server._status = LE_ERROR_IO_SOCKET, le_server );

        } else {

            /* Bind address to socket */
            if ( bind( le_server.sv_sock, ( struct sockaddr * ) ( & le_addr ), sizeof( struct sockaddr_in ) ) == _LE_SOCK_NULL ) {

                /* Close socket */
                close( le_server.sv_sock );

                /* Send message */
                return( le_server._status = LE_ERROR_IO_BIND, le_server );

            }

            /* Enable socket listening */
            if ( listen( le_server.sv_sock, _LE_USE_PENDING ) == _LE_SOCK_NULL ) {

                /* Close socket */
                close( le_server.sv_sock );

                /* Send message */
                return( le_server._status = LE_ERROR_IO_LISTEN, le_server );

            }

        }

        /* Return created structure */
        return( le_server );

    }

    le_void_t le_server_delete( le_server_t * const le_server ) {

        /* Deleted structure variables */
        le_server_t le_delete = LE_SERVER_C;

        /* Delete streams stack */
        le_server_io_close( le_server );

        /* Check socket - close socket */
        if ( le_server->sv_sock != _LE_SOCK_NULL ) close( le_server->sv_sock );

        /* Delete structure */
        * le_server = le_delete;

    }

/*
    source - server methods
 */

    le_void_t le_server( le_server_t * const le_server ) {

        /* Client socket variables */
        le_sock_t le_client = _LE_SOCK_NULL;

        /* Client address variables */
        struct sockaddr_in le_addr = LE_SOCKADDR_IN_C;

        /* Client address length variables */
        socklen_t le_len = sizeof( struct sockaddr_in );

        /* Check consistency */
        if ( le_server->sv_sock == _LE_SOCK_NULL ) return;

        /* Server connections management */
        for ( ; ; ) {

            /* Accept incoming connection */
            if ( ( le_client = accept( le_server->sv_sock, ( struct sockaddr * ) & le_addr, & le_len ) ) != _LE_SOCK_NULL ) {

                /* Switch on handshake */
                switch ( le_server_handshake( le_client ) ) {

                    /* System injection */
                    case ( LE_NETWORK_MODE_IMOD ) : {

                        /* Send authorisation */
                        if ( le_server_authorise( le_client, LE_NETWORK_MODE_IATH ) == LE_ERROR_SUCCESS ) {

                            /* Connection to system injection */
                            le_server_inject_client( le_server, le_client );

                        }

                    } break;

                    /* System query */
                    case ( LE_NETWORK_MODE_QMOD ) : {

                        /* Send authorisation */
                        if ( le_server_authorise( le_client, LE_NETWORK_MODE_QATH ) == LE_ERROR_SUCCESS ) {

                            /* Connection to system query */
                            le_server_query_client( le_server, le_client );

                        }

                    } break;

                    /* System times */
                    case ( LE_NETWORK_MODE_AMOD ) : {

                        /* Send authorisation */
                        if ( le_server_authorise( le_client, LE_NETWORK_MODE_AATH ) == LE_ERROR_SUCCESS ) {

                            /* Connection to system times */
                            le_server_times_client( le_server, le_client );

                        }

                    } break;

                    /* System configuration */
                    case ( LE_NETWORK_MODE_CMOD ) : {

                        /* Send authorisation */
                        if ( le_server_authorise( le_client, LE_NETWORK_MODE_CATH ) == LE_ERROR_SUCCESS ) {

                            /* Connection to system */
                            le_server_config_client( le_server, le_client );

                        }

                    } break;

                }

            }

            /* Close client connection */
            close( le_client );

        }

    }

/*
    source - client methods - handshake & authorisation
 */

    le_enum_t le_server_handshake( le_sock_t const le_socket ) {

        /* Socket i/o buffer variables */
        le_enum_t le_buffer = LE_NETWORK_MODE_NULL;

        /* Check consistency */
        if ( le_socket == _LE_SOCK_NULL ) return( LE_NETWORK_MODE_NULL );

        /* Read handshake - send message */
        if ( read( le_socket, & le_buffer, sizeof( le_enum_t ) ) != sizeof( le_enum_t ) ) return( LE_NETWORK_MODE_NULL );

        /* Return received handshake */
        return( le_buffer );

    }

    le_enum_t le_server_authorise( le_sock_t const le_socket, le_enum_t const le_auth ) {

        /* Check consistency - send message */
        if ( le_socket == _LE_SOCK_NULL ) return( LE_ERROR_IO_SOCKET );

        /* Write authorisation - send message */
        if ( write( le_socket, & le_auth, sizeof( le_enum_t ) ) != sizeof( le_enum_t ) ) return( LE_ERROR_IO_WRITE );

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - client methods - injection
 */

    le_void_t le_server_inject_client( le_server_t * const le_server, le_sock_t const le_client ) {

        /* Array access variables */
        le_array_sf_t le_access = LE_ARRAY_SF_C;

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

        /* Injection streaming loop */
        while ( le_retry < _LE_USE_RETRY ) {

            /* Read streaming bloc */
            if ( ( le_count = read( le_client, le_buffer + le_bridge, _LE_USE_MTU ) + le_bridge ) >= LE_ARRAY_SFL ) {

                /* Compute available records */
                le_round = le_count - ( le_count % LE_ARRAY_SFL );

                /* Parsing received streaming bloc */
                for ( le_parse = 0; le_parse < le_round; le_parse += LE_ARRAY_SFL ) {

                    /* Compute array access */
                    le_array_sf( le_buffer, le_parse, le_access );

                    /* Inject received element */
                    le_server_inject( le_server, & le_access );

                }

                /* Compute bridge value */
                if ( ( le_bridge = le_count % LE_ARRAY_SFL ) != 0 ) {

                    /* Apply circular condition */
                    memcpy( le_buffer, le_buffer + le_count - le_bridge, le_bridge );

                }                

            /* Update retry flag */
            le_retry = 0; } else { le_retry ++; }

        }

    }

    le_void_t le_server_inject( le_server_t * const le_server, le_array_sf_t const * const le_access ) {

        /* Address variables */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_server->sv_area - 1 );

        /* Class variables */
        le_class_t le_class = LE_CLASS_C;

        /* Depth variables */
        le_size_t le_parse = 0;
        le_size_t le_panex = 1;

        /* Offset variables */
        le_size_t le_offset = 0;
        le_size_t le_offnex = 0;

        /* System scale stream management - send message */
        if ( le_server_io_open( le_server, le_access->as_time[0] ) != LE_ERROR_SUCCESS ) return;

        /* Compute address index */
        le_address_set_pose( & le_addr, le_access->as_pose );

        /* Injection process */
        do {

            /* Class importation */
            if ( le_class_io_read( & le_class, le_offnex, le_server->sv_stream[le_parse] ) == LE_ERROR_SUCCESS ) {

                /* Inject element in class */
                le_class_set_push( & le_class, le_access->as_data );

            } else {

                /* Initialise class with element */
                le_class = le_class_create( le_access->as_data );

            }

            /* Retrieve daughter offset */
            le_offset = le_class_get_offset( & le_class, le_address_get_digit( & le_addr, le_parse ) );

            /* Check daughter state */
            if ( ( le_offset == _LE_OFFS_NULL ) && ( ( le_panex ) != le_server->sv_area ) ) {

                /* Seek next scale eof */
                fseek( le_server->sv_stream[le_panex], 0, SEEK_END );

                /* Assign eof offset */
                le_offset = ftell( le_server->sv_stream[le_panex] );

                /* Insert offset in class */
                le_class_set_offset( & le_class, le_address_get_digit( & le_addr, le_parse ), le_offset );

            }

            /* Class exportation */
            le_class_io_write( & le_class, le_offnex, le_server->sv_stream[le_parse] );

        /* Injection process condition */
        } while ( ( le_offnex = le_offset, ++ le_panex, ++ le_parse ) < le_server->sv_area );

    }
/*
    source - client methods - query
 */

    le_enum_t le_server_query_client( le_server_t * const le_server, le_sock_t const le_client ) {

        /* Message variables */
        le_enum_t le_message = LE_ERROR_SUCCESS;

        /* Array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Address variables */
        le_address_t le_address = LE_ADDRESS_C;

        /* Read and decompose query */
        le_address_io_read( & le_address, le_client );

        /* Send system query */
        le_server_query( le_server, & le_address, & le_array, 0, 0, le_client );

        /* Purge array to socket */
        le_message = le_array_io_stream( & le_array, le_client, 1 );

        /* Unallocate array memory */
        le_array_delete( & le_array );

        /* Send message */
        return( le_message );

    }

    le_void_t le_server_query( le_server_t * const le_server, le_address_t * const le_addr, le_array_t * const le_array, le_size_t const le_parse, le_size_t le_offset, le_sock_t const le_client ) {

        /* Class variables */
        le_class_t le_class = LE_CLASS_C;

        /* Address size variables */
        le_size_t le_size = le_address_get_size( le_addr );

        /* Query initialisation */
        if ( le_parse == 0 ) {

            /* Check consistency */
            if ( ( le_size + le_address_get_depth( le_addr ) ) >= le_server->sv_area ) return;

            /* System scale stream management */
            if ( le_server_io_open( le_server, le_address_get_time( le_addr ) ) != LE_ERROR_SUCCESS ) return;

        }

        /* Read class */
        if ( le_class_io_read( & le_class, le_offset, le_server->sv_stream[le_parse] ) == LE_ERROR_SUCCESS ) {

            /* Query process mode */
            if ( le_parse < le_size ) {

                /* Retreive daughter class offset */
                if ( ( le_offset = le_class_get_offset( & le_class, le_address_get_digit( le_addr, le_parse ) ) ) != _LE_OFFS_NULL ) {

                    /* Recursive query */
                    le_server_query( le_server, le_addr, le_array, le_parse + 1, le_offset, le_client );

                }

            } else if ( le_parse == le_size + le_address_get_depth( le_addr ) ) {

                /* Position array variables */
                le_real_t le_pose[3] = { 0.0 };

                /* Correct address size */
                le_address_set_size( le_addr, le_parse );

                /* Retreive class representative */
                le_address_get_pose( le_addr, le_pose );

                /* Restore address size */
                le_address_set_size( le_addr, le_size );

                /* Inject gathered element in array */
                le_array_set_pushrf( le_array, le_pose, le_class_get_data( & le_class ) );

                /* Query array streaming */
                le_array_io_stream( le_array, le_client, 0 );

            } else {

                /* Class daughters enumeration */
                for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                    /* Update address index digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* Retreive daughter class offset */
                    if ( ( le_offset = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                        /* Recursive query */
                        le_server_query( le_server, le_addr, le_array, le_parse + 1, le_offset, le_client );

                    }

                }

            }

        }

    }

/*
    source - client methods - times array
 */

    le_enum_t le_server_times_client( le_server_t const * const le_server, le_sock_t const le_client ) {

        /* Returned value variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* Array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Gather system time equivalences classes */
        le_array = le_server_times( le_server );

        /* Write array on socket */
        le_return = le_array_io_write( & le_array, le_client );

        /* Delete array */
        le_array_delete( & le_array );

        /* Send message */
        return( le_return );

    }

    le_array_t le_server_times( le_server_t const * const le_server ) {

        /* Time buffer variables */
        le_time_t le_buffer = _LE_TIME_NULL;

        /* Returned structure variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Directory variables */
        DIR * le_directory = NULL;

        /* Directory entity variables */
        struct dirent * le_entity = NULL;

        /* Create directory handle - send message */
        if ( ( le_directory = opendir( ( char * ) le_server->sv_path ) ) == NULL ) return( le_array );

        /* Directory entity enumeration */
        while ( ( le_entity = readdir( le_directory ) ) != NULL ) {

            /* Check entity type */
            if ( ( le_entity->d_type == DT_DIR ) && ( le_entity->d_name[0] != '.' ) ) {

                /* Convert directory name */
                le_buffer = le_server->sv_time * strtoull( le_entity->d_name, NULL, 10 );

                /* Push buffer in array */
                le_array_set_pushtf( & le_array, le_buffer );

            }

        }

        /* Delete directory handle */
        closedir( le_directory );

        /* Return structure */
        return( le_array );

    }

/*
    source - client methods - server configuration
 */

    le_enum_t le_server_config_client( le_server_t const * const le_server, le_sock_t const le_client ) {

        /* Returned value variables */
        le_enum_t le_return = LE_ERROR_SUCCESS;

        /* Array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Gather system configuration */
        le_array = le_server_config( le_server );

        /* Write array on socket */
        le_return = le_array_io_write( & le_array, le_client );

        /* Delete array */
        le_array_delete( & le_array );

        /* Send message */
        return( le_return );

    }

    le_array_t le_server_config( le_server_t const * const le_server ) {

        /* Returned structure variables */
        le_array_t le_array = LE_ARRAY_C;

        /* Compose configuration array */
        le_array_set_pushcf( & le_array, le_server->sv_area, le_server->sv_time );

        /* Return structure */
        return( le_array );

    }

/*
    source - i/o methods
 */

    le_enum_t le_server_io_open( le_server_t * const le_server, le_time_t const le_time ) {

        /* Temporal class variables */
        static le_time_t le_flag = _LE_TIME_NULL;

        /* Path variables */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

        /* Check necessities - send message */
        if ( ( le_time / le_server->sv_time ) == le_flag ) return( LE_ERROR_SUCCESS );

        /* Update temporal class */
        le_flag = le_time / le_server->sv_time;

        /* Compose temporal class directoy */
        sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P, le_server->sv_path, le_flag );

        /* Create directory */
        mkdir( ( char * ) le_path, 0777 );

        /* Create scale streams */
        for ( le_size_t le_parse = 0; le_parse < le_server->sv_area; le_parse ++ ) {

            /* Check stream state */
            if ( le_server->sv_stream[le_parse] != NULL ) fclose( le_server->sv_stream[le_parse] );

            /* Create scale path */
            sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_server->sv_path, le_flag, le_parse );

            /* Create scale stream (r+) */
            if ( ( le_server->sv_stream[le_parse] = fopen( ( char * ) le_path, "r+" ) ) == NULL ) {

                /* Create scale stream (w+) - send message */
                if ( ( le_server->sv_stream[le_parse] = fopen( ( char * ) le_path, "w+" ) ) == NULL ) return( LE_ERROR_IO_ACCESS );

            }

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

    le_void_t le_server_io_flush( le_server_t * const le_server ) {
        
        /* Parsing streams */
        for ( le_size_t le_parse = 0; le_parse < le_server->sv_area; le_parse ++ ) {

            /* Check stream state - flush stream */
            if ( le_server->sv_stream[le_parse] != NULL ) fflush( le_server->sv_stream[le_parse] );

        }

    }

    le_void_t le_server_io_close( le_server_t * const le_server ) {

        /* Parsing scales streams */
        for( le_size_t le_parse = 0; le_parse < le_server->sv_area; le_parse ++ ) {

            /* Check stream */
            if ( le_server->sv_stream[le_parse] != NULL ) {

                /* Close scale stream */
                fclose( le_server->sv_stream[le_parse] );

                /* Invalidate pointer */
                le_server->sv_stream[le_parse] = NULL;

            }

        }

    }

