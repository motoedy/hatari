/* 
 * Code to test Hatari symbol/address (re-)loading in src/debug/symbols.c
 */
#include <stdio.h>
#include <SDL_types.h>
#include <stdbool.h>
#include "debug_priv.h"
#include "symbols.h"
#include "main.h"
#include "log.h"

/* dummy variable for debugcpu.c */
Uint32 LogTraceFlags = TRACE_CPU_DISASM;

/* dummy symbols.c dep function */
void DebugUI_PrintCmdHelp(const char *cmd)
{
	fprintf(stderr, "Help!\n");
}

int main(int argc, const char *argv[])
{
	/* expected to fail */
	const char *fail_name[] = {
		"afoo",
		"zbar",
	};
	Uint32 fail_addr[] = {
		0x10,
		0x30,
	};
	/* expected to succeed */
	const char *success_name[] = {
		"_supexec",
		"_dos_mkdir",
		"_shel_find"
	};
	Uint32 success_addr[] = {
		0xe00dc6,
		0xe324d2,
	};

#define DO_CMD(cmd) Symbols_Command(ARRAYSIZE(cmd), cmd)
	char symbols[] = "symbols";
	char fname[] = "etos512.sym";
	char sname[] = "name";
	char saddr[] = "addr";
	char *cmd_load[] = { symbols, fname };
	char *cmd_show_byname[] = { symbols, sname };
	char *cmd_show_byaddr[] = { symbols, saddr };

	int i, tests = 0, errors = 0;
	const char *name;
	Uint32 addr;

	DO_CMD(cmd_load);
	DO_CMD(cmd_show_byaddr);
	fprintf(stderr, "\n");
	DO_CMD(cmd_show_byname);
	DO_CMD(cmd_load);	/* free + reload */

	fprintf(stderr, "\nStuff that should FAIL:\n");
	for (i = 0; i < ARRAYSIZE(fail_name); i++) {
		name = fail_name[i];
		if (Symbols_GetCpuAddress(SYMTYPE_ALL, name, &addr)) {
			fprintf(stderr, "*** Unexpected SUCCESS from '%s' (0x%08x) ***\n", name, addr);
			errors++;
		} else {
			fprintf(stderr, "- '%s'\n", name);
		}
	}
	tests += i;
	for (i = 0; i < ARRAYSIZE(fail_addr); i++) {
		addr = fail_addr[i];
		name = Symbols_GetByCpuAddress(addr);
		if (name) {
			fprintf(stderr, "*** Unexpected SUCCESS from 0x%08x (%s) ***\n", addr, name);
			errors++;
		} else {
			fprintf(stderr, "- 0x%08x\n", addr);
		}
	}
	tests += i;

	fprintf(stderr, "\nStuff that should SUCCEED:\n");
	for (i = 0; i < ARRAYSIZE(success_name); i++) {
		name = success_name[i];
		if (Symbols_GetCpuAddress(SYMTYPE_ALL, name, &addr)) {
			fprintf(stderr, "- '%s'\n", name);
		} else {
			fprintf(stderr, "*** Unexpected FAIL from '%s' ***\n", name);
			errors++;
		}
	}
	tests += i;
	for (i = 0; i < ARRAYSIZE(success_addr); i++) {
		addr = success_addr[i];
		name = Symbols_GetByCpuAddress(addr);
		if (name) {
			fprintf(stderr, "- 0x%08x: %s\n", addr, name);
		} else {
			fprintf(stderr, "*** Unexpected FAIL from 0x%08x ***\n", addr);
			errors++;
		}
	}
	tests += i;
	
	if (errors) {
		fprintf(stderr, "\n***Detected %d ERRORs in %d automated tests!***\n\n",
			errors, tests);
	} else {
		fprintf(stderr, "\nFinished without any errors!\n\n");
	}
	return errors;
}