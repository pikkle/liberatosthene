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

    /*! \file   eratosthene-system.h
     *  \author Nils Hamel <n.hamel@bluewin.ch>
     *
     *  System module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SYSTEM__
    # define __LE_SYSTEM__

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
    # include "eratosthene-address.h"
    # include "eratosthene-array.h"
    # include "eratosthene-class.h"

/*
    header - preprocessor definitions
 */

    /* Define pseudo-constructor */
    # define LE_SYSTEM_C { { 0 }, 0, 0, 0, 0, 0, NULL, LE_ERROR_SUCCESS }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_system_struct
     *  \brief system structure
     *
     *  This structure holds the server configuration data. The main parameters
     *  are the root path of the equivalence classes storage structure 
     *  \b sm_root and the space-time indexation parameters.
     *
     *  The space indexation is fixed through the number of scale \b sm_sdisc
     *  along which class are defined and stored. The \b sm_tdisc gives, in
     *  seconds, the linear and contigous size of the equivalence class in the
     *  time dimension.
     *
     *  An additional field is used to store the descriptors of the stream
     *  opened on classes storage file to reduce i/o operations.
     *
     *  \var le_system_struct::sm_root
     *  Storage structure path
     *  \var le_system_struct::sm_sdisc
     *  System number of scales
     *  \var le_system_struct::sm_tdisc
     *  System time discretisation
     *  \var le_system_struct::sm_scale
     *  Storage structure stream descriptors
     */

    typedef struct le_system_struct {

        le_char_t sm_root[256];
        le_size_t sm_sdisc;
        le_time_t sm_tdisc;
        le_size_t sm_smode;
        le_enum_t sm_imode;
        le_enum_t sm_qmode;
        FILE **   sm_scale;

    le_enum_t _status; } le_system_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates and returns a server system descriptor. It starts
     *  by reading server storage implementation configuration and checks its
     *  consistency.
     *
     *  \param le_system Server system descriptor
     *  \param le_root   Server storage implementation path
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_system_t le_system_create( le_char_t const * const le_root );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes a server system descriptor created by the function
     *  \b le_system_create.
     *
     *  \param le_system Server system descriptor
     */

    le_system_t le_system_delete( le_system_t * const le_system );

    /*! \brief accessor methods
     *
     *  This function returns the server system spatial indexation parameter.
     *
     *  \param le_system Server system descriptor
     *
     *  \return Server system spatial indexation parameter
     */

    le_size_t le_system_get_sdisc( le_system_t const * const le_system );

    /*! \brief accessor methods
     *
     *  This function returns the server system time indexation parameter.
     *
     *  \param le_system Server system descriptor
     *
     *  \return Server system time indexation parameter
     */

    le_time_t le_system_get_tdisc( le_system_t const * const le_system );

    le_hand_t le_system_set_format( le_system_t * const le_system, le_hand_t const le_hand );

    /*! \brief injection methods
     *
     *  This function injects the element provided as parameter in the server
     *  system storage structure. It waits the element position, time and color
     *  information to inject it in the structure.
     *
     *  See \b le_address_struct documentation for more information on frames.
     *
     *  \param le_system Server system descriptor
     *  \param le_pose   Element position array
     *  \param le_time   Element time
     *  \param le_data   Element color array
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_system_inject( le_system_t * const le_system, le_real_t * const le_pose, le_time_t const le_time, le_data_t const * const le_data );

    /*! \brief query methods
     *
     *  This function query the class structure storage representation reading
     *  a class address structure. It searches in the server system storage
     *  structure the class and ask the gathering function \b le_system_gather
     *  to fill the returned array with queried class representatives.
     *
     *  \param le_system Server system descriptor
     *  \param le_addr   Query address structure
     *
     *  \return Class representatives array
     */

    le_array_t le_system_query( le_system_t * const le_system, le_address_t * const le_addr );

    /*! \brief query methods
     *
     *  This function gather the daughter class representatives of a provided
     *  parent class based on the query depth and server system storage
     *  structure. It appends the found class representatives in the array
     *  provided as parameter.
     *
     *  \param le_system Server system descriptor
     *  \param le_array  Array filled with class representatives
     *  \param le_addr   Address structure of the parent class
     *  \param le_class  Class structure of the parent class
     *  \param le_head   Scale index of the parent class
     *  \param le_target Scale index of the target daughter classes
     */

    le_void_t le_system_gather( le_system_t * const le_system, le_array_t * const le_array, le_address_t * const le_addr, le_class_t * const le_class, le_size_t const le_head, le_size_t const le_target );

    /*! \brief i/o methods
     *
     *  This function creates the file streams to the server system storage
     *  structure for a provided time index.
     *
     *  \param le_system Server system descriptor
     *  \param le_time   Time equivalence class index
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_system_open( le_system_t * const le_system, le_time_t const le_time );

    /*! \brief i/o methods
     *
     *  This function forces to empty caches of file streams opened by the
     *  \b le_system_open function.
     *
     *  \param le_system Server system descriptor
     */

    le_void_t le_system_flush( le_system_t * const le_system );

    /*! \brief i/o methods
     *
     *  This function closes the file streams opened by the \b le_system_open
     *  function.
     *
     *  \param le_system Server system descriptor
     */

    le_void_t le_system_close( le_system_t * const le_system );

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

