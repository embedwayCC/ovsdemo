#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

char *varlist[10][3] = {
    { "<slot_id>", "1[0-4]|[1-9]" },
    { "<inports_id>", "[1-9]" },
    { "<outports_id>", "[1-7]" },
    { "<rule_id>", "6[0-4]|[1-5][0-9]|[1-9]" },
};

int variable_match_check(char *var, const char *str)
{
	char *pattern;
	int z, cflags = REG_EXTENDED;
	char ebuf[128];
	regex_t reg;
	regmatch_t pm[10];
	size_t nmatch = 10;
	int i, j;

    for (i = 0; i < 10; i++) {
        if (varlist[i]) {
            if (varlist[i][0] == NULL) {
                return 0;
            }
            if (strcmp(varlist[i][0], var) == 0) {
                pattern = varlist[i][1];
                //printf("pattern:%s\n", pattern);
                z = regcomp(&reg, pattern, cflags);
                if (z != 0) {
                    regerror(z, &reg, ebuf, sizeof(ebuf));
                    //fprintf(stderr, "%s: pattern '%s'\n", ebuf, pattern);
                    return 0;
                }
                //printf("str:%s\n", str);
                z = regexec(&reg, str, nmatch, pm, 0);
                if (z == 0) {
                    //printf("match\n");
                    for (j = 0; j < nmatch && pm[j].rm_so != -1; j++) {
                        char buf[128] = { 0 };
                        strncpy(buf, str+pm[j].rm_so, pm[j].rm_eo-pm[j].rm_so);
                        if (strlen(buf) == strlen(str)) {
                            //printf("true match\n");
                            regfree(&reg);
                            return 0;
                        }
                    }
                    //printf("not true match\n");
                    z = 1;
                } 
#if 0
                else if (z == REG_NOMATCH) {
                    //printf("not match\n");
                }
#endif
                else {
                    //printf("other error\n");
                }
                    
                regfree(&reg);
                return z;
            }
        }
    }
    return 0;
}
