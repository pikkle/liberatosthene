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

    /*! \file   eratosthene-door.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - door
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_DOOR__
    # define __LE_DOOR__

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
    # include "eratosthene-address.h"
    # include "eratosthene-array.h"
    # include "eratosthene-class.h"
    # include "eratosthene-pclass.h"
    # include "eratosthene-uv3.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    //# define LE_DOOR_C            { { 0 }, 0, 0, 0, 0, { NULL }, _LE_OFFS_NULL, { NULL }, _LE_OFFS_NULL, NULL, NULL, LE_ERROR_SUCCESS }
    # define LE_DOOR_C            { { 0 }, 0, 0, 0, 0, { NULL }, _LE_OFFS_NULL, { NULL }, _LE_OFFS_NULL, NULL, NULL, NULL, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    //# define LE_DOOR_C_SCT(s,c,t) { { 0 }, 0, s, c, t, { NULL }, _LE_OFFS_NULL, { NULL }, _LE_OFFS_NULL, NULL, NULL, LE_ERROR_SUCCESS }
    # define LE_DOOR_C_SCT(s,c,t) { { 0 }, 0, s, c, t, { NULL }, _LE_OFFS_NULL, { NULL }, _LE_OFFS_NULL, NULL, NULL, NULL, LE_ERROR_SUCCESS }

    /* define door link */
    # define LE_DOOR_PREV         ( 0 )
    # define LE_DOOR_NEXT         ( 1 )

    /* define door mode */
    # define LE_DOOR_READ         ( 0 )
    # define LE_DOOR_WRITE        ( 1 )

    /* define door state */
    # define LE_DOOR_LOCK         ( 0 )
    # define LE_DOOR_UNLOCK       ( 1 )

/*
    header - preprocessor macros
 */

    /* define stream mode */
    # define le_door_mode(m)      ( ( m ) == LE_DOOR_READ ? "r+" : "w+" )

/*
    header - type definition
 */

/*
    header - structures
 */


    /*! \struct le_unit_struct
     *  \brief unit structure ( revoked )
     *
     *  This structure holds the required information to manage and access a
     *  specific storage structure of a temporal unit.
     *
     *  The two first fields are used to store a copy of the server storage
     *  structure root path and the spatial configuration value that are used
     *  for unit manipulation.
     *
     *  The next field is used to store the time value associated to the unit.
     *  The value stored in this field is according to the equivalence classes
     *  point of view :
     *
     *      time_unit = time_utc / server_temporal_parameter
     *
     *  The unit time is then called "reduced".
     *
     *  The next field of the structure holds the stack of stream opened
     *  toward the file of the unit storage structure. Each scale file is then
     *  described by its corresponding stream in the pile.
     *
     *  A last field is used by the structure creation methods to specified the
     *  creation status of the structure. This allows to creation methods to
     *  return the structure instead of an error code.
     *
     *  \var le_unit_struct::un_root
     *  Server storage structure path
     *  \var le_unit_struct::un_scfg
     *  Server spatial configuration value : number of scale
     *  \var le_unit_struct::un_time
     *  Unit time in reduced form
     *  \var le_unit_struct::un_pile
     *  Unit stream pile
     *  \var le_unit_struct::_status
     *  Standard status field
     */

    typedef struct le_door_struct {

        le_char_t   dr_path[_LE_USE_PATH];
        le_size_t   dr_plen;

        le_size_t   dr_scfg;
        le_time_t   dr_tcfg;
        le_time_t   dr_time;

        le_file_t   dr_sacc[_LE_USE_DEPTH];
        le_size_t   dr_soff;

        le_file_t   dr_pacc[_LE_USE_DEPTH];
        le_size_t   dr_poff;
        le_file_t   dr_pdat;

        le_void_t * dr_prev;
        le_void_t * dr_next;

    le_enum_t _status; } le_door_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods ( revoked )
     *
     *  This function creates a unit structure according to the provided path
     *  and to the provided mode.
     *
     *  Two modes are available : \b LE_UNIT_READ and \b LE_UNIT_WRITE. The
     *  first mode triggers the creation of the unit structure according to an
     *  existing storage structure. It assign the provided time value, in its
     *  reduced form, before to create the stream descriptor to the scale files.
     *
     *  The second mode creates the storage structure directory if necessary and
     *  creates the scale files while creating the streams to them.
     *
     *  If any stream is not accessible or if the directory creation fails, the
     *  function returns the structure as it is.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param le_root   Server storage structure path
     *  \param le_scfg   Server spatial configuration value
     *  \param le_time   Unit time value, in reduced form
     *  \param le_mode   Access mode value
     *
     *  \return Returns the created unit
     */

    le_door_t le_door_create( le_char_t const * const le_root, le_size_t const le_scfg, le_time_t const le_tcfg, le_time_t const le_time, le_enum_t const le_mode );

    /*! \brief constructor/destructor methods ( revoked )
     *
     *  This function deletes the provided unit structure. It parses the scales
     *  of the unit and deletes each scale opened stream. It finally resets the
     *  unit structure fields.
     *
     *  \param le_unit Unit structure
     */

    le_void_t le_door_delete( le_door_t * const le_door );

    /* *** */

    le_time_t le_door_get_reduced( le_door_t const * const le_door );

    /* *** */

    le_enum_t le_door_get_nearest( le_door_t const * const le_prev, le_door_t const * const le_next, le_time_t const le_reduced );

    /* *** */

    le_enum_t le_door_get_mono( le_door_t const * const le_door );

    /* *** */

    le_enum_t le_door_get_poly( le_door_t const * const le_door );

    /* *** */

    le_enum_t le_door_get_equal( le_door_t const * const le_door, le_time_t const le_time );

    /* *** */

    le_enum_t le_door_get_anterior( le_door_t const * const le_door, le_door_t const * const le_candidate );

    /* *** */

    le_door_t * le_door_get_prev( le_door_t const * const le_door );

    /* *** */

    le_door_t * le_door_get_next( le_door_t const * const le_door );

    /* *** */

    le_enum_t le_door_set_state( le_door_t const * const le_door, le_enum_t const le_state );

    /* *** */

    le_void_t le_door_set_insert( le_door_t * const le_door, le_door_t * const le_prev, le_door_t * const le_next );

    /* *** */

    le_enum_t le_door_io_each_inject_dispatch( le_door_t const * const le_door, le_array_t * const le_array, le_sock_t const le_socket );

    /* *** */

    le_enum_t le_door_io_each_inject_merge( le_door_t const * const le_door, le_size_t const le_suffix );

    /*! \brief i/o methods ( revoked )
     *
     *  This function writes the content of the incoming array in the server
     *  storage structure.
     *
     *  The provided array is expect to have the UF3 format, that is a series
     *  of records composed of a coordinates 3-vector and a colour 3-vector.
     *
     *  For each array record, the function initiates an address structure using
     *  the record coordinates to compute the spatial index. The index is then
     *  used to drive the progression of the record injection through the scale
     *  files.
     *
     *  For a given scale, the class the record belong to is searched. If the
     *  class is found, the element is added to the class. Otherwise, a new
     *  class is created using the record to initialise it. The parent class
     *  offsets array is updated to take into account the new created class.
     *
     *  \param le_unit  Unit structure
     *  \param le_array Array structure
     */

    le_enum_t le_door_io_mono_inject_beta( le_door_t const * const le_door );

    /* *** */

    le_enum_t le_door_io_poly_inject_beta( le_door_t const * const le_door );

    /*! \brief i/o methods ( revoked )
     *
     *  This function searches the offset of the spatial class pointed by the
     *  spatial index provided through the address structure in the provided
     *  unit storage structure.
     *
     *  The spatial index digits are used to drive the progression through
     *  the structure scales. This function is mainly used to detect if the
     *  class pointed by the address exists or not.
     *
     *  \param le_unit Unit structure
     *  \param le_addr Address structure
     *
     *  \return Returns the class offset on success, _LE_OFFS_NULL otherwise
     */

    le_enum_t le_door_io_mono_detect( le_door_t * const le_door, le_address_t const * const le_addr );

    /* *** */

    le_enum_t le_door_io_poly_detect( le_door_t * const le_door, le_address_t const * const le_addr );

    /*! \brief i/o methods ( revoked )
     *
     *  This function is used to gather spatial classes positions and colours
     *  information to build a array using the UF3 format.
     *
     *  This function assume that the offset of the spatial class pointed by the
     *  address index is known (\b le_tree_io_offset()). Starting from the
     *  offsets array of this class, it starts to gather the position and
     *  colours by enumerating the class daughters and sub-daughters through a
     *  recursive process. It detect the gathering scale using the main class
     *  scale and the address additional depth (span).
     *
     *  If the provided array is not passed empty, its previous content is left
     *  unchanged, the function pushing the elements at the end of it.
     *
     *  \param le_unit   Unit structure
     *  \param le_addr   Address structure
     *  \param le_offset Class storage offset
     *  \param le_parse  Class storage scale
     *  \param le_span   Query additional depth
     *  \param le_array  Data array filled by the function
     */

    le_void_t le_door_io_mono_gather( le_door_t * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /* *** */

    le_void_t le_door_io_poly_gather( le_door_t * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /*! \brief i/o methods ( revoked )
     *
     *  This function implements a parallel version of \b le_tree_io_gather()
     *  function. It performs the same operations simply considering two times
     *  values.
     *
     *  As two parallel gathering processes take place in this function, it
     *  allows to consider the times comparison mode provided by the address
     *  structure. As elements for the two times are gathered at the same time,
     *  the function is able to easily implements the logical operators. The
     *  array is then filled with the results of the application of the logical
     *  operators.
     *
     *  If the provided array is not passed empty, its previous content is left
     *  unchanged, the function pushing the elements at the end of it.
     *  of it.
     *
     *  \param le_unia    Unit structure - ass. to time 1
     *  \param le_unib    Unit structure - ass. to time 2
     *  \param le_addr    Address structure
     *  \param le_mode    Address mode
     *  \param le_offseta Class storage offset - ass. to time 1
     *  \param le_offsetb Class storage offset - ass. to time 2
     *  \param le_parse   Class storage scale
     *  \param le_span    Query additional depth
     *  \param le_array   Data array filled by the function
     */

    le_void_t le_door_io_mono_parallel( le_door_t * const le_pdoor, le_door_t * const le_sdoor, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

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

