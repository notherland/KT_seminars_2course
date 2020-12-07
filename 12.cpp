#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdlib.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include<sys/time.h>
#include<sys/resource.h>

int main()
{
	printf("PID: %u\n", getpid());
	printf("PPID: %u\n", getppid());
	mode_t mask = umask(0);
	umask(mask);
	printf("UMASK: %u\n", mask);
	char *cwd = getcwd(NULL, 0);
	printf("CWD: %s\n", cwd);
	free(cwd);

	uid_t uid = getuid();
	struct passwd* passwd = getpwuid(uid);
	printf("pw_name: %s", passwd->pw_name);
	printf("pw_passwd: %s\n", passwd->pw_passwd);
	printf("pw_uid: %u\n", passwd->pw_uid);
	printf("pw_gid: %u\n", passwd->pw_gid);
	
	int ngroups = getgroups(0, 0);
	printf("num of groups: %d\n", ngroups);
	gid_t groups[ngroups];
	getgroups (ngroups, groups);
	printf("groups: ");
	for (int i = 0; i < ngroups; i++)
	{
		printf ("%d ", groups[i]);
	}

	struct group* group = getgrgid(passwd->pw_gid);
	printf("\ngroup name: %s\n", group->gr_name);
	printf("group passwd: %s\n", group->gr_passwd);
	printf("user priority: %d\n", getpriority(PRIO_USER, 0));

	return 0;
}
