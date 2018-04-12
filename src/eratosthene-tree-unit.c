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

    le_unit_t le_unit_create( le_char_t const * const le_path, le_time_t const le_time, le_enum_t const le_mode, le_size_t const le_scfg ) {

        /* created structure variable */
        le_unit_t le_unit = LE_UNIT_C_TIME( le_time );

        /* stream path variable */
        le_char_t le_stream[_LE_USE_STRING] = { 0 };

        /* check unit mode */
        if ( le_mode == LE_UNIT_WRITE ) {

            /* create unit directory */
            if ( mkdir( ( char * ) le_path, 0755 ) != 0 ) {

                /* send message */
                return( le_set_status( le_unit, LE_ERROR_IO_WRITE ) );

            }

        }

        /* parsing scale stream */
        for ( le_size_t le_parse = 0; le_parse < le_scfg; le_parse ++ ) {

            /* compose stream path */
            sprintf( ( char * ) le_stream, "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin", le_path, le_time, le_parse );

            /* create stream */
            if ( ( le_unit.un_pile[le_parse] = fopen( ( char * ) le_stream, le_unit_mode( le_mode ) ) ) == NULL ) {

                /* send message */
                return( le_set_status( le_unit, LE_ERROR_IO_ACCESS ) );

            }

            /* disable stream buffer */
            setvbuf( le_unit.un_pile[le_parse], NULL, _IONBF, 0);

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

