#include <fs.h>

typedef size_t (*ReadFn)(void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn)(const void *buf, size_t offset, size_t len);

typedef struct
{
    char *name;
    size_t size;
    size_t disk_offset;
    size_t open_offset;
    ReadFn read;
    WriteFn write;
} Finfo;

enum
{
    FD_STDIN,
    FD_STDOUT,
    FD_STDERR,
    FD_FB
};

size_t invalid_read(void *buf, size_t offset, size_t len)
{
    panic("should not reach here");
    return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len)
{
    panic("should not reach here");
    return 0;
}

extern size_t serial_write(const void *buf, size_t offset, size_t len);

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
    [FD_STDIN] = {"stdin", 0, 0, 0, invalid_read, invalid_write},
    [FD_STDOUT] = {"stdout", 0, 0, 0, invalid_read, serial_write},
    [FD_STDERR] = {"stderr", 0, 0, 0, invalid_read, serial_write},
#include "files.h"
};

#define FILE_NUM sizeof(file_table) / sizeof(Finfo)

size_t ramdisk_read(void *buf, size_t offset, size_t len);
size_t ramdisk_write(const void *buf, size_t offset, size_t len);

void init_fs()
{
    // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode)
{
    for (int i = 0; i < FILE_NUM; i++)
    {
        if (strcmp(pathname, file_table[i].name) == 0)
            return i;
    }
    printf("%s\n", pathname);
    panic("Couldn't find file");
    return -1;
}

size_t fs_read(int fd, void *buf, size_t len)
{
    assert(fd >= 0 && fd < FILE_NUM);
    size_t offset;
    if (file_table[fd].read != NULL)
        offset = ramdisk_read(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    else
        offset = ramdisk_read(buf, file_table[fd].open_offset, len);
    file_table[fd].open_offset += offset;
    return offset;
}

size_t fs_write(int fd, const void *buf, size_t len)
{
    assert(fd >= 0 && fd < FILE_NUM);
    size_t offset;
    if (file_table[fd].write != NULL)
        offset = ramdisk_write(buf, file_table[fd].disk_offset + file_table[fd].open_offset, len);
    else
        offset = ramdisk_write(buf, file_table[fd].open_offset, len);
    file_table[fd].open_offset += offset;
    return offset;
}

size_t fs_lseek(int fd, size_t offset, int whence)
{
    assert(fd >= 0 && fd < FILE_NUM);
    switch (whence)
    {
    case SEEK_SET:
        file_table[fd].open_offset = offset;
        break;
    case SEEK_CUR:
        file_table[fd].open_offset += offset;
        break;
    case SEEK_END:
        file_table[fd].open_offset = file_table[fd].size + offset;
        break;
    default:
        return -1;
    }
    if (file_table[fd].open_offset > file_table[fd].size)
        file_table[fd].open_offset = file_table[fd].size;
    return file_table[fd].open_offset;
}
int fs_close(int fd)
{
    file_table[fd].open_offset = 0;
    return 0;
}
