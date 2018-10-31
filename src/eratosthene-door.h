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
    # include "eratosthene-mclass.h"
    # include "eratosthene-pclass.h"
    # include "eratosthene-uv3.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_DOOR_C            { { 0 }, 0, 0, 0, 0, { NULL }, _LE_OFFS_NULL, { NULL }, _LE_OFFS_NULL, NULL, NULL, NULL, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
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

    /*! \struct le_door_struct
     *  \brief door structure
     *
     *  This structure holds the required information and data for accessing
     *  the storage structure of one time unit.
     *
     *  The two first fields store the path of the time unit and the length of
     *  the path itself.
     *
     *  The two next fields store a copy of the server space and time parameters
     *  in order for the structure to properly access the data.
     *
     *  The next field stores the time value of the time unit. The time unit
     *  is stored in its reduced form, that is the proper time devided by the
     *  value of the server time parameter.
     *
     *  The next two fields, an array and a size, are used to access mono-vertex
     *  related data in the storage structure. The array is used to store the
     *  stream descriptor opened toward the files of the storage structure. The
     *  amount of descriptor corresponds then to the amount of scale of the
     *  server, that is its space parameter value. The offset field coming after
     *  the array is used during data access to keep track of the offset, in a
     *  specific scale, of the data that have to be accessed.
     *
     *  The next three fields are similar than the previous one but for the case
     *  of polyvertex access. In addition to the stream descriptor stack and
     *  access offset, poly-vertex have to maintain an additional descriptor
     *  toward the file containing the poly-vertex definition.
     *
     *  The two last field are used to create and maintain linked list. The two
     *  pointer are used as \b le_door_t structure link toward the previous and
     *  next structure in the list, NULL being used as termination signal.
     *
     *  \var le_door_struct::dr_path
     *  Path of the time unit directory
     *  \var le_door_struct::dr_plen
     *  Length of the path of the time unit directory
     *  \var le_door_struct::dr_scfg
     *  Server spatial configuration parameter copy
     *  \var le_door_struct::dr_tcfg
     *  Server temporal configuration parameter copy
     *  \var le_door_struct::dr_time
     *  Time unit time value in reduced form
     *  \var le_door_struct::dr_macc
     *  Stream stack toward mono-vertex tree structure
     *  \var le_door_struct::dr_moff
     *  Mono-vertex offset tracker
     *  \var le_door_struct::dr_pacc
     *  Stream stack toward poly-vertex tree structure
     *  \var le_door_struct::dr_poff
     *  Poly-vertex offset tracker
     *  \var le_door_struct::dr_pdat
     *  Poly-vertex storage stream
     *  \var le_door_struct::dr_prev
     *  Linked list pointer (previous)
     *  \var le_door_struct::dr_next
     *  Linked list pointer (next)
     */

    typedef struct le_door_struct {

        le_char_t   dr_path[_LE_USE_PATH];
        le_size_t   dr_plen;

        le_size_t   dr_scfg;
        le_time_t   dr_tcfg;
        le_time_t   dr_time;

        le_file_t   dr_macc[_LE_USE_DEPTH];
        le_size_t   dr_moff;

        le_file_t   dr_pacc[_LE_USE_DEPTH];
        le_size_t   dr_poff;
        le_file_t   dr_pdat;

        le_void_t * dr_prev;
        le_void_t * dr_next;

    le_enum_t _status; } le_door_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates a door structure toward the time unit pointer by
     *  the provided time value.
     *
     *  The function starts by checking the state of the pointed directory
     *  according to the provided read/write mode. In case \b LE_DOOR_READ is
     *  provided as mode parameter and the corresponding storage directory is
     *  missing, the function throw and error.
     *
     *  In the case \b LE_DOOR_WRITE is provided as mode paramter and the
     *  corresponding storage directory is missing, the function creates it and
     *  initialise its content.
     *
     *  The function then creates the streams toward the poly-vertex storage
     *  file and toward the tree structure of the mono-vertex and poly-vertex
     *  data.
     *
     *  If any stream of the tree structures leads to an error during opening
     *  operation, the function fails and deletes itself, ensuring no stream
     *  descriptor remains open.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param le_root Server storage structure main path
     *  \param le_scfg Server spatial parameter
     *  \param le_tcfg Server temporal parameter
     *  \param le_time Temporal unit time
     *  \param le_mode Structure creation mode
     *
     *  \return Returns the created structure
     */

    le_door_t le_door_create( le_char_t const * const le_root, le_size_t const le_scfg, le_time_t const le_tcfg, le_time_t const le_time, le_enum_t const le_mode );

    /*! \brief constructor/destructor methods
     *
     *  This function is used to deletes the content of the provided door
     *  structure.
     *
     *  The function deletes all the opened stream descriptors and erase the
     *  structrue fields using default values.
     *
     *  \param le_door Door structure
     */

    le_void_t le_door_delete( le_door_t * const le_door );

    /*! \brief accessor methods
     *
     *  This function allows to obtain the time of the door structure in its
     *  reduce form, that is devided by the server temporal parameter.
     *
     *  \param le_door Door Structure
     *
     *  \return Return structure reduced time
     */

    le_time_t le_door_get_reduced( le_door_t const * const le_door );

    /*! \brief accessor methods
     *
     *  This function returns the previous door structure of the first provided
     *  structure or the next one of the second provided structure according to
     *  their proximity to the provided time value.
     *
     *  The function performs a temporal proximity checks between the two door
     *  structures candidates before to return the \b LE_DOOR_NEXT value or the
     *  \b LE_DOOR_PREV value according to the result.
     *
     *  The provided time value has to be given in its reduced form.
     *
     *  \param le_prev    Door structure
     *  \param le_next    Door structure
     *  \param le_reduced Time value, in reduced form
     *
     *  \return Returns LE_DOOR_PREV or LE_DOOR_NEXT according to the proximity
     *  to the time value
     */

    le_enum_t le_door_get_nearest( le_door_t const * const le_prev, le_door_t const * const le_next, le_time_t const le_reduced );

    /*! \brief accessor methods
     *
     *  This function returns a boolean value according to the state of the
     *  provided door structure tracking offset for mono-vertex.
     *
     *  If the door tracking offset points to a valid location, the function
     *  returns _LE_TRUE, _LE_FALSE otherwise.
     *
     *  \param le_door Door structure
     *
     *  \return Returns _LE_TRUE if the structure mono-vertex tracking offset
     *  points to a valid location, _LE_FALSE otherwise
     */

    le_enum_t le_door_get_mono( le_door_t const * const le_door );

    /*! \brief accessor methods
     *
     *  This function returns a boolean value according to the state of the
     *  poly-vertex trackin offset of the provided door structure.
     *
     *  If the door tracking offset points to a valid location, the function
     *  return _LE_TRUE, _LE_FALSE otherwise.
     *
     *  \param le_door Door structure
     *
     *  \return Returns _LE_TRUE if the structure poly-vertex tracking offset
     *  points to a valid location, _LE_FALSE otherwise
     */

    le_enum_t le_door_get_poly( le_door_t const * const le_door );

    /*! \brief accessor methods
     *
     *  This function returns a boolean value if the provided time is in the
     *  equivalence class of the provided door structure. The time value has to
     *  be in the server time frame (not in reduced form).
     *
     *  \param le_door Door structure
     *  \param le_time Time value
     *
     *  \return Returns _LE_TRUE if the provided time value belong to the
     *  structure temporal equivalence class.
     */

    le_enum_t le_door_get_equal( le_door_t const * const le_door, le_time_t const le_time );

    /*! \biref accessor methods
     *
     *  This function returns \b _LE_TRUE boolean value if the provided door
     *  structure is greater than the provided door candidate, \b _LE_FALSE
     *  otherwise. The comparison is made from the temporal point of view.
     *
     *  \param le_door      Door structure
     *  \param le_candidate Door structure
     *
     *  \return Returns _LE_TRUE if the candidate is anterior to the provided
     *  door structure from a temporal point of view
     */

    le_enum_t le_door_get_anterior( le_door_t const * const le_door, le_door_t const * const le_candidate );

    /*! \brief accessor methods
     *
     *  This function simply returns the pointer to the previous door structure
     *  of the provided one.
     *
     *  \param le_door Door structure
     *
     *  \return Returns the previous door structure in the linked list
     */

    le_door_t * le_door_get_prev( le_door_t const * const le_door );

    /*! \brief accessor methods
     *
     *  This function simply returns the pointer to the next door structure of
     *  the provided one.
     *
     *  \param le_door Door structure
     *
     *  \return Returns the next door structure in the linked list
     */

    le_door_t * le_door_get_next( le_door_t const * const le_door );

    /*! \brief accessor methods
     *
     *  This function checks if any chunks of dispatched data are present in the
     *  provided door storage structure for the specified type of data.
     *
     *  If 1 is provided as suffix value, the checks is performed on mono-vertex
     *  data. If 2 is provided, the check is made on the poly-vertex data.
     *
     *  In any case, if at least one chunk of data is detected for the specified
     *  type, the function returns \b _LE_TRUE, \b _LE_FALSE otherwise.
     *
     *  \param le_door   Door structure
     *  \param le_suffix Data type suffix
     *
     *  \return Returns _LE_TRUE if a data chunk is detected, _LE_FALSE
     *  otherwise
     */

    le_enum_t le_door_get_dispatch( le_door_t const * const le_door, le_size_t const le_suffix );

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

    le_enum_t le_door_io_mono_inject( le_door_t const * const le_door );

    /* *** */

    le_enum_t le_door_io_poly_inject( le_door_t const * const le_door );

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

