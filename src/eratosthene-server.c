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

    le_server_t le_server_create( le_sock_t const le_port, le_char_t const * const le_path ) {

        /* created structure variables */
        le_server_t le_server = LE_SERVER_C;

        /* address variables */
        struct sockaddr_in le_addr = LE_ADDRIN_C_PORT( le_port );

        /* stream variables */
        FILE * le_stream = NULL;

        /* parsing stream stack */
        for ( le_size_t le_parse = 0; le_parse < _LE_USE_STREAM; le_parse ++ ) {

            /* initialise stream time */
            le_server.sv_time[le_parse] = _LE_TIME_NULL;

            /* parsing stream handles */
            for ( le_size_t le_index = 0; le_index < _LE_USE_DEPTH; le_index ++ ) {

                /* initialise stream handle */
                le_server.sv_file[le_parse][le_index] = NULL;

            }

        }

        /* open configuration stream */
        if ( ( le_stream = fopen( strcat( strcat( ( char * ) le_server.sv_path, ( char * ) le_path ), "/system" ), "r" ) ) == NULL ) {

            /* send message */
            return( le_server._status = LE_ERROR_IO_ACCESS, le_server );

        } else {

            /* check configuration reading */
            if ( fscanf( le_stream, "%" _LE_SIZE_S " %" _LE_TIME_S, & le_server.sv_scfg, & le_server.sv_tcfg ) != 2 ) {

                /* close configuration stream */
                fclose( le_stream );

                /* send message */
                return( le_server._status = LE_ERROR_IO_READ, le_server );

            }

            /* close configuration stream */
            fclose( le_stream );

            /* remove server path configuration suffix */
            le_server.sv_path[strlen( ( char * ) le_path )] = '\0';

            /* check consistency */
            if ( ( le_server.sv_scfg <= 0 ) || ( le_server.sv_scfg >= _LE_USE_DEPTH ) ) {

                /* send message */
                return( le_server._status = LE_ERROR_DEPTH, le_server );

            }

            /* check consistency */
            if ( le_server.sv_tcfg <= 0 ) {

                /* Send message */
                return( le_server._status = LE_ERROR_TIME, le_server );

            }

        }

        /* create socket */
        if ( ( le_server.sv_sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == _LE_SOCK_NULL ) {

            /* send message */
            return( le_server._status = LE_ERROR_IO_SOCKET, le_server );

        } else {

            /* bind address to socket */
            if ( bind( le_server.sv_sock, ( struct sockaddr * ) ( & le_addr ), sizeof( struct sockaddr_in ) ) == _LE_SOCK_NULL ) {

                /* close socket */
                close( le_server.sv_sock );

                /* send message */
                return( le_server._status = LE_ERROR_IO_BIND, le_server );

            }

            /* enable socket listening */
            if ( listen( le_server.sv_sock, _LE_USE_PENDING ) == _LE_SOCK_NULL ) {

                /* close socket */
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

        /* check socket - close socket */
        if ( le_server->sv_sock != _LE_SOCK_NULL ) close( le_server->sv_sock );

        /* parsing stream stack */
        for ( le_size_t le_parse = 0; le_parse < _LE_USE_STREAM; le_parse ++ ) {

            /* parsing stream handles */
            for ( le_size_t le_index = 0; le_index < _LE_USE_STREAM; le_index ++ ) {

                /* check stream handle */
                if ( le_server->sv_file[le_parse][le_index] != NULL ) {

                    /* close stream handle */
                    fclose( le_server->sv_file[le_parse][le_index] );

                }

            }

        }

        /* delete structure */
        * le_server = le_delete;

    }

/*
    source - server methods
 */

    le_void_t le_server( le_server_t * const le_server ) {

        /* client socket variables */
        le_sock_t le_client = _LE_SOCK_NULL;

        /* client address variables */
        struct sockaddr_in le_addr = LE_ADDRIN_C;

        /* client address length variables */
        socklen_t le_len = sizeof( struct sockaddr_in );

        /* check consistency */
        if ( le_server->sv_sock == _LE_SOCK_NULL ) return;

        /* accept incoming connection */
        for (;;) if ( ( le_client = accept( le_server->sv_sock, ( struct sockaddr * ) & le_addr, & le_len ) ) != _LE_SOCK_NULL ) {

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

            /* close client connection */
            close( le_client );

        }

    }

/*
    source - client methods - injection
 */

    le_void_t le_server_inject_client( le_server_t * const le_server, le_sock_t const le_client ) {

        /* time variables */
        le_time_t le_time = _LE_TIME_NULL;

        /* parsing variables */
        le_size_t le_parse = 0;

        /* socket i/o count variables */
        le_size_t le_count = 0;
        le_size_t le_round = 0;

        /* socket i/o reading variables */
        le_size_t le_retry  = 0;
        le_size_t le_bridge = 0;

        /* socket i/o circular buffer variables */
        le_byte_t le_buffer[LE_BUFFER_STRM] = LE_BUFFER_C;

        /* read injection time - abort injection */
        if ( read( le_client, & le_time, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) return;

        /* injection streaming loop */
        while ( le_retry < _LE_USE_RETRY ) {

            /* read streaming bloc */
            if ( ( le_count = read( le_client, le_buffer + le_bridge, _LE_USE_MTU ) + le_bridge ) > 0 ) {

                /* compute available records */
                if ( ( le_round = le_count - ( le_count % LE_ARRAY_SD ) ) != 0 ) {

                    /* parsing received streaming bloc */
                    for ( le_parse = 0; le_parse < le_round; le_parse += LE_ARRAY_SD ) {

                        /* inject received element */
                        le_server_inject( le_server, le_array_sd_pose_( le_buffer, le_parse ), le_time, le_array_sd_data_( le_buffer, le_parse ) );

                    }

                    /* compute bridge value */
                    if ( ( le_bridge = le_count % LE_ARRAY_SD ) != 0 ) {

                        /* apply circular condition */
                        memcpy( le_buffer, le_buffer + le_count - le_bridge, le_bridge );

                    }

                /* update bridge value */
                } else { le_bridge = le_count; }

            /* update retry flag */
            le_retry = 0; } else { le_retry ++; }

        }

    }

    le_void_t le_server_inject( le_server_t * const le_server, le_real_t * const le_pose, le_time_t const le_time, le_data_t const * const le_data ) {

        /* class variables */
        le_class_t le_class = LE_CLASS_C;

        /* address variables */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_server->sv_scfg - 1 );

        /* stream variables */
        le_size_t le_stream = _LE_USE_STREAM;

        /* depth variables */
        le_size_t le_parse = 0;
        le_size_t le_panex = 1;

        /* offset variables */
        le_size_t le_offset = 0;
        le_size_t le_offnex = 0;

        /* system scale stream management */
        if ( ( le_stream = le_server_io_stream( le_server, le_time, LE_SERVER_WRITE ) ) == _LE_USE_STREAM ) {

            /* abort injection */
            return;

        }

        /* compute address index */
        le_address_set_pose( & le_addr, le_pose );

        /* injection process */
        do {

            /* class importation */
            if ( le_class_io_read( & le_class, le_offnex, le_server->sv_file[le_stream][le_parse] ) == LE_ERROR_SUCCESS ) {

                /* inject element in class */
                le_class_set_push( & le_class, le_data );

            } else {

                /* initialise class with element */
                le_class = le_class_create( le_data );

            }

            /* retrieve daughter offset */
            le_offset = le_class_get_offset( & le_class, le_address_get_digit( & le_addr, le_parse ) );

            /* check daughter state */
            if ( ( le_offset == _LE_OFFS_NULL ) && ( ( le_panex ) != le_server->sv_scfg ) ) {

                /* seek next scale eof */
                fseek( le_server->sv_file[le_stream][le_panex], 0, SEEK_END );

                /* assign eof offset */
                le_offset = ftell( le_server->sv_file[le_stream][le_panex] );

                /* insert offset in class */
                le_class_set_offset( & le_class, le_address_get_digit( & le_addr, le_parse ), le_offset );

            }

            /* class exportation */
            le_class_io_write( & le_class, le_offnex, le_server->sv_file[le_stream][le_parse] );

        /* injection process condition */
        } while ( ( le_offnex = le_offset, ++ le_panex, ++ le_parse ) < le_server->sv_scfg );

        /* flush injection stream */
        le_server_io_flush( le_server, le_stream );

    }
/*
    source - client methods - query
 */

    le_void_t le_server_query_client( le_server_t * const le_server, le_sock_t const le_client ) {

        /* array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* address variables */
        le_address_t le_addr = LE_ADDRESS_C;

        /* stream variables */
        le_size_t le_stream = _LE_USE_STREAM;

        /* read and decompose query */
        le_address_io_read( & le_addr, le_client );

        /* address time reduction */
        le_server_io_reduce( le_server, & le_addr );

        /* check address mode */
        if ( ( le_address_get_mode( & le_addr ) & 0x01 ) != 0 ) {

            /* create stream */
            le_stream = le_server_io_stream( le_server, le_address_get_time( & le_addr, 0 ), LE_SERVER_READ );

            /* send system query */
            le_server_query( le_server, & le_addr, & le_array, 0, 0, le_stream );

        }

        /* check address mode */
        if ( ( le_address_get_mode( & le_addr ) & 0x02 ) != 0 ) {

            /* create stream */
            le_stream = le_server_io_stream( le_server, le_address_get_time( & le_addr, 1 ), LE_SERVER_READ );

            /* send system query */
            le_server_query( le_server, & le_addr, & le_array, 0, 0, le_stream );

        }

        /* write query address on socket */
        le_address_io_write( & le_addr, le_client );

        /* purge array to socket */
        le_array_io_write( & le_array, le_client );

        /* unallocate array memory */
        le_array_delete( & le_array );

    }

    le_void_t le_server_query( le_server_t * const le_server, le_address_t * const le_addr, le_array_t * const le_array, le_size_t const le_parse, le_size_t le_offset, le_size_t const le_stream ) {

        /* class variables */
        le_class_t le_class = LE_CLASS_C;

        /* address variables */
        le_size_t le_size = le_address_get_size( le_addr );
        le_size_t le_span = le_address_get_span( le_addr );

        /* query initialisation */
        if ( le_parse == 0 ) {

            /* check consistency */
            if ( le_stream == _LE_USE_STREAM ) return;

            /* check consistency */
            if ( le_size + le_span >= le_server->sv_scfg ) return;

        }

        /* read class */
        if ( le_class_io_read( & le_class, le_offset, le_server->sv_file[le_stream][le_parse] ) == LE_ERROR_SUCCESS ) {

            /* query process mode */
            if ( le_parse < le_size ) {

                /* retreive daughter class offset */
                if ( ( le_offset = le_class_get_offset( & le_class, le_address_get_digit( le_addr, le_parse ) ) ) != _LE_OFFS_NULL ) {

                    /* recursive query */
                    le_server_query( le_server, le_addr, le_array, le_parse + 1, le_offset, le_stream );

                }

            } else if ( le_parse == le_size + le_span ) {

                /* position array variables */
                le_real_t le_pose[3] = { 0.0 };

                /* correct address size */
                le_address_set_size( le_addr, le_parse );

                /* retreive class representative */
                le_address_get_pose( le_addr, le_pose );

                /* restore address size */
                le_address_set_size( le_addr, le_size );

                /* inject gathered element in array */
                le_array_map_sd( le_array, le_pose, le_class_get_data( & le_class ) );

            } else {

                /* class daughters enumeration */
                for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                    /* update address index digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* retreive daughter class offset */
                    if ( ( le_offset = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                        /* recursive query */
                        le_server_query( le_server, le_addr, le_array, le_parse + 1, le_offset, le_stream );

                    }

                }

            }

        }

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
    source - i/o methods
 */

    le_size_t le_server_io_stream( le_server_t * const le_server, le_time_t le_time, le_enum_t const le_mode ) {

        /* stream stack variables */
        le_size_t le_head = le_server->sv_push;

        /* directory structure */
        DIR * le_directory = NULL;

        /* server storage path variables */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

        /* check consistency */
        if ( le_time == _LE_TIME_NULL ) {

            /* send message */
            return( _LE_USE_STREAM );

        }

        /* compute time equivalence class */
        le_time /= le_server->sv_tcfg;

        /* search stream stack for provided time */
        for ( le_size_t le_parse = 0; le_parse < _LE_USE_STREAM; le_parse ++ ) {

            /* check for matching time - return stream stack index */
            if ( le_server->sv_time[le_parse] == le_time ) return( le_parse );

        }

        /* compose directory path */
        sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P, le_server->sv_path, le_time );

        /* check if directory exists */
        if ( ( le_directory = opendir( ( char * ) le_path ) ) == NULL ) {

            /* check stream mode */
            if ( le_mode == LE_SERVER_READ ) {

                /* send message */
                return( _LE_USE_STREAM );

            } else {

                /* create directory */
                mkdir( ( char * ) le_path, 0755 );

            }

        } else {

            /* close directory */
            closedir( le_directory );

        }

        /* invalidate stream time */
        le_server->sv_time[le_head] = _LE_TIME_NULL;

        /* create stream */
        for ( le_size_t le_parse = 0; le_parse < le_server->sv_scfg; le_parse ++ ) {

            /* check previous file descriptor */
            if ( le_server->sv_file[le_head][le_parse] != NULL ) {

                /* close previous file descriptor */
                fclose( le_server->sv_file[le_head][le_parse] );

            }

            /* compose file path */
            sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_server->sv_path, le_time, le_parse );

            /* create read/write access descriptor - nice */
            if ( ( le_server->sv_file[le_head][le_parse] = fopen( ( char * ) le_path, "r+" ) ) == NULL ) {

                /* check stream mode */
                if ( le_mode == LE_SERVER_READ ) {

                    /* send message */
                    return( _LE_USE_STREAM );

                } else {

                    /* create read/write access descriptor - agressive */
                    if ( ( le_server->sv_file[le_head][le_parse] = fopen( ( char * ) le_path, "w+" ) ) == NULL ) {

                        /* send message */
                        return( _LE_USE_STREAM );

                    }

                }

            }

        }

        /* update stream time */
        le_server->sv_time[le_head] = le_time;

        /* update stream stack head */
        le_server->sv_push = ( le_server->sv_push + 1 ) % _LE_USE_STREAM;

        /* return created stream stack index */
        return( le_head );

    }

    le_void_t le_server_io_flush( le_server_t const * const le_server, le_size_t const le_stream ) {

        /* parsing stream handles */
        for ( le_size_t le_index = 0; le_index < le_server->sv_scfg; le_index ++ ) {

            /* check stream handle */
            if ( le_server->sv_file[le_stream][le_index] != NULL ) {

                /* flush handle output buffer */
                fflush( le_server->sv_file[le_stream][le_index] );

            }

        }

    }

    le_void_t le_server_io_reduce( le_server_t const * const le_server, le_address_t * const le_addr ) {

        /* directory structure variables */
        DIR * le_dir = NULL;

        /* entity structure variables */
        struct dirent * le_ent = NULL;

        /* address times variables */
        le_time_t le_timea = le_address_get_time( le_addr, 0 );
        le_time_t le_timeb = le_address_get_time( le_addr, 1 );

        /* nearest time variables */
        le_time_t le_neara = _LE_TIME_NULL;
        le_time_t le_nearb = _LE_TIME_NULL;

        /* time distance variables */
        le_time_t le_diffa = _LE_TIME_MAX;
        le_time_t le_diffb = _LE_TIME_MAX;

        /* time reduction variables */
        le_time_t le_times = _LE_TIME_NULL;
        le_time_t le_timed = _LE_TIME_NULL;

        /* open directory */
        le_dir = opendir( ( char * ) le_server->sv_path );

        /* enumerate entities */
        while ( ( le_ent = readdir( le_dir ) ) != NULL ) {

            /* regular directory required */
            if ( ( le_ent->d_type == DT_DIR ) && ( le_ent->d_name[0] != '.' ) ) {

                /* retreive allocation time */
                le_times = le_time_str( le_ent->d_name ) * le_server->sv_tcfg;

                /* compare differences */
                if ( ( le_timed = le_time_abs( le_timea - le_times ) ) < le_diffa ) {

                    /* update nearest time */
                    le_neara = le_times;

                    /* update nearest distance */
                    le_diffa = le_timed;

                }

                /* compare differences */
                if ( ( le_timed = le_time_abs( le_timeb - le_times ) ) < le_diffb ) {

                    /* update nearest time */
                    le_nearb = le_times;

                    /* update nearest distance */
                    le_diffb = le_timed;

                }

            }

        }

        /* update address structure */
        le_address_set_time( le_addr, 0, le_neara  );
        le_address_set_time( le_addr, 1, le_nearb );

        /* close directory */
        closedir( le_dir );

    }

