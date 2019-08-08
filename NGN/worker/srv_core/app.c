#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "cliportmap.h"
#include "nngn.h"

static num_str_t platform_table[] = {
	{PLATFORM_S320, "S320"},
	{PLATFORM_S40, "S40"},
};

static bcm_port_map_t g_bcm_port_map[] = {
    //SF3600 A3
    //F：20*10G B：9*40G + 4*20G
    {    
        BOARD_TYPE_SF3600,
        -1,  
        MODULE_FRONT,
        BCM_UNIT0,
        24,   
        0,
        {0},
        {0}
    },
    {
        BOARD_TYPE_SF3600,
        -1,  
        MODULE_BACK,
        BCM_UNIT0,
        56,   
        0,
        {-1},
        {0}
    },
    //F：20*10G B：9*40G + 4*20G
    {    
        BOARD_TYPE_SF3600_SW,
        -1,  
        MODULE_FRONT,
        BCM_UNIT0,
        24,   
        0,
        {0},
        {0}
    },
    {
        BOARD_TYPE_SF3600_SW,
        -1,  
        MODULE_BACK,
        BCM_UNIT0,
        56,   
        0,
        {-1},
        {0}
    },
    //F:16*10G     B:10*40G + 3*20G
	{
		BOARD_TYPE_SF3600_F16,
		-1,
		MODULE_FRONT,
		BCM_UNIT0,
		24,
		0,
		{0},
		{0}
	},
	{
		BOARD_TYPE_SF3600_F16,
		-1,
		MODULE_BACK,
		BCM_UNIT0,
		56,
		0,
		{-1},
		{0}
	},
    //ST3600
	{
		BOARD_TYPE_ST3600,
		-1,
		MODULE_FRONT,
		BCM_UNIT0,
		24,
		0xffffff,
		{25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48},
		{0}
    },
    {
		BOARD_TYPE_ST3600,
		-1,
		MODULE_BACK,
		BCM_UNIT0,
		24,
		0xffffff,
		{21,22,23,24,17,18,19,20,13,14,15,16,9,10,11,12,5,6,7,8,1,2,3,4},
		{0}
    },
    //ExTAP5100
	{
		BOARD_TYPE_ET5100,
		-1,
		MODULE_FRONT,
		BCM_UNIT0,
		24,
		0xffffff,
		{25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48},
		{0}
    },
    //AC1200
    {
         BOARD_TYPE_AC1200,
         -1,
         MODULE_FRONT,
         BCM_UNIT1,
         24,
         0xffffff,
         { 1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14, 17, 16, 19, 18, 21, 20, 23, 22},
         {0}
     },
    {
         BOARD_TYPE_AC1200,
         -1,
         MODULE_REAR,
         BCM_UNIT0,
         24,
         0xffffff,
         { 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
         {0}
     },
     {
         BOARD_TYPE_AC1200,
         -1,
         MODULE_BACK,
         BCM_UNIT1,
         1,
         0x1,
         {27},
         {0}
     },
    //Exprobe 70xx
    {
        BOARD_TYPE_EP7000,
        -1,
        MODULE_FRONT,
        BCM_UNIT0,
        56,
        0x0,
        {0},
        {0}
    },
    {
        BOARD_TYPE_EP7056,
        -1,
        MODULE_FRONT,
        BCM_UNIT0,
        50,
        0x3ffffffffffffllu,
        {0},
        {0}
    },
    {
        BOARD_TYPE_EP7048,
        -1,
        MODULE_FRONT,
        BCM_UNIT0,
        48,
        0xffffffffffffllu,
        {0},
        {0}
    },
    {
        BOARD_TYPE_EP7032,
        -1,
        MODULE_FRONT,
        BCM_UNIT0,
        26,
        0x3ffffff,
        {0},
        {0}
    },
    {
        BOARD_TYPE_EP7024,
        -1,
        MODULE_FRONT,
        BCM_UNIT0,
        24,
        0xffffff,
        {0},
        {0}
    },
    {
		BOARD_TYPE_AC3800,
		-1,
		MODULE_FRONT,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_AC3800,
		-1,
		MODULE_BACK,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_AC3800,
		-1,
		MODULE_REAR,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },    
    {
		BOARD_TYPE_AC2820,
		-1,
		MODULE_FRONT,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_AC2820,
		-1,
		MODULE_BACK,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_AC2820,
		-1,
		MODULE_REAR,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_AC4300,
		-1,
		MODULE_FRONT,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_AC4300,
		-1,
		MODULE_BACK,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_AC4300,
		-1,
		MODULE_REAR,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_SF4800,
		-1,
		MODULE_FRONT,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_SF4800,
		-1,
		MODULE_BACK,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },
    {
		BOARD_TYPE_SF4800,
		-1,
		MODULE_REAR,
		BCM_UNIT0,
		MAX_PORT_NUMBER,
		0x0,
		{},
    },

};


int OeAp_Get_My_Slot_Id(void)
{
	int slot = 0;
	char buf[80];
	FILE * fp;
	if ((fp= fopen(SYS_INFO_FILE, "r"))!=NULL) {
		while (fgets(buf,sizeof(buf),fp) != NULL) {
			if (strncmp(buf, "SLOT=", 5) == 0) {
				sscanf(buf, "SLOT=%2d", &slot);
			}
		}
		fclose(fp);
	}

	return slot - 1;
}

int OeAp_Get_Platform(void)
{
	FILE *fp;
	char plat_str[128] = {0};
	char buf[128];
	int platform;

    fp = fopen(STARTUP_CONFIG_FILE_NAME, "r");
    if (fp) {
        while (fgets(buf,sizeof(buf),fp) != NULL) {
		    if (strncmp(buf, "platform=", 9) == 0) {
				sscanf(buf, "platform=%s", plat_str);
                platform = GetNo(platform_table, plat_str, sizeof(platform_table) / sizeof(platform_table[0]));
                if (platform == INVALID_TABLE_NUMBER)
                    platform = PLATFORM_S40;
			}
        }
        fclose(fp);
    } else {
        platform = PLATFORM_S40;
    }

	return platform;
}

num_str_t board_type_table[] = {
    {BOARD_TYPE_AC1200, "AC1200"},
    {BOARD_TYPE_SF3600, "SF3600"},
    {BOARD_TYPE_SF3600_SW, "SF3600_SW"},
    {BOARD_TYPE_SF3600_F16, "SF3600_F16"},
    {BOARD_TYPE_ST3600, "ST3600"},
    {BOARD_TYPE_ET5100, "ET5100"},
    {BOARD_TYPE_EP7000, "EP7000"},
    {BOARD_TYPE_EP7024, "EP7024"},
    {BOARD_TYPE_AC3800, "AC3800"},
    {BOARD_TYPE_EP7032, "EP7032"},
    {BOARD_TYPE_EP7048, "EP7048"},
    {BOARD_TYPE_EP7056, "EP7056"},
    {BOARD_TYPE_SF4800, "SF4800"}, 
    {BOARD_TYPE_AC2820, "AC2820"},
    {BOARD_TYPE_AC4300, "AC4300"},
};


static int _get_type_from_file(void)
{
    char buf[256];
    FILE* fp=NULL;
    int type=INVALID_TABLE_NUMBER;
    int itable=0;
    int size=sizeof(board_type_table) / sizeof(board_type_table[0]);
    char* pCurrent_name=NULL;
    int   current_type=0;

    if ((fp = fopen(BOARD_CONF_FILE, "r")) != NULL)
    {
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            if (strncmp(buf, "BOARD_TYPE=", 11) == 0)
            {
                sscanf(buf, "BOARD_TYPE=%s", buf);
                for(itable=0; itable<size; itable++)
                {
                    pCurrent_name=board_type_table[itable].str;
                    current_type =board_type_table[itable].No;
                    if(strncmp(buf,pCurrent_name,strlen(buf))==0)
                    {
                        type=current_type;
                        break;
                    }
                }
            }
        }
        fclose(fp);
    }

    return type;
}



static int _get_str_type(char *major_str, char *minor_str, char *cpu_major_str, char *cpu_minor_str, int opmode)
{
	int type = INVALID_TABLE_NUMBER;

	num_str_t minor_board_type_table[] = {
		{BOARD_TYPE_SF3600, "102"},
		{BOARD_TYPE_ST3600, "401"},
		{BOARD_TYPE_ET5100, "501"},
	};
	
	num_str_t major_board_type_table[] = {
		{BOARD_TYPE_SF3600, "2202"},
		{BOARD_TYPE_AC1200, "2016"},
	};

   num_str_t cpu_board_type_table[] = {
        {BOARD_TYPE_EP7056, "607"},
        {BOARD_TYPE_EP7056, "2607"},
        {BOARD_TYPE_EP7056, "707"},
        {BOARD_TYPE_EP7056, "2707"},
        {BOARD_TYPE_EP7048, "e03"},
        {BOARD_TYPE_EP7048, "2e03"},
        {BOARD_TYPE_EP7048, "f03"},
        {BOARD_TYPE_EP7048, "2f03"},
        {BOARD_TYPE_EP7032, "1604"},
        {BOARD_TYPE_EP7032, "1704"},
        {BOARD_TYPE_EP7024, "1e00"},
        {BOARD_TYPE_EP7024, "3e00"},
        {BOARD_TYPE_EP7024, "1f00"},
        {BOARD_TYPE_EP7024, "3f00"},
    };

   //Get board type from /appfs/board.info first
    type=_get_type_from_file();
    if(type!=INVALID_TABLE_NUMBER)
    {
        return type;
    }

    //if NOT found, then via a traditional way
    type = GetNo(major_board_type_table, major_str, sizeof(major_board_type_table) / sizeof(major_board_type_table[0]));
    if (type == BOARD_TYPE_SF3600) {
        type = GetNo(minor_board_type_table, minor_str, sizeof(minor_board_type_table) / sizeof(minor_board_type_table[0]));
        if (type == BOARD_TYPE_SF3600 && opmode == 1)
            type = BOARD_TYPE_SF3600_F16;
        if (type == BOARD_TYPE_SF3600 && opmode == 2)
            type = BOARD_TYPE_SF3600_SW;
    } else if (!strncmp(cpu_major_str, "4728", 4)) {
        type = GetNo(cpu_board_type_table, cpu_minor_str, sizeof(cpu_board_type_table) / sizeof(cpu_board_type_table[0]));
        if (type == INVALID_TABLE_NUMBER)
            type = BOARD_TYPE_EP7000;
    } else if (!strncmp(major_str, "2200", 4)) {
        if(!strncmp(minor_str, "1101", 4))
            type = BOARD_TYPE_AC3800;
        else    
            type = BOARD_TYPE_AC2820;
    }

    if (type == INVALID_TABLE_NUMBER)
        type=BOARD_TYPE_UNKNOWN;

	return type;
}


board_type_t OeAp_Get_My_Board_Type(void)
{
	char buf[MAX_STRING_LEN];
	char major_board_type[7] = {0};
	char minor_board_type[7] = {0};
	char cpu_major_board_type[7] = {0};
	char cpu_minor_board_type[7] = {0};
    int opmode = 0;
	
    FILE *fp;
	memset(buf, 0, sizeof(buf));
	if ((fp = fopen(SYS_INFO_FILE, "r")) != NULL) {
		while (fgets(buf,sizeof(buf),fp) != NULL) {
			if (strncmp(buf, "FB_MAJOR_BOARD_TYPE=", 20) == 0) {
				sscanf(buf, "FB_MAJOR_BOARD_TYPE=%s", major_board_type);
			}
			if (strncmp(buf, "FB_MINOR_BOARD_TYPE=", 20) == 0) {
				sscanf(buf, "FB_MINOR_BOARD_TYPE=%s", minor_board_type);
			}
            if (strncmp(buf, "CPU_MAJOR_BOARD_TYPE=", 21) == 0) {
                sscanf(buf, "CPU_MAJOR_BOARD_TYPE=%s", cpu_major_board_type);
            }
            if (strncmp(buf, "CPU_MINOR_BOARD_TYPE=", 21) == 0) {
                sscanf(buf, "CPU_MINOR_BOARD_TYPE=%s", cpu_minor_board_type);
            }
            if (strncmp(buf, "OPMODE=", 7) == 0) {
                sscanf(buf, "OPMODE=%10d", &opmode);
            }
		}
		fclose(fp);
	}
	
	return (board_type_t)_get_str_type(major_board_type, minor_board_type, cpu_major_board_type, cpu_minor_board_type, opmode);

}

int OeAp_Get_Bcm_Unit_Port(bcm_port_map_t *bpm, int module, int port, int *bcm_unit, int *bcm_port)
{
    int flag = 0, rv = 0, unit = 0;
	int my_slot_id = 0,my_board_type = 0;

	my_slot_id = OeAp_Get_My_Slot_Id();
	my_board_type = OeAp_Get_My_Board_Type();

    if (my_board_type == BOARD_TYPE_AC1200) {
        if (module == MODULE_BACK) {
            if (port == my_slot_id)
                return -1;
            port = port > my_slot_id?port-1:port;
            if (bpm->board_type == BOARD_TYPE_AC1200) {
                flag = port >= bpm->port_number?1:0;
                port = (port >= bpm->port_number?port-1:port);
                unit = 0;
            }
        }
    }

    if((port < bpm->port_number) && (bpm->port_map[port] != -1)) {
        *bcm_unit = (flag==0?bpm->unit:unit);
        if (BOARD_TYPE_AC1200 == bpm->board_type
            && module == MODULE_BACK) {
            *bcm_port = AC1200_TO_S1_S2;
        } else {
            *bcm_port = bpm->port_map[port];
        }
        rv = 0;
    } else {
        rv = -1;
    }

    return rv;
}


bcm_port_map_t *OeAp_Get_Bcm_Portmap_By_Module(int module)
{
    int i;
	int my_board_type = 0;

	my_board_type = OeAp_Get_My_Board_Type();

    for (i= 0; i < sizeof(g_bcm_port_map)/sizeof(g_bcm_port_map[0]); i++) {
        if (my_board_type == g_bcm_port_map[i].board_type 
                && module == g_bcm_port_map[i].module)
            return (bcm_port_map_t *)&g_bcm_port_map[i];
    }

    return NULL; /*port is not valid*/
}

bcm_port_map_t *OeAp_Get_Bcm_Portmap_By_Sm(int slot, int module)
{
    int i;
	int my_board_type = 0;

	my_board_type = OeAp_Get_My_Board_Type();

    for (i= 0; i < sizeof(g_bcm_port_map)/sizeof(g_bcm_port_map[0]); i++) {
        if (my_board_type == g_bcm_port_map[i].board_type 
                && module == g_bcm_port_map[i].module)
            return (bcm_port_map_t *)&g_bcm_port_map[i];
    }

    return NULL; /*port is not valid*/
}


static int port_init_by_module(int module)
{
	int i;
    int tmp_num;
    int tmp_module;
    unsigned long long tmp_mask;
	struct dport_s dport;
	struct uport_s uport;
	bcm_port_map_t *bpm = NULL;

	bpm = OeAp_Get_Bcm_Portmap_By_Module(module);
	if (bpm == NULL) {
		return 0;
	}
    
	if (cli_portmap_load_config(0, 0) != 0) {
	    return -1;
    }

    /* 0:front,	1:back,	2:rear */
    tmp_module = module == MODULE_FRONT ? 0 : (module == MODULE_BACK ? 1 : 2);
    tmp_num = bpm->port_number;
    tmp_mask = bpm->port_mask;
	for (i = 0; i < MAX_PORT_NUMBER; i++) {
		memset(&dport, 0, sizeof(struct dport_s));
		memset(&uport, 0, sizeof(struct uport_s));
		dport.port_type = PORT_TYPE_XE;
		dport.slot = 0;
		dport.module = tmp_module;
		dport.port = i + 1;
		if (cli_portmap_d2u_port(&dport, &uport) == 0) {
		    bpm->port_map[i] = uport.uport_id;
		    bpm->port_mask |= (1ull << i);
			bpm->port_number++;
		} else {
			bpm->port_map[i] = -1;
            if (module == MODULE_BACK) {
                bpm->port_number++;
            }
		}
	}

    //add by wuxiaoyu, 2017-1-13
    if((module==MODULE_REAR) && (OeAp_Get_My_Board_Type()==BOARD_TYPE_AC3800))
    {
        bpm->port_number=16;
        for(i=16; i<MAX_PORT_NUMBER; i++)
        {
            bpm->port_map[i] = -1;
        }
    }

    /* Just resave accorading to static port number */
    board_type_t bd_type = OeAp_Get_My_Board_Type();
    if (bd_type == BOARD_TYPE_EP7000 || bd_type == BOARD_TYPE_EP7024
        || bd_type == BOARD_TYPE_EP7032 || bd_type == BOARD_TYPE_EP7048
        || bd_type == BOARD_TYPE_EP7056) {
        if (module == MODULE_FRONT) {
            bpm->port_number = tmp_num;
            bpm->port_mask = tmp_mask;
        }
    }

	cli_portmap_close_config();
	return 0;
}


int port_init()
{
    int ret = 0;
    board_type_t bd_type = OeAp_Get_My_Board_Type();

    if (bd_type != BOARD_TYPE_EP7000 && bd_type != BOARD_TYPE_EP7024
        && bd_type != BOARD_TYPE_EP7032 && bd_type != BOARD_TYPE_EP7048
        && bd_type != BOARD_TYPE_EP7056 && bd_type != BOARD_TYPE_AC3800
        && bd_type != BOARD_TYPE_AC2820
        && bd_type != BOARD_TYPE_ET5100
        && bd_type != BOARD_TYPE_SF4800
        && bd_type != BOARD_TYPE_AC4300) {
        return 0;
    }

	if ((ret = port_init_by_module(MODULE_FRONT)) != 0) {
		return ret;
	}

	if ((ret = port_init_by_module(MODULE_BACK)) != 0) {
		return ret;
	}

	if ((ret = port_init_by_module(MODULE_REAR)) != 0) {
		return ret;
	}
	
	return 0;
}

static void  handler_fn(int signal_value)
{
	switch(signal_value)
	{
		case SIGTERM:
			nsrv_destroy();
			close_om_pipe_line();
			close_worker_pipe_line();
			exit(0);
			break;
		case SIGINT:
			nsrv_destroy();
			close_om_pipe_line();
			close_worker_pipe_line();
			exit(0);
			break;
		default:
			break;
	}
}

int  setup_fault_trap(void)
{
	int rv = 0;
	sigset_t new,old,wait;
	struct sigaction act;

	act.sa_handler = handler_fn;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTERM, &act, 0);
	sigaction(SIGUSR1, &act, 0);

	sigemptyset(&new);
	sigaddset(&new, SIGINT);
	sigaddset(&new, SIGTERM);
	sigemptyset(&wait);
	sigaddset(&wait, SIGUSR1);
	sigprocmask(SIG_BLOCK, &new, &old);
	if(sigsuspend(&wait) != -1){
		printf("sigsuspend error");
		rv = -1;
	}
	sigprocmask(SIG_SETMASK, &old, NULL);

	return rv;
}


