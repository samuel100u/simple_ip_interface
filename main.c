#include <onion/onion.h>
#include <onion/log.h>
#include <onion/version.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

#define DEV "eth0"

int isValidIpAddress(const char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

void write_auto_run_script(const char *ip, const char *gw){


	FILE *fptr;
	
	char format[] = "#!/bin/sh -e \n"
					"sudo /etc/init.d/rpcbind restart\n"
					"sudo mount -t nfs %s /var/Maps/db/ &\n"
					"sudo touch /var/Maps/db/Welcome &\n"
					"cd .. && cd /var/WF && su pi -c 'sh runmono.sh'\n"
					"sudo iptables-restore < /etc/network/iptables.eth0_to_wlan0 &\n"
					"sleep 10\n"
					"sudo ip addr flush dev %s \n"
					"sudo ifconfig %s %s \n"
					"sudo route add default gw %s %s \n"
					"exit 0";
	
	
	
	
	char buf[256] = {};
	
	fptr = fopen("/etc/rc.local", "w");
	
	if(fptr == NULL)
	{
	  printf("Error!");
	  exit(1);
	}
	
	sprintf(buf,format,"192.168.0.16:/var/maps",DEV,DEV,ip,gw,DEV);

	fprintf(fptr,"%s",buf);
	
	fclose(fptr);
}



void change_static_ip(const char *ip){
	
	char format[] = "sudo ifconfig %s %s";
	char cmd[256] = {};

	sprintf(cmd,format,DEV,ip);
	system(cmd);
}

void change_gw_ip(const char *ip){
	
	char format[] = "sudo route add default gw %s %s";
	char cmd[256] = {};
	
	sprintf(cmd,format,ip,DEV);
	system(cmd);
}

int change_network_settings(const char *ip, const char *gw){

	if(isValidIpAddress(ip)&& isValidIpAddress(gw)){
			
		change_static_ip(ip);
		change_gw_ip(gw);
		write_auto_run_script(ip,gw);
		
	}else{
		return 0;
	}
	
	return 1;
}
onion_connection_status process_data(void *_, onion_request *req, onion_response *res){
	
	char temp[] =		"<html>\n"
								"<head>\n"
									"<title>%s</title>\n"
								"</head>\n"
							"%s\n"
								"<form>\n"
									"<input type=\"button\" value=\"Go back!\" onclick=\"history.back()\">\n"
									"</input>\n"
								"</form>\n"
						"</html>\n";
	
	if (onion_request_get_flags(req)&OR_HEAD){
		onion_response_write_headers(res);
		return OCS_PROCESSED;
	}
	
	
	const char *IP = onion_request_get_post(req,"ip");
	const char *GW = onion_request_get_post(req,"gw");
	
	
		
	int result = change_network_settings(IP, GW);
	
	if(result){
		onion_response_printf(res,temp, "successfully", "IP updates successfully.");
	}else{
		onion_response_printf(res,temp, "failed", "A valid IP address is required.");
	}

	return OCS_PROCESSED;
}

onion *o=NULL;

static void shutdown_server(int _){
	if (o)
		onion_listen_stop(o);
}


char config_html[] =	"<html>\n"
							"<head>\n"
								"<title>Config</title>\n"
							"</head>\n"
							"\n"
							"<form method=\"POST\" action=\"process_data\">\n"
							"IP:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
								"<input type=\"text\" name=\"ip\" required=\"required\">\n"
								"<br><br>Gateway: <input type=\"text\" name=\"gw\" required=\"required\">\n"
								"<br><br><input type=\"submit\">\n"
							"</form>\n"
						"</html>\n";



int main(int argc, char **argv){

	signal(SIGINT,shutdown_server);
	signal(SIGTERM,shutdown_server);
	
	
	ONION_VERSION_IS_COMPATIBLE_OR_ABORT();

	o = onion_new(O_POOL);

	onion_set_hostname(o,"0.0.0.0");
	
	onion_url *urls = onion_root_url(o);
	
	onion_url_add_static(urls, "config", config_html, HTTP_OK);
	
	onion_url_add(urls, "process_data", process_data);

	onion_listen(o);
	onion_free(o);
	
	return 0;
}
