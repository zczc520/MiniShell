#include "myerror.h"

void printerr(int errcode)
{
	switch(errcode){
	// success
	case ERR_SUCC:
		printf("Command executed successfully!\n");
		break;
	// overmuch error
	case ERR_OVER_TOKEN:
		printf("Error: Toomuch tokens!\n");
		break;
	case ERR_OVER_OPTION:
		printf("Error: Toomuch options!\n");
		break;
	case ERR_OVER_ARG:
		printf("Error: Toomuch arguments!\n");
		break;
	case ERR_OVER_INPUT:
		printf("Error: Toomuch input source!\n");
		break;
	case ERR_OVER_OUTPUT:
		printf("Error: Toomuch output channel!\n");
		break;
	case ERR_OVER_CMD:
		printf("Error: Toomuch commands!\n");
		break;
	// invalid error
	case ERR_INVALID_PIPE:
		printf("Error: Invalid using of pipe symbol!\n");
		break;
	case ERR_INVALID_OPTION:
		printf("Error: Invalid options!\n");
		break;
	case ERR_INVALID_NAME:
		printf("Error: Invalid command name!\n");
		break;
	case ERR_INVALID_ARG:
		printf("Error: Invalid argument!\n");
		break;
	case ERR_INVALID_INPUT:
		printf("Error: Invalid/Redundant/Lack of input source!\n");
		break;
	case ERR_INVALID_OUTPUT:
		printf("Error: Invalid/Redundant/Lack of output channel!\n");
		break;
	case ERR_INVALID_OUTPUT_TYPE:
		printf("Error: Invalid output type!\n");
		break;
	case ERR_INVALID_JOBID:
		printf("Error: Invalid job id!\n");
		break;
	case ERR_INVALID_EXIT:
		printf("Error: Can't exit now!\n");
		break;
	// C operation error
	case ERR_MEMCPY:
		printf("Error: Memcpy failed!\n");
		break;
	case ERR_STRCPY:
		printf("Error: Strcpy failed!\n");
		break;
	case ERR_CHDIR:
		printf("Error: Chdir failed!\n");
		break;
	// built-in error(errno)
	case ENOENT:
		printf("Error: No such file or directory!\n");
		break;
	// unknown error
	case ERR_UNKNOWN:default:
		printf("Error: Unknown error %d occurs!\n",errcode);
		break;	
	}
}
