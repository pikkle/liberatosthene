## Overview

The _liberatosthene_ library implements an earth scale 4D tile server through a dedicated space-time index formalism. Using the theoretical framework of the index, it implements a simple and reliable 4D data storage and access. This allows servers developed using this library to perform data injection and query with high efficiency over the network to provide 4D geographic services and information systems.

From a theoretical point of view, any type of point-based data, as referenced in space and time, can be considered through the index formalism. Currently, colours are considered as data. This allows servers built on this library to broadcast large amount of colorimetric points over the network, toward large and multiscale 4D geographic services information systems at earth scale.

## Eratosthene Project

The Eratosthene Project was initially imagined to answer the question of very large point-based models of cities and environments. Indeed, mapping a whole city at the level of the centimetre induces very large and heavy models that are difficult to manipulate, explore and exploit. Moreover, considering the time dimension implies a quick increase of the size of the models, making them impossible to handle as a single and monolithic object.

The Eratosthene Project is designed to answer this question by implementing an space-time indexation formalism able to handle, store and access very large 4D models of cities and environments. The indexation formalism is used to implement a server able to manage, store and broadcast very large 4D models distributed all over the earth surface and covering large period of time. In addition to the server, the Eratosthene Project also implements a graphical client able to communicate with the server and to offer a convenient way of browsing the models in both space and time.

The space-time indexation formalism is designed to efficiently manage the storage of the incoming 4D models and to offer and simple and efficient ways of addressing queries to the server allowing models broadcasting. The space-time indexation also allows to perform queries to the server in a intuitive way giving the possibility to both address the questions of the earth structures level of detail and time management. This allows to explore the entire earth surface in terms of 4D structures from large mountain chains in 2000 to very small details of cities in 1850.

The subsequent indexation formalism used to drive the storage and access of the data also naturally brings a standardised discretisation of space and time. Models injected in the information system are then standardised from both spatial and temporal point of view. This allows the implementation of binary and logical operators when considering a model at two different times. The operators allows to deduce a third model providing a differential point of view of the two models separated in time. The evolution of cities and environments can be enhanced and analysed in an efficient way, opening the way to more advanced geographic services.

## Copyright and License

**liberatosthene** - Nils Hamel <br >
Copyright (c) 2016-2018 DHLAB, EPFL

This program is licensed under the terms of the GNU GPLv3.

## Dependencies

The _liberatosthene_ comes with the following package dependencies (Ubuntu 16.04 LTS) :

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
