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

    # include "eratosthene-pool.h"

/*
    source - accessor methods
 */

    le_byte_t le_pool_get( le_pool_t const * const le_pool, le_enum_t const le_tid, le_byte_t const le_flag ) {

        /* return flag bit */
        return( ( le_pool->pl_flag[le_tid] ) & le_flag );

    }

/*
    source - mutator methods
 */

    le_void_t le_pool_set( le_pool_t * const le_pool, le_enum_t le_tid, le_byte_t const le_flag ) {

        /* set flag bit */
        le_pool->pl_flag[le_tid] |= le_flag;

    }

    le_void_t le_pool_set_clear( le_pool_t * const le_pool, le_enum_t const le_tid, le_byte_t const le_flag ) {

        /* reset flag bit */
        le_pool->pl_flag[le_tid] &= le_flag;

    }

    le_void_t le_pool_set_broadcast( le_pool_t * const le_pool, le_enum_t const le_tid, le_byte_t const le_flag ) {

        /* parsing pool */
        for ( le_size_t le_parse = 0; le_parse < _LE_USE_PENDING; le_parse ++ ) {

            /* avoid self-messaging */
            if ( le_parse != le_tid ) {

                /* broadcast message */
                le_pool->pl_flag[le_parse] |= le_flag;

            }

        }

    }

