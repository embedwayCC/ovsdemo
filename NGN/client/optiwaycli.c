#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readline/readline.h"
#include "readline/history.h"

#include "def.h"
#include "appdefs.h"
#include "nngn_struct.h"
#include "nngn.h"
#include "my_parser.h"

static char *progname;
int console = 0;
int g_end_flag = 0; /*for omc maincli*/

#define PrintEndFlag() {printf("\005\n");}
#define PrintRebootFlag() {printf("\004\n");}

#define EXIT            "exit"
#define SWITCH          "switch"
#define EXITSHELL            "exitshell"
#define RESET_SYSTEM    "reset system"
#define RESTORE_CONFIG  "restore configuration default"
#define IMPORT_CONFIG   "import configuration ftp"
#define SET_PLATFORM    "set platform"
#define RESET_BOARD     "reset board"
#define RESET_SYSTEM_QUIET    "reset system quiet"
#define RESTORE_CONFIG_QUIET  "restore configuration default quiet"
#define TAB_COMPLETION_REQ_HEAD "completion"
#define TAB_COMPLETION_RES_HEAD "matches"
#define TAB_COMPLETION_EMPTY_STRING "#"

int cli_exit_shell(void)
{
    int rv = 0, console = 0;

    if(isatty(0))
    {
        console = ! ((strcmp(ttyname(0), "/dev/ttyS0")) && (strcmp(ttyname(0), "/dev/console")));
    }

    if(console) {
        rv = execl("/bin/login", "login", "root", (char *) 0);
    }

    return rv;
}

int is_abnormal_command(char *buf)
{
    char *cmd_start = NULL;
    char *answer = NULL;
    int i = 0;
    int rv = 0;

    for (i = 0; i < strlen(buf); i++){
        if(isblank(buf[i]) == 0){
            cmd_start = buf+i;
            break;
        }
    }

	if(NULL==cmd_start)
	{
		return -1; 
	}

	if(strncasecmp(cmd_start, EXITSHELL, strlen(EXITSHELL)) == 0) {
        cli_exit_shell();
		exit(0);
	}

	//TODO by caicai
	#if 0
    if(strncasecmp(cmd_start, SWITCH, strlen(SWITCH)) == 0) {
        switch_to_other_cmdline(cmd_start);
        return -1;
    }
	#endif

	if(strncasecmp(cmd_start, EXIT, strlen(EXIT)) == 0) {
		exit(0);
	}

	//TODO by caicai
	#if 0
    if (strncasecmp(cmd_start, TAB_COMPLETION_REQ_HEAD, strlen(TAB_COMPLETION_REQ_HEAD)) == 0) {
        abnormal_cmd_tab_completion(cmd_start);
        return -1;
    }
	#endif
    if(strncasecmp(cmd_start, RESET_SYSTEM_QUIET, strlen(RESET_SYSTEM_QUIET)) == 0) {
        if (g_end_flag) {
            PrintRebootFlag();
        }
        return 0;
    }
    if(strncasecmp(cmd_start, RESTORE_CONFIG_QUIET, strlen(RESTORE_CONFIG_QUIET)) == 0) {
        if (g_end_flag) {
            PrintRebootFlag();
        }
        return 0;
    }
    if(strncasecmp(cmd_start, RESET_SYSTEM, strlen(RESET_SYSTEM)) == 0){
        if (g_end_flag) {
            PrintRebootFlag();
            return 0;
        }
        printf("The system is about to restart.\n");
        printf("Do you want to continue? (y/n): ");
        answer = readline("");
        if(answer[0] == 'y' || answer[0] == 'Y')
            rv = 0;
        else
            rv = -1;
        free(answer);
    } else if(strncasecmp(cmd_start, RESTORE_CONFIG, strlen(RESTORE_CONFIG)) == 0){
        if (g_end_flag) {
            PrintRebootFlag();
            return 0;
        }
        printf("The default configuration is about to be restored, and the board will be restarted\n");
        printf("Do you want to continue? (y/n): ");
        answer = readline("");
        if(answer[0] == 'y' || answer[0] == 'Y')
            rv = 0;
        else
            rv = -1;
        free(answer);
    } else if(strncasecmp(cmd_start, IMPORT_CONFIG, strlen(IMPORT_CONFIG)) == 0){
        if (g_end_flag) {
            return 0;
        }
        printf("This opreate will clear current configuration.\n");
        printf("Do you want to continue? (y/n): ");
        answer = readline("");
        if(answer[0] == 'y' || answer[0] == 'Y')
            rv = 0;
        else
            rv = -1;
        free(answer);
    }else if(strncasecmp(cmd_start, SET_PLATFORM, strlen(SET_PLATFORM)) == 0){
        if (g_end_flag) {
            PrintRebootFlag();
            return 0;
        }
        printf("The board is about to restart.\n");
        printf("Do you want to continue? (y/n): ");
        answer = readline("");
        if(answer[0] == 'y' || answer[0] == 'Y')
            rv = 0;
        else
            rv = -1;
        free(answer);
    }else if(strncasecmp(cmd_start, RESET_BOARD, strlen(RESET_BOARD)) == 0){
        if (g_end_flag) {
            return 0;
        }
        printf("The board is about to restart.\n");
        printf("Do you want to continue? (y/n): ");
        answer = readline("");
        if(answer[0] == 'y' || answer[0] == 'Y')
            rv = 0;
        else
            rv = -1;
        free(answer);
    }

	return rv;
}


int read_line()
{
    int rv = 0;
    char *s = NULL;
	char cmd_buf[CMD_LEN_MAX + 1] = {0};
#if 0
    struct Logo_B logo;
#endif

    using_history();
    stifle_history(20);          
#if 0
    memset(&logo, 0, sizeof(struct Logo_B));
    get_userdef_logo_info(&logo);
    cli_print_logo(&logo);
#endif

    do {
        memset(cmd_buf, 0, sizeof(cmd_buf));
        if (g_end_flag == 0) {
            while(s == NULL || *s == 0) {
				#if 0
                if (logo.cli_prompt[0] != '\0') {
                    s = readline(logo.cli_prompt);
                } else {
                    s = readline("CLI> ");
                }
				#else
                    s = readline("CLI> ");
				#endif
            }
            add_history(s);
            strncpy(cmd_buf, s, CMD_LEN_MAX);
			strcat(cmd_buf, "\n");
            free(s);
            s = NULL;            
        } else {
            char *line = NULL;
            size_t len = 0;
            ssize_t read;

            read = getline(&line, &len, stdin);
            if (read <= 1) {
                if (line != NULL) {
                    free(line);
                    line = NULL;
                }
                if (g_end_flag != 0) {
                    PrintEndFlag();
                }
                continue;
            }
            line[read - 1] = 0;
            add_history(line);
            strncpy(cmd_buf, line, CMD_LEN_MAX);
			strcat(cmd_buf, "\n");
            free(line);
            line = NULL;
        }
		rv = is_abnormal_command(cmd_buf);
		if(rv == 0){
			rv = start_cli_cmd(cmd_buf);
		}

        if (g_end_flag != 0) {
            PrintEndFlag();
        }
    } while(1);
    return rv;
}


int start_cli(char *cmd, FILE *fp)
{
    size_t n = 0;
    int rv = 0;
    char *readbuf = NULL;
	char cmd_buf[CMD_LEN_MAX + 1] = {0};

    if(cmd) {
        //cli -c 
		rv = is_abnormal_command(cmd);
		start_cli_cmd(cmd);
    } else if(fp) {
        //cli -f 
        memset(cmd_buf, 0, sizeof(cmd_buf));
        while(getline(&readbuf, &n, fp) != -1){
            strncpy(cmd_buf, readbuf, CMD_LEN_MAX);
            cmd_buf[strlen(cmd_buf)-1] = 0;

			rv = is_abnormal_command(cmd_buf);
			rv = start_cli_cmd(cmd_buf);
            if (rv != 0) {
                break;
            }
            memset(cmd_buf, 0, sizeof(cmd_buf));
            free(readbuf);
            readbuf = NULL;
        }
        free(readbuf);
        readbuf = NULL;
    } else {
        //cli
        rv = read_line();
    }
    return rv;
}

int get_console(void)
{
    int console = 0;
    
    if (isatty(0)) {
        console = !((strcmp(ttyname(0), "/dev/ttyS0")) && (strcmp(ttyname(0), "/dev/console")));
    }
    
    return console;
}

int main(int argc, char **argv)
{
	int rv = 0;
    int opt;
    char *cmdstr = NULL;
    FILE *fp=NULL;

    console = get_console();
    setlinebuf(stdout);
    progname = basename(argv[0]);

    while ((opt = getopt(argc, argv, "c:f:")) > 0) {
        switch (opt) {
        case 'c':
            cmdstr = strdup(optarg);
            break;
        case 'f':
            if( (fp = fopen( optarg, "r"))== NULL) {
                perror("open");
                exit(1);
            }
            break;
        case 'h':
        default:
            printf("Usage: cli [-c <cmd>] [ -f <file> ]\n");
            exit(0);
            break;
        }
    }

	nclt_new();

	init_readline();
	start_cli(cmdstr, fp);
    
	if(cmdstr) {
        free(cmdstr);
    }

    if(fp) {
        fclose(fp);
    }

	nclt_destroy();
	return rv;
}

