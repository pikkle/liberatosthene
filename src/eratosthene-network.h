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
    header - internal includes
 */

    # include "eratosthene.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor - external */
    # define LE_SOCKADDR_IN_C           { AF_INET, 0         , { INADDR_ANY }, { 0 } }
    # define LE_SOCKADDR_IN_C_PORT( p ) { AF_INET, htons( p ), { INADDR_ANY }, { 0 } }

    /* define pseudo-constructor */
    # define LE_NETWORK_C               { 0 }

    /* define network standard buffers */
    # define LE_NETWORK_SB_ADDR         ( _LE_USE_DEPTH + sizeof( le_size_t ) * 2 + sizeof( le_byte_t ) + sizeof( le_time_t ) * 2 )
    # define LE_NETWORK_SB_SYNC         ( _LE_USE_MTU )
    # define LE_NETWORK_SB_STRM         ( _LE_USE_MTU * 2 )

    /* define network modes */
    # define LE_NETWORK_MODE_NULL       ( 0x00 )
    # define LE_NETWORK_MODE_IMOD       ( 0x01 )
    # define LE_NETWORK_MODE_QMOD       ( 0x02 )
    # define LE_NETWORK_MODE_CMOD       ( 0x06 )
    # define LE_NETWORK_MODE_IATH       ( 0x81 )
    # define LE_NETWORK_MODE_QATH       ( 0x82 )
    # define LE_NETWORK_MODE_CATH       ( 0x86 )

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

