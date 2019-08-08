#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "my_parser.h" 

int get_history_number(char **history_list)
{
    int i = 0;
    int matched_number = 0;
	char *p = NULL;
    char *substr = NULL;
    static char g_rl_str[MAX_BUF_SIZE] = { 0 };

    if (history_list == NULL) {
        return 0;
    }

    memset(g_rl_str, 0, sizeof(char) * MAX_BUF_SIZE);
	strncpy(g_rl_str, rl_line_buffer, MAX_BUF_SIZE);

    for (i = 0, p = g_rl_str; ;i++, p = NULL) {
        substr = strtok(p, " \t\n");
        if (NULL == substr){
            break;
        } else {
            history_list[i] = substr;
        }
    }
	matched_number = i;
    
    return matched_number;
}

int token_is_variable(char c)
{
    if (c < 'a' || c > 'z') {
        return 1;
    }

    return 0;
}

int get_history_matched_number(int index, int history_number, char **history_list, int *at_end)
{
    int i = 0;
    int j = 0;
    int matched_number = 0;

    *at_end = 0;

    for (i = 0; i < history_number; i++) {
        if (strncmp(history_list[i], helplist[index][i+1], strlen(helplist[index][i+1])) == 0) {
            /* keyword match */
            matched_number++;
        } else if (strncmp(history_list[i], helplist[index][i+1], strlen(history_list[i])) == 0) {
            /* partly matched keywork */
            matched_number++;
            if (matched_number == atoi(helplist[index][0])) {
                *at_end = 1;
            }
        } else if (token_is_variable(helplist[index][i+1][0])) {
            /* for variable, not keyword */
            /* before this position is all matched */
            if (matched_number == i) {
                if (tokenlist[history_number-1]) {
                    if (tokenlist[history_number-1][0]) {
                        int token_len = atoi(tokenlist[history_number-1][0]);
                        for (j = 0; j <token_len; j++) {
                            if (strncmp(history_list[i], tokenlist[history_number-1][j+1], strlen(history_list[i])) == 0) {
                                break;
                            }
                        }
                        if (j == token_len) {
                            matched_number++;
                        }
                    }
                }
            }
        }
    }
    return matched_number;
}

char *command_generator(const char *text, int state)
{
	static int index = 0;
    static int end = 0;
    static int help_matched = 0; 
    static char *help_matched_list[MAX_CMD_NUM] = { 0 };
    static int history_number = 0;
	static char *history_list[MAX_CMD_WORD_NUM] = { 0 };

    int i = 0;
    int j = 0;
    char *token = NULL;
    int at_end = 0;
    int list_index = 0;
    int matched_number = 0;
	
	if (!state) {
		index = 0;
        end = 0;
        help_matched = 0;
        memset(help_matched_list, 0, sizeof(char *) * MAX_CMD_NUM);

        memset(history_list, 0, sizeof(char *) * MAX_CMD_WORD_NUM);
        history_number = get_history_number(history_list);
	}

    if (strcmp(text, "") == 0) {
        list_index = history_number + 1;
    } else {
        list_index = history_number;
    }

    while (helplist[index]) {
        if (helplist[index][0]) {
            if (atoi(helplist[index][0]) >= history_number) {
				matched_number = get_history_matched_number(index, history_number, history_list, &at_end);
                if (matched_number == atoi(helplist[index][0]) && at_end == 0) {
                    token = helplist[index][list_index];
                    if (!end && strcmp(text, "") != 0) {
                        end = 1;
                        if (token_is_variable(token[0])) {
                            if (variable_match_check(token, text) == 0) {
                                if (token[0] == '[') {
                                    /* FIXME: not safe */
                                    char buf[16] = { 0 };
                                    strncpy(buf, token+1, strlen(token)-2);
                                    if (strncmp(text, buf, strlen(text)) == 0) {
                                        return strdup(buf);
                                    } else {
                                        return NULL;
                                    }
                                }
                                return strdup("");
                            } else {
                                return NULL;
                            }
                        } else {
                            return strdup("");
                        }
                    } else if (!end && token && token_is_variable(token[0])) {
                        end = 1;
                        return strdup("");
                    } else {
                        return NULL;
                    }
                }

                if (list_index - matched_number <= 1) {
                    token = helplist[index][list_index];
                    if (token == NULL) {
                        index++;
                        return NULL;
                    }
                    if (strncasecmp(text, token, strlen(text)) == 0 || token_is_variable(token[0])) {
                        help_matched_list[help_matched] = token;
                        for (i = 0; i < help_matched; i++) {
                            if (strcmp(help_matched_list[i], token) == 0) {
                                help_matched_list[help_matched] = NULL;
                                break;
                            } else if (token_is_variable(token[0]) && strcmp(text, "") != 0 &&
                                    strncmp(text, help_matched_list[i], strlen(text)) == 0) {
                                break;
                            }
                        }
                        if (token_is_variable(token[0]) && strcmp(text, "") != 0 && help_matched == 0) {
                            if (tokenlist[history_number-1]) {
                                if (tokenlist[history_number-1][0]) {
                                    int token_len = atoi(tokenlist[history_number-1][0]);
                                    for (j = 0; j <token_len; j++) {
                                        if (strncmp(text, tokenlist[history_number-1][j+1], strlen(text)) == 0) {
                                            i++;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        if (i == help_matched) {
                            if (token_is_variable(token[0]) && strcmp(text, "") != 0) {
                                if (variable_match_check(token, text) == 0) {
                                    index++;
                                    help_matched++;
                                    return strdup("");
                                }
                            } else if (matched_number == history_number) {
                                index++;
                                help_matched++;
                                return strdup(token);
                            }
                        }
                    }
                }
            }
            index++;
        } else {
            break;
        }
    }

	return (char *)NULL;
}

char **cmd_completion(const char *text, int start, int end)
{
	char **matches = rl_completion_matches(text, command_generator);

	return matches;
}

void init_readline()
{      
	rl_readline_name = "";
    /* replace the default rl_filename_completion_function */
	rl_completion_entry_function = command_generator;
    /* our match function */
	rl_attempted_completion_function = cmd_completion;
}
