#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Detecting current Operating System
#define OS_ERROR 0
#if defined(__linux__)
	#define IS_LINUX 1
	#define IS_WIN 0
	#define CLRSCR "clear"
#elif defined(_WIN32)
	#define IS_LINUX 0
	#define IS_WIN 1
	#define	CLRSCR "cls"
#else
	// in case of os detection error, exit program
	#define OS_ERROR 1
#endif
#define MAX_HOSTS 254
#define MAX_IP_LEN 16

/*
 *                              IN THE NAME OF ALLAH,
 * History: 12-07-2025
 * Developer: MOHAMED ARRAF (https://www.linkedin.com/in/mohamedarraf)
 * Description: Scan for available nodes on the network.
 * Please note that this program is still under development, for now, it is based on OS level
 * for the ping process and it might be upgraded later to be based on low level ping process.
 * functions:
 * 	ping() is the pinging process handed by the operating system.
 * 	ping_config() has the configuration needed for ping process (based on OS).
 * 	get_net_portion() modifies network address by removing last octet to be filled then
 *      with host portion value.
 *      generate_ips() generates all 245 ip addresses of the network.
 */

void	ping(char*	cmd, char*	potn, char	hosts_addr[][MAX_IP_LEN], char	online_hosts[][MAX_IP_LEN])
{
        /*
	 * system() returns a value of the termination status of child shell used to
	 * execute command, e.g: value=0 => ping got a positive echo reply from peer.
	 * info:
	 *  man system
	 *  man ping
	 *  search on the internet for ping exit status or test it yourself.
	 */
	int	exit_status;
	char	full_cmd[50];

	for (int i = 0; i < MAX_HOSTS; i++)
	{
		snprintf(full_cmd, sizeof(full_cmd), "%s %s %s", cmd, hosts_addr[i], potn);
		exit_status = system(full_cmd);
		if (exit_status == 0)
			strcpy(online_hosts[i], hosts_addr[i]);
		else
		{
			// 0 = host is down, useful when printing online ones
			online_hosts[i][0] = 0;
		}
	}
}

void	ping_config(char*	cmd, int	cmd_size, char*	potn, char*	response_timeout)
{
	if (IS_LINUX)
	{
		snprintf(cmd, cmd_size, "ping -c1 -nq -W %s", response_timeout);
		strcpy(potn, "> /dev/null");
	}
	else if (IS_WIN)
	{
		// if in windows output is not showing up, delet "> nul" in cmd bellow
		snprintf(cmd, cmd_size, "ping -n 1 -w %s", response_timeout);
		strcpy(potn, "> nul");
	}
}

void	generate_ips(char	hosts_addr[][MAX_IP_LEN], char*	network_portion)
{
	int	host_portion;
	
        // first host portion value
	host_portion = 1;
	for (int i = 0; i < MAX_HOSTS; i++)
	{
		snprintf(hosts_addr[i], MAX_IP_LEN, "%s%d", network_portion, host_portion);
		host_portion++;
	}
}

void	get_net_portion(char*	net_addr)
{
        /*
	 * n_dots = number of dots in net_addr, useful to split host and network portions.
	 * loop below scans for dots in network address,
	 * when n_dots == 3, replace the next item(192.168.1.0: in this case 0) with NUL
	 */
	int	n_dots;
	int     i;

	i = 0;
	n_dots = 0;
	while (net_addr[i])
	{
		if (net_addr[i] == '.')
			n_dots++;
		if (n_dots == 3)
			net_addr[i + 1] = '\0';
		i++;
	}
}

int	main(int	argc, char**	argv)
{
	char	cmd[50]; // ping (OS based) command but without ip and potn
	char	potn[20]; // Ping Output To Null (potn)
	char	response_timeout[10]; // timeout of ping if no response is received
	char	net_addr[MAX_IP_LEN];
	char	hosts_addr[MAX_HOSTS][MAX_IP_LEN];
	char	online_hosts[MAX_HOSTS][MAX_IP_LEN];

	if (OS_ERROR)
	{
		printf("Error, OS detection failed!\n");
		return 2;
	}
	if (argc == 3 && strlen(argv[1]) >= 7 && !(strlen(argv[2]) > 9))
	{
		strcpy(net_addr, argv[1]);
		strcpy(response_timeout, argv[2]);
	}
	else
	{
		// printf("argc == %d\nargv[1] == %s\nargv[2] == %s", argc, argv[1], argv[2]);
		printf("Error: bad or missing argument!\n");
		printf("Usage: %s <ip_address> <timeout>\n", argv[0]);
		printf("Note: timeout for linux(seconds), for windows(ms)\n");
		printf("Example:\n%s 192.168.0.0 0.1\n%s 10.0.0.0 1\n", argv[0], argv[0]);
		return 1;
	}
	get_net_portion(net_addr);
	generate_ips(hosts_addr, net_addr);
	ping_config(cmd, sizeof(cmd), potn, response_timeout);
	printf("netscan by @medoxe (https://www.linkedin.com/in/mohamedarraf)\n");
	printf("Scanning for online hosts. Estimated time: 30s\n");
	ping(cmd, potn, hosts_addr, online_hosts);

	printf("Online hosts:\n");
	for (int i = 0; i < MAX_HOSTS; i++)
	{
		if (online_hosts[i][0])
			printf("%s\n", online_hosts[i]);
	}
	return 0;
}
