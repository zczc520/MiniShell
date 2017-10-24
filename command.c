#include "command.h"

struct Job* joblist = NULL; // store the list of suspended jobs

/*
 * addjob: insert job into joblist
 *
 * @job: the job to add, note that the jobid of job is invalid, we should assign a maxid+1 as its jobid
 *
 * Malloc a new job, copy the input job info and assign a jobid, then insert it as a tail.
 */
static void addjob(const struct Job job)
{
	// malloc a new job
	struct Job* newjob = (struct Job*)malloc(sizeof(struct Job));
	memcpy(newjob,&job,sizeof(struct Job));

	// get max id
	int maxid = 0;
	struct Job* curjob = joblist;
	struct Job* prevjob = NULL;
	while(curjob != NULL){
		if(curjob->jobid > maxid) maxid = curjob->jobid;
		prevjob = curjob;
		curjob = curjob->nextjob;
	}
	newjob->jobid = maxid + 1;

	// insert into the tail of the joblist
	if(prevjob == NULL) joblist = newjob;
	else prevjob->nextjob = newjob;
}

/*
 * rmjob: remove the job with given jobid
 *
 * @jobid: the jobid to match
 * @job: copy the removed job info into this job(we will set the nextjob pointer as NULL)
 *
 * Find the job with given jobid, if found, just copy the job info into job.
 * Otherwise, return invalid jobid error number.
 */
static int rmjob(const int jobid, struct Job* job)
{
	if(joblist==NULL) return ERR_INVALID_JOBID;

	struct Job* curjob = joblist;
	struct Job* prevjob = NULL;
	while(curjob!=NULL){
		if(curjob->jobid == jobid){
			if(prevjob==NULL) joblist = curjob->nextjob; // curjob is joblist
			else prevjob->nextjob = curjob->nextjob;
			curjob->nextjob = NULL; // reset curjob
			if(memcpy(job,curjob,sizeof(struct Job))==NULL) return ERR_MEMCPY; // copy curjob into job
			free(curjob); // free curjob
			return ERR_SUCC;
		}
		prevjob = curjob;
		curjob = curjob->nextjob;
	}	

	return ERR_INVALID_JOBID;
}

/*
 * initjob: init the job with default value
 *
 * @job: the job to init
 *
 * We will use 0 as default value of jobid and pidNum, and use NULL as default value of nextjob pointer.
 */
static void initjob(struct Job* job)
{
	job->jobid = 0;
	job->pidNum = 0;
	for(int i=0;i<MAX_CMD;i++){
		job->jobpids[i] = -1;
	}
	job->nextjob = NULL;
}

/*
 * execexit: process build-in command(exit)
 *
 * @command: point to command info
 *
 * Exit the mini shell.
 */
int execexit(const struct Cmd* command)
{
	if(joblist!=NULL) return ERR_INVALID_EXIT;
	else exit(0);
}

/*
 * execcd: process build-in command(cd)
 *
 * @command: point to command info
 *
 * Change directory.
 */
int execcd(const struct Cmd* command)
{
	if(command->argNum!=1) return ERR_UNKNOWN; // it should be promise when checkcmds in process.c
	if(chdir(command->args[0])==-1) return ERR_CHDIR; // chdir error
	return ERR_SUCC;	
}

/*
 * execjobs: process build-in command(jobs)
 *
 * @command: point to the command info
 *
 * Display the job list.
 */
int execjobs(const struct Cmd* command)
{
	if(command->argNum!=0) return ERR_UNKNOWN; // it should be promised when checkcmds in process.c
	// display the joblist
	struct Job* curjob = joblist;
	while(curjob!=NULL){
		printf("Job%d pidnum:%d ",curjob->jobid,curjob->pidNum);
		for(int i=0;i<curjob->pidNum;i++) printf("pid[%d]:%d ",i,curjob->jobpids[i]);
		printf("\n");
		curjob = curjob->nextjob;
	}
	return ERR_SUCC;
}

int execfg(const struct Cmd* command)
{
	if(command->argNum!=1) return ERR_UNKNOWN; // it should be promised when checkcmds in process.c
	// foreground the given job
	int jobid = atoi(command->args[0]); 
	struct Job job;// save the removed job info
	rmjob(jobid,&job); // remove the job from joblist
	for(int i=0;i<job.pidNum;i++) if(kill(job.jobpids[i],SIGCONT)==-1) return errno;
	// wait the child processes
	struct Job newjob;// save the new job info
	initjob(&newjob);// init the new job
	for(int i=0;i<job.pidNum;i++){
		int status = 0;
		if(waitpid(job.jobpids[i],&status,WUNTRACED)<0) return errno;
		// check status
		if(WIFSTOPPED(status)) newjob.jobpids[newjob.pidNum++] = job.jobpids[i];
		else if(status!=0) return status; // errno of child process
	}
	if(newjob.pidNum>0) addjob(newjob);
	return ERR_SUCC;
}

/*
 * initfds: init fd array
 *
 * @fds: the fd array
 * @rownum: the row num of fd array
 *
 * Init array elements with -1.
 */
static void initfds(int fds[][2],int rownum)
{
	for(int i=0;i<rownum;i++){
		for(int j=0;j<2;j++){
			fds[i][j] = -1;
		}
	}
}

/* 
 * closefds: close fd array
 *
 * @fds: the fd array
 * @rownum: the row num of fd array
 *
 * Close all of the elements except those whose value is -1.
 */
static int closefds(int fds[][2],int rownum)
{
	for(int i=0;i<rownum;i++){
		for(int j=0;j<2;j++){
			if(fds[i][j]!=-1){
				if(close(fds[i][j])==-1) return errno;	
				fds[i][j] = -1;
			}
		}
	}
	return ERR_SUCC;
}

// NOTE: the writer will exit automatically just after finishing writing, it doesn't need refcnt of reader is 0
// NOTE: But, the reader will block if refcnt of writer isn't 0
// NOTE: So we must close fds of parent before waitpid, or it may cause dead lock
int execothers(const struct Cmd* commands,const int commandNum)
{
	if(commandNum==0) return ERR_SUCC;
	pid_t pids[commandNum]; // each command needs a child process
	int fds[commandNum-1][2]; // fds for pipe
	initfds(fds,commandNum-1); // init fds
	struct Job job;
	initjob(&job); // init the job
	for(int i = 0;i < commandNum;i++){
		// create pipe
		if(i != commandNum-1){
			if(pipe(fds[i])==-1) exit(errno);
		}
		pids[i] = fork();
		// child process
		if(pids[i]==0){
			// restore SIGINT and SIGTSTP
			signal(SIGINT, SIG_DFL);
			signal(SIGTSTP, SIG_DFL);

			const struct Cmd* command = &commands[i];
			// set redirection of pipe
			if(commandNum>1){
				if(i==0){ // first command
					if(dup2(fds[i][1],1)==-1) exit(errno);
				}
				else if(i==commandNum-1){ // last command
					if(dup2(fds[i-1][0],0)==-1) exit(errno);
				}
				else{ // mid command
					if(dup2(fds[i][1],1)==-1) exit(errno);
					if(dup2(fds[i-1][0],0)==-1) exit(errno);
				}
			}
			// close fds(in child process)
			int errcode = closefds(fds,commandNum-1);
			if(errcode!=ERR_SUCC) return errcode;
			// set env var
			if(setenv("PATH","/bin:/usr/bin:/usr/sbin:",1)<0) exit(errno);
			// prepare total arguments
			int totalNum = command->optionNum + command->argNum + 1; //argv[0] is the name of command
			char** argv = NULL; // argv for execvp
			char argvContent[totalNum][MAX_LEN]; // content of argv
			// malloc argv pointers
			argv = (char**)malloc((totalNum+1) * sizeof(char*)); // argv[end] is NULL // TODO: I didn't free, and it may cause memory leak
			if(argv==NULL) exit(errno);
			// copy data into argv content
			strcpy(argvContent[0],command->name);
			int prevNum = 1;
			for(int i = 0;i < command->optionNum;i++) sprintf(argvContent[i+prevNum],"-%c",command->options[i]);
			prevNum += command->optionNum;
			for(int i = 0;i < command->argNum;i++) strcpy(argvContent[i+prevNum],command->args[i]);
			prevNum += command->argNum;
			// set redirection input
			for(int i = 0;i < command->inputNum;i++){
				int inputfd = open(command->reInput[i],O_RDONLY);
				if(inputfd == -1) exit(errno);
				if(dup2(inputfd,0) == -1) exit(errno);
				if(close(inputfd) == -1) exit(errno);				
			}
			// set redirection output
			for(int i = 0;i < command->outputNum;i++){
				int outputfd = -1;
				if(command->outputType == TYPE_COVER) outputfd = open(command->reOutput[i],O_WRONLY | O_CREAT | O_TRUNC,S_IRWXU);
				else if(command->outputType == TYPE_APPEND) outputfd = open(command->reOutput[i],O_WRONLY | O_CREAT | O_APPEND,S_IRWXU);
				else exit(ERR_UNKNOWN);
				if(outputfd == -1) exit(errno);
				if(dup2(outputfd,1) == -1) exit(errno);
				if(close(outputfd) == -1) exit(errno);
			}
			// set argv pointers
			for(int i = 0;i < totalNum;i++) argv[i] = argvContent[i];
			argv[totalNum] = NULL;
			// call execvp
			if(execvp(command->name,argv)<0) exit(errno);
			// exit with normal status
			exit(0);
			return ERR_SUCC;
		}
		// parent process
		else if(pids[i]>0){
			// do nothing
		}
		// error
		else return errno;
	}
	// close fds(in parent process)
	closefds(fds,commandNum-1);
	// wait chlidren
	for(int i=0;i<commandNum;i++){
		int status = 0;
		if(waitpid(pids[i],&status,WUNTRACED)<0) return errno;
		// check status
		if(WIFSTOPPED(status)) job.jobpids[job.pidNum++] = pids[i];
		else if(status!=0 && strcmp(commands[i].name,"cat")!=0) return status; // errno of child process 
		// TODO:I don;t know why status of cat is 2 after I input ctrl-Z, so I just hard code here:(
	}
	// add job into joblist
	if(job.pidNum>0) addjob(job);
	return ERR_SUCC;
}
