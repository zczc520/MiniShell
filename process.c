#include "process.h"

/*
 * gettokens: get tokens from str
 *
 * @str: input string from standard input(console)
 * @tokens: 2-dimensional array to save the tokens from str
 * @tokenNum: num of tokens
 *
 * Parse the input string, and save the tokens into 2nd param, save the
 * num of tokens into 3rd param.
 */
static int gettokens(const char* str, char** tokens, int* tokenNum)
{
	tokenNum = 0; // num of tokens
	char tempstr[MAX_LEN]; // a copy string for str
	if(strcpy(tempstr,str)==NULL) return ERR_STRCPY; 
	char* ptr = NULL;
	if(ptr = strtok(tempstr," ")){
		if(strcpy(tokens[tokenNum++],ptr)==NULL) return ERR_STRCPY;
		while(ptr = strtok(NULL," ")){
			if(tokenNum>=MAX_TOKEN) return ERR_OVER_TOKEN; // too many tokens
			else{
				if(strcpy(tokens[tokenNum++],ptr)==NULL) return ERR_STRCPY;
			}
		}
	}
	return ERR_SUCC;	
}

/*
 * initcmd: init command
 *
 * @command: the command which need to init
 */
static void initcmd(struct Cmd* command)
{
	// init struct Cmd
	command.optionNum = 0;
	command.argNum = 0;
	command.intputNum = 0;
	command.outputNum = 0;
	command.outputType = TYPE_UNKNOWN;
}

/*
 * savecmd: save the command into commands
 *
 * @command: one command wait to save
 * @commands: array of struct Cmd to store new command
 * @commandNum: current num of commands
 *
 * Save the new command into commands array, this function will increase commandNum auto
 */
static int savecmd(const struct Cmd* command, struct Cmd* commands, int* commandNum)
{
	if(commandNum>=MAX_CMD) return ERR_OVER_CMD;	
	if(memcpy(commands[(*commandNum)++]),command,sizeof(struct Cmd))==NULL) return ERR_MEMCPY;
	return ERR_SUCC;	
}

/*
 * getcmds: get commands from tokens
 *
 * @tokens: 2-dimensional array which stores tokens
 * @tokenNumL num of tokens
 * @commands: array of struct Cmd to save parsed commands from tokens
 * @commandNum: num of commands
 *
 * Parse the input tokens which are delimited by blankspace from input string.
 * Then save the parsed commands into an array of struct Cmd.
 * If there is any error occurs, return the erro code immediately.
 */
static int getcmds(const char** tokens,const int tokenNum,struct Cmd* commands,int* commandNum)
{
	int tokenIndex = 0; // current index of tokens
	*commandNum = 0; 

	while(true){
		// parse the tokens to get command
		struct Cmd tempCmd; // save temp command
		initcmd(&tempCmd); // init the command

		// get command name
		if(tokenIndex >= tokenNum){
			if(*commandNum == 0) break; // blank command(tokenNum == 0)
			else return ERR_INVALID_PIPE; // blank after pipe symbol
		}
		if(strcpy(tempCmd.name,tokens[tokenIndex++])==NULL) return ERR_STRCPY;

		// get command options, arguments and redirection info
		if(tokenIndex >= tokenNum){
		   	// this is a command without any options or arguments
			int errcode = savecmd(&tempCmd,commands,commandNum);
			if(errcode!=ERR_SUCC) return errcode;
			else break;
		}
		char* token = tokens[tokenIndex++]; // save current token
		bool isReInput = false; // whether has redirection input symbol
		bool isReOutput = false; // whether has redirection output symbol
		int outputType = TYPE_UNKNOWN; // redirection type of output
		// if get a pipe symbol, then just goto parse next command
		while(strcmp(token,SYMBOL_PIPE)!=0){
			// options
			if(token[0] == '-'){
				int len = strlen(token);
				for(int i = 1;i < len;i++){
					if(tempCmd.optionNum>=MAX_OPTION) return ERR_OVER_OPTION; // toomuch options
					tempCmd.options[tempCmd.optionNum++] = token[i]; // save the option character
				}				
			}
			// redirection input
			else if(strcmp(token,SYMBOL_REINPUT)==0) isReInput = true;
			// redirection output(cover type)
			else if(strcmp(token,SYMBOL_REOUTPUT_COVER)==0){
				isReOutput = true;
				outputType = TYPE_COVER;
			}
			// redirection output(append type)
			else if(strcmp(token,SYMBOL_REOUTPUT_APPEND)==0){
				isReOutput = true; 
				outputType = TYPE_APPEND;
			}
			// arguments or reinput file or reoutput file
			else{
				// reinput file
				if(isReInput){
					isReInput = false;
					if(tempCmd.inputNum>=MAX_INPUT) return ERR_OVER_INPUT; // toomuch input source
					if(strcpy(tempCmd.reInput[tempCmd.inputNum++],token)==NULL) return ERR_STRCPY; // copy the reinput file name
				}
				// reoutput file
				else if(isReOutput){
					isReOutput = false;
					if(tempCmd.outputNum>=MAX_OUTPUT) return ERR_OVER_OUTPUT; // toomuch output source
					if(strcpy(tempCmd.reOutput[tempCmd.outputNum++],token)==NULL) return ERR_STRCPY; // copy the reoutput file name
					tempCmd.outputType = outputType; // save the reoutput type(cover or append)
				}
				// arguments
				else{
					if(tempCmd.argNum>=MAX_ARG) return ERR_OVER_ARG;
					if(strcpy(tempCmd.args[tempCmd.argNum++],token)==NULL) return ERR_STRCPY; // copy the argument info	
				}
			}
				
			// get next token
			if(tokenIndex >= tokenNum){
				// this is the last token(not pipe symbol), so that's just the last command, we just need to save it and break
				int errcode = savecmd(&tempCmd,commands,commandNum);
				if(errcode!=ERR_SUCC) return errcode;
				else break;
			}
			char* token = tokens[tokenIndex++];
		}
	}

	return ERR_SUCC;
}

/*
 * checkoptions: check option characters
 *
 * @options: the options array of some struct Cmd
 * @optionsNum: the num of option characters
 */
static int checkoptions(const char* options,const int optionNum)
{
	for(int i = 0;i < optionNum;i++){
		if((options[i]>='a'&&options[i]<='z') || (options[i]>='A'&&options[i]<='Z')){} // do nothing
		else return ERR_INVALID_OPTION; // invalid option character
	}
	return ERR_SUCC;
}

/*
 * checkcmds: check commands
 * 
 * @commands: the command array to check
 * @commandNum: the num of commands
 *
 * Check the commands of the array one by one, if any error occurs, just return.
 */
static int checkcmds(const struct Cmd* commands,const int commandNum)
{
	for(int i = 0;i < commandNum;i++){
		struct Cmd* curcmd = commands[i]; // command info
		char* curname = curcmd->name; // command name

		// ls
		if(strcmp(curname,CMDSTR__LS)==0){
			int errcode = checkoptions(curcmd->options,curcmd->optionNum);
			if(errcode!=ERR_SUCC) return errcode;
			if(curcmd->inputNum!=0) return ERR_INVALID_INPUT // redundant reinput
			if(curcmd->outputNum!=0 && curcmd->outputType!=TYPE_COVER && curcmd->outputType!=TYPE_APPEND) return ERR_INVALID_OUTPUT_TYPE; // invalid output type
		}
		// exit
		else if(strcmp(curname,CMDSTR_EXIT)==0){
			if(curcmd->optionNum!=0) return ERR_INVALID_OPTION; // redundant option
			if(curcmd->argNum!=0) return ERR_INVALID_ARG; // redundant argument
			if(curcmd->inputNum!=0) return ERR_INVALID_INPUT; // redundant reinput
			if(curcmd->outputNum!=0) return ERR_INVALID_OUTPUT; // redundant output
		}
		// cd
		else if(strcmp(curname,CMDSTR_CD)==0){
			int errcode = checkoptions(curcmd->options,curcmd->optionNum);
			if(errcode!=ERR_SUCC) return errcode;
			if(curcmd->argNum!=1) return ERR_INVALID_ARG; // invalid argument num
			if(curcmd->inputNum!=0) return ERR_INVALID_INPUT; // redundant reinput
			if(curCmd->outputNum!=0) return ERR_INVALID_OUTPUT; // redundant reoutput
		}
		// invalid command name
		else return ERR_INVALID_NAME;
	}
	return ERR_SUCC;
}

/*
 * execmds: execute the commands in the array
 *
 * @commands: the command array
 * @commandNum: the num of command array
 *
 * Execute the commands in the array one by one, note that the commands in the array is connected by pipe symbol, so the output of prev command is the
 * intput of next command.
 */
static int execmds(const struct Cmd* commands,const int commandNum)
{
	for(int i = 0;i < commandNum;i++){
		struct Cmd* curcmd = commands[i];
		char* curname = curcmd->name;
		int errcode = ERR_SUCC;

		// ls
		if(strcmp(curname,CMDSTR_LS)==0) errcode = execls(curcmd);
		// exit
		else if(strcmp(curname,CMDSTR_EXIT)==0) errcode = execexit(curcmd);
		// cd
		else if(strcmp(curname,CMDSTR_CD)==0) errcode = execcd(curcmd);
		// invalid command name
		else errcode = ERR_INVALID_NAME;
	}	
	return errcode;
}

/*
 * process: process input line
 *
 * @str: input string from standard input(console)
 *
 * Get tokens first, parse the tokens to get commands, and then check if the commands are valid,
 * execute it at last.
 */
int process(const char* str)
{
	// get tokens from the input string
	int tokenNum = 0; // num of tokens
	char tokens[MAX_TOKEN][MAX_LEN]; // save parsed tokens
	int errcode = gettokens(str,tokens,&tokenNum);
	if(errcode!=ERR_SUCC) return errcode;
		
	// get commands from above tokens
	int cmdNum = 0; // num of commands
	struct Cmd commands[MAX_CMD]; // save parsed commands	
	errcode = getcmds(tokens,tokenNum,commands,&commandNum);
	if(errcode!=ERR_SUCC) return errcode;

	// check commands
	errcode = checkcmds(commands,cmdNum);
	if(errcode!=ERR_SUCC) return errcode;

	// execute commands
	errcode = execmds(commands,cmdNum);
	if(errcode!=ERR_SUCC) return errcode;
}