#include <inttypes.h>
#include <linux/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ioctl.h>

#define QUERY_FLAG		0x80000000
#define BIND_FLAG		0xC0000000
#define PARAMETER_FLAG		0x40000000

#define NVME_IOCTL_IO_CMD	_IOWR('N', 0x43, struct nvme_passthru_cmd)


void* ioctl_buf;

struct nvme_passthru_cmd {
	__u8	opcode;
	__u8	flags;
	__u16	rsvd1;
	__u32	nsid;
	__u32	cdw2;
	__u32	cdw3;
	__u64	metadata;
	__u64	addr;
	__u32	metadata_len;
	__u32	data_len;
	__u32	cdw10;
	__u32	cdw11;
	__u32	cdw12;
	__u32	cdw13;
	__u32	cdw14;
	__u32	cdw15;
	__u32	timeout_ms;
	__u32	result;
};

#ifdef DEBUG
void show_command(int opcode, void ** buf, __u32 data_len, __u32 lba, int isQuery) {
	printf("opcode       : %02x\n", opcode);
	printf("flags        : %02x\n", 0);
	printf("rsvd1        : %04x\n", 0);
	printf("nsid         : %08x\n", 0);
	printf("cdw2         : %08x\n", 0);
	printf("cdw3         : %08x\n", 0);
	printf("data_len     : %08x\n", data_len);
	printf("metadata_len : %08x\n", 0);
	printf("addr         : %"PRIx64"\n", (uint64_t)(uintptr_t)buf);
	printf("metadata     : %08x\n", 0);
	printf("cdw10        : %08x\n", lba);
	printf("cdw11        : %08x\n", 0);
	printf("cdw12        : %08x\n", 0);
	printf("cdw13        : %08x\n", (isQuery)?QUERY_FLAG:0);
	printf("cdw14        : %08x\n", 0);
	printf("cdw15        : %08x\n", 0);
	printf("timeout_ms   : %08x\n", 0);
}
#endif

int init_ioctl () {
	int err;

	if (err=posix_memalign(&ioctl_buf, getpagesize(), 4096)) {
		fprintf(stderr, "Can not allocate payload \n");
		return err;
	}

	return 0;
}

int submit_ioctl (int fd, int opcode, const char* query, __u32 data_len, __u32 lba, int isQuery,  char** result) {
	__u32 dword13;

	if (isQuery == 1) {
		dword13 = QUERY_FLAG;
	} else if (isQuery == 2) {
		dword13 = BIND_FLAG;
	} else if (isQuery == 3) {
		dword13 = PARAMETER_FLAG;
	} else {
		dword13 = 0;
	}

	memset (ioctl_buf, 0, 4096);

	if (query && opcode == 1) {
		strcpy ((char*)ioctl_buf, query);
	}


#ifdef DEBUG
	// print command setting when compiled with DEBUG option.
	show_command (opcode, ioctl_buf, data_len, lba, isQuery);
#endif
	struct nvme_passthru_cmd cmd = {
		.opcode		= opcode,
		.flags		= 0,
		.rsvd1		= 0,
		.nsid		= 0,
		.cdw2		= 0,
		.cdw3		= 0,
		.metadata	= 0,
		.addr		= (__u64)(uintptr_t) ioctl_buf,
		.metadata_len	= 0,
		.data_len	= data_len,
		.cdw10		= lba,
		.cdw11		= 0,
		.cdw12		= 0,
		.cdw13		= dword13,
		.cdw14		= 0,
		.cdw15		= 0,
		.timeout_ms	= 0,
		.result		= 0,
	};

	int err = ioctl(fd, NVME_IOCTL_IO_CMD, &cmd);

	if(err==0 && opcode==2) {
		*result = (char*) ioctl_buf;
	}

  //else {
	//*result = NULL;
	//}

	//printf("ERRNO: %d\n",err);
	return err;
}



