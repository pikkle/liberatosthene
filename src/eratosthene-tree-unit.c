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

    # include "eratosthene-tree-unit.h"

/*
    source - constructor/destructor methods
 */

    le_unit_t le_unit_create( le_char_t const * const le_root, le_time_t const le_time, le_enum_t const le_mode, le_size_t const le_scfg, le_enum_t const le_buffer ) {

        /* created structure variable */
        le_unit_t le_unit = LE_UNIT_C_TIME( le_time );

        /* stream path variable */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

        /* check unit mode */
        if ( le_mode == LE_UNIT_WRITE ) {

            /* compose directory path */
            sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P, le_root, le_time );

            /* create unit directory */
            if ( mkdir( ( char * ) le_path, 0755 ) != 0 ) {

                /* send message */
                return( le_set_status( le_unit, LE_ERROR_IO_WRITE ) );

            }

        }

        /* parsing scale stream */
        for ( le_size_t le_parse = 0; le_parse < le_scfg; le_parse ++ ) {

            /* compose stream path */
            sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_root, le_time, le_parse );

            /* create stream */
            if ( ( le_unit.un_pile[le_parse] = fopen( ( char * ) le_path, le_unit_mode( le_mode ) ) ) == NULL ) {

                /* send message */
                return( le_set_status( le_unit, LE_ERROR_IO_ACCESS ) );

            }

            /* check buffer mode */
            if ( le_buffer == LE_UNIT_BIGN ) {

                /* disable stream buffer */
                setvbuf( le_unit.un_pile[le_parse], NULL, _IONBF, 0 );

            }

        }

        /* return create structure */
        return( le_unit );

    }

    le_void_t le_unit_delete( le_unit_t * const le_unit ) {

        /* deleted structure variable */
        le_unit_t le_delete = LE_UNIT_C;

        /* parsing scale stream */
        for ( le_size_t le_parse = 0; le_parse < _LE_USE_DEPTH; le_parse ++ ) {

            /* check stream state */
            if ( le_unit->un_pile[le_parse] != NULL ) {

                /* delete stream */
                fclose( le_unit->un_pile[le_parse] );

            }

        }

        /* delete structure */
        ( * le_unit ) = le_delete;

    }

/*
    source - accessor methods
 */

    le_file_t le_unit_get_stream( le_unit_t const * const le_unit, le_size_t const le_scale ) {

        /* return unit stream */
        return( le_unit->un_pile[le_scale] );

    }

    le_time_t le_unit_get_time( le_unit_t const * const le_unit ) {

        /* return unit time */
        return( le_unit->un_time );

    }

    le_enum_t le_unit_get_prior( le_unit_t const * const le_unit, le_unit_t const * const le_candidate ) {

        /* compare unit time */
        if ( le_candidate->un_time < le_unit->un_time ) {

            /* send result */
            return( _LE_TRUE );

        } else {

            /* send result */
            return( _LE_FALSE );

        }

    }

/*
    source - mutator methods
 */

    le_void_t le_unit_set_optimise( le_unit_t * const le_unit, le_char_t const * const le_root ) {

        /* string variable */
        le_char_t le_path[_LE_USE_STRING] = { 0 };

        /* string variable */
        le_char_t le_over[_LE_USE_STRING] = { 0 };

        /* dual stream variable */
        le_file_t le_dual[_LE_USE_DEPTH] = { NULL };

        /* offset array variable */
        le_size_t le_head[_LE_USE_DEPTH] = { 0 };

        /* parsing variable */
        le_size_t le_parse = 0;

        /* parsing scales */
        while ( le_parse < _LE_USE_DEPTH ) {

            /* check stream state */
            if ( le_unit->un_pile[le_parse] != NULL ) {

                /* compose dual stream path */
                sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".dual", le_root, le_unit->un_time, le_parse );

                /* create dual stream */
                le_dual[le_parse] = fopen( ( char * ) le_path, le_unit_mode( LE_UNIT_WRITE ) );

                /* update index */
                le_parse ++;

            /* abort duplication */
            } else { le_parse = _LE_USE_DEPTH; }

        }

        /* optimise unit storage */
        le_unit_set_arrange( le_unit, le_dual, 0, le_head, 0 );

        /* parsing scale */
        while ( ( -- le_parse ) >= 0 ) {

            /* check stream state */
            if ( le_unit->un_pile[le_parse] != NULL ) {

                /* close stream */
                fclose( le_unit->un_pile[le_parse] );

                /* replace stream */
                le_unit->un_pile[le_parse] = le_dual[le_parse];

                /* compose dual stream path */
                sprintf( ( char * ) le_path, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".dual", le_root, le_unit->un_time, le_parse );

                /* compose stream path */
                sprintf( ( char * ) le_over, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_root, le_unit->un_time, le_parse );

                /* overwrite stream with dual stream */
                rename( ( char * ) le_path, ( char * ) le_over );

            }

        }

    }

    le_void_t le_unit_set_arrange( le_unit_t * const le_unit, le_file_t const * const le_dual, le_size_t const le_offset, le_size_t * const le_head, le_size_t const le_scale ) {

        /* offset variable */
        le_size_t le_enum = _LE_OFFS_NULL;

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* read class - unit */
        le_class_io_read( & le_class, le_offset, le_unit->un_pile[le_scale] );

        /* parsing class offsets */
        for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

            /* retrieve offset */
            if ( ( le_enum = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                /* update class offset */
                le_class_set_offset( & le_class, le_digit, le_head[le_scale + 1] );

                /* recursive optimisation process */
                le_unit_set_arrange( le_unit, le_dual, le_enum, le_head, le_scale + 1 );

            }

        }

        /* write class - dual */
        le_class_io_write( & le_class, le_head[le_scale], le_dual[le_scale] );

        /* update head array */
        le_head[le_scale] += LE_CLASS_ARRAY;

    }

