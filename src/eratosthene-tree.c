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

    # include "eratosthene-tree.h"

/*
    source - constructor/destructor methods
 */

    le_tree_t le_tree_create( le_char_t * const le_path, le_size_t const le_scfg, le_time_t const le_tcfg ) {

        /* created structure variable */
        le_tree_t le_tree = LE_TREE_C_CONF( le_path, le_scfg, le_tcfg );

        /* directory variable */
        DIR * le_directory = NULL;

        /* directory entity variable */
        struct dirent * le_entity = NULL;

        /* check consistency */
        if ( le_tree.tr_root == NULL ) {

            /* send message */
            return( le_set_status( le_tree, LE_ERROR_IO_ACCESS ) );

        }

        /* compute reduced comb */
        le_tree.tr_comb /= le_tree.tr_tcfg;

        /* open and check directory */
        if ( ( le_directory = opendir( ( char * ) le_tree.tr_root ) ) == NULL ) {

            /* send message */
            return( le_set_status( le_tree, LE_ERROR_IO_READ ) );

        }

        /* directory entity enumeration */
        while ( ( ( le_entity = readdir( le_directory ) ) != NULL ) && ( le_tree._status == LE_ERROR_SUCCESS ) ) {

            /* check consistency */
            if ( le_entity->d_type != DT_DIR ) {

                /* continue entity enumeration */
                continue;

            }

            /* check consistency */
            if ( ( * le_entity->d_name ) == '.' ) {

                /* continue entity enumeration */
                continue;

            }

            /* create and push unit */
            if ( le_tree_set_push( & le_tree, le_time_str( le_entity->d_name ) * le_tree.tr_tcfg, LE_UNIT_READ ) == NULL ) {

                /* update structure status */
                le_tree._status = LE_ERROR_IO_ACCESS;

            }

        }

        /* close directory */
        closedir( le_directory );

        /* return created structure */
        return( le_tree );

    }

    le_void_t le_tree_delete( le_tree_t * const le_tree ) {

        /* deleted structure variable */
        le_tree_t le_delete = LE_TREE_C;

        /* check unit stack */
        if ( le_tree->tr_size > 0 ) {

            /* parsing unit stack */
            for ( le_size_t le_parse = 0; le_parse < le_tree->tr_size; le_parse ++ ) {

                /* delete stack unit */
                le_unit_delete( le_tree->tr_unit + le_parse );

            }

            /* release stack memory */
            free( le_tree->tr_unit );

        }

        /* delete structure */
        ( * le_tree ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_unit_t * le_tree_get_inject( le_tree_t * const le_tree, le_time_t const le_time ) {

        /* reduced time variable */
        le_time_t le_reduce = le_time / le_tree->tr_tcfg;

        /* parsing variable */
        le_size_t le_parse = le_tree->tr_size;

        /* parsing tree units */
        while ( ( -- le_parse ) >= 0 ) {

            /* check time */
            if ( le_unit_get_time( le_tree->tr_unit + le_parse ) == le_reduce ) {

                /* return unit */
                return( le_tree->tr_unit + le_parse );

            }

        }
        
        /* create and push unit */
        return( le_tree_set_push( le_tree, le_time, LE_UNIT_WRITE ) );

    }

    le_unit_t * le_tree_get_query( le_tree_t const * const le_tree, le_address_t * const le_addr, le_size_t const le_addrt, le_size_t * const le_offset ) {

        /* time variable */
        le_time_t le_time = le_address_get_time( le_addr, le_addrt ) / le_tree->tr_tcfg;

        /* index variable */
        le_size_t le_dindex = le_tree->tr_size;
        le_size_t le_gindex = 0;

        /* time variable */
        le_time_t le_dtime = 0;
        le_time_t le_gtime = 0;

        /* parsing variable */
        le_size_t le_select = _LE_SIZE_NULL;

        /* check tree state */
        if ( le_tree->tr_size == 0 ) {

            /* send message */
            return( NULL );

        }

        /* case study */
        if ( le_time <= le_unit_get_time( le_tree->tr_unit ) ) {

            /* initialise index */
            le_dindex = -1;

        } else {

            /* case study */
            if ( le_time >= le_unit_get_time( le_tree->tr_unit + le_tree->tr_size - 1 ) ) {

                /* initialise index */
                le_gindex = le_dindex --;

            } else {

                /* initialise index */
                while ( le_time < le_unit_get_time( le_tree->tr_unit + ( -- le_dindex ) ) );

                /* initialise index */
                le_gindex = le_dindex + 1;

            }

        }

        /* temporal reduction procedure */
        while ( ( le_dindex >= 0 ) || ( le_gindex < le_tree->tr_size ) ) {

            /* case study */
            if ( le_dindex < 0 ) {

                /* push index */
                le_select = le_gindex ++;

                /* comb condition */
                if ( ( le_unit_get_time( le_tree->tr_unit + le_select ) - le_time ) > le_tree->tr_comb ) {

                    /* send message */
                    return( NULL );

                }

            } else {

                /* case study */
                if ( le_gindex >= le_tree->tr_size ) {

                    /* push index */
                    le_select = le_dindex --;

                    /* comb condition */
                    if ( ( le_time - le_unit_get_time( le_tree->tr_unit + le_select ) ) > le_tree->tr_comb ) {

                        /* send message */
                        return( NULL );

                    }

                } else {

                    /* decreasing index distance */
                    le_dtime = le_time - le_unit_get_time( le_tree->tr_unit + le_dindex );

                    /* growing index distance */
                    le_gtime = le_unit_get_time( le_tree->tr_unit + le_gindex ) - le_time;

                    /* case study */
                    if ( le_dtime < le_gtime ) {

                        /* push index */
                        le_select = le_dindex --;

                        /* comb condition */
                        if ( le_dtime > le_tree->tr_comb ) {

                            /* send message */
                            return( NULL );

                        }

                    } else {

                        /* push index */
                        le_select = le_gindex ++;

                        /* comb condition */
                        if ( le_gtime > le_tree->tr_comb ) {

                            /* send message */
                            return( NULL );

                        }

                    }

                }

            }

            /* check selected index */
            if ( ( ( * le_offset ) = le_tree_io_offset( le_tree->tr_unit + le_select, le_addr ) ) != _LE_OFFS_NULL ) {

                /* return unit */
                return( le_tree->tr_unit + le_select );

            }

        }

        /* send message */
        return( NULL );

    }

/*
    source - mutator methods
 */

    le_unit_t * le_tree_set_push( le_tree_t * const le_tree, le_time_t const le_time, le_enum_t const le_mode ) {

        /* unit variable */
        le_unit_t le_unit = LE_UNIT_C;

        /* memory swap variable */
        le_unit_t * le_swap = NULL;

        /* parsing variable */
        le_size_t le_parse = le_tree->tr_size;

        /* reduced time variable */
        le_time_t le_reduce = le_time / le_tree->tr_tcfg;

        /* unit stack memory re-allocation */
        if ( ( le_swap = realloc( le_tree->tr_unit, sizeof( le_unit_t ) * ( le_tree->tr_size + 1 ) ) ) == NULL ) {

            /* send message */
            return( NULL );

        }

        /* swap memory allocation */
        le_tree->tr_unit = le_swap;

        /* create unit */
        if ( le_get_status( le_unit = le_unit_create( le_tree->tr_root, le_reduce, le_mode, le_tree->tr_scfg ) ) != LE_ERROR_SUCCESS ) {

            /* delete unit */
            le_unit_delete( & le_unit );

            /* send message */
            return( NULL );

        }

        /* insertion process */
        while ( ( le_parse > 0 ) && ( le_unit_get_prior( le_tree->tr_unit + le_parse - 1, & le_unit ) == _LE_TRUE ) ) {

            /* unit shifting */
            le_tree->tr_unit[le_parse] = le_tree->tr_unit[le_parse-1];

            /* update index */
            le_parse --;

        }

        /* insert unit */
        le_tree->tr_unit[le_parse] = le_unit;

        /* update tree size */
        le_tree->tr_size ++;

        /* return unit pointer */
        return( le_tree->tr_unit + le_parse );

    }

/*
    source - i/o methods
 */

    le_size_t le_tree_io_offset( le_unit_t * const le_unit, le_address_t * const le_addr ) {

        /* returned value variables */
        le_size_t le_offset = 0;

        /* parsing variables */
        le_size_t le_parse = 0;

        /* address size variables */
        le_size_t le_size = le_address_get_size( le_addr );

        /* follow offsets */
        do {

            /* read class offset */
            le_offset = le_class_io_offset( le_offset, le_address_get_digit( le_addr, le_parse ), le_unit_get_stream( le_unit, le_parse ) );

        /* update index and condition */
        } while ( ( ( ++ le_parse ) < le_size ) && ( le_offset != _LE_OFFS_NULL ) );

        /* return offset */
        return( le_offset );

    }

    le_void_t le_tree_io_inject( le_unit_t * const le_unit, le_array_t const * const le_array, le_size_t const le_scfg ) {

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* address variable */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_scfg - 1 );

        /* digit variable */
        le_size_t le_digit = 0;

        /* scale variable */
        le_size_t le_scale = 0;
        le_size_t le_ahead = 0;

        /* offset variable */
        le_size_t le_forward = 0;
        le_size_t le_current = 0;

        /* array size variable */
        le_size_t le_size = le_array_get_size( le_array );

        /* parsing array */
        for ( le_size_t le_index = 0; le_index < le_size; le_index += LE_ARRAY_UF3 ) {

            /* compute address digits */
            le_address_set_pose( & le_addr, le_array_mac_pose( le_array, le_index ) );

            /* reset scale */
            le_scale = 0;
            le_ahead = 1;

            /* reset offset */
            le_forward = 0;
            le_current = 0;

            /* injection process */
            do {

                /* class importation */
                if ( le_class_io_read( & le_class, le_current, le_unit_get_stream( le_unit, le_scale ) ) == LE_ERROR_SUCCESS ) {

                    /* inject element in class */
                    le_class_set_push( & le_class, le_array_mac_data( le_array, le_index ) );

                } else {

                    /* initialise class with element */
                    le_class = le_class_create( le_array_mac_data( le_array, le_index ) );

                }

                /* extract address digit */
                le_digit = le_address_get_digit( & le_addr, le_scale );

                /* retrieve daughter offset */
                le_forward = le_class_get_offset( & le_class, le_digit );

                /* check daughter state */
                if ( ( le_forward == _LE_OFFS_NULL ) && ( le_ahead != le_scfg ) ) {

                    /* seek next scale eof */
                    fseek( le_unit_get_stream( le_unit, le_ahead ), 0, SEEK_END );

                    /* compute and check injection offset */
                    if ( ( le_forward = ftell( le_unit_get_stream( le_unit, le_ahead ) ) ) < _LE_OFFS_NULL ) {

                        /* insert offset in class */
                        le_class_set_offset( & le_class, le_digit, le_forward );

                    }

                }

                /* class exportation */
                le_class_io_write( & le_class, le_current, le_unit_get_stream( le_unit, le_scale ) );

            /* injection process condition */
            } while ( ( ( le_current = le_forward, ++ le_ahead, ++ le_scale ) < le_scfg ) && ( le_forward < _LE_OFFS_NULL ) );

        }

    }

    le_void_t le_tree_io_gather( le_unit_t * const le_unit, le_address_t * const le_addr, le_size_t le_offset, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* enumeration boundary */
        if ( le_parse == le_span ) {

            /* update array size */
            le_array_set( le_array, LE_ARRAY_UF3 );

            /* psuh element position */
            le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

            /* push element data */
            le_class_io_data( le_offset, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unit, le_parse ) );

        } else {

            /* read class */
            le_class_io_readf( & le_class, le_offset, le_unit_get_stream( le_unit, le_parse ) );

            /* enumerate daughter classes */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* extract class offset */
                if ( ( le_offset = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_tree_io_gather( le_unit, le_addr, le_offset, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

    le_void_t le_tree_io_parallel( le_unit_t * const le_unia, le_unit_t * const le_unib, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t le_offseta, le_size_t le_offsetb, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variable */
        le_class_t le_classa = LE_CLASS_C;
        le_class_t le_classb = LE_CLASS_C;

        /* message variable */
        le_enum_t le_msga = LE_ERROR_IO_READ;
        le_enum_t le_msgb = LE_ERROR_IO_READ;

        /* enumeration boundary */
        if ( le_parse == le_span ) {

            /* switch on mode */
            if ( le_mode == 3 ) {

                /* update array size */
                le_array_set( le_array, LE_ARRAY_UF3 );

                /* psuh element position */
                le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                /* logical or operator */
                if ( le_offseta != _LE_OFFS_NULL ) {

                    /* push element data */
                    le_class_io_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );

                } else {

                    /* push element data */
                    le_class_io_data( le_offsetb, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unib, le_parse ) );

                }

            } else if ( le_mode == 4 ) {

                /* logical and operator */
                if ( ( le_offseta != _LE_OFFS_NULL ) && ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* psuh element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element data */
                    le_class_io_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );

                }

            } else if ( le_mode == 5 ) {

                /* logical xor operator */
                if ( ( le_offseta != _LE_OFFS_NULL ) && ( le_offsetb == _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* psuh element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element data */
                    le_class_io_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );

                } else if ( ( le_offseta == _LE_OFFS_NULL ) && ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* psuh element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element data */
                    le_class_io_data( le_offsetb, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unib, le_parse ) );

                }

            }

        } else {

            /* check offset */
            if ( le_offseta != _LE_OFFS_NULL ) {

                /* read class */
                le_msga = le_class_io_readf( & le_classa, le_offseta, le_unit_get_stream( le_unia, le_parse ) );

            }

            /* check offset */
            if ( le_offsetb != _LE_OFFS_NULL ) {

                /* read class */
                le_msgb = le_class_io_readf( & le_classb, le_offsetb, le_unit_get_stream( le_unib, le_parse ) );

            }

            /* enumerate daughter classes */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* check message */
                if ( le_msga == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    le_offseta = le_class_get_offset( & le_classa, le_digit );

                /* reset offset */
                } else { le_offseta = _LE_OFFS_NULL; }

                /* check message */
                if ( le_msgb == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    le_offsetb = le_class_get_offset( & le_classb, le_digit );

                /* reset offset */
                } else { le_offsetb = _LE_OFFS_NULL; }

                /* check offsets */
                if ( ( le_offseta != _LE_OFFS_NULL ) || ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_tree_io_parallel( le_unia, le_unib, le_addr, le_mode, le_offseta, le_offsetb, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

