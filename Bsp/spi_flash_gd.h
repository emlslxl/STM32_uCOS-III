/*
 * File      : spi_flash_gd.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2016, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-10-11     fullhan      copy from winbond flash
 */

#ifndef SPI_FLASH_GD_H_
#define SPI_FLASH_GD_H_

#include "board.h"

#define RT_DEVICE_CTRL_BLK_GETGEOME     0x10            /**< get geometry information   */

struct rt_device_blk_geometry
{
    uint32_t sector_count;                           /**< count of sectors */
    uint32_t bytes_per_sector;                       /**< number of bytes per sector */
    uint32_t block_size;                             /**< number of bytes to erase one block */
};

struct spi_flash_device
{
    SPI_TypeDef          *SPIx;
    struct rt_device_blk_geometry   geometry;
};



#endif /* SPI_FLASH_GD_H_ */
