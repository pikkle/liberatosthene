/*
 *  liberatosthene
 *
 *      Nils Hamel - nils.hamel@bluewin.ch
 *      Copyright (c) 2016-2017 EPFL CDH DHLAB
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

    /*! \struct le_server_struct
     *  \brief server structure
     *
     *  This structure is the principal structure of the library as it holds
     *  the descriptor of the servers.
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
     *  of the data. This structure is then the heart allowing these operation
     *  to be fulfilled.
     *
     *  A first field is dedicated to store the server socket descriptor. As any
     *  server, an eratosthene server maintain a service on a computer available
     *  through a predefined port. This field store the socket descriptor set
     *  to listen to client queries.
     *
     *  Three fields are dedicated to the server configuration. The first one
     *  holds a character array storing the main path of the storage structure.
     *  This path is used to access storage structure files. The two remaining
     *  configuration fields are used to store the space and time parameters,
     *  The space parameter gives the number of scale engaged in the storage
     *  structure. It follows that any space index used to access or store data
     *  has to contain less digit than this configuration value. The time value
     *  gives the size of the temporal equivalence classes. Both parameters fix
     *  the server resolution power in both space and time.
     *
     *  The last field of the structure holds a sub-structure dedicated to the
     *  management of i/o operation made on the storage structure. The server
     *  has to give this structure to any i/o method accessing the storage
     *  structure. More information on the stream structure can be found in the
     *  stream module documentation.
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
     *  time, to equivalence relation are defined : one on the time range and
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
     *  browse the spatial scales to find or store any point. Again, two point
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
     *  \var le_server_struct::sv_stream
     *  Server i/o stream sub-system
     */

    typedef struct le_server_struct {

        le_sock_t   sv_sock;

        le_char_t * sv_path;
        le_size_t   sv_scfg;
        le_time_t   sv_tcfg;

    le_enum_t _status; } le_server_t;

    typedef struct le_server_box_struct {

        le_sock_t   bx_sock;
        le_proc_t   bx_proc;
        le_void_t * bx_srvp;

    } le_server_box_t, le_box_t;

/*
    header - function prototypes
 */

    /*! \brief constructor/destructor methods
     *
     *  This function creates a server structure and initialises it contents
     *  according to the provided parameters. It initialise the stream sub
     *  system and reads the server configuration file hold in the storage
     *  structure pointed by the provided path.
     *
     *  It also create the socket descriptor used by the server to listen to
     *  client queries.
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
     *  closing the socket descriptor and by deleting the stream sub system.
     *  The function ends by clearing the structure fields.
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
     * \return Returns LE_ERROR_SUCCESS on success, an error code otherwise
     */

    le_enum_t le_server_set_config( le_server_t * const le_server );

    /*! \brief i/o methods - principale
     *
     *  With the structure constructor and destructor methods, the function is
     *  part of the main methods. As the structure is created, it starts an
     *  infinite loop waiting for client connections.
     *
     *  As a client connects to the server, the function create a socket for the
     *  incoming connection and analysis the client handshake. The connection
     *  is then dispatch to the specific server sub-process according to the
     *  handshake analysis.
     *
     *  As the client query is processed by the sub-process, the server closes
     *  the dedicated socket and returns back to the listening loop.
     *
     *  \param le_server Server structure
     */

    le_void_t le_server_io( le_server_t * const le_server );

    /* *** */

    le_void_t * le_server_io_client( le_void_t * le_box_ );

    /* *** */

    le_enum_t le_server_io_agreement( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_array, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function is a specific server sub-process.
     *
     *  It is called as the dispatcher receives an injection query from a remote
     *  client. It reads the injection time and data array from the provided
     *  socket and, after security checks, calls the stream injection function.
     *
     *  \param le_server Server structure
     *  \param le_client Client socket descriptor - server-side
     */

    le_enum_t le_server_io_inject( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_array, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function is a specific server sub-process.
     *
     *  It is called as the dispatcher receives an time reduction query from a
     *  remote client. It reads the address structure from the socket and ask,
     *  for the time(s) engaged by the address mode, the stream function to
     *  reduce it(them). As time(s) is(are) reduced, the address structure is
     *  updated and written back on the client socket.
     *
     *  Time reduction is a specific features sideways from proper data query.
     *  A client does not know anything about data available through the server.
     *  It follows, as it provides times through the address used for query,
     *  that almost always these times does not fit any time class available in
     *  the storage structure. The server searches then the best non-empty, from
     *  the spatial index point of view, time class. As a result, the server
     *  update the address times - explaining why they are written back to the
     *  client during data queries.
     *
     *  For some graphical clients, it is important to be able to know how the
     *  server will affect the address times (time reduction) before to perform
     *  the proper query, explaining this functionality.
     *
     *  \param le_server Server structure
     *  \param le_client Client socket descriptor - server-side
     */

    le_enum_t le_server_io_reduce( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_array, le_sock_t const le_socket );

    /*! \brief i/o methods
     *
     *  This function is a specific server sub-process.
     *
     *  It is called as the dispatcher receive an data query from a remote
     *  client. It reads the query address structure from the socket and gather
     *  the relevant data through specific stream method according to the time
     *  reduction (see \b le_server_io_reduce()) result and the address mode.
     *
     *  As the points are gathered from the storage structure, the function
     *  writes back the time-reduced address and the gathered data array on the
     *  on the client socket.
     *
     *  \param le_server Server structure
     *  \param le_client Client socket descriptor - server-side
     */

    le_enum_t le_server_io_query( le_server_t * const le_server, le_stream_t * const le_stream, le_array_t * const le_array, le_sock_t const le_socket );

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

