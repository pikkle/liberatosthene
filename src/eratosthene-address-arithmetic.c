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

    # include "eratosthene-address-arithmetic.h"

/*
    source - mutator methods
 */

    le_enum_t le_address_set_value( le_address_t * const le_address, le_size_t le_offset, le_size_t le_value ) {

        /* Offset base variables */
        le_size_t le_base = _LE_USE_BASE;

        /* Check consistency */
        if ( le_offset >= le_address->as_size ) {

            /* Send message */
            return( LE_ERROR_DEPTH );

        }

        /* Clear address digits */
        for ( le_size_t le_parse = le_offset + 1; le_parse < le_address->as_size; le_parse ++ ) le_address->as_addr[le_parse] = 0;

        /* Convert value into address index */
        while ( le_offset >= 0 ) {

            /* Compute offset base */
            if ( le_offset < LE_GEODESY_ASYP ) {

                /* Assign offset base */
                le_base = _LE_USE_BASE << 2;

            } else if ( le_offset < LE_GEODESY_ASYA ) {

                /* Assign offset base */
                le_base = _LE_USE_BASE << 1;

            } else {

                /* Assign offset base */
                le_base = _LE_USE_BASE;

            }

            /* Assign digit and update offset */
            le_address->as_addr[le_offset--] = le_value % le_base;

            /* Compute remaining value */
            le_value /= le_base;

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

