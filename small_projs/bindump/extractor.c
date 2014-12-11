#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "extractor.h"
#include "log.h"
#include "pe.h"
#include "util.h"

//TODO: not portable

#define MAX_SECTIONS 42

inline int
get_pe_offset(void *ptr, uint32_t maxsz)
{
	struct dos_hdr *dos;

	dos = ptr;

	if (maxsz < sizeof(*dos))
		return -1;

	if (dos->pe_offset > 0x1000) // sanity check
		return -1;

	return dos->pe_offset;
}

/*
 * Parse pe header, and try to calculate PE size using maximum section data offset.
 *
 */
//TODO: borders check?
static int
get_pe_size(void *peptr, uint32_t maxsz, int pe_header_sz)
{
	struct pe32_header *pe;
	int pesz, i, n;

	pesz = 0;
	pe = peptr;
	n = pe->base_header.n_sections;

	DEBUG(LOG_DEFAULT, "%p n sections %d\n", peptr, n);

	if (n > MAX_SECTIONS)
		return 0;

	for (i = 0; i < n; i++) {
		struct section_tentry *t;
		int tmp, off;

		off = pe_header_sz + (i * sizeof(*t));
		DEBUG(LOG_DEFAULT, "secion offset %x\n", off);

		if (off + sizeof(*t) > maxsz)
			return 0;

		t = peptr + off;
		tmp = t->raw_offset + t->raw_size;

		if (tmp > pesz)
			pesz = tmp;
	}

	return pesz;
}

int
get_exe(void *ptr, uint32_t maxsz)
{
	struct pe32_header *pe;
	char *sig = "MZ";
	char *p;
	int i, pesz, peoff;

	p = ptr;

	//TODO How we handle this under encoding?
	for (i = 0; i < strlen(sig); i++) {
		if (sig[i] != p[i])
			return 0;
	}

	peoff = get_pe_offset(ptr, maxsz);
	if (peoff <= 0 || peoff + sizeof(*pe) >= maxsz)
		return 0;

	pe = ptr + peoff;
	
	if (pe->base_header.magic != PE_MAGIC)
		return 0;

	printf("offset %x\n", peoff);

	if (pe->optional_header.opt_magic != PE_OPT_32) {
		DEBUG(LOG_DEFAULT, "pe32\n");
		pesz = get_pe_size(pe, maxsz - peoff, sizeof(struct pe32_header));
	} else if (pe->optional_header.opt_magic != PE_OPT_64) {
		DEBUG(LOG_DEFAULT, "pe32+\n");
		pesz = get_pe_size(pe, maxsz - peoff, sizeof(struct pe64_header));
	} else {
		DEBUG(LOG_DEFAULT,
		    "unsupported pe version %x (optional magic mismatch)\n",
		    pe->optional_header.opt_magic);
		return 0;
	}
	
	if (pesz > maxsz)
		pesz = maxsz;

	DEBUG(LOG_DEFAULT, "pesz = %x\n", pesz);

	return pesz;
}

int
get_elf(void *ptr, uint32_t maxsz)
{
	return 0;
}

static int
parse_tok(char *buf, int sz)
{
	int i;
	static char *delims = " \t\n\r><";

	for (i = 0; i < sz; i++, buf++) {
		if (*buf == '\0' || strchr(delims, *buf) != NULL)
			break;
		if (isprint(*buf) == FALSE)
			break;
	}

	return i;
}

struct string {
	char *s;
	int sz;
};

#define STR_INIT(s)	{s, sizeof(s) - 1}

int
get_http(void *ptr, uint32_t maxsz)
{
	int i;
	int len, off;
	char *p;

	//FIXME: Optimize me
	struct string pref[] = {
	    STR_INIT("http://"),
	    STR_INIT("https://")
	};

	p = ptr;
	off = 0;

	for (i = 0; i < ARRSZ(pref); i++) {
		if (maxsz < pref[i].sz)
			return 0;
		if (memcmp(p, pref[i].s, pref[i].sz - 1) == 0) {
			off += pref[i].sz;
			break;
		}
	}

	if (off == 0)
		return 0;

	len = parse_tok(p + off, maxsz - off);

	if (len < 4)		//for example ze.ee
		return 0;

	return off + len;
}

