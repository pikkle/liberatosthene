/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2019 DHLAB, EPFL
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

    # include "eratosthene.h"

/*
    source - accessor methods
 */

    le_enum_t le_get_exist( le_char_t const * const le_path ) {

        /* statistic structure variable */
        struct stat le_stat = LE_STAT_C;

        /* retrieve element statistics */
        if ( stat( ( char * ) le_path, & le_stat ) == 0 ) {

            /* send message */
            return( _LE_TRUE );

        /* send message */
        } else { return( _LE_FALSE ); }

    }
