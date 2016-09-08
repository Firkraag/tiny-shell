void eval(char *cmdline) 
{
    char *argv[MAXARGS]; /* Argument list execve() */
    char buf[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */
    pid_t pid;           /* Process id */
    
    strcpy(buf, cmdline);
    bg = parseline(buf, argv); 
//	printf("argv[0] is %s\n", argv[0]);
//	printf("argv[1] is %s\n", argv[1]);
    if (argv[0] == NULL)  
		return;   /* Ignore empty lines */

    if (!builtin_command(argv)) { 
		sigset_t mask;
		Sigemptyset(&mask);
		Sigaddset(&mask, SIGCHLD);
		Sigprocmask(SIG_BLOCK, &mask, NULL);

		if ((pid = Fork()) == 0) {   /* Child runs user job */
			Sigprocmask(SIG_UNBLOCK, &maks, NULL);
			/* Set the process group ID for the job to the PID of the child */
			setpgid(0, 0);
			/* discard the signal handlers inherited from the parent */
			Signal(SIGINT, SIG_DFL);
			Signal(SIGTSTP, SIG_DFL);
			if (execve(argv[0], argv, environ) < 0) {
				printf("%s: Command not found.\n", argv[0]);
				exit(0);
			}
		}

		create_job(pid);
		Sigprocmask(SIG_UNBLOCK, &maks, NULL);
		if (!bg) {
	    	int status;

	    	/* 
	    	Set foreground process group ID of the controlling terminal to the PID of the child,
	    	then the terminal driver will send a SIGINT(SIGTSTP) signal to the child when we type ctrl-c(ctrl-z).
	    	*/
	    	tcsetpgrp(STDIN_FILENO, pid);
	    	/*
	    	wait for a child to be terminated or stopped
	    	*/
	    	if (waitpid(pid, &status, WUNTRACED) > 0)
				if (WIFEXITED(status))
					delete_job(pid);
				else if (WIFSIGNALED(status))
				{
					delete_job(pid);	
					printf("Job %d terminated by signal: %d", (int) pid, WTERMSIG(status));
				}
				else if (WIFSTOPPED(status))
				{
					fore2back(pid);
					tcsetpgrp(STDIN_FILENO, pid);
					printf("Job [%d] stopped by signal: %d", jid, WSTOPSIG(status));
				}
			else if (errno != ECHILD)
				unix_error("waitfg: waitpid error");
		}
		else
	    	printf("%d %d %s", jid, (int) pid, cmdline);
    }
    return;
}