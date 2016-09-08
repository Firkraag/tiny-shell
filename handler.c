void handler_chld(int sig)
{
	int status;
	pid_t pid;

	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		if (WIFEXITED(status))
			deletejob(j);
		else if(WIFSIGNALED(status))
		{
			printf("Job [%d] %d terminated by signal: %d", jid, (int) pid, WTERMSIG(status));	
			deletejob(j);
		}

		/* when the parent catches a SIGCHLD signal, 
		it means the child process either exited normally 
		or terminated because of a signal that was not caught, 
		so we are sure the child process is not stopped.
		*/    
		/*
		else if (WIFSTOPPED(status))
		{
			fore2back(pid);
			printf("Job [%d] stopped by signal: %d", jid, WSTOPSIG(status));
		}
		*/
	}

	if (error != ECHILD)
		unix_error("waitfg: waitpid error");
	
	return;
}

/*void handler_int(int sig)
{
	Kill(foreground_pgid, SIGINT);
	return;
}

void handler_tstp(int sig)
{
	Kill(foreground_pgid, SIGTSTP);
	fore2back(foreground_pgid);
	return;
}
*/