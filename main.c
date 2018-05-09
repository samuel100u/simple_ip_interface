/** Licensed under AGPL 3.0. (C) 2010 David Moreno Montero. http://coralbits.com */
#include <onion/onion.h>
#include <onion/log.h>
#include <onion/version.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

int isValidIpAddress(const char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

int change_static_ip(const char *ip){
	
	char format[] = "sudo ifconfig eth0 %s";
	char cmd[256] = {};
	
	if(isValidIpAddress(ip)){
			
		sprintf(cmd,format,ip);
		system(cmd);
		//system("sudo route add default gw 192.168.1.1 eth0");

	}else{
		return 0;
	}
	
	return 1;
}


int change_gw_ip(const char *ip){
	
	char format[] = "sudo route add default gw %s eth0";
	char cmd[256] = {};
	
	if(isValidIpAddress(ip)){
			
		sprintf(cmd,format,ip);
		system(cmd);
		
	}else{
		return 0;
	}
	
	return 1;
}


onion_connection_status process_data(void *_, onion_request *req, onion_response *res){
	
	char temp[250] =			"<html>\n"
								"<head>\n"
									"<title>%s</title>\n"
								"</head>\n"
							"\n"
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
	
	
	const char *IP = onion_request_get_post(req,"text");
	
		
	int result = change_static_ip(IP);
		
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
							"IP: "
								"<input type=\"text\" name=\"text\" required=\"required\">\n"
								"<input type=\"submit\">\n"
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
