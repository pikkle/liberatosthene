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
    source - accessor methods
 */

    le_size_t le_address_get_dist( le_address_t const * const le_addr1, le_address_t const * const le_addr2 ) {

        /* Address size variables */
        le_size_t le_size = ( le_addr1->as_size < le_addr2->as_size ) ? le_addr1->as_size : le_addr2->as_size;

        /* Returned value variables */
        le_size_t le_return = le_size + 1;

        /* Computing address distance */
        for ( le_size_t le_parse = le_size; le_parse >= 0; le_parse -- ) {

            /* Check digits */
            if ( le_addr1->as_addr[le_parse] != le_addr2->as_addr[le_parse] ) le_return = le_parse;

        }

        /* Return computed distance */
        return( le_return );

    }

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
        while ( le_offset != _LE_SIZE_MAX ) {

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

    le_enum_t le_address_set_add( le_address_t * const le_addr, le_address_t const * const le_addr1, le_address_t const * const le_addr2 ) {

        /* Remainder variables */
        le_byte_t le_remain = 0;

        /* Offset base variables */
        le_size_t le_base = _LE_USE_BASE;

        /* Check consistency */
        if ( le_addr1->as_size != le_addr2->as_size ) {

            /* Send message */
            return( LE_ERROR_DEPTH );

        }

        /* Initialise result structure */
        * le_addr = * le_addr1;

        /* Address addition */
        for ( le_size_t le_parse = le_addr->as_size; le_parse != _LE_SIZE_MAX; le_parse -- ) {

            /* Add current digits */
            le_addr->as_addr[le_parse] += le_addr2->as_addr[le_parse] + le_remain;

            /* Compute remainder */
            le_remain = le_addr->as_addr[le_parse] / le_base;

            /* Compute address digit */
            le_addr->as_addr[le_parse] %= le_base;

        }

        /* Send message */
        return( LE_ERROR_SUCCESS );

    }

