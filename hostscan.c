#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESPONSE_TIMEOUT "0.1"
#define CLRSCR "clear"
#define MAX_HOSTS 254
#define MAX_IP_LEN 16

/*
 * IN THE NAME OF ALLAH,
 * Date: 12-07-2024
 * Developer: MOHAMED ARRAF (https://www.linkedin.com/in/mohamedarraf)
 * Description: Scan for available nodes on the network.
 * Please note that this program is still under development, for now, it is based on OS level
 * for the ping process and it might be upgraded later to be based on low level ping process.
 * functions:
 * 	ping() is the pinging process handed by the operating system.
 * 	get_net_portion() modifies network address by removing last octet to be filled then
 *      with host portion value.
 *      generate_ips() generates all 245 ip addresses of the network.
 */

void	ping(char	hosts_addr[][MAX_IP_LEN], char	online_hosts[][MAX_IP_LEN])
{
	int	exit_status;
	char	command[50];

	/*
	 * system() returns a value of the termination status of child shell
	 * used to execute command, e.g: value=0 => ping got a positive echo reply
	 * from peer.
	 * info: man system
	 * 	 man ping
	 * 	 search on the internet for ping exit codes or test it yourself.
	 */
	for (int i = 0; i < MAX_HOSTS; i++)
	{
		snprintf(command, sizeof(command), "ping -c1 -nq -W %s %s", RESPONSE_TIMEOUT, hosts_addr[i]);
		exit_status = system(command);
		system(CLRSCR);
		if (exit_status == 0)
		{
			strcpy(online_hosts[i], hosts_addr[i]);
		}
		else if (exit_status != 0)
		{
			// 0 = host is down, useful when printing online ones
			online_hosts[i][0] = 0;
		}
	}
}

void	generate_ips(char	hosts_addr[][MAX_IP_LEN], char*	network_portion)
{
	int	host_portion;
	
        // first host portion value
	host_portion = 1;
	for (int i = 0; i < MAX_HOSTS; i++)
	{
		snprintf(hosts_addr[i], sizeof(hosts_addr[MAX_HOSTS]), "%s%d", network_portion, host_portion);
		host_portion++;
	}
}

void	get_net_portion(char*	net_addr)
{
	int	n_dots;
	int     i;

	i = 0;
	n_dots = 0;
	/*
	 * n_dots = number of dots in net_addr, useful to split host and network portions.
	 * loop below scans for dots in network address,
	 * when n_dots == 3, replace the next item(192.168.1.0: in this case 0) with NUL
	 */
	while (net_addr[i])
	{
		if (net_addr[i] == '.')
		{
			n_dots++;
		}
		if (n_dots == 3)
		{
			net_addr[i + 1] = '\0';
		}
		i++;
	}
}

int	main(int	argc, char**	argv)
{
	char	net_addr[MAX_IP_LEN];
	char	hosts_addr[MAX_HOSTS][MAX_IP_LEN];
	char	online_hosts[MAX_HOSTS][MAX_IP_LEN];

	if (argc == 2)
	{
		strcpy(net_addr, argv[1]);
	}
	else
	{
		printf("Error: bad or missing argument\n");
		return 1;
	}
	get_net_portion(net_addr);
	generate_ips(hosts_addr, net_addr);
	ping(hosts_addr, online_hosts);
	system(CLRSCR);

	printf("Online hosts:\n");
	for (int i = 0; i < MAX_HOSTS; i++)
	{
		if (online_hosts[i][0])
		{
			printf("  %s\n", online_hosts[i]);
		}
	}
	return 0;
}
