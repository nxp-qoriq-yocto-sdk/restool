/*
 * Copyright (C) 2014 Freescale Semiconductor, Inc.
 * Author: Lijun Pan <Lijun.Pan@freescale.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation  and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of any
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include "restool.h"
#include "utils.h"
#include "fsl_dpaiop.h"

enum mc_cmd_status mc_status;

/**
 * dpaiop info command options
 */
enum dpaiop_info_options {
	INFO_OPT_HELP = 0,
	INFO_OPT_VERBOSE,
};

static struct option dpaiop_info_options[] = {
	[INFO_OPT_HELP] = {
		.name = "help",
		.has_arg = 0,
		.flag = NULL,
		.val = 0,
	},

	[INFO_OPT_VERBOSE] = {
		.name = "verbose",
		.has_arg = 0,
		.flag = NULL,
		.val = 0,
	},

	{ 0 },
};

C_ASSERT(ARRAY_SIZE(dpaiop_info_options) <= MAX_NUM_CMD_LINE_OPTIONS + 1);

/**
 * dpaiop create command options
 */
enum dpaiop_create_options {
	CREATE_OPT_HELP = 0,
	CREATE_OPT_AIOP_ID,
	CREATE_OPT_AIOP_CONTAINER,
};

static struct option dpaiop_create_options[] = {
	[CREATE_OPT_HELP] = {
		.name = "help",
		.has_arg = 0,
		.flag = NULL,
		.val = 0,
	},

	[CREATE_OPT_AIOP_ID] = {
		.name = "aiop-id",
		.has_arg = 1,
		.flag = NULL,
		.val = 0,
	},

	[CREATE_OPT_AIOP_CONTAINER] = {
		.name = "aiop-container-id",
		.has_arg = 1,
		.flag = NULL,
		.val = 0,
	},

	{ 0 },
};

C_ASSERT(ARRAY_SIZE(dpaiop_create_options) <= MAX_NUM_CMD_LINE_OPTIONS + 1);

/**
 * dpaiop destroy command options
 */
enum dpaiop_destroy_options {
	DESTROY_OPT_HELP = 0,
};

static struct option dpaiop_destroy_options[] = {
	[DESTROY_OPT_HELP] = {
		.name = "help",
		.has_arg = 0,
		.flag = NULL,
		.val = 0,
	},

	{ 0 },
};

C_ASSERT(ARRAY_SIZE(dpaiop_destroy_options) <= MAX_NUM_CMD_LINE_OPTIONS + 1);

static const struct flib_ops dpaiop_ops = {
	.obj_open = dpaiop_open,
	.obj_close = dpaiop_close,
	.obj_get_irq_mask = dpaiop_get_irq_mask,
	.obj_get_irq_status = dpaiop_get_irq_status,
};

static int cmd_dpaiop_help(void)
{
	static const char help_msg[] =
		"\n"
		"restool dpaiop <command> [--help] [ARGS...]\n"
		"Where <command> can be:\n"
		"   info - displays detailed information about a DPAIOP object.\n"
		"   create - creates a new child DPAIOP under the root DPRC.\n"
		"   destroy - destroys a child DPAIOP under the root DPRC.\n"
		"\n"
		"For command-specific help, use the --help option of each command.\n"
		"\n";

	printf(help_msg);
	return 0;
}

static void print_dpaiop_state(uint32_t state)
{
	printf("DPAIOP state: ");
	switch (state) {
	case DPAIOP_STATE_RESET_DONE:
		printf("DPAIOP_STATE_RESET_DONE\n");
		break;
	case DPAIOP_STATE_RESET_ONGOING:
		printf("DPAIOP_STATE_RESET_ONGOING\n");
		break;
	case DPAIOP_STATE_LOAD_DONE:
		printf("DPAIOP_STATE_LOAD_DONE\n");
		break;
	case DPAIOP_STATE_LOAD_ONGIONG:
		printf("DPAIOP_STATE_LOAD_ONGIONG\n");
		break;
	case DPAIOP_STATE_LOAD_ERROR:
		printf("DPAIOP_STATE_LOAD_ERROR\n");
		break;
	case DPAIOP_STATE_BOOT_ONGOING:
		printf("DPAIOP_STATE_BOOT_ONGOING\n");
		break;
	case DPAIOP_STATE_BOOT_ERROR:
		printf("DPAIOP_STATE_BOOT_ERROR\n");
		break;
	case DPAIOP_STATE_RUNNING:
		printf("DPAIOP_STATE_RUNNING\n");
		break;
	default:
		assert(false);
		break;
	}
}

static int print_dpaiop_attr(uint32_t dpaiop_id,
			struct dprc_obj_desc *target_obj_desc)
{
	uint16_t dpaiop_handle;
	int error;
	struct dpaiop_attr dpaiop_attr;
	struct dpaiop_sl_version dpaiop_sl_version;
	uint32_t state;
	bool dpaiop_opened = false;

	error = dpaiop_open(&restool.mc_io, 0, dpaiop_id, &dpaiop_handle);
	if (error < 0) {
		mc_status = flib_error_to_mc_status(error);
		ERROR_PRINTF("MC error: %s (status %#x)\n",
			     mc_status_to_string(mc_status), mc_status);
		goto out;
	}
	dpaiop_opened = true;
	if (0 == dpaiop_handle) {
		DEBUG_PRINTF(
			"dpaiop_open() returned invalid handle (auth 0) for dpaiop.%u\n",
			dpaiop_id);
		error = -ENOENT;
		goto out;
	}

	memset(&dpaiop_attr, 0, sizeof(dpaiop_attr));
	error = dpaiop_get_attributes(&restool.mc_io, 0, dpaiop_handle,
					&dpaiop_attr);
	if (error < 0) {
		mc_status = flib_error_to_mc_status(error);
		ERROR_PRINTF("MC error: %s (status %#x)\n",
			     mc_status_to_string(mc_status), mc_status);
		goto out;
	}
	assert(dpaiop_id == (uint32_t)dpaiop_attr.id);

	printf("dpaiop version: %u.%u\n", dpaiop_attr.version.major,
	       dpaiop_attr.version.minor);
	printf("dpaiop id: %d\n", dpaiop_attr.id);
	printf("plugged state: %splugged\n",
		(target_obj_desc->state & DPRC_OBJ_STATE_PLUGGED) ? "" : "un");

	memset(&dpaiop_sl_version, 0, sizeof(dpaiop_sl_version));
	error = dpaiop_get_sl_version(&restool.mc_io, 0, dpaiop_handle,
					&dpaiop_sl_version);
	if (error < 0) {
		mc_status = flib_error_to_mc_status(error);
		ERROR_PRINTF("MC error: %s (status %#x)\n",
			     mc_status_to_string(mc_status), mc_status);
		goto out;
	}
	printf("dpaiop server layer version: %u.%u.%u\n",
		dpaiop_sl_version.major,
		dpaiop_sl_version.minor,
		dpaiop_sl_version.revision);

	error = dpaiop_get_state(&restool.mc_io, 0, dpaiop_handle, &state);
	if (error < 0) {
		mc_status = flib_error_to_mc_status(error);
		ERROR_PRINTF("MC error: %s (status %#x)\n",
			     mc_status_to_string(mc_status), mc_status);
		goto out;
	}
	print_dpaiop_state(state);
	print_obj_label(target_obj_desc);

	error = 0;

out:
	if (dpaiop_opened) {
		int error2;

		error2 = dpaiop_close(&restool.mc_io, 0, dpaiop_handle);
		if (error2 < 0) {
			mc_status = flib_error_to_mc_status(error2);
			ERROR_PRINTF("MC error: %s (status %#x)\n",
				     mc_status_to_string(mc_status), mc_status);
			if (error == 0)
				error = error2;
		}
	}

	return error;
}

static int print_dpaiop_info(uint32_t dpaiop_id)
{
	int error;
	struct dprc_obj_desc target_obj_desc;
	uint32_t target_parent_dprc_id;
	bool found = false;

	memset(&target_obj_desc, 0, sizeof(struct dprc_obj_desc));
	error = find_target_obj_desc(restool.root_dprc_id,
				restool.root_dprc_handle, 0, dpaiop_id,
				"dpaiop", &target_obj_desc,
				&target_parent_dprc_id, &found);
	if (error < 0)
		goto out;

	if (strcmp(target_obj_desc.type, "dpaiop")) {
		printf("dpaiop.%d does not exist\n", dpaiop_id);
		return -EINVAL;
	}

	error = print_dpaiop_attr(dpaiop_id, &target_obj_desc);
	if (error < 0)
		goto out;

	if (restool.cmd_option_mask & ONE_BIT_MASK(INFO_OPT_VERBOSE)) {
		restool.cmd_option_mask &= ~ONE_BIT_MASK(INFO_OPT_VERBOSE);
		error = print_obj_verbose(&target_obj_desc, &dpaiop_ops);
	}

out:
	return error;
}

static int cmd_dpaiop_info(void)
{
	static const char usage_msg[] =
	"\n"
	"Usage: restool dpaiop info <dpaiop-object> [--verbose]\n"
	"   e.g. restool dpaiop info dpaiop.5\n"
	"\n"
	"--verbose\n"
	"   Shows extended/verbose information about the object\n"
	"   e.g. restool dpaiop info dpaiop.5 --verbose\n"
	"\n";

	uint32_t obj_id;
	int error;

	if (restool.cmd_option_mask & ONE_BIT_MASK(INFO_OPT_HELP)) {
		printf(usage_msg);
		restool.cmd_option_mask &= ~ONE_BIT_MASK(INFO_OPT_HELP);
		error = 0;
		goto out;
	}

	if (restool.obj_name == NULL) {
		ERROR_PRINTF("<object> argument missing\n");
		printf(usage_msg);
		error = -EINVAL;
		goto out;
	}

	error = parse_object_name(restool.obj_name, "dpaiop", &obj_id);
	if (error < 0)
		goto out;

	error = print_dpaiop_info(obj_id);
out:
	return error;
}

static int cmd_dpaiop_create(void)
{
	static const char usage_msg[] =
		"\n"
		"Usage: restool dpaiop create --aiop-id=<number> --aiop-container=<container-name>\n"
		"\n"
		"--aiop-container=<container-name>\n"
		"   Specifies the AIOP container name, e.g. dprc.3, dprc.4, etc.\n"
		"\n"
		"OPTIONS:\n"
		"--aiop-id=<number>\n"
		"Specifies the AIOP ID. Currently aiop container could only hold one dpaiop.\n"
		"Valid number is 0. Default number is 0.\n"
		"\n"
		"e.g. create a DPAIOP\n"
		"   restool dpaiop create --aiop-id=0 --aiop-container=dprc.3\n"
		"   restool dpaiop create --aiop-container=dprc.3\n"
		"\n";

	int error;
	long val;
	char *endptr;
	char *str;
	struct dpaiop_cfg dpaiop_cfg;
	uint16_t dpaiop_handle;
	struct dpaiop_attr dpaiop_attr;
	uint32_t obj_id;

	if (restool.cmd_option_mask & ONE_BIT_MASK(CREATE_OPT_HELP)) {
		printf(usage_msg);
		restool.cmd_option_mask &= ~ONE_BIT_MASK(CREATE_OPT_HELP);
		return 0;
	}

	if (restool.obj_name != NULL) {
		ERROR_PRINTF("Unexpected argument: \'%s\'\n\n",
			     restool.obj_name);
		printf(usage_msg);
		return -EINVAL;
	}

	if (restool.cmd_option_mask & ONE_BIT_MASK(CREATE_OPT_AIOP_ID)) {
		restool.cmd_option_mask &=
			~ONE_BIT_MASK(CREATE_OPT_AIOP_ID);
		errno = 0;
		str = restool.cmd_option_args[CREATE_OPT_AIOP_ID];
		val = strtol(str, &endptr, 0);

		if (STRTOL_ERROR(str, endptr, val, errno) ||
		    (val != 0)) {
			printf(usage_msg);
			return -EINVAL;
		}

		dpaiop_cfg.aiop_id = val;
	} else {
		dpaiop_cfg.aiop_id = 0;
	}

	if (restool.cmd_option_mask &
	    ONE_BIT_MASK(CREATE_OPT_AIOP_CONTAINER)) {
		restool.cmd_option_mask &=
			~ONE_BIT_MASK(CREATE_OPT_AIOP_CONTAINER);
		error = parse_object_name(
			restool.cmd_option_args[CREATE_OPT_AIOP_CONTAINER],
			"dprc", &obj_id);
		if (error < 0) {
			printf(usage_msg);
			return error;
		}

		dpaiop_cfg.aiop_container_id = obj_id;
	} else {
		ERROR_PRINTF("--aiop-container option missing\n");
		printf(usage_msg);
		return -EINVAL;
	}

	error = dpaiop_create(&restool.mc_io, 0, &dpaiop_cfg, &dpaiop_handle);
	if (error < 0) {
		mc_status = flib_error_to_mc_status(error);
		ERROR_PRINTF("MC error: %s (status %#x)\n",
			     mc_status_to_string(mc_status), mc_status);
		return error;
	}

	memset(&dpaiop_attr, 0, sizeof(struct dpaiop_attr));
	error = dpaiop_get_attributes(&restool.mc_io, 0, dpaiop_handle,
					&dpaiop_attr);
	if (error < 0) {
		mc_status = flib_error_to_mc_status(error);
		ERROR_PRINTF("MC error: %s (status %#x)\n",
			     mc_status_to_string(mc_status), mc_status);
		return error;
	}
	print_new_obj("dpaiop", dpaiop_attr.id,
		restool.cmd_option_args[CREATE_OPT_AIOP_CONTAINER]);

	error = dpaiop_close(&restool.mc_io, 0, dpaiop_handle);
	if (error < 0) {
		mc_status = flib_error_to_mc_status(error);
		ERROR_PRINTF("MC error: %s (status %#x)\n",
			     mc_status_to_string(mc_status), mc_status);
		return error;
	}

	return 0;
}

static int cmd_dpaiop_destroy(void)
{
	static const char usage_msg[] =
		"\n"
		"Usage: restool dpaiop destroy <dpaiop-object>\n"
		"   e.g. restool dpaiop destroy dpaiop.9\n"
		"\n";

	int error;
	int error2;
	uint32_t dpaiop_id;
	uint16_t dpaiop_handle;
	bool dpaiop_opened = false;

	if (restool.cmd_option_mask & ONE_BIT_MASK(DESTROY_OPT_HELP)) {
		printf(usage_msg);
		restool.cmd_option_mask &= ~ONE_BIT_MASK(DESTROY_OPT_HELP);
		return 0;
	}

	if (restool.obj_name == NULL) {
		ERROR_PRINTF("<object> argument missing\n");
		printf(usage_msg);
		error = -EINVAL;
		goto out;
	}

	if (in_use(restool.obj_name, "destroyed")) {
		error = -EBUSY;
		goto out;
	}

	error = parse_object_name(restool.obj_name, "dpaiop", &dpaiop_id);
	if (error < 0)
		goto out;

	if (!find_obj("dpaiop", dpaiop_id)) {
		error = -EINVAL;
		goto out;
	}

	error = dpaiop_open(&restool.mc_io, 0, dpaiop_id, &dpaiop_handle);
	if (error < 0) {
		mc_status = flib_error_to_mc_status(error);
		ERROR_PRINTF("MC error: %s (status %#x)\n",
			     mc_status_to_string(mc_status), mc_status);
		goto out;
	}
	dpaiop_opened = true;
	if (0 == dpaiop_handle) {
		DEBUG_PRINTF(
			"dpaiop_open() returned invalid handle (auth 0) for dpaiop.%u\n",
			dpaiop_id);
		error = -ENOENT;
		goto out;
	}

	error = dpaiop_destroy(&restool.mc_io, 0, dpaiop_handle);
	if (error < 0) {
		mc_status = flib_error_to_mc_status(error);
		ERROR_PRINTF("MC error: %s (status %#x)\n",
			     mc_status_to_string(mc_status), mc_status);
		goto out;
	}
	dpaiop_opened = false;
	printf("dpaiop.%u is destroyed\n", dpaiop_id);

out:
	if (dpaiop_opened) {
		error2 = dpaiop_close(&restool.mc_io, 0, dpaiop_handle);
		if (error2 < 0) {
			mc_status = flib_error_to_mc_status(error2);
			ERROR_PRINTF("MC error: %s (status %#x)\n",
				     mc_status_to_string(mc_status), mc_status);
			if (error == 0)
				error = error2;
		}
	}

	return error;
}

struct object_command dpaiop_commands[] = {
	{ .cmd_name = "help",
	  .options = NULL,
	  .cmd_func = cmd_dpaiop_help },

	{ .cmd_name = "info",
	  .options = dpaiop_info_options,
	  .cmd_func = cmd_dpaiop_info },

	{ .cmd_name = "create",
	  .options = dpaiop_create_options,
	  .cmd_func = cmd_dpaiop_create },

	{ .cmd_name = "destroy",
	  .options = dpaiop_destroy_options,
	  .cmd_func = cmd_dpaiop_destroy },

	{ .cmd_name = NULL },
};

