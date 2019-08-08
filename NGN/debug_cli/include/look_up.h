#ifndef LOOK_UP_H
#define LOOK_UP_H

#define UD_NUM_SOURCE 128

typedef struct {
	char *key_name;
	int key_val;
} spec_key_t;

char *find_word(char *c, char **start, char **end, int *token );
int get_int2_key(char *key, int l);
int find_str_times(char*, char, int);

#endif
