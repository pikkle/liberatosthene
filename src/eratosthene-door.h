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

    /*! \brief accessor methods
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

    /*! \brief mutator methods
     *
     *  This function allows to update the storage structure state pointed by
     *  the provided door structure. If \b LE_DOOR_LOCK is provided, the door
     *  storage structure is locked, indicating other process that it can not
     *  be access for writting. If \b LE_DOOR_UNLOCK is provided, the function
     *  unlock the door storage structure.
     *
     *  \param le_door  Door structure
     *  \param le_state Lock state
     *
     *  \return Returns _LE_TRUE if the state was able to be updated, _LE_FALSE
     *  otherwise
     */

    le_enum_t le_door_set_state( le_door_t const * const le_door, le_enum_t const le_state );

    /*! \brief mutator methods
     *
     *  This function allows to insert the provided door strcuture between the
     *  two other provided structure in the linked list they define. The links
     *  of the provided structure are updated along with the links of the two
     *  other structures of the linked list.
     *
     *  \param le_door Door strcuture
     *  \param le_prev Previous door structure in the linked list
     *  \param le_next Next door structure in the linked list
     */

    le_void_t le_door_set_insert( le_door_t * const le_door, le_door_t * const le_prev, le_door_t * const le_next );

    /*! \brief i/o methods
     *
     *  This function is responsible of recieving the injected data from a
     *  connected clients.
     *
     *  This function performs two important writting operation on the provided
     *  door storage structure.
     *
     *  The first operation is to dispatch the incoming data into two groups of
     *  chunks that are mono-vertex and poly-vertex chunks. It continue the
     *  dispatch until the connected client stops sending data.
     *
     *  The second operation the function performs is the sorting of the chunks
     *  of each of the two groups, mono-vertex and poly-vertex. Each groups of
     *  data is dispatch in chunks of a small and controlled size. As one chunk
     *  reaches the limiting size, the function sorts its content before to
     *  write it in the provided door storage structure.
     *
     *  The sorting of the chunks is performed considering a merge-sort process
     *  with a comparison function based on the spatial index of the data
     *  positions coordinates.
     *
     *  At the end of the process performed by the function, the door storage
     *  strcuture ends with two groups of files containing each the dispatched
     *  an sorted data content for the mono-vertex and poly-vertex.
     *
     *  The input data are expected to be in uv3 format. The output data are
     *  kept in the same format.
     *
     *  Because the function writes on the door storage strcuture, the function
     *  assumes that the door has been locked.
     *
     *  \param le_door   Door structure
     *  \param le_array  Socket-array used for client data reading
     *  \param le_socket Socket of the connected client
     *
     *  \return Return LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_door_io_each_inject_dispatch( le_door_t const * const le_door, le_array_t * const le_array, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function is reponsible of the merging of the chunks created during
     *  the an injection process by the \b le_door_io_each_inject_dispatch()
     *  function.
     *
     *  The function reads the chunks of the specified group, using the suffix
     *  parameter, and merge them into a single chunks. The merging of the
     *  chunks can be seen as the last steps of a global merge-sort initiated
     *  by the dispatch function.
     *
     *  The sorting of the chunks into a single file is made considering a merge
     *  sort based on the spatial index of the element position containing in
     *  the processed chunks.
     *
     *  The input data are expected to be in uv3 format. The output data are
     *  kept in the same format.
     *
     *  Because the function writes on the door storage strcuture, the function
     *  assumes that the door has been locked.
     *
     *  \param le_door   Door structure
     *  \param le_suffix Data type suffix
     *
     *  \return Return LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_door_io_each_inject_merge( le_door_t const * const le_door, le_size_t const le_suffix );

    /*! \brief i/o methods
     *
     *  This function creates the storage tree structure using the sorted and
     *  merged files of the data injected by a client for mono-vertex data. This
     *  function is usually placed after the injection dispatch and merging.
     *
     *  The function read the mono-vertex sorted data and creates the tree
     *  structure using a transversal approach (in opposition to tree-driven
     *  writing process). This allows the function to writes tree structure in
     *  an efficient way.
     *
     *  For mono-vertex data, the only data needed to represent a point in the
     *  equivalence class strcuture is only a color that is packed within the
     *  tree along with the linking offsets.
     *
     *  The input data are expected to be in uv3 format.
     *
     *  Because the function writes on the door storage strcuture, the function
     *  assumes that the door has been locked.
     *
     *  \param le_door Door structure
     *
     *  \return Return LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_door_io_mono_inject( le_door_t const * const le_door );

    /*! \brief i/o methods
     *
     *  This function creates the storage tree structure using the sorted and
     *  merged files of the data injected by a client for poly-vertex data. This
     *  function is usually placed after the injection dispatch and merging.
     *
     *  The function reads the poly-vertex sorted data and creates the tree
     *  structure using a transversal approach (in opposition to tree-driven
     *  writing process). This allows the function to writes tree strcutures in
     *  an efficient way.
     *
     *  For poly-vertex, the definition of the graphical primitive are kept in
     *  the injection sorted and merged file. The tree structure holds link to
     *  this file along with the linking offset.
     *
     *  The input data are expected to be in the uv3 formats.
     *
     *  Because the function writes on the door storage strcuture, the function
     *  assumes that the door has been locked.
     *
     *  \param le_door Door structure
     *
     *  \return Return LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_door_io_poly_inject( le_door_t const * const le_door );

    /*! \brief i/o methods
     *
     *  This function searches the offset of the spatial equivalence class that
     *  is pointed by the index hold in the provided address structure for
     *  mono-vertex.
     *
     *  The function follows the tree structure links driven by the spatial
     *  index to determine if any data can be found in the pointed class. The
     *  function returns \b _LE_TRUE if the class is found, and \b _LE_FALSE
     *  in the other case, indicating that no data can be found further away.
     *
     *  This function is used before to perform any query over the data hold in
     *  the mono-vertex tree structure. In the case the class is found, it
     *  updates the traking offset of the provided door strcuture for the
     *  subsequent gathering processes.
     *
     *  \param le_door Door structure
     *  \param le_addr Address structure
     *
     *  \return Returns _LE_TRUE if the class pointed by the address exists,
     *  _LE_FALSE otherwise.
     */

    le_enum_t le_door_io_mono_detect( le_door_t * const le_door, le_address_t const * const le_addr );

    /*! \brief i/o methods
     *
     *  This function searches the offset of the spatial equivalence class that
     *  is pointed by the index hold in the provided address structure for
     *  poly-vertex.
     *
     *  The function follows the tree structure links driven by the spatial
     *  index to determine if any data can be found in the pointed class. The
     *  function returns \b _LE_TRUE if the class is found, and \b _LE_FALSE
     *  in the other case, indicating that no data can be found further away.
     *
     *  This function is used before to perform any query over the data hold in
     *  the poly-vertex tree structure. In the case the class is found, it
     *  updates the traking offset of the provided door strcuture for the
     *  subsequent gathering processes.
     *
     *  \param le_door Door structure
     *  \param le_addr Address structure
     *
     *  \return Returns _LE_TRUE if the class pointed by the address exists,
     *  _LE_FALSE otherwise.
     */

    le_enum_t le_door_io_poly_detect( le_door_t * const le_door, le_address_t const * const le_addr );

    /*! \brief i/o methods
     *
     *  This function performs the data gathering on mono-vertex tree structure
     *  for data queries. The gathered data are packed in the provided array
     *  structure following the uv3 format.
     *
     *  This function assumes that the class on which the gathering processs
     *  exists and is pointed by the door structure mono-vertex tracking
     *  offset. Starting with this offset, the function enumerates all existing
     *  daughter cells to gather the data in the scale pointed by the span
     *  value. The provided address structure is used as the enumeration
     *  backbone through its digit array.
     *
     *  The enuemration is peformed in a recursive way by enumerating all link
     *  of all subsequent classes and following their own link. The process is
     *  made until the targeted scale is reached.
     *
     *  For mono-vertex, the position of the data element is deduced from the
     *  backbone address structure. The color of the elements is directly
     *  extracted from the tree structure.
     *
     *  If the provided array is not empty, its content is kept and the gathered
     *  data are packed after the existing ones.
     *
     *  \param le_door  Door structure
     *  \param le_addr  Address structure
     *  \param le_parse Current enumerated scale
     *  \param le_span  Target scale
     *  \param le_array Array structure
     */

    le_void_t le_door_io_mono_gather( le_door_t * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /*! \brief i/o methods
     *
     *  This function performs the data gathering on poly-vertex tree structure
     *  for data queries. The gathered data are packed in the provided array
     *  structure following the uv3 format.
     *
     *  This function assumes that the class on which the gathering processs
     *  exists and is pointed by the door structure poly-vertex tracking
     *  offset. Starting with this offset, the function enumerates all existing
     *  daughter cells to gather the data in the scale pointed by the span
     *  value. The provided address structure is used as the enumeration
     *  backbone through its digit array.
     *
     *  The enuemration is peformed in a recursive way by enumerating all link
     *  of all subsequent classes and following their own link. The process is
     *  made until the targeted scale is reached.
     *
     *  For poly-vertex, the tree structure contains links to the elements
     *  stored in the injection sorted and merged file. This file is then read,
     *  following the found links, to extract the elementd definition to pack
     *  them in the provided array structure.
     *
     *  If the provided array is not empty, its content is kept and the gathered
     *  data are packed after the existing ones.
     *
     *  \param le_door  Door structure
     *  \param le_addr  Address structure
     *  \param le_parse Current enumerated scale
     *  \param le_span  Target scale
     *  \param le_array Array structure
     */

    le_void_t le_door_io_poly_gather( le_door_t * const le_door, le_address_t * const le_addr, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /*! \brief i/o methods
     *
     *  This function implements a parallel version of the process implemented
     *  in the \b le_door_io_mono_gather() function.
     *
     *  The process is identical but performed on the tree structure in the
     *  same time. The goal of the implementation is to allows, for mono-vertex
     *  data, to compute convolutional models in the fly.
     *
     *  The currently implemented convolution are the logical OR, AND and XOR.
     *  As the parallel gethering goes on, the function compare of one, two or
     *  none of the data are missing on one of the two tree structure. This is
     *  on this basis that the function applies the logical convolutions.
     *
     *  If the provided array is not empty, its content is kept and the gathered
     *  data are packed after the existing ones.
     *
     *  \param le_pdoor  Door structure - primary
     *  \param le_sdoor  Door structure - secondary
     *  \param le_addr   Address structure
     *  \param le_mode   Convolution mode
     *  \param le_parse  Current enumerated scale
     *  \param le_span   Target scale
     *  \param le_array  Array structure
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

