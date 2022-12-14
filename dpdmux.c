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
#include "fsl_mc_sys.h"
#include "fsl_mc_cmd.h"
#include "fsl_dpdmux.h"
#include "fsl_dpdmux_cmd.h"

int dpdmux_open(struct fsl_mc_io *mc_io,
		uint32_t cmd_flags,
		int dpdmux_id,
		uint16_t *token)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_OPEN,
					  cmd_flags,
					  0);
	DPDMUX_CMD_OPEN(cmd, dpdmux_id);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	*token = MC_CMD_HDR_READ_TOKEN(cmd.header);

	return 0;
}

int dpdmux_close(struct fsl_mc_io *mc_io,
		 uint32_t cmd_flags,
		 uint16_t token)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_CLOSE,
					  cmd_flags,
					  token);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_create(struct fsl_mc_io *mc_io,
		  uint32_t cmd_flags,
		  const struct dpdmux_cfg *cfg,
		  uint16_t *token)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_CREATE,
					  cmd_flags,
					  0);
	DPDMUX_CMD_CREATE(cmd, cfg);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	*token = MC_CMD_HDR_READ_TOKEN(cmd.header);

	return 0;
}

int dpdmux_destroy(struct fsl_mc_io *mc_io,
		   uint32_t cmd_flags,
		   uint16_t token)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_DESTROY,
					  cmd_flags,
					  token);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_enable(struct fsl_mc_io *mc_io,
		  uint32_t cmd_flags,
		  uint16_t token)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_ENABLE,
					  cmd_flags,
					  token);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_disable(struct fsl_mc_io *mc_io,
		   uint32_t cmd_flags,
		   uint16_t token)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_DISABLE,
					  cmd_flags,
					  token);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_is_enabled(struct fsl_mc_io *mc_io,
		      uint32_t cmd_flags,
		      uint16_t token,
		      int *en)
{
	struct mc_command cmd = { 0 };
	int err;
	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_IS_ENABLED,
					  cmd_flags,
					  token);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_IS_ENABLED(cmd, *en);

	return 0;
}

int dpdmux_reset(struct fsl_mc_io *mc_io,
		 uint32_t cmd_flags,
		 uint16_t token)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_RESET,
					  cmd_flags,
					  token);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_set_irq(struct fsl_mc_io	*mc_io,
		   uint32_t		cmd_flags,
		   uint16_t		token,
		   uint8_t		irq_index,
		   struct dpdmux_irq_cfg *irq_cfg)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_SET_IRQ,
					  cmd_flags,
					  token);
	DPDMUX_CMD_SET_IRQ(cmd, irq_index, irq_cfg);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_get_irq(struct fsl_mc_io *mc_io,
		   uint32_t cmd_flags,
		   uint16_t		token,
		   uint8_t		irq_index,
		   int			*type,
		   struct dpdmux_irq_cfg *irq_cfg)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_GET_IRQ,
					  cmd_flags,
					  token);
	DPDMUX_CMD_GET_IRQ(cmd, irq_index);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_GET_IRQ(cmd, *type, irq_cfg);

	return 0;
}

int dpdmux_set_irq_enable(struct fsl_mc_io *mc_io,
			  uint32_t cmd_flags,
			  uint16_t token,
			  uint8_t irq_index,
			  uint8_t en)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_SET_IRQ_ENABLE,
					  cmd_flags,
					  token);
	DPDMUX_CMD_SET_IRQ_ENABLE(cmd, irq_index, en);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_get_irq_enable(struct fsl_mc_io *mc_io,
			  uint32_t cmd_flags,
			  uint16_t token,
			  uint8_t irq_index,
			  uint8_t *en)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_GET_IRQ_ENABLE,
					  cmd_flags,
					  token);
	DPDMUX_CMD_GET_IRQ_ENABLE(cmd, irq_index);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_GET_IRQ_ENABLE(cmd, *en);

	return 0;
}

int dpdmux_set_irq_mask(struct fsl_mc_io *mc_io,
			uint32_t cmd_flags,
			uint16_t token,
			uint8_t irq_index,
			uint32_t mask)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_SET_IRQ_MASK,
					  cmd_flags,
					  token);
	DPDMUX_CMD_SET_IRQ_MASK(cmd, irq_index, mask);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_get_irq_mask(struct fsl_mc_io *mc_io,
			uint32_t cmd_flags,
			uint16_t token,
			uint8_t irq_index,
			uint32_t *mask)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_GET_IRQ_MASK,
					  cmd_flags,
					  token);
	DPDMUX_CMD_GET_IRQ_MASK(cmd, irq_index);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_GET_IRQ_MASK(cmd, *mask);

	return 0;
}

int dpdmux_get_irq_status(struct fsl_mc_io *mc_io,
			  uint32_t cmd_flags,
			  uint16_t token,
			  uint8_t irq_index,
			  uint32_t *status)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_GET_IRQ_STATUS,
					  cmd_flags,
					  token);
	DPDMUX_CMD_GET_IRQ_STATUS(cmd, irq_index);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_GET_IRQ_STATUS(cmd, *status);

	return 0;
}

int dpdmux_clear_irq_status(struct fsl_mc_io *mc_io,
			    uint32_t cmd_flags,
			    uint16_t token,
			    uint8_t irq_index,
			    uint32_t status)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_CLEAR_IRQ_STATUS,
					  cmd_flags,
					  token);
	DPDMUX_CMD_CLEAR_IRQ_STATUS(cmd, irq_index, status);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_get_attributes(struct fsl_mc_io *mc_io,
			  uint32_t cmd_flags,
			  uint16_t token,
			  struct dpdmux_attr *attr)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_GET_ATTR,
					  cmd_flags,
					  token);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_GET_ATTR(cmd, attr);

	return 0;
}

int dpdmux_ul_set_max_frame_length(struct fsl_mc_io *mc_io,
				   uint32_t cmd_flags,
				   uint16_t token,
				   uint16_t max_frame_length)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_UL_SET_MAX_FRAME_LENGTH,
					  cmd_flags,
					  token);
	DPDMUX_CMD_UL_SET_MAX_FRAME_LENGTH(cmd, max_frame_length);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_set_default_if(struct fsl_mc_io *mc_io,
			  uint32_t cmd_flags,
			  uint16_t token,
			  uint16_t if_id,
			  int no_default_if)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_SET_DEFAULT_IF,
					  cmd_flags,
					  token);
	DPDMUX_CMD_SET_DEFAULT_IF(cmd, if_id, no_default_if);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_get_default_if(struct fsl_mc_io *mc_io,
			  uint32_t cmd_flags,
			  uint16_t token,
			  uint16_t *if_id)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_GET_DEFAULT_IF,
						  cmd_flags,
						  token);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_GET_DEFAULT_IF(cmd, *if_id);

	return 0;
}

int dpdmux_ul_reset_counters(struct fsl_mc_io *mc_io,
			     uint32_t cmd_flags,
			     uint16_t token)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_UL_RESET_COUNTERS,
					  cmd_flags,
					  token);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_if_set_accepted_frames(struct fsl_mc_io *mc_io,
				  uint32_t cmd_flags,
				  uint16_t token,
				  uint16_t if_id,
				  const struct dpdmux_accepted_frames *cfg)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_IF_SET_ACCEPTED_FRAMES,
					  cmd_flags,
					  token);
	DPDMUX_CMD_IF_SET_ACCEPTED_FRAMES(cmd, if_id, cfg);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_if_get_attributes(struct fsl_mc_io *mc_io,
			     uint32_t cmd_flags,
			     uint16_t token,
			     uint16_t if_id,
			     struct dpdmux_if_attr *attr)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_IF_GET_ATTR,
					  cmd_flags,
					  token);
	DPDMUX_CMD_IF_GET_ATTR(cmd, if_id);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_IF_GET_ATTR(cmd, attr);

	return 0;
}

int dpdmux_if_remove_l2_rule(struct fsl_mc_io *mc_io,
			     uint32_t cmd_flags,
			     uint16_t token,
			     uint16_t if_id,
			     const struct dpdmux_l2_rule *rule)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_IF_REMOVE_L2_RULE,
					  cmd_flags,
					  token);
	DPDMUX_CMD_IF_REMOVE_L2_RULE(cmd, if_id, rule);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_if_add_l2_rule(struct fsl_mc_io *mc_io,
			  uint32_t cmd_flags,
			  uint16_t token,
			  uint16_t if_id,
			  const struct dpdmux_l2_rule *rule)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_IF_ADD_L2_RULE,
					  cmd_flags,
					  token);
	DPDMUX_CMD_IF_ADD_L2_RULE(cmd, if_id, rule);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_if_get_counter(struct fsl_mc_io		*mc_io,
			  uint32_t			cmd_flags,
			  uint16_t			token,
			  uint16_t			if_id,
			  enum dpdmux_counter_type	counter_type,
			  uint64_t			*counter)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_IF_GET_COUNTER,
					  cmd_flags,
					  token);
	DPDMUX_CMD_IF_GET_COUNTER(cmd, if_id, counter_type);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_IF_GET_COUNTER(cmd, *counter);

	return 0;
}

int dpdmux_if_set_link_cfg(struct fsl_mc_io *mc_io,
			   uint32_t cmd_flags,
			   uint16_t token,
			   uint16_t if_id,
			   struct dpdmux_link_cfg *cfg)
{
	struct mc_command cmd = { 0 };

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_IF_SET_LINK_CFG,
					  cmd_flags,
					  token);
	DPDMUX_CMD_IF_SET_LINK_CFG(cmd, if_id, cfg);

	/* send command to mc*/
	return mc_send_command(mc_io, &cmd);
}

int dpdmux_if_get_link_state(struct fsl_mc_io *mc_io,
			     uint32_t cmd_flags,
			     uint16_t token,
			     uint16_t if_id,
			     struct dpdmux_link_state *state)
{
	struct mc_command cmd = { 0 };
	int err;

	/* prepare command */
	cmd.header = mc_encode_cmd_header(DPDMUX_CMDID_IF_GET_LINK_STATE,
					  cmd_flags,
					  token);
	DPDMUX_CMD_IF_GET_LINK_STATE(cmd, if_id);

	/* send command to mc*/
	err = mc_send_command(mc_io, &cmd);
	if (err)
		return err;

	/* retrieve response parameters */
	DPDMUX_RSP_IF_GET_LINK_STATE(cmd, state);

	return 0;
}
