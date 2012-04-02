/*
 *  Multi2Sim Tools
 *  Copyright (C) 2011  Rafael Ubal (ubal@ece.neu.edu)
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <visual-private.h>


/*
 * Trace Line Processing Functions
 */


static void visual_mem_system_set_transient_tag(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_set_block(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
	struct visual_mod_t *mod;

	char *mod_name;
	char *state;

	int set;
	int way;

	unsigned int tag;

	/* Get fields */
	mod_name = trace_line_get_symbol_value(trace_line, "cache");
	set = trace_line_get_symbol_value_int(trace_line, "set");
	way = trace_line_get_symbol_value_int(trace_line, "way");
	tag = trace_line_get_symbol_value_hex(trace_line, "tag");
	state = trace_line_get_symbol_value(trace_line, "state");

	/* Get module */
	mod = hash_table_get(visual_mem_system->mod_table, mod_name);
	if (!mod)
		panic("%s: invalid module name '%s'", __FUNCTION__, mod_name);

	/* Set block */
	visual_mod_set_block(mod, set, way, tag, state);
}


static void visual_mem_system_set_sharer(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_clear_sharer(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_clear_all_sharers(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_set_owner(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_new_access(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_end_access(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_new_access_mod(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_end_access_mod(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_new_access_block(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_end_access_block(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_access(struct visual_mem_system_t *system,
	struct trace_line_t *trace_line)
{
}


static void visual_mem_system_read_checkpoint(struct visual_mem_system_t *system, FILE *f)
{
}


static void visual_mem_system_write_checkpoint(struct visual_mem_system_t *system, FILE *f)
{
}



/*
 * Public Functions
 */


struct visual_mem_system_t *visual_mem_system;


void visual_mem_system_init(void)
{
	struct trace_line_t *trace_line;

	/* State file */
	state_file_new_category(visual_state_file, "Memory hierarchy",
		(state_file_read_checkpoint_func_t) visual_mem_system_read_checkpoint,
		(state_file_write_checkpoint_func_t) visual_mem_system_write_checkpoint,
		NULL, visual_mem_system);
	/* Replace with (state_file_refresh_func_t) visual_mem_system_widget_refresh */

	state_file_new_command(visual_state_file, "mem.set_transient_tag",
		(state_file_process_trace_line_func_t) visual_mem_system_set_transient_tag,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.set_block",
		(state_file_process_trace_line_func_t) visual_mem_system_set_block,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.set_sharer",
		(state_file_process_trace_line_func_t) visual_mem_system_set_sharer,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.clear_sharer",
		(state_file_process_trace_line_func_t) visual_mem_system_clear_sharer,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.clear_all_sharers",
		(state_file_process_trace_line_func_t) visual_mem_system_clear_all_sharers,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.set_owner",
		(state_file_process_trace_line_func_t) visual_mem_system_set_owner,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.new_access",
		(state_file_process_trace_line_func_t) visual_mem_system_new_access,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.end_access",
		(state_file_process_trace_line_func_t) visual_mem_system_end_access,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.new_access_mod",
		(state_file_process_trace_line_func_t) visual_mem_system_new_access_mod,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.end_access_mod",
		(state_file_process_trace_line_func_t) visual_mem_system_end_access_mod,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.new_access_block",
		(state_file_process_trace_line_func_t) visual_mem_system_new_access_block,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.end_access_block",
		(state_file_process_trace_line_func_t) visual_mem_system_end_access_block,
		visual_mem_system);
	state_file_new_command(visual_state_file, "mem.access",
		(state_file_process_trace_line_func_t) visual_mem_system_access,
		visual_mem_system);

	/* Allocate */
	visual_mem_system = calloc(1, sizeof(struct visual_mem_system_t));
	if (!visual_mem_system)
		fatal("%s: out of memory", __FUNCTION__);

	/* Initialize */
	visual_mem_system->mod_table = hash_table_create(0, FALSE);
	visual_mem_system->net_table = hash_table_create(0, FALSE);

	/* Parse header in state file */
	STATE_FILE_FOR_EACH_HEADER(visual_state_file, trace_line)
	{
		char *command;

		/* Get command */
		command = trace_line_get_command(trace_line);
		assert(strcmp(command, "c"));

		if (!strcmp(command, "mem.new_mod"))
		{
			struct visual_mod_t *mod;

			mod = visual_mod_create(trace_line);
			hash_table_insert(visual_mem_system->mod_table, mod->name, mod);
		}
		else if (!strcmp(command, "mem.new_net"))
		{
			struct visual_net_t *net;

			net = visual_net_create(trace_line);
			hash_table_insert(visual_mem_system->net_table, net->name, net);
		}
	}
}


void visual_mem_system_done(void)
{
	struct visual_mod_t *mod;
	struct visual_net_t *net;

	char *mod_name;
	char *net_name;

	/* Free modules */
	HASH_TABLE_FOR_EACH(visual_mem_system->mod_table, mod_name, mod)
		visual_mod_free(mod);
	hash_table_free(visual_mem_system->mod_table);

	/* Free networks */
	HASH_TABLE_FOR_EACH(visual_mem_system->net_table, net_name, net)
		visual_net_free(net);
	hash_table_free(visual_mem_system->net_table);

	/* Rest */
	free(visual_mem_system);
}
