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

    /*! \file   eratosthene-tree-unit.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - tree unit module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_TREE_UNIT__
    # define __LE_TREE_UNIT__

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
    # include "eratosthene-class.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_UNIT_C          { 0, { NULL }, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    # define LE_UNIT_C_TIME(t)  { t, { NULL }, LE_ERROR_SUCCESS }

    /* define stream mode */
    # define LE_UNIT_READ       ( 0 )
    # define LE_UNIT_WRITE      ( 1 )

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
     *  The first field is used to store the time associated to the unit. The
     *  value stored in this field is according to the equivalence classes point
     *  of view :
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
     *  \var le_unit_struct::un_time
     *  Unit time in reduced form
     *  \var le_unit_struct::un_pile
     *  Unit stream pile
     *  \var le_unit_struct::_status
     *  Standard status field
     */

    typedef struct le_unit_struct {

        le_time_t un_time;
        le_file_t un_pile[_LE_USE_DEPTH];

    le_enum_t _status; } le_unit_t;

/*
    header - function prototypes
 */

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
     *  \param le_mode   Access mode value
     *  \param le_time   Unit time value, in reduced form
     *  \param le_scfg   Server spatial configuration value
     *
     *  \return Returns the created unit
     */

    le_unit_t le_unit_create( le_char_t const * const le_root, le_time_t const le_time, le_enum_t const le_mode, le_size_t const le_scfg );

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
     *  \param le_root Server storage structure path
     */

    le_void_t le_unit_set_optimise( le_unit_t * const le_unit, le_char_t const * const le_root );

    /*! \brief mutator methods
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

    le_void_t le_unit_set_arrange( le_unit_t * const le_unit, le_file_t const * const le_dual, le_size_t const le_offset, le_size_t * const le_head, le_size_t const le_scale );

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

