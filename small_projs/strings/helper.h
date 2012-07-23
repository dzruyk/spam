#ifndef __HELPER_H__
#define __HELPER_H__

enum stat_t {
	stat_ok = 0,
	stat_err = 1,
	stat_eof = 2,
};

stat_t get_new_line(char *s, int sz, FILE *fp);

#endif

