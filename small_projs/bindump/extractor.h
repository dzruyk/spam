#ifndef _EXTRACTOR_H_
#define _EXTRACTOR_H_


int get_exe(void *ptr, uint32_t maxsz);

int get_elf(void *ptr, uint32_t maxsz);

int get_http(void *ptr, uint32_t maxsz);

#endif
