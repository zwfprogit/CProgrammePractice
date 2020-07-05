/*改变文件的访问权限*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


int main(int argc,char **argv)
{
#if 0
	int mode; 		//权限
	int mode_u; 	//所有者的权限
	int mode_g; 	//所属组的权限
	int mode_o; 	//其他用户的权限
	char *path;

	// 检查参数的合法性
	if(argc < 3){
		printf("%s <mode num> <target file>\n",argv[0]);
		exit(0);
	}

	// 获取命令行参数
	mode = atoi(argv[1]);
	if(mode > 777 || mode < 0){
		printf("mode num error\n");
		exit(0);
	}

	mode_u = mode / 100;
	mode_g = (mode - mode_u * 100) / 10;
	mode_o = mode % 10;

	mode = (mode_u * 8 * 8) + mode_g * 8 + mode_o; //八进制转换
	printf("mode to octonary %o\n",mode);
	path = argv[2];

	if(chmod(path,mode) < 0){
		perror("chmod error");
		exit(1);
	}
#else
#if 0
struct stat {
	unsigned long   st_dev;         /* Device.  */
	unsigned long   st_ino;         /* File serial number.  */
	unsigned int    st_mode;        /* File mode.  */
	unsigned int    st_nlink;       /* Link count.  */
	unsigned int    st_uid;         /* User ID of the file's owner.  */
	unsigned int    st_gid;         /* Group ID of the file's group. */
	unsigned long   st_rdev;        /* Device number, if device.  */
	unsigned long   __pad1;
	long            st_size;        /* Size of file, in bytes.  */
	int             st_blksize;     /* Optimal block size for I/O.  */
	int             __pad2;
	long            st_blocks;      /* Number 512-byte blocks allocated. */
	long            st_atime;       /* Time of last access.  */
	unsigned long   st_atime_nsec;
	long            st_mtime;       /* Time of last modification.  */
	unsigned long   st_mtime_nsec;
	long            st_ctime;       /* Time of last status change.  */
	unsigned long   st_ctime_nsec;
	unsigned int    __unused4;
	unsigned int    __unused5;
};
#endif
#include <time.h>
	struct stat buf;

	// 检查参数的合法性
	if(argc != 2){
		printf("Usage: %s <filename>\n",argv[0]);
		exit(0);
	}
	// 获取文件属性
	if(stat(argv[1],&buf) == -1){
		perror("stat:");
		exit(1);
	}
	// 打印文件属性
	printf("device is: %ld\n",buf.st_dev);
	printf("inode is: %ld\n",buf.st_ino);
	printf("mode is: %d\n",buf.st_mode);
	printf("number of hard links is: %ld\n",buf.st_nlink);
	printf("user id of owner is: %d\n",buf.st_uid);
	printf("group id of owner is: %d\r\n",buf.st_gid);

	printf("total size,in bytes is: %ld\n",buf.st_size);
	printf("block size for filesystem I/O is: %ld\n",buf.st_blksize);
	printf("number of blocks allocated is: %ld\r\n",buf.st_blocks);

	printf("time of last access is: %s\n",ctime(&buf.st_atime));
	printf("time of last modification is: %s\n",ctime(&buf.st_mtime));
	printf("time of last change is: %s\n",ctime(&buf.st_ctime));
#endif
	return 0;
}