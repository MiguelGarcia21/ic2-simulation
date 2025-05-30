#include <errno.h>
#include <sys/types.h>

// Use int instead of ptrdiff_t for simplicity
void *_sbrk(int incr) {
    extern char _end; // Defined by the linker
    static char *heap_end = &_end;
    char *prev_heap_end = heap_end;
    heap_end += incr;
    return (void *)prev_heap_end;
}

// Stub implementations for other system calls
int _close(int file) { return -1; }
int _fstat(int file, void *st) { return 0; }
int _isatty(int file) { return 1; }
int _lseek(int file, int offset, int whence) { return 0; }
int _read(int file, void *ptr, unsigned int len) { return 0; }
int _write(int file, const void *ptr, unsigned int len) { return len; }
void _exit(int status) { while(1); }
int _kill(int pid, int sig) { return -1; }
int _getpid(void) { return 1; }