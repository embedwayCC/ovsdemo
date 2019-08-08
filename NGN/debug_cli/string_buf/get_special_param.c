#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "get_special_param.h"
#include "look_up.h"

int get_slot_num(unsigned long long slot_mask)
{
	int i = 0;
	int num = 0;

	for(i = 0; i < MAX_SLOT_NUM; i++){
		if(slot_mask & (1 << i)){
			num++;
		}
	}
	
	return num;
}

int str2map(char *s, unsigned long long *map)
{
	char *ret;
	char buf[80] = {0};
	char *str = buf;
	int a,b,i,tmp;

	if (!s)
		return 1;

	strcpy(buf,s);
	*map = 0;

	while (1) {
		ret = (char *)strsep(&str,",");
		if (ret == NULL)
			break;

		if (strlen(ret) > 2) {
			if (sscanf(ret, "%d-%d", &a, &b) != 2)
				return 1;
			if(a > b){
				tmp = a;
				a = b;
				b = tmp;
			}
			if ((a > b) || (b > 32) || (a < 1))
				return 1;

			for(i = a; i <= b; i++)
			*map |= 1 << (i - 1);
		} else if (strlen(ret) == 1) {
			if ((ret[0] == 'r') || (ret[0] == 'R'))
				*map |= 1 << MODULE_REAR;
			else if ((ret[0] == 'f') || (ret[0] == 'F'))
				*map |= 1 << MODULE_FRONT;
			else if ((ret[0] == 'b') || (ret[0] == 'B'))
				*map |= 1 << MODULE_BACKPLANE;
			else if ((ret[0] == '*'))
				*map |= ALL_PORT_MASK;
			else {
				a = atoi(ret);
				if ((a > 32) || (a < 1))
					return 1;
				*map |= 1 << (a - 1);
			}
		} else if (strlen(ret) == 2) {
			a = atoi(ret);
			if ((a > 32) || (a < 1))
				return 1;
			*map |= 1 << (a - 1);
		}
	}

	return 0;
}




int get_key_mask(string_buf *buf,char *val_str, int l)
{
	char *data = NULL;
	char *mask = NULL;
	char *tmp_str = NULL;
	int equal_num = 0, num = 0;
	int mask_num = 0, size_num = 0;

	mask_num = find_str_times(val_str,'/',l);
	size_num = find_str_times(val_str,'-',l);
	num = find_str_times(val_str,':',l);

	if(mask_num == 0 && size_num == 0){
		if(num >= 5){
			string_buf_puts_format(buf, "\"%.*s\"", l, val_str);
		}else{
			string_buf_puts_format(buf, "%.*s", l, val_str);
		}
	}else{
		data = (char *)malloc(CLI_MAX_STR_LEN);
		mask = (char *)malloc(CLI_MAX_STR_LEN);
		tmp_str = (char *)malloc(CLI_MAX_STR_LEN);
		memset(data,0,CLI_MAX_STR_LEN);
		memset(mask,0,CLI_MAX_STR_LEN);
		memset(tmp_str,0,CLI_MAX_STR_LEN);
		memcpy(tmp_str,val_str,l);

		equal_num = sscanf(tmp_str,"%[^/]/%s",data,mask);
		if(equal_num != 2){
			equal_num = sscanf(tmp_str,"%[^-]-%s",data,mask);
			if(equal_num != 2){
				return -1;
			}
		}
		fprintf(stderr,"line:%d func:%s data:%s mask:%s num:%d\n",__LINE__,__func__,data,mask,num);
		if(num >= 5){
			string_buf_puts_format(buf,"[\"%s\",\"%s\"]",data,mask);
		}else{
			string_buf_puts_format(buf,"[%s,%s]",data,mask);
		}

		if(data){
			free(data);
			data = NULL;
		}
		if(mask){
			free(mask);
			mask = NULL;
		}
		if(tmp_str){
			free(tmp_str);
			tmp_str = NULL;
		}
	}

	return 0;
}

int get_smp_val(string_buf *buf,char *val_str, int l, unsigned long long *slot_mask)
{
	unsigned long long slot = 0, module = 0, port = 0;
	char *tmp_str = NULL;
	char *s_str = NULL, *m_str = NULL, *p_str = NULL; 

	tmp_str = (char *)malloc(CLI_MAX_STR_LEN);
	memcpy(tmp_str,val_str,l);

	s_str = (char *)strsep(&tmp_str,"/");
	m_str = (char *)strsep(&tmp_str,"/");
	p_str = (char *)strsep(&tmp_str,"/");

	str2map(s_str,&slot);
	str2map(m_str,&module);
	str2map(p_str,&port);

	string_buf_puts_format(buf,"%lld,%lld,%lld",slot,module,port);
	*slot_mask = slot;

	if(tmp_str){
		free(tmp_str);
	}

	return 0;
}
