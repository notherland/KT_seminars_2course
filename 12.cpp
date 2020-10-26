#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <grp.h>
#include <pwd.h>

using namespace std;

int main()
{
	printf("PID: %u\n", getpid());
	printf("PPID: %u\n", getppid());
	printf("UMASK: %u\n", umask(0));
	printf("CWD: %s\n", get_current_dir_name());

	uid_t uid = getuid();
	struct passwd* passwd = getpwuid(uid);
	printf("pw_name: %s", passwd->pw_name);
	printf("pw_passwd: %s\n", passwd->pw_passwd);
	printf("pw_uid: %u\n", passwd->pw_uid);
	printf("pw_gid: %u\n", passwd->pw_gid);
	
	printf("groups: %d\n", getgroups(0, 0));

	struct group* group = getgrgid(passwd->pw_gid);
	printf("group name: %s\n", group->gr_name);
	printf("group passwd: %s\n", group->gr_passwd);
	printf("user priority: %s\n", getpriority(PRIO_USER, 0));

}
