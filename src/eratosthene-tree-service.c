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

    # include "eratosthene-tree-service.h"

/*
    source - i/o method
 */

    le_void_t le_service_io_inject( le_unit_t * const le_unit, le_array_t const * const le_array, le_size_t const le_scfg ) {

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* address variable */
        le_address_t le_addr = LE_ADDRESS_C_SIZE( le_scfg - 1 );

        /* digit variable */
        le_size_t le_digit = 0;

        /* scale variable */
        le_size_t le_scale = 0;
        le_size_t le_ahead = 0;

        /* offset variable */
        le_size_t le_forward = 0;
        le_size_t le_current = 0;

        /* array size variable */
        le_size_t le_size = le_array_get_size( le_array );

        /* parsing array */
        for ( le_size_t le_index = 0; le_index < le_size; le_index += LE_ARRAY_UF3 ) {

            /* compute address digits */
            le_address_set_pose( & le_addr, le_array_mac_pose( le_array, le_index ) );

            /* reset scale */
            le_scale = 0;
            le_ahead = 1;

            /* reset offset */
            le_forward = 0;
            le_current = 0;

            /* injection process */
            do {

                /* class importation */
                if ( le_class_io_read( & le_class, le_current, le_unit_get_stream( le_unit, le_scale ) ) == LE_ERROR_SUCCESS ) {

                    /* inject element in class */
                    le_class_set_push( & le_class, le_array_mac_data( le_array, le_index ) );

                } else {

                    /* initialise class with element */
                    le_class = le_class_create( le_array_mac_data( le_array, le_index ) );

                }

                /* extract address digit */
                le_digit = le_address_get_digit( & le_addr, le_scale );

                /* retrieve daughter offset */
                le_forward = le_class_get_offset( & le_class, le_digit );

                /* check daughter state */
                if ( ( le_forward == _LE_OFFS_NULL ) && ( le_ahead != le_scfg ) ) {

                    /* seek next scale eof */
                    fseek( le_unit_get_stream( le_unit, le_ahead ), 0, SEEK_END );

                    /* compute and check injection offset */
                    if ( ( le_forward = ftell( le_unit_get_stream( le_unit, le_ahead ) ) ) < _LE_OFFS_NULL ) {

                        /* insert offset in class */
                        le_class_set_offset( & le_class, le_digit, le_forward );

                    }

                }

                /* class exportation */
                le_class_io_write( & le_class, le_current, le_unit_get_stream( le_unit, le_scale ) );

            /* injection process condition */
            } while ( ( ( le_current = le_forward, ++ le_ahead, ++ le_scale ) < le_scfg ) && ( le_forward < _LE_OFFS_NULL ) );

        }

    }

    le_void_t le_service_io_gather( le_unit_t * const le_unit, le_address_t * const le_addr, le_size_t le_offset, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variable */
        le_class_t le_class = LE_CLASS_C;

        /* enumeration boundary */
        if ( le_parse == le_span ) {

            /* update array size */
            le_array_set( le_array, LE_ARRAY_UF3 );

            /* psuh element position */
            le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

            /* push element data */
            le_class_det_data( le_offset, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unit, le_parse ) );

        } else {

            /* read class */
            le_class_io_readf( & le_class, le_offset, le_unit_get_stream( le_unit, le_parse ) );

            /* enumerate daughter classes */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* extract class offset */
                if ( ( le_offset = le_class_get_offset( & le_class, le_digit ) ) != _LE_OFFS_NULL ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_service_io_gather( le_unit, le_addr, le_offset, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

    le_void_t le_service_io_parallel( le_unit_t * const le_unia, le_unit_t * const le_unib, le_address_t * const le_addr, le_byte_t const le_mode, le_size_t le_offseta, le_size_t le_offsetb, le_size_t const le_parse, le_size_t const le_span, le_array_t * const le_array ) {

        /* class variable */
        le_class_t le_classa = LE_CLASS_C;
        le_class_t le_classb = LE_CLASS_C;

        /* message variable */
        le_enum_t le_msga = LE_ERROR_IO_READ;
        le_enum_t le_msgb = LE_ERROR_IO_READ;

        /* enumeration boundary */
        if ( le_parse == le_span ) {

            /* switch on mode */
            if ( le_mode == 3 ) {

                /* update array size */
                le_array_set( le_array, LE_ARRAY_UF3 );

                /* psuh element position */
                le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                /* logical or operator */
                if ( le_offseta != _LE_OFFS_NULL ) {

                    /* push element data */
                    le_class_det_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );

                } else {

                    /* push element data */
                    le_class_det_data( le_offsetb, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unib, le_parse ) );

                }

            } else if ( le_mode == 4 ) {

                /* logical and operator */
                if ( ( le_offseta != _LE_OFFS_NULL ) && ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* psuh element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element data */
                    le_class_det_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );

                }

            } else if ( le_mode == 5 ) {

                /* logical xor operator */
                if ( ( le_offseta != _LE_OFFS_NULL ) && ( le_offsetb == _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* psuh element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element data */
                    le_class_det_data( le_offseta, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unia, le_parse ) );

                } else if ( ( le_offseta == _LE_OFFS_NULL ) && ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update array size */
                    le_array_set( le_array, LE_ARRAY_UF3 );

                    /* psuh element position */
                    le_address_get_pose_( le_addr, le_parse, le_array_mac_lpose( le_array ) );

                    /* push element data */
                    le_class_det_data( le_offsetb, le_array_mac_ldata( le_array ), le_unit_get_stream( le_unib, le_parse ) );

                }

            }

        } else {

            /* check offset */
            if ( le_offseta != _LE_OFFS_NULL ) {

                /* read class */
                le_msga = le_class_io_readf( & le_classa, le_offseta, le_unit_get_stream( le_unia, le_parse ) );

            }

            /* check offset */
            if ( le_offsetb != _LE_OFFS_NULL ) {

                /* read class */
                le_msgb = le_class_io_readf( & le_classb, le_offsetb, le_unit_get_stream( le_unib, le_parse ) );

            }

            /* enumerate daughter classes */
            for ( le_size_t le_digit = 0; le_digit < _LE_USE_BASE; le_digit ++ ) {

                /* check message */
                if ( le_msga == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    le_offseta = le_class_get_offset( & le_classa, le_digit );

                /* reset offset */
                } else { le_offseta = _LE_OFFS_NULL; }

                /* check message */
                if ( le_msgb == LE_ERROR_SUCCESS ) {

                    /* extract class offset */
                    le_offsetb = le_class_get_offset( & le_classb, le_digit );

                /* reset offset */
                } else { le_offsetb = _LE_OFFS_NULL; }

                /* check offsets */
                if ( ( le_offseta != _LE_OFFS_NULL ) || ( le_offsetb != _LE_OFFS_NULL ) ) {

                    /* update address digit */
                    le_address_set_digit( le_addr, le_parse, le_digit );

                    /* recursive enumeration */
                    le_service_io_parallel( le_unia, le_unib, le_addr, le_mode, le_offseta, le_offsetb, le_parse + 1, le_span, le_array );

                }

            }

        }

    }

