/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 DHLAB, EPFL
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

    /*! \file   eratosthene-stream.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - stream module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_STREAM__
    # define __LE_STREAM__

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
    # define LE_STREAM_C        { NULL, 0, 0, 0, NULL, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    # define LE_STREAM_I(p,s,t) { p, s, t, 0, NULL, LE_ERROR_SUCCESS }

    /* define pseudo-constructor */
    # define LE_STREAM_UNIT_C   { _LE_TIME_NULL, { NULL } }

    /* define stream mode */
    # define LE_STREAM_READ     ( 0 )
    # define LE_STREAM_WRITE    ( 1 )

    /* define path model */
    # define LE_STREAM_TIME     ( "%s/%" _LE_TIME_P )
    # define LE_STREAM_FILE     ( "%s/%" _LE_TIME_P "/scale-%03" _LE_SIZE_P ".bin" )

/*
    header - preprocessor macros
 */

    /* define file mode macro */
    # define le_stream_mode(m) ( ( m ) == LE_STREAM_READ ? "r+" : "w+" )

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_stream_unit_struct
     *  \brief unitary stream structure
     *
     *  This structure holds the information required to access an equivalence
     *  class on time dimension. It holds two fields that are the class time
     *  value and the class file descriptors.
     *
     *  The time value hold by the structure is used to access the directory
     *  that stores the class storage files. It is not a proper time value as it
     *  is divided by the server time parameter, the storage directories being
     *  named this way.
     *
     *  The file descriptors stack holds the handles to class storage files. The
     *  file descriptors are initiated as the unit is set in order to reduce
     *  the amount of operations on file system. The file descriptors are closed
     *  as the main stream structure is deleted, usually at server shutdown.
     *
     *  \var le_stream_unit_struct::su_time
     *  Stream unit time value
     *  \var le_stream_unit_struct::su_file
     *  Stream unit file descriptors stack
     */

    typedef struct le_stream_unit_struct {

        le_time_t   su_time;
        le_file_t   su_file[_LE_USE_DEPTH];

    } le_stream_unit_t, le_unit_t;

    /*! \struct le_stream_struct
     *  \brief stream structure
     *
     *  This structure holds the stream units stack and is used by the server as
     *  a sub-system to perform i/o operations on the server storage structure.
     *
     *  In the first place, this structure holds three fields that receive, at
     *  structure creation, a copy of the server storage structure path and
     *  a copy of the server space and time configuration values. These values
     *  are required to access the storage structure.
     *
     *  The main structure fields are the stream units stack and its size. The
     *  stream units stack is used to store the file descriptors of each
     *  temporal class found in the storage structure. During the structure
     *  creation, the storage structure is scanned to detect all the temporal
     *  classes and their directory. A stream unit is created for each detected
     *  class. This allows the stream module to perform i/o operations on the
     *  server storage structure without having to constantly create and delete
     *  the required file descriptors.
     *
     *  A specific constraint is applied on the stream units stack : it has to
     *  be ordered from a temporal point of view :
     *
     *      unit[i].su_time < unit[j].su_time
     *
     *  as i < j. This constraint is imposed to allow function to implements
     *  efficient search algorithms.
     *
     *  As this structure is deleted, usually at server shutdown, the stream
     *  units of the stack are parsed one by one and deleted. The stream units
     *  stack is then unallocated.
     *
     *  \var le_stream_struct::sr_root
     *  Server storage structure path
     *  \var le_stream_struct::sr_scfg
     *  Server spatial configuration value : number of scale
     *  \var le_stream_struct::sr_tcfg
     *  Server temporal configuration value : size of temporal classes
     *  \var le_stream_struct::sr_size
     *  Stream units stack size
     *  \var le_stream_struct::sr_strm
     *  Stream units stack
     */

    typedef struct le_stream_struct {

        le_char_t * sr_root;
        le_size_t   sr_scfg;
        le_time_t   sr_tcfg;

        le_size_t   sr_size;
        le_unit_t * sr_strm;

    le_enum_t _status; } le_stream_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function is used to initiate a stream structure usually used by the
     *  server to operate i/o operations on its storage structure.
     *
     *  As the parameters are assigned to the structure fields, the function
     *  starts to scan the storage structure to list the temporal equivalence
     *  classes hold by the storage structure. For each found temporal class, it
     *  creates a stream unit and adds it to the stream units stack. At end, the
     *  returned stream structure contains all the required file descriptors to
     *  access any file of the storage structure.
     *
     *  As the function returns the created stream structure, the status of the
     *  creation is set using the \b _status reserved structure field.
     *
     *  \param le_path Server storage structure path
     *  \param le_scfg Server spatial configuration value
     *  \param le_tcfg Server temporal configuration value
     *
     *  \return Returns created stream structure
     */

    le_stream_t le_stream_create( le_char_t * const le_path, le_size_t const le_scfg, le_time_t const le_tcfg );

    /*! \brief constructor/destructor methods
     *
     *  This function is used to deleted the provided stream structure. It reads
     *  the content of the stream units stack and deletes the stream units one
     *  by one by closing their file descriptors. The stack memory itself is
     *  then unallocated and the structure fields are cleared using default
     *  values.
     *
     *  \param le_stream Stream structure
     */

    le_void_t le_stream_delete( le_stream_t * const le_stream );

    /*! \brief accessor methods
     *
     *  This function is used to get the access index to a stream unit of the
     *  stack. It searches the stream unit that match, exactly, the provided
     *  time value.
     *
     *  The result of the function depends on the mode value : \b LE_STREAM_READ
     *  indicates that the stream unit has to exist. If the stream unit is not
     *  found in the stack, and invalid index is returned.
     *
     *  As \b LE_STREAM_WRITE is provided as mode, the function creates a new
     *  temporal class in the storage structure if the stream unit was not found
     *  and adds a new stream unit to the stack. This function is mainly used
     *  during data injection.
     *
     *  The provided time value has to be a proper time, not divided by the
     *  server time configuration value.
     *
     *  \param le_stream Stream structure
     *  \param le_time   Stream unit time value
     *  \param le_mode   Stream unit access mode (r/w)
     *
     *  \return Returns stream unit index on success, _LE_SIZE_NULL otherwise
     */

    le_size_t le_stream_get_strict( le_stream_t * const le_stream, le_time_t const le_time, le_enum_t const le_mode );

    /*! \brief accessor methods (outdated : addition of time limitation)
     *
     *  This function is used to get the access index to a stream unit of the
     *  stack. It searches the stream unit that is the nearest to the address
     *  time value pointed by \b le_addrt and that is not empty from the
     *  provided address spatial index point of view.
     *
     *  As the nearest non empty stream unit is found, the function replaces the
     *  address time by the related temporal class lower edge :
     *
     *      address(time(le_addrt)) = class(time) * server_space_value
     *
     *  The stream unit index is then returned by the function. If the function
     *  fails, the address time is replaced using the \b _LE_TIME_NULL value.
     *
     *  In addition, the caller can pass a pointer to a size value that gets,
     *  in case of non null pointer, the offset of the found non empty spatial
     *  class. As the function has to search the spatial class in the storage
     *  structure, returning its offset allows the caller subsequent processes
     *  to avoid to search it again.
     *
     *  The provided time value has to be a proper time, not divided by the
     *  server time configuration value.
     *
     *  \param le_stream Stream structure
     *  \param le_addr   Address structure
     *  \param le_addrt  Address structure time index
     *  \param le_option Returned class offset - optional variable
     *
     *  \return Returns stream unit index on success, _LE_SIZE_NULL otherwise
     */

    le_size_t le_stream_get_reduct( le_stream_t const * const le_stream, le_address_t * const le_addr, le_size_t const le_addrt, le_size_t * const le_option );

    /*! \brief mutator methods
     *
     *  This function creates the stream unit related to the temporal class
     *  pointed by the provided time value.
     *
     *  The function searches if the time value points to an existing storage
     *  structure temporal class. As the class is found, the function creates
     *  a new stream unit on the stack and initialises it for the found class.
     *
     *  If the class is not found in the storage structure, the behaviour of the
     *  function depends on the access mode : \b LE_STREAM_READ indicates the
     *  function to fail. Providing \b LE_STREAM_WRITE tells the function to
     *  create the class storage directory and files and to create a stream unit
     *  for the created class.
     *
     *  If a new stream unit is created by the function, it inserts it in the
     *  stream units stack following the order constraint.
     *
     *  The provided time value has to be a proper time, not divided by the
     *  server time configuration value.
     *
     *  \param le_stream Stream structure
     *  \param le_time   Stream unit time value
     *  \param le_mode   Stream unit access mode (r/w)
     *
     *  \return Returns stream unit undex on success, _LE_SIZE_NULL otherwise
     */

    le_size_t le_stream_set( le_stream_t * const le_stream, le_time_t le_time, le_enum_t const le_mode );

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
     *  \param le_stream Stream structure
     *  \param le_unit   Stream unit stack index
     *  \param le_array  Data array
     */

    le_void_t le_stream_io_inject( le_stream_t const * const le_stream, le_size_t const le_unit, le_array_t const * const le_array );

    /*! \brief i/o methods
     *
     *  This function searches the offset of the spatial class pointed by the
     *  spatial index provided through the address structure in the provided
     *  temporal class storage structure.
     *
     *  The spatial index digits are used to drive the progression through
     *  the structure scales. This function is mainly used to detect if the
     *  class pointed by the address exists or not.
     *
     *  \param le_stream Stream structure
     *  \param le_unit   Stream unit stack index
     *  \param le_addr   Address structure
     *
     *  \return Returns the class offset on success, _LE_OFFS_NULL otherwise
     */

    le_size_t le_stream_io_offset( le_stream_t const * const le_stream, le_size_t const le_unit, le_address_t const * const le_addr );

    /*! \brief i/o methods
     *
     *  This function is used to gather spatial classes positions and colours
     *  information to build a array using the UF3 format.
     *
     *  This function assume that the offset of the spatial class pointed by the
     *  address index is known (\b le_stream_io_offset()). Starting from the
     *  offsets array of this class, it starts to gather the position and
     *  colours by enumerating the class daughters and sub-daughters through a
     *  recursive process. It detect the gathering scale using the main class
     *  scale and the address additional depth (span).
     *
     *  The array is filled with the gathered the daughters and sub-daughters
     *  spatial classes representative. The array is filled by pushing the
     *  gathered elements at its end, leaving the prior content unchanged.
     *
     *  \param le_stream Stream structure
     *  \param le_unit   Stream unit stack index
     *  \param le_addr   Address structure
     *  \param le_offset Spatial class offset
     *  \param le_parse  Spatial class scale
     *  \param le_span   Query additional depth
     *  \param le_array  Data array filled by the function
     */

    le_void_t le_stream_io_gather( le_stream_t const * const le_stream, le_size_t const le_unit, le_address_t * const le_addr, le_size_t le_offset, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

    /*! \brief i/o methods
     *
     *  This function implements a parallel version of \b le_stream_io_gather()
     *  function. It performs the same operations simply considering distinct
     *  times explaining why some parameters are doubled.
     *
     *  As two parallel gathering process take place in this function, it allows
     *  it to consider the times comparison mode provided by the address. As
     *  elements for the two times are gathered at the same time, the function
     *  is able to easily implements the logical operators. The array is then
     *  filled with the results of the application of the logical operators.
     *
     *  The array is filled with the gathered the daughters and sub-daughters
     *  spatial classes representative. The array is filled by pushing the
     *  gathered elements at its end, leaving the prior content unchanged.
     *
     *  \param le_stream Stream structure
     *  \param le_uni1   Stream unit stack index - time 1
     *  \param le_uni2   Stream unit stack index - time 2
     *  \param le_addr   Address structure
     *  \param le_off1   Spatial class offset - time 1
     *  \param le_off2   Spatial class offset - time 2
     *  \param le_parse  Spatial class scale
     *  \param le_span   Query additional depth
     *  \param le_array  Data array filled by the function
     */

    le_void_t le_stream_io_parallel( le_stream_t const * const le_stream, le_size_t const le_uni1, le_size_t const le_uni2, le_address_t * const le_addr, le_size_t le_off1, le_size_t le_off2, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array );

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

