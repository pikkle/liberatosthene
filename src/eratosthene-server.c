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

        /* assign server path */
        strcpy( ( char * ) le_server.sv_path, ( char * ) le_path );

        /* create server configuration */
        if ( ( le_server._status = le_server_set_config( & le_server ) ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( le_server );

        }

        /* create server stack */
        if ( ( le_server._status = le_server_set_enum( & le_server ) ) != LE_ERROR_SUCCESS ) {

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

        /* check stack */
        if ( le_server->sv_time != NULL ) {

            /* release stack memory */
            free( le_server->sv_time );

        }

        /* check socket */
        if ( le_server->sv_sock != _LE_SOCK_NULL ) {

            /* close socket */
            close( le_server->sv_sock );

        }

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

        /* client address length variables */
        socklen_t le_len = sizeof( struct sockaddr_in );

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
        le_stream_t le_stream = LE_STREAM_C;

        /* array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* read time - abort injection */
        if ( read( le_client, & le_time, sizeof( le_time_t ) ) != sizeof( le_time_t ) ) return;

        /* read client array */
        le_array_io_read( & le_array, le_client );

        /* check array size - security check */
        if ( le_array_get_size( & le_array ) >= LE_ARRAY_SD ) {

            /* check array size - security check */
            if ( ( le_array_get_size( & le_array ) % LE_ARRAY_SD ) == 0 ) {

                /* create stream */
                if ( ( le_stream = le_stream_create( le_server->sv_path, le_time, le_server->sv_scfg, le_server->sv_tcfg, LE_STREAM_WRITE ) )._status == LE_ERROR_SUCCESS ) {

                    /* inject array */
                    le_server_inject( le_server, & le_array, & le_stream );

                    /* create server stack */
                    le_server_set_enum( le_server );

                }

                /* delete stream */
                le_stream_delete( & le_stream );

            }

        }

        /* delete array */
        le_array_delete( & le_array );

    }

    le_void_t le_server_inject( le_server_t * const le_server, le_array_t const * const le_array, le_stream_t const * const le_stream ) {

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
                if ( le_class_io_read( & le_class, le_offnex, le_stream_get( le_stream, le_parse ) ) == LE_ERROR_SUCCESS ) {

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
                    fseek( le_stream_get( le_stream, le_panex ), 0, SEEK_END );

                    /* assign eof offset */
                    le_offset = ftell( le_stream_get( le_stream, le_panex ) );

                    /* insert offset in class */
                    le_class_set_offset( & le_class, le_address_get_digit( & le_addr, le_parse ), le_offset );

                }

                /* class exportation */
                le_class_io_write( & le_class, le_offnex, le_stream_get( le_stream, le_parse ) );

            /* injection process condition */
            } while ( ( le_offnex = le_offset, ++ le_panex, ++ le_parse ) < le_server->sv_scfg );
            

        }

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
        le_stream_t le_stream = LE_STREAM_C;

        /* address variables */
        le_size_t le_size = 0;
        le_size_t le_span = 0;

        /* read and decompose query */
        le_address_io_read( & le_addr, le_client );

        /* extract address parameters */
        le_size = le_address_get_size( & le_addr );
        le_span = le_address_get_span( & le_addr );

        /* check consistency */
        if ( ( le_size + le_span ) >= le_server->sv_scfg ) {

            /* abort query */
            return;

        }

        /* check address mode */
        if ( ( le_address_get_mode( & le_addr ) & 0x01 ) != 0 ) {

            /* reduce address time */
            le_server_reduce( le_server, & le_addr, 0 );

            /* create stream */
            if ( ( le_stream = le_stream_create( le_server->sv_path, le_address_get_time( & le_addr, 0 ), le_server->sv_scfg, le_server->sv_tcfg, LE_STREAM_READ ) )._status == LE_ERROR_SUCCESS ) {

                /* send system query */
                le_server_query( le_server, & le_addr, le_size, le_span, & le_array, 0, 0, & le_stream );

            }

            /* delete stream */
            le_stream_delete( & le_stream );

        }

        /* check address mode */
        if ( ( le_address_get_mode( & le_addr ) & 0x02 ) != 0 ) {

            /* reduce address time */
            le_server_reduce( le_server, & le_addr, 1 );

            /* create stream */
            if ( ( le_stream = le_stream_create( le_server->sv_path, le_address_get_time( & le_addr, 1 ), le_server->sv_scfg, le_server->sv_tcfg, LE_STREAM_READ ) )._status == LE_ERROR_SUCCESS ) {

                /* send system query */
                le_server_query( le_server, & le_addr, le_size, le_span, & le_array, 0, 0, & le_stream );

            }

            /* delete stream */
            le_stream_delete( & le_stream );

        }

        /* write query address on socket */
        le_address_io_write( & le_addr, le_client );

        /* purge array to socket */
        le_array_io_write( & le_array, le_client );

        /* unallocate array memory */
        le_array_delete( & le_array );

    }

    le_void_t le_server_query( le_server_t * const le_server, le_address_t * const le_addr, le_size_t const le_size, le_size_t const le_span, le_array_t * const le_array, le_size_t const le_parse, le_size_t le_offset, le_stream_t const * const le_stream ) {

        /* class variables */
        le_class_t le_class = LE_CLASS_C;

        /* read class */
        if ( le_class_io_read( & le_class, le_offset, le_stream_get( le_stream, le_parse ) ) == LE_ERROR_SUCCESS ) {

            /* query process mode */
            if ( le_parse < le_size ) {

                /* retreive daughter class offset */
                if ( ( le_offset = le_class_get_offset( & le_class, le_address_get_digit( le_addr, le_parse ) ) ) != _LE_OFFS_NULL ) {

                    /* recursive query */
                    le_server_query( le_server, le_addr, le_size, le_span, le_array, le_parse + 1, le_offset, le_stream );

                }

            } else if ( le_parse == ( le_size + le_span ) ) {

                /* pepare array for injection */
                le_array_set( le_array, LE_ARRAY_SD );

                /* correct address size */
                le_address_set_size( le_addr, le_parse );

                /* inject position coordinates */
                le_address_get_pose( le_addr, ( le_real_t * ) le_array_sd_pose_al( le_array ) );

                /* inject color components */
                le_class_get_data( & le_class, ( le_data_t * ) le_array_sd_data_al( le_array ) );

                /* restore address size */
                le_address_set_size( le_addr, le_size );

            } else {

                /* class daughters enumeration */
                for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                    /* update address index digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* retreive daughter class offset */
                    if ( ( le_offset = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                        /* recursive query */
                        le_server_query( le_server, le_addr, le_size, le_span, le_array, le_parse + 1, le_offset, le_stream );

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

    le_enum_t le_server_set_enum( le_server_t * const le_server ) {

        /* parsing variables */
        le_diff_t le_parse = 0;

        /* time variables */
        le_time_t le_time = _LE_TIME_NULL;

        /* memory allocation swap variables */
        le_void_t * le_swap = NULL;

        /* directory structure variables */
        DIR * le_dir = NULL;

        /* entity structure variables */
        struct dirent * le_ent = NULL;

        /* check stack state */
        if ( le_server->sv_time != NULL ) {

            /* release stack memory */
            free( le_server->sv_time );

            /* invalidate pointer */
            le_server->sv_time = NULL;

        }

        /* open directory */
        le_dir = opendir( ( char * ) le_server->sv_path );

        /* enumerate directory entities */
        while ( ( le_ent = readdir( le_dir ) ) != NULL ) {

            /* select storage directory */
            if ( ( le_ent->d_type == DT_DIR ) && ( le_ent->d_name[0] != '.' ) ) {

                /* reallocate memory */
                if ( ( le_swap = realloc( le_server->sv_time, ( le_server->sv_size + 1 ) * sizeof( le_time_t ) ) ) == NULL ) {

                    /* send message */
                    return( LE_ERROR_MEMORY );

                }

                /* assign memory allocation */
                le_server->sv_time = ( le_time_t * ) le_swap;

                /* convert directory name to time */
                le_time = le_time_str( le_ent->d_name );

                /* reset insertion search */
                le_parse = 0;

                /* search insertion position */
                while ( le_parse < le_server->sv_size ) {

                    /* check position */
                    if ( le_server->sv_time[le_parse] <= le_time ) le_parse ++; else break;

                }

                /* shift array elements */
                for ( le_diff_t le_shift = le_server->sv_size; le_shift > le_parse; le_shift -- ) {

                    /* shift element */
                    le_server->sv_time[le_shift] = le_server->sv_time[le_shift - 1];

                }

                /* ordered insertion */
                le_server->sv_time[le_parse] = le_time;

                /* update stack size */
                le_server->sv_size ++;

            }

        }

        /* close directory */
        closedir( le_dir );

        /* send message */
        return( LE_ERROR_SUCCESS );

    }

/*
    source - specialised methods
 */

    le_void_t le_server_reduce_beta( le_server_t * const le_server, le_address_t * const le_addr, le_size_t const le_index ) {

        /* address time variables */
        le_time_t le_atime = le_address_get_time( le_addr, le_index ) / le_server->sv_tcfg;

        /* address span variables */
        le_size_t le_span = le_address_get_span( le_addr );

        /* teme variables */
        le_time_t le_try = _LE_TIME_NULL;
        le_time_t le_red = _LE_TIME_NULL;

        /* index variables */
        le_diff_t le_lindex = 0;
        le_diff_t le_uindex = 0;

        /* array variables */
        le_array_t le_array = LE_ARRAY_C;

        /* stream variables */
        le_stream_t le_stream = LE_STREAM_C;

        /* reset address span */
        le_address_set_span( le_addr, 0 );

        /* search intervalle of address time */
        while ( ( le_lindex < le_server->sv_size ) && ( le_atime >= le_server->sv_time[le_lindex] ) ) le_lindex ++;

        /* create boundaries of address time intervalle */
        le_uindex = le_lindex;
        le_lindex = le_lindex - 1;

        /* search local-reduced time */
        while ( ( le_lindex >= 0 ) || ( le_uindex < le_server->sv_size ) ) {

            /* check intervalle boundary */
            if ( le_lindex >= 0 ) {

                /* check intervalle boundary */
                if ( le_uindex < le_server->sv_size ) {

                    /* compute proximal time */
                    if ( le_time_abs( le_server->sv_time[le_lindex] - le_atime ) < le_time_abs( le_server->sv_time[le_uindex] - le_atime ) ) {

                        /* assign reduced time candidate */
                        le_try = le_server->sv_time[le_lindex] * le_server->sv_tcfg;

                        /* update search */
                        le_lindex --;

                    } else {

                        /* assign reduced time candidate */
                        le_try = le_server->sv_time[le_uindex] * le_server->sv_tcfg;

                        /* update search */
                        le_uindex ++;

                    }

                } else {

                    /* assign reduced time candidate */
                    le_try = le_server->sv_time[le_lindex] * le_server->sv_tcfg;

                    /* update search */
                    le_lindex --;

                }

            } else {

                /* assign reduced time candidate */
                le_try = le_server->sv_time[le_uindex] * le_server->sv_tcfg;

                /* update search */
                le_uindex ++;

            }

            /* assign address candidate */
            le_address_set_time( le_addr, le_index, le_try );

            /* create stream */
            le_stream = le_stream_create( le_server->sv_path, le_try, le_server->sv_scfg, le_server->sv_tcfg, LE_STREAM_READ );

            /* perform time reduction query */
            le_server_query( le_server, le_addr, le_address_get_size( le_addr ), 0, & le_array, 0, 0, & le_stream );

            /* delete stream */
            le_stream_delete( & le_stream );

            /* check time reduction query */
            if ( le_array_get_size( & le_array ) > 0 ) {

                /* assign reduced time */
                le_red = le_try;

                /* cancel search */
                le_lindex = -1;
                le_uindex = le_server->sv_size;

            }

            /* clear array */
            le_array_set_size( & le_array, 0 );

        }

        /* retore address span */
        le_address_set_span( le_addr, le_span );

        /* assign address local-reduced time */
        le_address_set_time( le_addr, le_index, le_red );

    }

    le_void_t le_server_reduce( le_server_t const * const le_server, le_address_t * const le_addr, le_size_t const le_index ) {

        /* address time variables */
        le_time_t le_atime = le_address_get_time( le_addr, le_index ) / le_server->sv_tcfg;

        /* reduction variables */
        le_time_t le_rtime = _LE_TIME_NULL;
        le_time_t le_dtime = _LE_TIME_MAX;
        le_time_t le_stime = _LE_TIME_NULL;

        /* parsing server stack */
        for ( le_size_t le_parse = 0; le_parse < le_server->sv_size; le_parse ++ ) {

            /* compute and check distance */
            if ( ( le_stime = le_time_abs( le_server->sv_time[le_parse] - le_atime ) ) < le_dtime ) {

                /* update distance */
                le_dtime = le_stime;

                /* update reduced time */
                le_rtime = le_server->sv_time[le_parse];

            }

        }

        /* assign address time */
        le_address_set_time( le_addr, le_index, ( le_rtime != _LE_TIME_NULL ) ? le_rtime * le_server->sv_tcfg : le_rtime );

    }

