#define _FILE_OFFSET_BITS 64
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include "ceph/src/include/cephfs/libcephfs.h"
#include <openssl/sha.h>

#define MB64 (1<<26)

int main(int argc, const char **argv)
{
        struct ceph_mount_info *cmount; 
        int ret, fd, len;
	SHA_CTX ctx;
	unsigned char result[SHA_DIGEST_LENGTH];
	unsigned char result_str[2*SHA_DIGEST_LENGTH + 1];
        char buf[1024];
	int i, n;
	//creating a cephfs
        ret = ceph_create(&cmount, NULL);
        if (ret) {
                fprintf(stderr, "ceph_create=%d\n", ret);
                exit(1);
        }
	//reading config file
        ret = ceph_conf_read_file(cmount, "ceph/build/ceph.conf");
        if (ret) {
                fprintf(stderr, "ceph_conf_read_file=%d\n", ret);
                exit(1);
        }
	
	//mounting cephfs
        ret = ceph_mount(cmount, NULL);
        if (ret) {
                fprintf(stderr, "ceph_mount=%d\n", ret);
                exit(1);
        }

        //accessing the file
        ret = ceph_chdir(cmount, "/");
        if (ret) {
                fprintf(stderr, "ceph_chdir=%d\n", ret);
                exit(1);
        }

        fd = ceph_open(cmount, argv[1], O_RDONLY, 0777); 
        if (fd < 0) {
 		if (fd == -2)
		    printf("File Not Found!\n");
		fprintf(stderr, "ceph_open=%d\n", fd);
                exit(1);
        }
	
	//computing sha1sum of the file
	SHA1_Init (&ctx);
	while ((n = ceph_read (cmount, fd, buf, 1024, -1)) > 0) {
		SHA1_Update(&ctx, buf, n);
	}
	SHA1_Final(result, &ctx);

	for (i=0; i<SHA_DIGEST_LENGTH; i++)
		sprintf((char *)&(result_str[i*2]), "%02x",result[i]);

	result_str[(2*SHA_DIGEST_LENGTH)] = '\0';
        printf("\nSHA1 of %s is %s\n", argv[1], result_str);

        ceph_shutdown(cmount);

        return 0;
}

