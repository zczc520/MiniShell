#include "error.h"

void printerr(int errcode)
{
	switch errcode{
	case EXEC_SUCCESS:
		printf("Command executed successfully!\n");
		break;
	case INVALID_CMD:
		printf("Error: Invalid command!\n");
		break;
	}
}
