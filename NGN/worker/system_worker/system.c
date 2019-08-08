#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include <net/if.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "system.h"
#include "foperate.h"
#include "appdefs.h"
#include "version.h"
#include "app.h"

char g_device_name[MAX_PB_STR_LEN] = DEFAULT_DEVICE_NAME;
char g_system_name[MAX_PB_STR_LEN];


#define MAX_CPU_NUM     1
#define CPU_CHECK_TIME  5

typedef struct cpu_usage_info_s {
    int cpu_all_last[MAX_CPU_NUM];
    int cpu_all_cur[MAX_CPU_NUM];
    int cpu_idle_last[MAX_CPU_NUM];
    int cpu_idle_cur[MAX_CPU_NUM];
    int cpu_util[MAX_CPU_NUM];
} cpu_usage_info_t;

pthread_mutex_t cpu_mutex;
static cpu_usage_info_t cpu_usage;

static int cpu_usage_get_by_id(int id, int *all_vs, int *idle_vs)
{
	FILE *fp;
	int user = 0, nice = 0, system = 0, idle = 0, total = 0;
	int iowait = 0, irq = 0, softirq = 0, stealstolen = 0, guest = 0;
	char cpuid[10] = {0};
	char buf[201] = {0};
	char cpu[21] = {0};
	int flag = 0;

	/* id >=0 */
	if (id +1 > MAX_CPU_NUM) {
		return -1;
	}
	
	snprintf(cpuid, 10, "cpu%d", id);
	fp = fopen("/proc/stat", "r+");
	if(fp != NULL) {
	    while(fgets(buf, sizeof(buf), fp)) {
	        if(strstr(buf, cpuid)) {
	            sscanf(buf, "%s %10d %10d %10d %10d %10d %10d %10d %10d %10d", 
                        cpu, &user, &nice, &system, &idle,
	                    &iowait, &irq, &softirq, &stealstolen, &guest);
	            flag = 1;
	            break;
	        }
	    }
	    fclose(fp);
	}
    
	total = user + nice + system + idle + iowait + irq + softirq + stealstolen + guest;
	
	all_vs[id] = flag ? total : -1;//fix me later
	idle_vs[id] = flag ? idle : -1;
	return 0;
}

static int cpu_use_get(int *all_vs, int *idle_vs)
{
	int rv = 0;
	int i = 0;

	for(i = 0; i < MAX_CPU_NUM; i++) {
		rv = cpu_usage_get_by_id(i, all_vs, idle_vs);
		if (rv != 0) {
			break;
		}
	}

	return rv;
}

static int cpu_use_util_set(void)
{
	int i = 0;
	int all1 = 0, all2 = 0;
	int idle1 = 0, idle2 = 0;

	pthread_mutex_lock(&cpu_mutex);
	for (i = 0; i < MAX_CPU_NUM; i++) {
		all1 = cpu_usage.cpu_all_last[i];
		idle1 = cpu_usage.cpu_idle_last[i];
		all2 = cpu_usage.cpu_all_cur[i];
		idle2 = cpu_usage.cpu_idle_cur[i];
		if (all1 == -1 || all2 == -1 || 
			idle1 == -1 || idle2 == -1) {
			cpu_usage.cpu_util[i] = -1;//fix me later
		} else {
			cpu_usage.cpu_util[i] = (float)(all2-all1-(idle2-idle1)) / (all2-all1)*100 ; 
		}
	}
	pthread_mutex_unlock(&cpu_mutex);
	
	return 0;
}

static int cpu_use_util_get(int id, int *cpu_util)
{
    if (id +1 > MAX_CPU_NUM) {
		return -1;
	}
	pthread_mutex_lock(&cpu_mutex);
	*cpu_util = cpu_usage.cpu_util[id];
	pthread_mutex_unlock(&cpu_mutex);
	
	return 0;
}

static void *cpu_info_process(void *arg)
{
	while (1) {
		cpu_use_get(cpu_usage.cpu_all_last, cpu_usage.cpu_idle_last);
		sleep(CPU_CHECK_TIME);
		cpu_use_get(cpu_usage.cpu_all_cur, cpu_usage.cpu_idle_cur);
		
		cpu_use_util_set();
		sleep(1);
	}
	return NULL;
}

int system_cpu_usage_info_thread(void)
{    
	int err;    
	pthread_t ntid;
	
	memset(&cpu_usage, 0, sizeof(cpu_usage));
	pthread_mutex_init(&cpu_mutex, NULL);
	
	err = pthread_create(&ntid, NULL, cpu_info_process, NULL);    
	if (err != 0) { 
	    return -1;
	}    
	   
	return 0; 
}   


int set_board_platform(int platform)
{	
    FILE * fp;

    ow_mkdir(CONFIG_FILE_PATH);
    if ((fp = fopen(STARTUP_CONFIG_FILE_NAME, "w")) == NULL) {
        printf("can't write startup_save.conf\n");
        return OE_RST_FILE_ERR;
    }
    fprintf(fp, "platform=%s\n", platform == PLATFORM_S320?"S320":"S40");
    fclose(fp);

    return OE_RST_OK;
}


void system_reset_system(void)
{
	int rv = 0;

    if (OeAp_Get_Platform() == PLATFORM_S320)
        sleep(1);
    else 
        sleep(3);
    sync();

    rv = system("/sbin/reboot");
} 

int system_set_platform(int slot_id, int platform)
{ 
    int rv = OE_RST_OK;

    if(slot_id == OeAp_Get_My_Slot_Id() + 1){
        if (platform != OeAp_Get_Platform()) {	
            rv = set_board_platform(platform);
            if (rv != 0)
                return rv;

            system_reset_system(); 
        }
    } else if(OeAp_Get_Platform() == PLATFORM_S40)
        rv = OE_RST_INVLD_PRM;

    return rv;
}

int system_set_sysname(char *device_name)
{ 
    int rv = OE_RST_OK;

    if (strlen(device_name) >= MAX_PB_STR_LEN)
        return OE_RST_INVLD_PRM;

    strcpy(g_device_name, device_name);
    g_device_name[strlen(device_name)] = 0;

    return rv;
}

int system_set_datetime(int slot, char *sys_time)
{ 
    struct tm tm;
    time_t time;
    struct timeval timeval;
    char *format1 =  "%Y-%m-%d %H:%M:%S";
    char *format2 =  "%Y/%m/%d %H:%M:%S";
    int rv = OE_RST_OK;

    if (slot == (OeAp_Get_My_Slot_Id() + 1)) {
        if ((NULL == strptime(sys_time, format1, &tm))&&(NULL == strptime(sys_time, format2, &tm))){
            return(rv = OE_RST_INVLD_PRM);
        }
        
        time = mktime(&tm);
        memset(&timeval, 0, sizeof(struct timeval));
        timeval.tv_sec = time;
        rv = settimeofday(&timeval, NULL);
        if (rv != 0)
            rv = OE_RST_INT_ERR;
    }
    
	return rv;
}



int system_set_timezone(int slot, char *timezone)
{
    int rv = OE_RST_OK;
    int tz_hour;
    char *p;
    char tz_buf[16] = {0};
    FILE *fd = NULL;
    
    if (slot == (OeAp_Get_My_Slot_Id() + 1)) {
        /* Parse timezone, such as "yyyy-mm-dd hh:mm:ss GMT+8", and can not be 
         * out of range: GMT-12 ~ GMT+12 */
        if (strncmp(timezone, "GMT", 3) != 0) {
            return OE_RST_INVLD_PRM;
        } else {
            p = timezone + 3;
            tz_hour = atoi(p);
            if (tz_hour < -12 || tz_hour > 12) {
                return OE_RST_INVLD_PRM;
            }
        }

        if (tz_hour < 0) {
            sprintf(tz_buf, "GMT%s%d", "+", -tz_hour);
        } else if (tz_hour > 0) {
            sprintf(tz_buf, "GMT%s%d", "-", tz_hour);
        } else {
            sprintf(tz_buf, "GMT");
        }
        
        setenv("TZ", tz_buf, 1);
        fd = fopen(TIME_ZONE_FILE, "w+");
        if (fd) {
            fprintf(fd, "TZ=%s\r\n", timezone);
            fclose(fd);
        }
    }
    
	return rv;  
}

static int _save_ipconfig(char *iface, int ip, int mask)
{
    char buf[256] = {0};
    FILE *fp;
    char filename[256] = {0};
    struct in_addr in;

    memset(filename, 0, sizeof(filename));
	#ifdef _PLATFORM_X86_UBUNTU_
        sprintf(filename, "/etc/sysconfig/network-scripts/ifcfg-%s",iface);
	#else
    fp = fopen("/configfs/active", "r");
    if (fp == NULL)
        return 0;

    memset(buf, 0, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0') {
            memset(buf, 0, sizeof(buf));
            continue;
        }
    }
    fclose(fp);
    
	if (buf[0] != 0) {
        int len = strlen(buf);
        if (buf[len - 1] == '\n') {
            buf[len - 1] = 0;
        }
        sprintf(filename, "/configfs/%s/sysconfig/network-scripts/ifcfg-%s", buf, iface);
	}

	#endif


    fp = fopen(filename, "w");
    if (fp == NULL)
        return 0;

    fprintf(fp, "DEVICE=\"%s\"\n", iface);      
    in.s_addr = ip;
    fprintf(fp, "IPADDR=%s\n", inet_ntoa(in));
    in.s_addr = mask;
    fprintf(fp, "NETMASK=%s\n", inet_ntoa(in));
    fprintf(fp, "BOOTPROTO=none\n");

    fclose(fp);

    return 0;
}


int system_ipconfig(char *iface, int ip, int mask)
{
    struct in_addr in;
    char cmd[128];
    int len;
    int route_add_flag = 0;

    in.s_addr = ntohl(ip);;

    memset(cmd, 0, sizeof(cmd));

    if (ip){
        sprintf(cmd, "/sbin/ifconfig %s %s ", iface, inet_ntoa(in));
	}
    else{
		fprintf(stderr,"ip is 0");
        sprintf(cmd, "/sbin/ifconfig %s down ", iface);
	}

    if (ip && mask)
    {
        len = strlen(cmd);
        in.s_addr = mask;
        sprintf(cmd + len, "netmask %s ", inet_ntoa(in));
    }

    if (OeAp_Get_Platform() == PLATFORM_S320) {
        len = strlen(cmd);
        sprintf(cmd + len, "down");
    } else {
        route_add_flag = 1;
    }

    if (system(cmd))
    {
        return -1;
    }

    if (route_add_flag == 1) {
        struct in_addr in;
        in.s_addr = system_get_gateway_static();

        if (in.s_addr != 0) {
            char cmd[256];
            memset(cmd, 0, sizeof(cmd));
            sprintf(cmd, "route add default gw %s dev eth0", inet_ntoa(in));
        }
    }

    _save_ipconfig(iface, ip, mask);

    return 0;
}

#define RTF_GATEWAY     0x0002
static int _get_gateway_dynamic(void)
{
    char buf[256];
    int ifl = 0;
    int flgs, ref, use, metric;
    unsigned long int dest, gateway, mask;
    int  rc = 0;
    FILE *fp;

    fp = fopen("/proc/net/route", "r");
    if (fp == NULL)
        return 0;

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        ifl = 0;
        while (buf[ifl] != ' ' && buf[ifl] != '\t' && buf[ifl] != '\0')
            ifl++;
        buf[ifl] = 0;    /* interface */
        if (strcmp(buf, "eth0") != 0)
            continue;

        if (sscanf(buf + ifl + 1, "%16lx%16lx%8X%10d%10d%10d%16lx",
                    &dest, &gateway, &flgs, &ref, &use, &metric, &mask)!=7) {
            continue;
        }

        if (dest == 0 && (flgs & RTF_GATEWAY) != 0) {
            rc = gateway;
        }
    }

    fclose(fp);

    return rc;
}

static int _save_gateway(int ip)
{
    char buf[256];
    FILE *fp;
    char filename[256];
    struct in_addr in;

    in.s_addr = ip;

    fp = fopen("/configfs/active", "r");
    if (fp == NULL)
        return 0;

    memset(buf, 0, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0') {
            memset(buf, 0, sizeof(buf));
            continue;
        }
    }
    fclose(fp);

    if (buf[0] != 0) {
        int len = strlen(buf);
        if (buf[len - 1] == '\n') {
            buf[len - 1] = 0;
        }

        memset(filename, 0, sizeof(filename));
        sprintf(filename, "/configfs/%s/sysconfig/network-scripts/route-cfg", buf);
        fp = fopen(filename, "w");
        if (fp == NULL)
            return 0;

        fprintf(fp, "DESTIP_0=default\n");
        fprintf(fp, "DESTMASK_0=0.0.0.0\n");
        fprintf(fp, "GW_0=%s\n", inet_ntoa(in));
        fprintf(fp, "DEVICE_0=eth0\n");

        fclose(fp);
    }

    return 0;
}

int system_get_gateway_static(void)
{
    char buf[256];
    int  rc = 0;
    FILE *fp;
    char filename[256];

    fp = fopen("/configfs/active", "r");
    if (fp == NULL)
        return 0;

    memset(buf, 0, sizeof(buf));
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0') {
            memset(buf, 0, sizeof(buf));
            continue;
        }
    }
    fclose(fp);

    if (buf[0] != 0) {
        int len = strlen(buf);
        if (buf[len - 1] == '\n') {
            buf[len - 1] = 0;
        }

        memset(filename, 0, sizeof(filename));
        sprintf(filename, "/configfs/%s/sysconfig/network-scripts/route-cfg", buf);
        fp = fopen(filename, "r");
        if (fp == NULL)
            return 0;

        memset(buf, 0, sizeof(buf));
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            if (strncmp(buf, "GW_0=", strlen("GW_0=")) == 0) {
                rc = (int)inet_network(buf + strlen("GW_0="));
                break;
            }
        }
        fclose(fp);
    }

    return rc;
}



int system_gateway(int ip)
{
    char cmd[128]; 
    struct in_addr in;

    //slave om board
    if (OeAp_Get_Platform() == PLATFORM_S320) {
        in.s_addr = system_get_gateway_static();

        if (in.s_addr == ip)
            return 0;
    } else {
        in.s_addr = _get_gateway_dynamic();

        if (in.s_addr == ip)
            return 0;

        if (in.s_addr != 0) {
            if(system("/sbin/route del default 2>/dev/null") != 0){
				fprintf(stderr,"Error:can not del default route\n");
			}
        }

        in.s_addr = ip;
        if (in.s_addr != 0) {
            sprintf(cmd, "/sbin/route add default gw %s dev eth0 2>/dev/null ", inet_ntoa(in));
            if (system(cmd) != 0) {
                fprintf(stderr,"Error:can not set %s as gateway.\n", inet_ntoa(in));
                return -1;
            }
        }
    }

    _save_gateway(ip);

    return 0;
}

int system_opmode_set(int slot_id, int mode)
{
    FILE *fp = NULL;

    if (slot_id == (OeAp_Get_My_Slot_Id() + 1)) {
        fp = fopen("/appfs/oe_opmode", "w");
        if (fp) {
            fprintf(fp, "%d\n", mode);
            fclose(fp);
        } else {
            return OE_RST_FILE_ERR;
        }
    } else if(OeAp_Get_Platform() == PLATFORM_S40)
        return OE_RST_INVLD_PRM;

    return 0;
}

int system_rcload_bcm_config(int slot_id, char *filename)
{
    struct stat f_s; 
    char buf[1024] = {0}; 

    if (slot_id == (OeAp_Get_My_Slot_Id() + 1)) {
        if (access(filename, F_OK) == 0 
                && stat(filename, &f_s) != -1
                && f_s.st_size != 0) { 

            sprintf(buf, "echo -e \"0:\nrcload %s\nexit\"| /usr/local/bcm/bcm.user.proxy 1>/dev/null 2>&1", filename);
            if(system("killall -9 bcm.user.proxy 1>/dev/null 2>&1")){
				fprintf(stderr,"kill bcm.user.proxy fail\n");
			}
            if (system(buf) != 0)
                return OE_RST_INT_ERR;
        } else {
            return OE_RST_FILE_ERR;
        }    
    } else if(OeAp_Get_Platform() == PLATFORM_S40)
        return OE_RST_INVLD_PRM;

    return 0;
}

int system_set_chassis_id(int id)
{
    int rv = 0;
    FILE *fp = NULL;

    fp = fopen("/appfs/chassis_id.conf", "w");
    if (fp) {
        fprintf(fp, "CHASSIS_ID=%d\n", id);
        fclose(fp);
    } else {
        rv = OE_RST_FILE_ERR;
    }

    return rv;
}

int system_set_warm_syn(int able)
{
    char buf[256];
	int rv = 0;

    if(able==0)
    {
        sprintf(buf,"echo syn_from_local=1 > %s \r\n", CONFIG_SYNCONFIG_FILE_NAME);
        rv = system(buf);
    }
    else
    {
        sprintf(buf,"echo syn_from_local=0 > %s \r\n", CONFIG_SYNCONFIG_FILE_NAME);
        rv = system(buf);
    }

    return rv;
}

int set_chassis_mode(int mode)
{   
    FILE *fp = NULL;
	int rv = 0;

    if((mode!=DUAL_STAR_MODE) && (mode!=DUAL_DUAL_STAR_MODE)){
        return OE_RST_INVLD_PRM;
	}
    
	fp = fopen("/appfs/chassis_mode.conf", "w");
    if (fp) {
        fprintf(fp, "CHASSIS_MODE=%d\n", mode);
        fclose(fp);
    } else {
        rv = OE_RST_FILE_ERR;
    }

	return rv;
}
