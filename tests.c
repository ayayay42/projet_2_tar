#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "lib_tar.h"

/**
 * You are free to use this file to write tests for your implementation
 */

void debug_dump(const uint8_t *bytes, size_t len) {
    for (int i = 0; i < len;) {
        printf("%04x:  ", (int) i);

        for (int j = 0; j < 16 && i + j < len; j++) {
            printf("%02x ", bytes[i + j]);
        }
        printf("\t");
        for (int j = 0; j < 16 && i < len; j++, i++) {
            printf("%c ", bytes[i]);
        } 
        printf("\n");
    }
}

int main(int argc, char **argv) {
    /* if (argc < 2) {
        printf("Usage: %s tar_file\n", argv[0]);
        return -1;
    }

    int fd = open(argv[1] , O_RDONLY);
    if (fd == -1) {
        perror("open(tar_file)");
        return -1;
    } */
    /*
    int ret = check_archive(fd);
    printf("check_archive returned %d\n", ret);
    */
    /*
    int is_dir_archive = is_dir(fd, "archive_3_files");
    int is_dir_summer = is_dir(fd, "archive_3_files/summer.jpg");
    
    printf("\n is dir returned %d for archive and %d for summer\n", is_dir_archive, is_dir_summer);
    printf("\n\n");
    */
    /*
    int is_file_summer = is_file(fd, "archive_3_files/summer.jpg");
    int is_file_archive = is_file(fd, "archive_3_files");
    
    printf("is file returned %d for summer and %d for archive_3\n", is_file_summer, is_file_archive);
    */

    //int is_link = is_symlink(fd, "my_symlink");
    //printf("is symlink returned %d\n", is_link);
    return 0;
}
