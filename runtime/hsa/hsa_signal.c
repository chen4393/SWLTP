/*
 *  Multi2Sim
 *  Copyright (C) 2012  Yifan Sun (yifansun@coe.neu.edu)
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

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "debug.h"
#include "hsa.h"

hsa_status_t HSA_API
    hsa_signal_create(hsa_signal_value_t initial_value, uint32_t num_consumers,
                      const hsa_agent_t *consumers, hsa_signal_t *signal)
{
	// Init arguments
	unsigned int args[8];
	memcpy(args + 1, &initial_value, 8);
	memcpy(args + 3, &num_consumers, 4);
	memcpy(args + 4, &consumers, 8);
	memcpy(args + 6, &signal, 8);

	if (!hsa_runtime)
	{
		return HSA_STATUS_ERROR_NOT_INITIALIZED;
	}
	else
	{
		ioctl(hsa_runtime->fd, SignalCreate, args);
		return (hsa_status_t)args[0];
	}
	return HSA_STATUS_SUCCESS;
}


hsa_status_t HSA_API hsa_signal_destroy(hsa_signal_t signal)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return HSA_STATUS_SUCCESS;
}


hsa_signal_value_t HSA_API hsa_signal_load_acquire(hsa_signal_t signal)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return HSA_STATUS_SUCCESS;
}


hsa_signal_value_t HSA_API hsa_signal_load_relaxed(hsa_signal_t signal)
{
	unsigned long long args[2] = {0};
	memcpy(args + 1, &signal, 8);

	// Call driver function
	ioctl(hsa_runtime->fd, SignalLoadRelaxed, args);

	// Return value
	return args[0];
}


void HSA_API
    hsa_signal_store_relaxed(hsa_signal_t signal, hsa_signal_value_t value)
{
	unsigned int args[4] = {0};
	memcpy(args, &signal, 8);
	memcpy(args + 2, &value, 8);

	// Call driver function
	ioctl(hsa_runtime->fd, SignalStoreRelaxed, args);

	return;
}


void HSA_API
    hsa_signal_store_release(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


hsa_signal_value_t HSA_API
    hsa_signal_exchange_acq_rel(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return 0;
}


hsa_signal_value_t HSA_API
    hsa_signal_exchange_acquire(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return 0;
}


hsa_signal_value_t HSA_API
    hsa_signal_exchange_relaxed(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return 0;
}


hsa_signal_value_t HSA_API
    hsa_signal_exchange_release(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return 0;
}


hsa_signal_value_t HSA_API hsa_signal_cas_acq_rel(hsa_signal_t signal,
                                                  hsa_signal_value_t expected,
                                                  hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return 0;
}


hsa_signal_value_t HSA_API hsa_signal_cas_acquire(hsa_signal_t signal,
                                                  hsa_signal_value_t expected,
                                                  hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return 0;
}


hsa_signal_value_t HSA_API hsa_signal_cas_relaxed(hsa_signal_t signal,
                                                  hsa_signal_value_t expected,
                                                  hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return 0;
}


hsa_signal_value_t HSA_API hsa_signal_cas_release(hsa_signal_t signal,
                                                  hsa_signal_value_t expected,
                                                  hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return 0;
}


void HSA_API
    hsa_signal_add_acq_rel(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_add_acquire(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_add_relaxed(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_add_release(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_subtract_acq_rel(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_subtract_acquire(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_subtract_relaxed(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_subtract_release(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_and_acq_rel(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_and_acquire(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_and_relaxed(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_and_release(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_or_acq_rel(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_or_acquire(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_or_relaxed(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_or_release(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_xor_acq_rel(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_xor_acquire(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_xor_relaxed(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


void HSA_API
    hsa_signal_xor_release(hsa_signal_t signal, hsa_signal_value_t value)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return;
}


hsa_signal_value_t HSA_API
    hsa_signal_wait_acquire(hsa_signal_t signal,
                            hsa_signal_condition_t condition,
                            hsa_signal_value_t compare_value,
                            uint64_t timeout_hint,
                            hsa_wait_state_t wait_state_hint)
{
	unsigned long long signal_value;
	while (1)
	{
		// Get signal value
		signal_value = hsa_signal_load_relaxed(signal);
		printf("Signal value %lld\n", signal_value);

		// Check if condition satisfies
		if (condition == HSA_SIGNAL_CONDITION_EQ)
		{
			if (signal_value == compare_value)
				break;
		}
		else if (condition == HSA_SIGNAL_CONDITION_NE)
		{
			if (signal_value != compare_value)
				break;
		}
		else if (condition == HSA_SIGNAL_CONDITION_LT)
		{
			if (signal_value < compare_value)
				break;
		}
		else if (condition == HSA_SIGNAL_CONDITION_GTE)
		{
			if (signal_value >= compare_value)
				break;
		}
	}
	return 0;
}


hsa_signal_value_t HSA_API
    hsa_signal_wait_relaxed(hsa_signal_t signal,
                            hsa_signal_condition_t condition,
                            hsa_signal_value_t compare_value,
                            uint64_t timeout_hint,
                            hsa_wait_state_t wait_state_hint)
{
	__HSA_RUNTIME_NOT_IMPLEMENTED__
	return 0;
}
