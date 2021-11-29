#ifndef _UNISTD_H
#define _UNISTD_H    1

// The Open Group Base Specifications Issue 6
// IEEE Std 1003.1, 2004 Edition
// Copyright � 2001-2004 The IEEE and The Open Group

/* This file intended to serve as a drop-in replacement for 
 *  unistd.h on Windows
 *  Please add functionality as neeeded 
 */

#include <stdlib.h>

#ifdef __linux__
    //linux code goes here
    #include <sys/io.h>
#elif _WIN32
    // windows code goes here
    #include <io.h>
    #include <process.h>    /* for getpid() and the exec..() family */
    #include <direct.h>     /* for _getcwd() and _chdir() */
#else
// #error "OS not supported!"
#endif

#define srandom srand
#define random rand

/* Values for the second argument to access.
   These may be OR'd together.  */
#define R_OK    4       /* Test for read permission.  */
#define W_OK    2       /* Test for write permission.  */
//#define   X_OK    1   /* execute permission - unsupported in windows*/
#define F_OK    0       /* Test for existence.  */

#define access _access
#define dup2 _dup2
#define execve _execve
#define ftruncate _chsize
#define unlink _unlink
#define fileno _fileno
#define getcwd _getcwd
#define chdir _chdir
#define isatty _isatty
#define lseek _lseek
/* read, write, and close are NOT being #defined here, 
because while there are file handle specific versions for Windows, 
they probably don't work for sockets. 
You need to look at your app and consider whether to call e.g. closesocket(). */

#define ssize_t int

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

/* should be in some equivalent to <sys/types.h> */
#ifdef __linux__
    //linux code goes here

#elif _WIN32
    // windows code goes here
    // typedef __int8         int8_t;
    typedef __int16           int16_t;
    typedef __int32           int32_t;
    typedef __int64           int64_t;
    typedef unsigned __int8   uint8_t;
    typedef unsigned __int16  uint16_t;
    typedef unsigned __int32  uint32_t;
    typedef unsigned __int64  uint64_t;
#else
// #error "OS not supported!"
#endif

#endif /* unistd.h  */
