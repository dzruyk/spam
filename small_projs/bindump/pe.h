#ifndef _PE_H_
#define _PE_H_

#pragma pack(push,1)

#define PE_MAGIC	0x00004550

struct dos_hdr {
	unsigned short      magic;	//MZ
	unsigned short      padding[29];
	uint32_t            pe_offset;
};

#define PE_OPT_32 0x010B
#define PE_OPT_64 0x020B
#define PE32_DIRECTORY_SZ 0x80

struct base_header {
	uint32_t magic;		// PE\0\0
	uint16_t machine;
	uint16_t n_sections;
	uint32_t timestamp;
	uint32_t symb_table_ptr;
	uint32_t nsymbols;
	uint16_t opt_header_sz;
	uint16_t charact;
};

#define opt_header_arch(arch_type) 							\
	arch_type image_base;							\
	uint32_t sect_align;	/* Alignment of sections when loaded in RAM */	\
	uint32_t file_align;							\
	uint16_t os_versions[2];						\
	uint16_t img_versions[2];						\
	uint16_t subsys_versions[2];						\
	uint32_t reserved1;							\
	uint32_t image_size;							\
	uint32_t header_size;							\
	uint32_t checksum;							\
	uint16_t subsystem;							\
	uint16_t dll_charact;							\
	arch_type stacksizes[2];						\
	arch_type heapsizes[2];							\
	uint32_t loaderflg;							\
	uint32_t stuff

struct pe32_header {
	struct base_header base_header;
	struct {
		uint16_t opt_magic;
		char linker_version[2];
		uint32_t code_size;	/* Size of code section or sum if multiple code sections */
		uint32_t idata_size;	/* Initialized data size */
		uint32_t udata_size;	/* Unitialized data size */
		uint32_t entry_point;
		uint32_t code_base;
		uint32_t data_base;

		opt_header_arch(uint32_t);
	} optional_header;

	struct {
		char padding[PE32_DIRECTORY_SZ];
	} dir;
};

struct pe64_header {
	struct base_header base_header;
	struct {
		uint16_t opt_magic;
		char linker_version[2];
		uint32_t code_size;	/* Size of code section or sum if multiple code sections */
		uint32_t idata_size;	/* Initialized data size */
		uint32_t udata_size;	/* Unitialized data size */
		uint32_t entry_point;
		uint32_t code_base;

		opt_header_arch(uint64_t);
	} optional_header;

	struct {
		char padding[PE32_DIRECTORY_SZ];
	} dir;
};

struct section_tentry {
	char name[8];
	uint32_t virt_size;
	uint32_t virt_addr;
	uint32_t raw_size;
	uint32_t raw_offset;
};

#pragma pack(pop)

#endif
