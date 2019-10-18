#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysmacros.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>

void print_info (struct stat, char);
void permissions(struct stat, char);
void link_path(void);

void print_info (struct stat sb, char type) {

    char ctime[80];
    char atime[80];
    char mtime[80];

    strftime(ctime, 80, "%Y-%m-%d %H:%M:%S %z (%Z) %a (local)", localtime(&sb.st_ctime));
    strftime(atime, 80, "%Y-%m-%d %H:%M:%S %z (%Z) %a (local)", localtime(&sb.st_atime));
    strftime(mtime, 80, "%Y-%m-%d %H:%M:%S %z (%Z) %a (local)", localtime(&sb.st_mtime));

    printf("  Device ID number:         %ld\n", (long) sb.st_dev);
    printf("  I-node number:            %ld\n", (long) sb.st_ino);
    printf("  Mode:                     "); permissions(sb, type); printf("        (%lo in octal)\n", (unsigned long) sb.st_mode % 512);
    printf("  Link count:               %ld\n", (long) sb.st_nlink);
    printf("  Owner Id:                 %s             (UID = %ld)\n", getpwuid(sb.st_uid)->pw_name, (long) sb.st_uid);
    printf("  Group Id:                 %s             (GID = %ld)\n", getgrgid(sb.st_gid)->gr_name, (long) sb.st_gid);
    printf("  Preferred I/O block size: %ld bytes\n", (long) sb.st_blksize);
    printf("  File size:                %lld bytes\n",(long long) sb.st_size);
    printf("  Blocks allocated:         %lld\n",      (long long) sb.st_blocks);

    printf("Last status change:         %s\n",  ctime);
    printf("Last file access:           %s\n",  atime);
    printf("Last file modification:     %s\n",  mtime);
    return;
}

void permissions(struct stat fileStat, char type) {

    printf("%c", type);
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");

    return;
}

void link_path (void) {



}

int
main(int argc, char *argv[])
{
    struct stat sb;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (lstat(argv[1], &sb) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    printf("File: %s\n", argv[1]);
    printf("  File type:                ");

    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:  
        printf("block device\n");
        print_info(sb, '-');
        break;
    case S_IFCHR:  
        printf("character device\n");
        print_info(sb, '-');
        break;
    case S_IFDIR:  
        printf("directory\n");       
        print_info(sb, 'd');
        break;
    case S_IFIFO:  
        printf("FIFO/pipe\n");       
        print_info(sb, 'p');
        break;
    case S_IFLNK:  
        printf("Symbolic link\n");
        print_info(sb, 'l');
        break;
    case S_IFREG:  
        printf("regular file\n");
        print_info(sb, '-');
        break;
    case S_IFSOCK: 
        printf("socket\n");
        print_info(sb, 's');
        break;
    default:       
        printf("unknown?\n");
        break;
    }

    exit(EXIT_SUCCESS);
}
