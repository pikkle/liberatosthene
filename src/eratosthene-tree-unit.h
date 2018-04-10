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

    /* *** */

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
     *  creates the scale file while creating the streams to them.
     *
     *  If any stream is not accessible or of the directory creation fails, the
     *  function returns the structure as it is a sets the \b _status field
     *  accordingly.
     *
     *  \param le_path Path to the unit storage structure
     *  \param le_mode Unit creation mode
     *  \param le_time Unit time value, in reduced form
     *  \param le_scfg Server spatial parameter (number of scales)
     *
     *  \return Returns the created unit
     */

    le_unit_t le_unit_create( le_char_t const * const le_path, le_enum_t const le_mode, le_time_t const le_time, le_size_t const le_scfg );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the provided unit structure. It parses the scales
     *  of the unit and deletes each scale opened streams. It finally resets the
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
     *  \param le_scale Scale value
     *
     *  \return Returns unit scale stream file descriptor
     */

    le_file_t le_unit_get_stream( le_unit_t const * const le_unit, le_size_t const le_scale );

    /*! \brief accessor methods
     *
     *  This function simply returns the time of the provided unit. The returned
     *  time is in reduce form.
     *
     *  \param le_unit Unit structure
     *
     *  \return Returns the unit time
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
     *  \return Returns _LE_TRUE on condition, _LE_FALSE otherwise
     */

    le_enum_t le_unit_get_prior( le_unit_t const * const le_unit, le_unit_t const * const le_candidate );

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

