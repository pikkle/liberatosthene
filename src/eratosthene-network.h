/*
 *  liberatosthene - geodetic system
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
 *
 */

    /*! \file   eratosthene-network.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  Network module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_NETWORK__
    # define __LE_NETWORK__

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    extern "C" {
    # endif

/*
    header - includes
 */

    # include "eratosthene.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor - external */
    # define LE_SOCKADDR_IN_C           { 0 }
    # define LE_SOCKADDR_IN_C_PORT( p ) { AF_INET, htons( p ), { INADDR_ANY }, { 0 } }

    /* Define pseudo-constructor */
    # define LE_NETWORK_BUFFER_C        { 0 }

    /* Define state flags */
    # define LE_NETWORK_NULL            ( -1 )

    /* Define server configuration */
    # define LE_NETWORK_PORT            ( 555 )
    # define LE_NETWORK_PEND            ( 5 )

    /* Define network modes */
    # define LE_NETWORK_MODE_NULL       ( 0x00 )
    # define LE_NETWORK_MODE_IMOD       ( 0x01 )
    # define LE_NETWORK_MODE_QMOD       ( 0x02 )
    # define LE_NETWORK_MODE_IATH       ( 0x81 )
    # define LE_NETWORK_MODE_QATH       ( 0x82 )

    /* Define network buffer size */
    # define LE_NETWORK_BUFFER_HAND     ( 1 )
    # define LE_NETWORK_BUFFER_ADDR     ( 256 )
    # define LE_NETWORK_BUFFER_SYNC     ( ( _LE_USE_MTU / LE_ARRAY_ELEM_LEN ) * LE_ARRAY_ELEM_LEN )

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

/*
    header - function prototypes
 */

/*
    header - C/C++ compatibility
 */

    # ifdef __cplusplus
    }
    # endif

/*
    header - inclusion guard
 */

    # endif

