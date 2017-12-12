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

    /*! \file   eratosthene-server.h
     *  \author Nils Hamel <nils.hamel@bluewin.ch>
     *
     *  liberatosthene - server module
     */

/*
    header - inclusion guard
 */

    # ifndef __LE_SERVER__
    # define __LE_SERVER__

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
    # include "eratosthene-client.h"
    # include "eratosthene-stream.h"

/*
    header - external includes
 */

/*
    header - preprocessor definitions
 */

    /* define pseudo-constructor */
    # define LE_SERVER_C { _LE_SOCK_NULL, NULL, 0, 0, LE_ERROR_SUCCESS }

/*
    header - preprocessor macros
 */

/*
    header - type definition
 */

/*
    header - structures
 */

    /*! \struct le_server_ring_struct
     *  \brief server ring structure
     *
     *  This structure describe a thread launch in the main server loop to
     *  handle a client connection. It stores the required information needed
     *  by the thread to perform answer to the client request.
     *
     *  The first field of the structure is related to the thread ID and is used
     *  to manage the thread itself. The second thread holds the server socket
     *  opened toward the client handle by the thread. The last fields stores
     *  a pointer to the main server structure that is used to access server
     *  configuration and storage.
     *
     *  \var le_server_ring_struct::rg_proc
     *  Thread process ID
     *  \var le_server_ring_struct::rg_sock
     *  Server socket descriptor toward handled client
     *  \var le_server_ring_struct::rg_srvp
     *  Pointer to the main server structure
     */

    typedef struct le_server_ring_struct {

        le_proc_t   rg_proc;
        le_sock_t   rg_sock;
        le_void_t * rg_srvp;

    } le_server_ring_t, le_ring_t;

    /*! \struct le_server_struct
     *  \brief server structure
     *
     *  This structure is the principal structure of the library as it holds
     *  the descriptor of a servers.
     *
     *  The server offers a 4D mapping service of the earth through to storage,
     *  access and network broadcasting of colorimetric point. Its storage
     *  structure, data injection and data query are driven by the formalism
     *  of spatiotemporal index. In other words, the server can be seen as a
     *  4D tile server based on the theoretical framework of index.
     *
     *  A server is then responsible of receiving data from client during
     *  injection, data broadcasting during client query, broadcasting of its
     *  main parameter through client specific query and the storage management
     *  of the data. This structure is then the heart allowing these operations
     *  to be fulfilled.
     *
     *  A first field is dedicated to store the server socket descriptor. As any
     *  server, an eratosthene server maintain a service on a computer available
     *  through a predefined port. This field store the socket descriptor set
     *  to listen to client queries.
     *
     *  Three fields are dedicated to the server configuration. The first one
     *  holds a characters array storing the main path of the storage structure.
     *  This path is used to access storage structure files. The two remaining
     *  configuration fields are used to store the space and time parameters,
     *  The space parameter gives the number of scale engaged in the storage
     *  structure. It follows that any space index used to access or store data
     *  has to contain less digit than this configuration value. The time value
     *  gives the size of the temporal equivalence classes. Both parameters fix
     *  the server resolution power in both space and time.
     *
     *  A last field is used by the structure creation methods to specified the
     *  creation status of the structure. This allows to creation methods to
     *  return the structure instead of an error code.
     *
     *  As the server offers a 4D information system through the formalism of
     *  spatiotemporal index, one has to take into account geographic ranges
     *  allowed by indexation. For both longitude and latitude, the canonical
     *  ranges are considered (with coordinates always in radian) : [-pi,pi] and
     *  [-pi/2,pi/2]. A more specific range is considered for height values
     *  (with coordinates always in meter above WGS84 ellipsoid) : [-h,h] where
     *  h equal to \b LE_ADDRESS_MAXH (address module). Any point injected in
     *  the server has to be inside the defined ranges.
     *
     *  The server data storage and access are driven by the spatiotemporal
     *  index formalism. As any point is assumed to be referenced in space and
     *  time, two equivalence relations are defined : one on the time range and
     *  the other along the spatial scales. The time relation simply establish
     *  homogeneous ranges along the time dimension collapsing all time in each
     *  range to a single equivalence class. It follow that two time falling in
     *  the same class cannot be distinguished anymore after injection. The
     *  structure time parameter gives the size, in seconds, of time equivalence
     *  classes.
     *
     *  Each temporal class holds a spatial storage unit. Each unit contain
     *  scales (files) in amount given by the structure spatial configuration
     *  value. On the model of the relation defined on time, a similar relation
     *  is set on each scale : each scale, in term of range, is split in 2^i
     *  ranges identified as equivalence classes. Any given point belong to a
     *  given class at each scale. Index are used, through their digits to
     *  browse the spatial scales to find or store any point. Again, two points
     *  falling in the same class in a specific scale cannot be separated after
     *  injection. In other words, index digits are the address allowing to go
     *  from a scale to another following the logic of octrees.
     *
     *  \var le_server_struct::sv_sock
     *  Server socket descriptor
     *  \var le_server_struct::sv_path
     *  Server storage structure path
     *  \var le_server_struct::sv_scfg
     *  Server spatial configuration value : number of scale
     *  \var le_server_struct::sv_tcfg
     *  Server temporal configuration value : size of temporal classes
     *  \var le_server_struct::_status
     *  Status field
     */

    typedef struct le_server_struct {

        le_sock_t   sv_sock;

        le_char_t * sv_path;
        le_size_t   sv_scfg;
        le_time_t   sv_tcfg;

    le_enum_t _status; } le_server_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates a server structure and initialises its content
     *  according to the provided parameters. It reads the server configuration
     *  file hold in the storage structure pointed by the provided path.
     *
     *  It also create the socket descriptor used by the server to listen to
     *  client connections.
     *
     *  This function returning the created structure, the status is stored in
     *  the structure itself using the reserved \b _status field.
     *
     *  \param le_port Server service port
     *  \param le_path Server storage structure path
     *
     *  \return Returns created server structure
     */

    le_server_t le_server_create( le_char_t * const le_path, le_sock_t const le_port );

    /*! \brief constructor/destructor methods
     *
     *  This function deletes the server structure provided as parameter by
     *  closing the socket descriptor. The function ends by clearing the fields
     *  of the structure.
     *
     *  \param le_server Server structure
     */

    le_void_t le_server_delete( le_server_t * const le_server );

    /*! \brief mutator methods
     *
     *  This function reads the configuration file at the root of the server
     *  storage structure. To locate the storage structure, this function uses
     *  the path hold by the structure itself, assumed to be correctly filled.
     *
     *  The function opens and reads the two space and time configuration values
     *  stored in the file (text mode) and perform basic consistency checks.
     *
     *  \param le_server Server structure
     *
     *  \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_server_set_config( le_server_t * const le_server );

    /*! \brief i/o methods
     *
     *  With the structure constructor and destructor methods, the function is
     *  part of the main methods. As the structure is created, it starts an
     *  infinite loop waiting for client connections.
     *
     *  As a client connects to the server, the function searches a free ring,
     *  that is a thread descriptor structure (\b le_ring_t). The found ring
     *  is initialised with the created socket toward the client and a thread
     *  is created base on the ring descriptor. The created thread being
     *  responsible of the client management, the function main loop continue
     *  to listen to new incoming connections.
     *
     *  \param le_server Server structure
     */

    le_void_t le_server_io( le_server_t * const le_server );

    /*! \brief i/o methods
     *
     *  This function is executed by threads launched by the \b le_server_io()
     *  function to manage a client connection. Its role is to run an infinite
     *  loop waiting for client request. As requests arrive, the function
     *  analyse them and call the specific process that answer the request.
     *
     *  Before to start the infinite loop, this function creates and initialise
     *  a stream structure toward the server storage structure. This structure
     *  is needed by the other i/o methods to performs operation on the storage
     *  structure.
     *
     *  \param le_ring_ Pointer to thread ring descriptor
     */

    le_void_t * le_server_io_ring( le_void_t * le_ring_ );

    /*! \brief i/o methods
     *
     *  This i/o method is responsible of answering server configuration request
     *  from client. It simply packs the server configuration values, that are
     *  the number of space scale and the time equivalences classes length, in
     *  an array structure before to send it back to the remote client.
     *
     *  \param le_server Server structure
     *  \param le_stream Ring stream structure
     *  \param le_stack  Ring array stack pointer
     *  \param le_socket Socket to remote client
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_server_io_auth( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This i/o methods is responsible of data injection in the server storage
     *  structure. It expects a time packed in the first client array that is
     *  used to access the storage structure. It then expects a data array that
     *  contains the geographic coordinates and colour of the points to inject
     *  in the server storage structure.
     *
     *  \param le_server Server structure
     *  \param le_stream Ring stream structure
     *  \param le_stack  Ring array stack pointer
     *  \param le_socket Socket to remote client
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_server_io_inject( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function reads the query addresses packed in the client array and
     *  gather the relevant data through specific stream method.
     *
     *  As the points are gathered from the storage structure, the function
     *  writes the resulting data array on the client socket after having
     *  compressed the sent array.
     *
     *  \param le_server Server structure
     *  \param le_stream Ring stream structure
     *  \param le_stack  Ring array stack pointer
     *  \param le_socket Socket to remote client
     *
     *  \return Returns _LE_TRUE on success, _LE_FALSE otherwise
     */

    le_enum_t le_server_io_query( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_stack, le_sock_t const le_socket );

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

