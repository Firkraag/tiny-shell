#include "csapp.h"

void foreground_processing(job *j)
{
	pid_t pid = j->pid;
	int jid = j->jid
	int status;

	if (waitpid(pid, &status, WUNTRACED) < 0)
		if (error != ECHILD)
			unix_error("waitfg: waitpid error");
	else if (WIFSTOPPED(status))
	{
		fore2back(j);
		printf("Job [%d] %d stopped by signal: %d", jid, (int) pid, WSTOPSIG(status));
	}
	else if (WIFEXITED(status))
		deletejob(j);
	else if(WIFSIGNALED(status))
	{
		printf("Job [%d] %d terminated by signal: %d", jid, (int) pid, WTERMSIG(status));	
		deletejob(j);
	}
}

