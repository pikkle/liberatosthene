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

        /* parsing variable */
        le_size_t le_select = _LE_SIZE_NULL;

        /* check tree state */
        if ( le_tree->tr_size == 0 ) {

            /* assign offset */
            ( * le_offset ) = _LE_OFFS_NULL;

            /* send message */
            return( NULL );

        }

        /* case study */
        if ( le_time <= le_unit_get_time( le_tree->tr_unit ) ) {

            /* initialise index */
            le_dindex = -1;
            le_gindex = +0;

        } else {

            /* case study */
            if ( le_time >= le_unit_get_time( le_tree->tr_unit + le_tree->tr_size - 1 ) ) {

                /* initialise index */
                le_dindex = le_tree->tr_size - 1;
                le_gindex = le_tree->tr_size;

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

            } else {

                /* case study */
                if ( le_gindex >= le_tree->tr_size ) {

                    /* push index */
                    le_select = le_dindex --;

                } else {

                    /* case study */
                    if ( ( le_time - le_unit_get_time( le_tree->tr_unit + le_dindex ) ) < ( le_unit_get_time( le_tree->tr_unit + le_gindex ) - le_time ) ) {

                        /* push index */
                        le_select = le_dindex --;

                    } else {

                        /* push index */
                        le_select = le_gindex ++;

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

    le_unit_t * le_tree_get_reduct( le_tree_t const * const le_tree, le_address_t * const le_addr, le_size_t const le_atime, le_size_t * const le_offset ) {

        /* testing */
        return( NULL );

    }

/*
    source - mutator methods
 */

    le_unit_t * le_tree_set_push( le_tree_t * const le_tree, le_time_t const le_time, le_enum_t const le_mode ) {

        /* unit variable */
        le_unit_t le_unit = LE_UNIT_C;

        /* unit path variable */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

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

        /* compose unit path */
        sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P, le_tree->tr_root, le_reduce  );

        /* create unit */
        if ( le_get_status( le_unit = le_unit_create( le_path, le_mode, le_reduce, le_tree->tr_scfg ) ) != LE_ERROR_SUCCESS ) {

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
            le_offset = le_class_det_offset( le_offset, le_address_get_digit( le_addr, le_parse ), le_unit_get_stream( le_unit, le_parse ) );

        /* update index and condition */
        } while ( ( ( ++ le_parse ) < le_size ) && ( le_offset != _LE_OFFS_NULL ) );

        /* return offset */
        return( le_offset );

    }

