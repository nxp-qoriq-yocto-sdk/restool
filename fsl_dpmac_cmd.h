/* Copyright 2013-2015 Freescale Semiconductor Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * * Neither the name of the above-listed copyright holders nor the
 * names of any contributors may be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 *
 * ALTERNATIVELY, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") as published by the Free Software
 * Foundation, either version 2 of that License or (at your option) any
 * later version.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _FSL_DPMAC_CMD_H
#define _FSL_DPMAC_CMD_H

/* DPMAC Version */
#define DPMAC_VER_MAJOR				2
#define DPMAC_VER_MINOR				1

/* Command IDs */
#define DPMAC_CMDID_CLOSE			0x800
#define DPMAC_CMDID_OPEN			0x80c
#define DPMAC_CMDID_CREATE			0x90c
#define DPMAC_CMDID_DESTROY			0x900

#define DPMAC_CMDID_GET_ATTR			0x004
#define DPMAC_CMDID_RESET			0x005

#define DPMAC_CMDID_SET_IRQ			0x010
#define DPMAC_CMDID_GET_IRQ			0x011
#define DPMAC_CMDID_SET_IRQ_ENABLE		0x012
#define DPMAC_CMDID_GET_IRQ_ENABLE		0x013
#define DPMAC_CMDID_SET_IRQ_MASK		0x014
#define DPMAC_CMDID_GET_IRQ_MASK		0x015
#define DPMAC_CMDID_GET_IRQ_STATUS		0x016
#define DPMAC_CMDID_CLEAR_IRQ_STATUS		0x017

#define DPMAC_CMDID_MDIO_READ			0x0c0
#define DPMAC_CMDID_MDIO_WRITE			0x0c1
#define DPMAC_CMDID_GET_LINK_CFG		0x0c2
#define DPMAC_CMDID_SET_LINK_STATE		0x0c3
#define DPMAC_CMDID_GET_COUNTER			0x0c4

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_CREATE(cmd, cfg) \
	MC_CMD_OP(cmd, 0, 0,  32, int,      cfg->mac_id)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_OPEN(cmd, dpmac_id) \
	MC_CMD_OP(cmd, 0, 0,  32, int,	    dpmac_id)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_SET_IRQ(cmd, irq_index, irq_addr, irq_val, user_irq_id) \
do { \
	MC_CMD_OP(cmd, 0, 0,  8,  uint8_t,  irq_index);\
	MC_CMD_OP(cmd, 0, 32, 32, uint32_t, irq_val);\
	MC_CMD_OP(cmd, 1, 0,  64, uint64_t, irq_addr); \
	MC_CMD_OP(cmd, 2, 0,  32, int,	    user_irq_id); \
} while (0)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_GET_IRQ(cmd, irq_index) \
	MC_CMD_OP(cmd, 0, 32, 8,  uint8_t,  irq_index)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_RSP_GET_IRQ(cmd, type, irq_addr, irq_val, user_irq_id) \
do { \
	MC_RSP_OP(cmd, 0, 0,  32, uint32_t, irq_val); \
	MC_RSP_OP(cmd, 1, 0,  64, uint64_t, irq_addr); \
	MC_RSP_OP(cmd, 2, 0,  32, int,	    user_irq_id); \
	MC_RSP_OP(cmd, 2, 32, 32, int,	    type); \
} while (0)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_SET_IRQ_ENABLE(cmd, irq_index, en) \
do { \
	MC_CMD_OP(cmd, 0, 0,  8,  uint8_t,  en); \
	MC_CMD_OP(cmd, 0, 32, 8,  uint8_t,  irq_index);\
} while (0)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_GET_IRQ_ENABLE(cmd, irq_index) \
	MC_CMD_OP(cmd, 0, 32, 8,  uint8_t,  irq_index)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_RSP_GET_IRQ_ENABLE(cmd, en) \
	MC_RSP_OP(cmd, 0, 0,  8,  uint8_t,  en)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_SET_IRQ_MASK(cmd, irq_index, mask) \
do { \
	MC_CMD_OP(cmd, 0, 0,  32, uint32_t, mask);\
	MC_CMD_OP(cmd, 0, 32, 8,  uint8_t,  irq_index);\
} while (0)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_GET_IRQ_MASK(cmd, irq_index) \
	MC_CMD_OP(cmd, 0, 32, 8,  uint8_t,  irq_index)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_RSP_GET_IRQ_MASK(cmd, mask) \
	MC_RSP_OP(cmd, 0, 0,  32, uint32_t, mask)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_GET_IRQ_STATUS(cmd, irq_index) \
	MC_CMD_OP(cmd, 0, 32, 8,  uint8_t,  irq_index)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_RSP_GET_IRQ_STATUS(cmd, status) \
	MC_RSP_OP(cmd, 0, 0,  32, uint32_t, status)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_CLEAR_IRQ_STATUS(cmd, irq_index, status) \
do { \
	MC_CMD_OP(cmd, 0, 0,  32, uint32_t, status); \
	MC_CMD_OP(cmd, 0, 32, 8,  uint8_t,  irq_index);\
} while (0)

/*                cmd, param, offset, width, type,	arg_name */
#define DPMAC_RSP_GET_ATTRIBUTES(cmd, attr) \
do { \
	MC_RSP_OP(cmd, 0, 0,  32, int,			attr->phy_id);\
	MC_RSP_OP(cmd, 0, 32, 32, int,			attr->id);\
	MC_RSP_OP(cmd, 1, 0,  16, uint16_t,		attr->version.major);\
	MC_RSP_OP(cmd, 1, 16, 16, uint16_t,		attr->version.minor);\
	MC_RSP_OP(cmd, 1, 32,  8, enum dpmac_link_type,	attr->link_type);\
	MC_RSP_OP(cmd, 1, 40,  8, enum dpmac_eth_if,	attr->eth_if);\
	MC_RSP_OP(cmd, 2, 0,  64, uint64_t,		attr->max_rate);\
} while (0)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_MDIO_READ(cmd, cfg) \
do { \
	MC_CMD_OP(cmd, 0, 0,  8,  uint8_t,  cfg->phy_addr); \
	MC_CMD_OP(cmd, 0, 8,  8,  uint8_t,  cfg->reg); \
} while (0)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_RSP_MDIO_READ(cmd, data) \
	MC_RSP_OP(cmd, 0, 16, 16, uint16_t, data)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_MDIO_WRITE(cmd, cfg) \
do { \
	MC_CMD_OP(cmd, 0, 0,  8,  uint8_t,  cfg->phy_addr); \
	MC_CMD_OP(cmd, 0, 8,  8,  uint8_t,  cfg->reg); \
	MC_CMD_OP(cmd, 0, 16, 16, uint16_t, cfg->data); \
} while (0)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_RSP_GET_LINK_CFG(cmd, cfg) \
do { \
	MC_RSP_OP(cmd, 0, 0,  64, uint64_t, cfg->options); \
	MC_RSP_OP(cmd, 1, 0,  64, uint64_t, cfg->rate); \
} while (0)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_SET_LINK_STATE(cmd, cfg) \
do { \
	MC_CMD_OP(cmd, 0, 0,  64, uint64_t, cfg->options); \
	MC_CMD_OP(cmd, 1, 0,  64, uint64_t, cfg->rate); \
	MC_CMD_OP(cmd, 2, 0,  1,  int,      cfg->up); \
} while (0)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_CMD_GET_COUNTER(cmd, type) \
	MC_CMD_OP(cmd, 0, 0,  8, enum dpmac_counter, type)

/*                cmd, param, offset, width, type, arg_name */
#define DPMAC_RSP_GET_COUNTER(cmd, counter) \
	MC_RSP_OP(cmd, 1, 0, 64, uint64_t, counter)

#endif /* _FSL_DPMAC_CMD_H */
