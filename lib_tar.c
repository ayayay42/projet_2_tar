#include "lib_tar.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/**
 * Checks whether the archive is valid.
 *
 * Each non-null header of a valid archive has:
 *  - a magic value of "ustar" and a null,
 *  - a version value of "00" and no null,
 *  - a correct checksum
 *
 * @param tar_fd A file descriptor pointing to the start of a file supposed to contain a tar archive.
 *
 * @return a zero or positive value if the archive is valid, representing the number of non-null headers in the archive,
 *         -1 if the archive contains a header with an invalid magic value,
 *         -2 if the archive contains a header with an invalid version value,
 *         -3 if the archive contains a header with an invalid checksum value
 */
int check_archive(int tar_fd) {

    int non_null_headers = 0;

    char buffer[MAX_HEADER_SIZE];



    while (read(tar_fd, buffer, MAX_HEADER_SIZE) <= MAX_HEADER_SIZE){
        printf("read: %ld\n", read(tar_fd, buffer, MAX_HEADER_SIZE));

        if (buffer[0] == '\0')
        {
            break;
        }
        
        tar_header_t * header = (tar_header_t *) buffer;

        if (header->name[0] == '\0')
        {
            printf("HERE MY DUDES\n");
            break;
        }

        //int len = sizeof(tar_header_t);
        //uint8_t* headerfunct = (uint8_t *) header;
        if (strncmp(header->magic, TMAGIC, TMAGLEN-1) != 0){
            if (strncmp(header->magic, TMAGIC, TMAGLEN) != 0){
                printf("inside the loop\n");
                return -1;
            }
            printf("outside the loop\n");
            return -1;
        }
        
        int ascii_val[] = {0, 0};

        printf("good magic: %ld, %d\n", TAR_INT(header->version), *ascii_val);     
        if (TAR_INT(header->version) != *ascii_val){
            return -2;
        }
        printf("good version\n");
        int my_checksm = 0;
        for (int i = 0; i < MAX_HEADER_SIZE; i++){
            if (i >= 148 && i < 156){
                my_checksm += ' ';
            }
            else{
                my_checksm += buffer[i];
            }
        }
        printf("just checkin the sum\n");
        if (TAR_INT(header->chksum) != my_checksm){
            return -3;
        }
        non_null_headers++;
        printf("non_null_headers: %d\n", non_null_headers);
        lseek(tar_fd, TAR_INT(header->size), SEEK_CUR);
    }
    printf("outside while\n");
    return non_null_headers;
}

/**
 * Checks whether an entry exists in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive,
 *         any other value otherwise.
 */
int exists(int tar_fd, char *path) {
    char buffer[MAX_HEADER_SIZE];

    lseek(tar_fd, 0, SEEK_SET);

    while (read(tar_fd, buffer, MAX_HEADER_SIZE) <= MAX_HEADER_SIZE){
        printf("######\nbuffer: %s de taille: %ld\n", buffer, strlen(buffer));
        tar_header_t * header = (tar_header_t *) buffer;
        printf("header->name: %s de taille: %ld\n", header->name, strlen(header->name));
        printf("path: %s\n\n", path);
        if (header->name[0] == '\0')
        {
            printf("reached end\n\n");
            lseek(tar_fd, (-1)* strlen(header->name), SEEK_CUR);
            break;
        }
        printf("compare: %d\n\n", strncmp(header->name, path, strlen(path)));
        if (strncmp(header->name, path, strlen(path)) == 0){
            return 1;
        }
        printf("just before seek\n");
        lseek(tar_fd, MAX_HEADER_SIZE - sizeof(buffer), SEEK_CUR);
        printf("bufferà la fin: %s de taille: %ld\n######\n", buffer, strlen(buffer));

    }
    printf("outside while\n");
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a directory.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a directory,
 *         any other value otherwise.
 */
int is_dir(int tar_fd, char *path) {
    printf("#####\n debugging is_dir\n");
    char buffer[MAX_HEADER_SIZE];

    lseek(tar_fd, 0, SEEK_SET);

    while (read(tar_fd, buffer, MAX_HEADER_SIZE) <= MAX_HEADER_SIZE){
        printf("\nnew iteration\n");
        tar_header_t * header = (tar_header_t *) buffer;
        if (header->name[0] == '\0')
        {
            printf("reached end\n\n");
            lseek(tar_fd, (-1)* strlen(header->name), SEEK_CUR);
            break;
        }
        printf("compare path: %d\n\n", strncmp(header->name, path, strlen(path)));
        printf("header flag and name: %d, %s\n", header->typeflag, header->name);
        if (strncmp(header->name, path, strlen(path)) == 0){
            if (header->typeflag == DIRTYPE){
                printf("inside if dir\n");
                return 1;
            }
            else printf("outside if dir so not a dir but good path\n");
        }
        lseek(tar_fd, MAX_HEADER_SIZE - sizeof(buffer), SEEK_CUR);

    }
    printf("outside while\n");
    return 0;
    
}

/**
 * Checks whether an entry exists in the archive and is a file.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a file,
 *         any other value otherwise.
 */
int is_file(int tar_fd, char *path) {
    printf("\n\n\n#####\n debugging file\n");

    char buffer[MAX_HEADER_SIZE];

    lseek(tar_fd, 0, SEEK_SET);

    while (read(tar_fd, buffer, MAX_HEADER_SIZE) <= MAX_HEADER_SIZE){
        printf("\nnew iteration\n");
        tar_header_t * header = (tar_header_t *) buffer;
        if (header->name[0] == '\0')
        {
            printf("reached end\n\n");
            lseek(tar_fd, (-1)* strlen(header->name), SEEK_CUR);
            break;
        }
        printf("compare: %d\n\n", strcmp(header->name, path));
        printf("path is: %s\n", path);
        printf("header flag and name: %d, %s\n", header->typeflag, header->name);
        if (strncmp(header->name, path, strlen(path)) == 0){
            if (header->typeflag == REGTYPE || header->typeflag == AREGTYPE){
                printf("inside if file\n");
                return 1;
            }
            else {
                printf("outside if file so not a file but good path\n");
                break;
            }
        }
        lseek(tar_fd, MAX_HEADER_SIZE - sizeof(buffer), SEEK_CUR);

    }
    printf("outside while\n");
    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a symlink.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 * @return zero if no entry at the given path exists in the archive or the entry is not symlink,
 *         any other value otherwise.
 */
int is_symlink(int tar_fd, char *path) {
    printf("\n\n\n#####\n debugging symlink\n");

    char buffer[MAX_HEADER_SIZE];

    lseek(tar_fd, 0, SEEK_SET);

    while (read(tar_fd, buffer, MAX_HEADER_SIZE) <= MAX_HEADER_SIZE){
        printf("\nnew iteration\n");
        tar_header_t * header = (tar_header_t *) buffer;
        if (header->name[0] == '\0')
        {
            printf("reached end\n\n");
            lseek(tar_fd, (-1)* strlen(header->name), SEEK_CUR);
            break;
        }
        printf("compare: %d\n\n", strcmp(header->name, path));
        printf("path is: %s\n", path);
        printf("header flag and name: %d, %s\n", header->typeflag, header->name);
        if (strncmp(header->name, path, strlen(path)) == 0){
            if (header->typeflag == SYMTYPE){
                printf("inside if link\n");
                return 1;
            }
            else {
                printf("outside if link so not a link but good path\n");
                break;
            }
        }
        lseek(tar_fd, MAX_HEADER_SIZE - sizeof(buffer), SEEK_CUR);

    }
    printf("outside while\n");
    return 0;
}


/**
 * Lists the entries at a given path in the archive.
 * list() does not recurse into the directories listed at the given path.
 *
 * Example:
 *  dir/          list(..., "dir/", ...) lists "dir/a", "dir/b", "dir/c/" and "dir/e/"
 *   ├── a
 *   ├── b
 *   ├── c/
 *   │   └── d
 *   └── e/
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive. If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param entries An array of char arrays, each one is long enough to contain a tar entry path.
 * @param no_entries An in-out argument.
 *                   The caller set it to the number of entries in `entries`.
 *                   The callee set it to the number of entries listed.
 *
 * @return zero if no directory at the given path exists in the archive,
 *         any other value otherwise.
 */
int list(int tar_fd, char *path, char **entries, size_t *no_entries) {
  int count = 0;
  int index = 0;

  if (!is_dir(tar_fd, path) && !is_symlink(tar_fd, path)) {
    *no_entries = 0;
    return 0;
  }

  char *record = malloc(100);
  if (!record) return -1;
  strcpy(record, "/");

  while (1) {
    tar_header_t header;
    if (pread(tar_fd, &header, sizeof(tar_header_t), count*sizeof(tar_header_t)) < 0) return -1;

    if (!strcmp(header.name, path)) {
      if (header.typeflag == DIRTYPE) {
        int counter = count + 1; 
        while (1) {
          tar_header_t entry;
          if (pread(tar_fd, &entry, sizeof(tar_header_t), counter*sizeof(tar_header_t)) < 0) return -1;


          if (!strncmp(entry.name, path, strlen(path))) {
            if (strncmp(entry.name, record, strlen(record))) {
              memcpy(entries[index], entry.name, strlen(entry.name));
              index++;
              strcpy(record, entry.name); 
            }
          }
          else if (!(header.typeflag == LNKTYPE || header.typeflag == SYMTYPE)) {
            *no_entries = index;
            return 1;
          }

          if (TAR_INT(entry.size) % MAX_HEADER_SIZE == 0) {
            counter += (1 + TAR_INT(entry.size) / MAX_HEADER_SIZE);
          }
          else {
            counter += (2 + TAR_INT(entry.size) / MAX_HEADER_SIZE);
          }
        }
      }

      else if (header.typeflag == LNKTYPE || header.typeflag == SYMTYPE) {
        if (is_file(tar_fd, header.linkname + 2)) {
          return list(tar_fd, header.linkname + 2, entries, no_entries);
        }
        return list(tar_fd, strcat(header.linkname, "/") + 2, entries, no_entries);
      }
    }

    // tar end
    if (!strlen((char *) &header)) {
      tar_header_t header2;
      if (pread(tar_fd, &header2, sizeof(tar_header_t), (count+1)*sizeof(tar_header_t)) < 0) return -1;
      
      if (!strlen((char *) &header2)) {
        *no_entries = 0;
        return 0;
      }
    }

    // block num
    if (TAR_INT(header.size) % MAX_HEADER_SIZE == 0) {
      count += (1 + TAR_INT(header.size) / MAX_HEADER_SIZE);
    }
    else {
      count += (2 + TAR_INT(header.size) / MAX_HEADER_SIZE);
    }
  }
  return 0;
}

/**
 * Reads a file at a given path in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive to read from.  If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param offset An offset in the file from which to start reading from, zero indicates the start of the file.
 * @param dest A destination buffer to read the given file into.
 * @param len An in-out argument.
 *            The caller set it to the size of dest.
 *            The callee set it to the number of bytes written to dest.
 *
 * @return -1 if no entry at the given path exists in the archive or the entry is not a file,
 *         -2 if the offset is outside the file total length,
 *         zero if the file was read in its entirety into the destination buffer,
 *         a positive value if the file was partially read, representing the remaining bytes left to be read to reach
 *         the end of the file.
 *
 */
ssize_t read_file(int tar_fd, char *path, size_t offset, uint8_t *dest, size_t *len) {

  tar_header_t header;

  lseek(tar_fd, 0, SEEK_SET);

  while (MAX_HEADER_SIZE == read(tar_fd, &header, MAX_HEADER_SIZE)) {
    

    if (strcmp(header.name, path) == 0) {
      if ( header.typeflag == '\0' || header.typeflag == '0' || header.typeflag == SYMTYPE) {
        int file_size = TAR_INT(header.size);
        
        if (lseek(tar_fd, offset, SEEK_CUR) < 0) return -1;

        if (offset > file_size) return -2;

        ssize_t file_to_read = read(tar_fd, dest, *len);
        if (file_to_read < 0) return -1;

        *len = file_to_read;

        return (file_to_read < file_size - offset) ? (file_size - offset - file_to_read) : 0;

      } else {
        return -1;
      }
    }

    lseek(tar_fd, TAR_INT(header.size), 1);
  }


  return -1;
}