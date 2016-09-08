#include "csapp.h"

pid_t jid2pid(int jid);

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
    if (!strcmp(argv[0], "quit")) /* quit command */
		exit(0);  
    
    if (!strcmp(argv[0], "&"))    /* Ignore singleton & */
		return 1;
	
	if (!strcmp(argv[0], "bg")) /* bg command */
	{
		pid_t pid;
	
		if (argv[1][0] == '%')
		{
			pid = jid2pid(atoi(++(argv[1])));
		}
		else
			pid = atoi(argv[1]);

		if (pid > 0)
			Kill(pid, SIGCONT);
		
		return 1;
	}

    if (!strcmp(argv[0], "fg")) /* fg command */
	{
		int option = (argv[1][0] == '%');
		/* If option == 1, use jid to delete job; otherwise, use pid */
		//pid_t pid = deletejob(option ? atoi(++(argv[1])) : atoi(argv[1]), option);

		if (argv[1][0] == '%')
		{
			pid = jid2pid(atoi(++(argv[1])));
		}
		else
			pid = atoi(argv[1]);

		if (pid > 0)
		{
			back2fore(pid);
			Kill(-1 * pid, SIGCONT);
			foreground_processing(pid);
		}
		return 1;
	}


	if (!strcmp(argv[0], "pwd"))
	{
		printf("%s\n", getenv("PWD"));
		return 1;
	}

    if (!strcmp(argv[0], "&")) {   /* Ignore singleton & */
	//	printf("&\n");
		return 1;
	}
	//printf("%s is not a builtin command\n", argv[0]);
    return 0;                     /* Not a builtin command */
}
