#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * IN THE NAME OF ALLAH,
 * Date: 12-07-2024
 * Developer: MOHAMED ARRAF (medoxe)
 * Description: Scan for available nodes on the network.
 * Please note that this program is still under development, for now, it is based on OS level
 * for the ping process and it might be upgraded later to be based on low level ping process.
 * functions:
 * 	ping() is the pinging process handed by the operating system.
 * 	get_net_portion() modifies network address by removing last octet to be filled then
 *      with host portion value. It returns index of last octet placeholder.
 *      generate_ips() generates all 245 ip addresses of the network by doing ASCII math.
 */

#define RESPONSE_TIMEOUT "0.1"
#define CLRSCR "clear"

void	ping(char	hosts_addr[][16], int	row_size, char	online_hosts[][16])
{
	int	exit_code;
	char	command[50];
	int	i;

	/*
	 * system() returns a value of the termination status of child shell
	 * used to execute command, e.g: value=0 => ping got a positive echo reply
	 * from peer.
	 * info: man system
	 * 	 man ping
	 * 	 search on the internet for ping exit codes or test it yourself.
	 */
	for (i = 0; i < row_size; i++)
	{
		snprintf(command, sizeof(command), "ping -c1 -nq -W %s %s", RESPONSE_TIMEOUT, hosts_addr[i]);
		exit_code = system(command);
		system(CLRSCR);
		if (exit_code == 0)
		{
			strcpy(online_hosts[i], hosts_addr[i]);
		} else if (exit_code != 0)
		{
			online_hosts[i][0] = 0; // 0 = host is down, useful when printing online ones
		}
	}
}

int	get_net_portion(char*	net_addr)
{
	int	host_index;
	int	n_dots; // number of dots in net_addr, useful to split host and network portions
	int     i;

	i = 0;
	n_dots = 0;
	/*
	 * loop below scans for dots in network address
	 * when n_dots == 3, replace the next item(192.168.1.0: in this case 0) with
	 * null terminator to replace it then with host portion
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
	host_index = i;
	return host_index;
}

void	generate_ips(char	hosts_addr[][16], char*	network_portion, int	host_index,  int	row_size)
{
	int	host_value;
	int	i;

	host_value = 1; // first host portion value
	for (i = 0; i < row_size; i++)
	{
		strcpy(hosts_addr[i], network_portion); // assing net portion to element i
		if (host_value < 10) // ip range [1-9]
		{
			hosts_addr[i][host_index] = host_value + 48;
			hosts_addr[i][host_index + 1] = '\0';
		}
		else if (host_value > 9 && host_value < 100) // ip range [10-99]
		{
			hosts_addr[i][host_index] = (host_value / 10) + 48;
			hosts_addr[i][host_index + 1] = (host_value % 10) + 48;
			hosts_addr[i][host_index + 2] = '\0';
		}
		else if (host_value > 99) // ip range [100-254]
		{
			hosts_addr[i][host_index] = (host_value / 100) + 48;
			hosts_addr[i][host_index + 1] = ((host_value % 100) / 10) + 48; 
			hosts_addr[i][host_index + 2] = (host_value % 10) + 48;
			hosts_addr[i][host_index + 3] = '\0';
		}
		host_value++;
	}
	hosts_addr[row_size - 1][0] = '\0'; // last item
}

int	main(void)
{
	/*
	 * those variables are valide to networks with /24 notation.
	 * host_id represents the last octet in the network ip address
	 * net_addr be like ...0
	 * hosts_addr represents all ips in range of /24 network
	 * row_size is the size of hosts_addr[][] row
	 */
	char*	host_id;
	char*	net_addr;
	char	hosts_addr[255][16];
	int	host_index;
	int	row_size;
	char	online_hosts[255][16];
	int	i;

	host_id = malloc(4 * sizeof(char));
	net_addr = malloc(16 * sizeof(char));
	host_id[2] = '\0';
	row_size = sizeof(hosts_addr) / sizeof(hosts_addr[0]);
	
	// wait for user input
	printf("Network address to scan(only networks with /24 notation!): ");
	fgets(net_addr, sizeof(net_addr) * 2, stdin);

	host_index = get_net_portion(net_addr);
	generate_ips(hosts_addr, net_addr, host_index, row_size);
	ping(hosts_addr, row_size, online_hosts);
	system(CLRSCR);

	printf("Online hosts:\n");
	for (i = 0; i < row_size; i++)
	{
		if (online_hosts[i][0])
		{
			printf("%s\n", online_hosts[i]);
		}
	}
	free(net_addr);
	free(host_id);
	return (0);
}
