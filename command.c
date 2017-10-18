#include "command.h"
#include <stdlib.h>

int execls(const struct Cmd* command)
{
	// TODO
	return ERR_SUCC;
}

int execexit(const struct Cmd* command)
{
	exit(0);
}

int execcd(const struct Cmd* command)
{
	if(command->argNum!=1) return ERR_UNKNOWN; // it should be promise when checkcmds in process.c
	if(chdir(command->args[0])==-1) return ERR_CHDIR; // chdir error
	return ERR_SUCC;	
}
