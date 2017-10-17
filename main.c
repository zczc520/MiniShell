#include "command.h"
#include "error.h"

int main(int argc,char **argv)
{
	// save input string
	char inputStr[MAX_LEN];
	// save cwd
	char cwd[MAX_LEN];

	// wait for command
	while(true){
		// print prompt
		if(getcwd(cwd,MAX_LEN)){
			printf("MiniShell %s:",cwd);
		}
		else{
			printf("Get current work directory failed!\n");
			exit(-1);
		}

		// get input string
		if(gets(inputStr)!=NULL){
			// execute command
			int errcode = execmd(inputStr);

			// print error message
			printerr(errcode);
		}
		else{
			printf("Input string is null! MiniShell is terminating...\n");
			exit(-1);
		}
	}
}
