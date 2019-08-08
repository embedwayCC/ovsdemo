#ifndef JS_H
#define JS_H

int json2msgpack(char *input, char *out_msg_data, int *len);
int msgpack2json(char *recv_data, int);

#endif
