#include "process.h"

static void rmenter(char* inputStr)
{
	for(int i = 0;i < MAX_LEN;i++){
		if(inputStr[i]=='\n') inputStr[i]='\0';
	}
}

int main(int argc,char **argv)
{
	// ignore the SIGINT and SIGTSTP signal in minishell
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);

	// save input string
	char inputStr[MAX_LEN];
	// save cwd
	char cwd[MAX_LEN];

	// wait for command
	while(1){
		// print prompt
		if(getcwd(cwd,MAX_LEN)){
			printf("MiniShell:%s$ ",cwd);
		}
		else{
			printf("Get current work directory failed!\n");
			exit(-1);
		}

		// get input string
		if(fgets(inputStr,MAX_LEN,stdin)!=NULL){
			// remove \n
			rmenter(inputStr);

			// execute command
			int errcode = process(inputStr);

			// print error message
			printerr(errcode);
		}
		else{
			printf("Input string is null! MiniShell is terminating...\n");
			exit(-1);
		}
	}
}
