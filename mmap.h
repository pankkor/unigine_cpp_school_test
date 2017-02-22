//
// Single-file cross-platform read only mmap
//
// Usage:
// #define MMAP_IMPL
// #include "mmap.h"
//
#if !defined(GR_MMAP_H)
#define GR_MMAP_H

#include <stddef.h>

// mmap file read only
//
// Return values:
//      mmaped buffer if succeeds or NULL if fails
//      mmaped size in out_size parameter
void *gr_mmap_file(const char *filepath, size_t *out_size);

// munmmap file
// Returns values:
//      false if fails
bool gr_munmap_file(void *data, size_t size);

#endif

#if defined(MMAP_IMPL)

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__MACH__)
    #include <unistd.h>
    #if defined(_POSIX_MAPPED_FILES)
        #define POSIX_MMAP
    #endif
#endif

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif defined(POSIX_MMAP)
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/mman.h>
#else
    #error "Your system doesn't have mmap support"
#endif

void *gr_mmap_file(const char *filepath, size_t *out_size) {
    void *data = NULL;
    size_t size = 0;

#if defined(_WIN32)
    HANDLE map;
    HANDLE file = CreateFileA(filepath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    size = GetFileSize(file, NULL);
    if (size != INVALID_FILE_SIZE && size != 0) {
        map = CreateFileMappingA(file, NULL, PAGE_READONLY, 0, size, NULL);
        if (map) {
            data = (char *)MapViewOfFile(map, FILE_MAP_READ, 0, 0, size);
            CloseHandle(map);
        }
    }
    CloseHandle(file);

#elif defined(POSIX_MMAP)
    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        return NULL;
    }

    size = lseek(fd, 0, SEEK_END);
    if (size > 0) {
        data = (char *)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (data == MAP_FAILED) {
            data = NULL;
        }
    }
    close(fd);
#endif

    if (out_size) {
        *out_size = size;
    }
    return data;
}

bool gr_munmap_file(void *data, size_t size) {
#if defined(_WIN32)
    // suppress warning
    (void)size;
    return UnmapViewOfFile(data);
#elif defined(POSIX_MMAP)
    return munmap(data, size) != -1;
#endif
}

#endif
