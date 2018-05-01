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

    /*! \file   eratosthene-unit.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - unit module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_UNIT__
    # define __LE_UNIT__

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

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_UNIT_C          { NULL, 0, 0, { NULL }, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    # define LE_UNIT_C_TIME(t)  { NULL, 0, t, { NULL }, LE_ERROR_SUCCESS }

    /* define stream mode */
    # define LE_UNIT_READ       ( 0 )
    # define LE_UNIT_WRITE      ( 1 )

    /* define unit state */
    # define LE_UNIT_OPEN       ( 0 )
    # define LE_UNIT_LOCK       ( 1 )

/*
    header - preprocessor macros
 */

    /* define stream mode */
    # define le_unit_mode(m)    ( ( m ) == LE_UNIT_READ ? "r+" : "w+" )

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_unit_struct
     *  \brief unit structure
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

    typedef struct le_unit_struct {

        le_char_t * un_root;
        le_size_t   un_scfg;

        le_time_t   un_time;
        le_file_t   un_pile[_LE_USE_DEPTH];

    le_enum_t _status; } le_unit_t;

/*
    header - function prototypes
 */

    /* *** */

    /*! \brief constructor/destructor methods
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

    le_unit_t le_unit_create( le_char_t * const le_root, le_size_t const le_scfg, le_time_t const le_time, le_enum_t const le_mode );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided unit structure. It parses the scales
     *  of the unit and deletes each scale opened stream. It finally resets the
     *  unit structure fields.
     *
     *  \param le_unit Unit structure
     */

    le_void_t le_unit_delete( le_unit_t * const le_unit );

    /*! \brief accessor methods
     *
     *  This function returns the unit stream that corresponds to the provided
     *  scale value.
     *
     *  \param le_unit  Unit structure
     *  \param le_scale Scale offset
     *
     *  \return Returns unit scale stream file descriptor
     */

    le_file_t le_unit_get_stream( le_unit_t const * const le_unit, le_size_t const le_scale );

    /*! \brief accessor methods
     *
     *  This function simply returns the time of the provided unit. The returned
     *  time is in reduced form.
     *
     *  \param le_unit Unit structure
     *
     *  \return Returns the unit time, in reduced form
     */

    le_time_t le_unit_get_time( le_unit_t const * const le_unit );

    /*! \brief accessor methods
     *
     *  This function compares the provided unit and the provided candidate unit
     *  and returns _LE_TRUE if the candidate is prior to the actual unit in
     *  terms of time. It returns _LE_FASLE otherwise.
     *
     *  \param le_unit      Unit structure
     *  \param le_candidate Unit structure
     *
     *  \return Returns _LE_TRUE on unit anteriority, _LE_FALSE otherwise
     */

    le_enum_t le_unit_get_prior( le_unit_t const * const le_unit, le_unit_t const * const le_candidate );

    /*! \brief mutator methods
     *
     *  This function is used to lock the storage structure of a temporal unit.
     *  The function creates a special files, called a locker, that indicates
     *  other processes that the unit is locked for both reading and writing.
     *
     *  The provided time indicates which unit has to be locked or unlocked
     *  according to the state parameter. If \b LE_UNIT_LOCK is provided as
     *  state parameter, the function creates the locker file. If the unit is
     *  already locked, the function does nothing.
     *
     *  If the \b LE_UNIT_OPEN value is provided as state variable, the function
     *  deletes the unit locker. If the unit is already unlocked, the function
     *  does nothing.
     *
     *  Note that this function does not check, for efficiency purpose, if the
     *  provided time value corresponds or not to a valid unit storage.
     *
     *  \param le_unit  Unit structure
     *  \param le_state Unit lock state
     */ 

    le_void_t le_unit_set_lock( le_unit_t * const le_unit, le_enum_t const le_state );

    /* *** */

    /*! \brief mutator methods
     *
     *  This function allows to optimise the storage structure of the provided
     *  unit structure. The function rewrites the storage structure of the unit
     *  to allow faster read operations.
     *
     *  The function starts by creating "dual" files for each scales of the unit
     *  in which the content of the current file is re-written in an optimised
     *  fashion. At the end of the process, the "dual" files are move to over-
     *  write the unit previous files.
     *
     *  This operation being heavy in terms of read and write operation, it can
     *  be long on large unit storage structure.
     *
     *  \param le_unit Unit structure
     */

    le_void_t le_unit_set_optimise( le_unit_t * const le_unit );

    /*! \brief i/o methods
     *
     *  This recursive function is the function used by the storage optimisation
     *  procedure implemented in \b le_unit_set_optimise(). This function is
     *  used to read the storage structure as the tree it defines. In the same
     *  time, the read classes of the different scales are rewritten in their
     *  "tree-reading" order in the provided "dual" files. The recursive process
     *  ends as the tree is entirely read, and so, written in the "dual" files.
     *
     *  The "dual" stream array has to have, at least, a size equal to the
     *  server space parameter. In addition, it as to contain already created
     *  file descriptor toward the optimisation "dual" files.
     * 
     *  In the same way, the class writing offsets tracker array (\b le_head)
     *  has the same constraint on its size. In addition, its content has to be
     *  zero for each of it used elements.
     *
     *  \param le_unit   Unit structure
     *  \param le_dual   Dual stream array
     *  \param le_offset Class offset
     *  \param le_head   Class writing offsets tracker
     *  \param le_scale  Scale number
     */

    le_void_t le_unit_io_arrange( le_unit_t * const le_unit, le_file_t const * const le_dual, le_size_t const le_offset, le_size_t * const le_head, le_size_t const le_scale );

    /*! \brief i/o methods
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

    le_size_t le_unit_io_offset( le_unit_t * const le_unit, le_address_t * const le_addr );

    /*! \brief i/o methods
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

    le_void_t le_unit_io_inject( le_unit_t * const le_unit, le_array_t const * const le_array );

    /*! \brief i/o methods
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

    le_void_t le_unit_io_gather( le_unit_t * const le_unit, le_address_t * const le_addr, le_size_t le_offset, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /*! \brief i/o methods
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

    le_void_t le_unit_io_parallel( le_unit_t * const le_unia, le_unit_t * const le_unib, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t le_offseta, le_size_t le_offsetb, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

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

