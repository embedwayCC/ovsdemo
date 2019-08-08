#ifndef __OS_H__
#define __OS_H__

#ifdef WIN32
#include <winsock.h>
#include "winthread.h"

#define inet_network(ip) ntohl(inet_addr(ip))
#define strcasecmp strcmpi

struct pthread_rwlock_t_
{
  pthread_mutex_t mtxExclusiveAccess;
  pthread_mutex_t mtxSharedAccessCompleted;
  pthread_cond_t cndSharedAccessCompleted;
  int nSharedAccessCount;
  int nExclusiveAccessCount;
  int nCompletedSharedAccessCount;
  int nMagic;
};

struct pthread_rwlockattr_t_
{
  int pshared;
};

typedef INT32 int32_t;
typedef INT64 int64_t;

#define strtoll _strtoi64

typedef struct pthread_rwlock_t_ * pthread_rwlock_t;
typedef struct pthread_rwlockattr_t_ * pthread_rwlockattr_t;

#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#endif

#endif
