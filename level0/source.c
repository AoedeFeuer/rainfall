#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define _GNU_SOURCE

int	main(int ac, char **av)
{
	char	*shell_name;
	char	*args[2];
	gid_t	gid;
	uid_t	uid;

	if (atoi(av[1]) == 423)
	{
		shell_name = strdup("/bin/sh");
		args[0] = shell_name;
		args[1] = NULL;

		gid = getegid();
		uid = eteuid();
		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);

		execv("/bin/sh", args);
	}
	else
	{
		fwrite("No !\n", 5, 1, stderr);
	}
	return(0);
}