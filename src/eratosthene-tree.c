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

    le_tree_t le_tree_create( le_char_t * const le_root, le_size_t const le_scfg, le_time_t const le_tcfg ) {

        /* created structure variable */
        le_tree_t le_tree = LE_TREE_C_CONF( le_root, le_scfg, le_tcfg );

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

            /* select directories */
            if ( le_entity->d_type == DT_DIR ) {

                /* avoid special directories */
                if ( ( * le_entity->d_name ) != '.' ) {

                    /* create and push unit */
                    le_tree_set_push( & le_tree, le_time_str( le_entity->d_name ) * le_tree.tr_tcfg, LE_UNIT_READ );

                }

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

    le_unit_t * le_tree_get_unit( le_tree_t * const le_tree, le_time_t const le_time, le_enum_t const le_mode ) {

        /* reduced time variable */
        le_time_t le_reduce = le_time / le_tree->tr_tcfg;

        /* parsing variable */
        le_size_t le_parse = le_tree->tr_size;

        /* parsing tree units */
        while ( ( -- le_parse ) >= 0 ) {

            /* compare times */
            if ( le_unit_get_time( le_tree->tr_unit + le_parse ) == le_reduce ) {

                /* return unit */
                return( le_tree->tr_unit + le_parse );

            }

        }

        /* check mode */
        if ( le_mode == LE_UNIT_WRITE ) {

            /* create and push unit */
            return( le_tree_set_push( le_tree, le_time, LE_UNIT_WRITE ) );

        /* return unit */
        } else { return( NULL ); }

    }

    le_unit_t * le_tree_get_query_beta( le_tree_t const * const le_tree, le_address_t * const le_addr, le_size_t const le_addrt, le_size_t * const le_offset ) {

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
            if ( ( ( * le_offset ) = le_unit_io_offset( le_tree->tr_unit + le_select, le_addr ) ) != _LE_OFFS_NULL ) {

                /* return unit */
                return( le_tree->tr_unit + le_select );

            } else {

                le_char_t le_path[_LE_USE_PATH] = { 0 };

                le_size_t le_length = 0;

                le_unit_t * le_temp = le_tree->tr_unit + le_select;

                sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/2/", le_temp->un_root, le_temp->un_time );

                le_length = strlen( ( char * ) le_path );

                for ( le_size_t le_parse = 0; le_parse < le_address_get_size( le_addr ); le_parse ++ ) {

                    le_path[le_length+le_parse] = le_address_get_digit( le_addr, le_parse ) + 48;

                }

                if ( le_get_exist( le_path ) == _LE_TRUE ) {

                    return( le_temp );

                }

            }

        }

        /* send message */
        return( NULL );

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
            if ( ( ( * le_offset ) = le_unit_io_offset( le_tree->tr_unit + le_select, le_addr ) ) != _LE_OFFS_NULL ) {

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

        /* create unit */
        if ( le_get_status( le_unit = le_unit_create( le_tree->tr_root, le_tree->tr_scfg, le_reduce, le_mode ) ) != LE_ERROR_SUCCESS ) {

            /* send message */
            return( NULL );

        }

        /* unit stack memory re-allocation */
        if ( ( le_swap = realloc( le_tree->tr_unit, sizeof( le_unit_t ) * ( le_tree->tr_size + 1 ) ) ) == NULL ) {

            /* delete created unit */
            le_unit_delete( & le_unit );

            /* send message */
            return( NULL );

        }

        /* swap memory allocation */
        le_tree->tr_unit = le_swap;

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

