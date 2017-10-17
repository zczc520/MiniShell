#include "command.h"

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
 * @command: the command which need to init
 */
static void initcmd(struct Cmd* command)
{
	// init struct Cmd
	command.optionNum = 0;
	command.argNum = 0;
	command.intputNum = 0;
	command.outputNum = 0;
}

/*
 * savecmd: save the command into commands
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
					if((token[i]>='a'&&token[i]<='z') || (token[i]>='A'&&token[i]<='Z')){
						tempCmd.options[tempCmd.optionNum++] = token[i]; // save the option character
					}
					else{
						return ERR_INVALID_OPTION; // invalid option character
					}
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
 * execmd: execute command
 *
 * @str: input string from standard input(console)
 *
 * Get tokens first, then parse the tokens to check if the command is valid,
 * execute it at last.
 */
int execmd(const char* str)
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
}
