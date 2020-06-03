## Overview

The _liberatosthene_ library implements an Earth scale 4D tile server through a dedicated multi-scale space-time indexation formalism. Using the theoretical framework of the index, it implements a simple and reliable 4D geographical data storage and access system. This allows software developed using this library to perform large-scale data injection and browsing with high efficiency over the network to provide remote 4D geographic services and information systems. Initially designed for point-based data, the indexation formalism and the library implementation allow now to manage polygonal representations along with large scale point-based models to create a rich 4D model of the Earth.

## Eratosthene Project

The Eratosthene Project was initially designed to address the challenge of very large point-based models of cities and environments. Indeed, mapping a whole city at the level of the centimeter induces very large and heavy models that are difficult to manipulate, explore and exploit. Moreover, considering the time dimension implies a quick increase of the size of the models, making them impossible to handle as a single and monolithic object.

The Eratosthene Project is designed to provide a solution by considering a space-time indexation formalism. This indexation formalism is used to implement a server able to manage, store and broadcast over the network very large 4D models distributed all over the Earth surface and covering large periods of time. In addition to the server, the Eratosthene Project also implements a graphical client able to communicate with the remote servers to offer a convenient way to browse the models in both space and time.

The space-time indexation formalism is designed to efficiently manage the storage of the incoming 4D models and to offer a simple and efficient way of addressing queries to the server to allow models broadcasting. The space-time indexation also allows to perform queries to the server in an intuitive way giving the possibility to both address the challenge of the Earth structures level of details and time management. This allows to explore the entire Earth surface in terms of 4D structures from large mountain chains in 2000 to very small details of cities in 1850.

The subsequent indexation formalism used to drive the storage and access of the data also naturally brings a standardised discretisation of space and time. Point-based models injected in the Eratosthene information system are then standardised from both spatial and temporal point of views. This allows the implementation of binary and logical operators when considering a model at two different times. The operators allow to deduce a third model on the fly providing a differential point of view of the two models separated in time. The evolution of cities and environments can be enhanced and analysed in an efficient way, opening the way to more advanced geographic services.

The ability of indexation formalism to consider polygonal models in addition to large scale point-based models allows to enhance the possibilities in terms of 4D geographic services. More than the possibility to automatically manage the polygonal models level of details and large scale display, the indexation formalism also allows compositions on the fly of mixed models allowing to compare different type of models, from lidar point-based models to land registers.

## Copyright and License

**liberatosthene** - Nils Hamel <br >
Copyright (c) 2016-2020 DHLAB, EPFL

This program is licensed under the terms of the GNU GPLv3.

## Dependencies

The _liberatosthene_ comes with the following package (Ubuntu 16.04 LTS) dependencies ([Instructions](DEPEND.md)) :

* build-essential

The code documentation is built using Doxygen.

## Compilation

To build the project, including the sub-modules, use make with the following targets :

    $ make clean-all && make all

To rebuild the binaries without rebuilding sub-modules, use the make targets :

    $ make clean && make build

To only rebuild sub-modules, use the make command with the targets :

    $ make clean-module && make module

To generate the documentation of the project, use the targets :

    $ make clean-doc && make doc

and the targets :

    $ make clean-all-doc && make all-doc

to generate the documentation of the project and its sub-modules.
