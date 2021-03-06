/*
 *  Squeeze2xxx - squeezebox emulator gateway
 *
 *  (c) Adrian Smith 2012-2014, triode1@btinternet.com
 *	(c) Philippe 2015-2017, philippe_44@outlook.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __SQUEEZEDEFS_H
#define __SQUEEZEDEFS_H

#define VERSION "v0.2.9.0"" ("__DATE__" @ "__TIME__")"

#include "platform.h"

#if LINUX || OSX || FREEBSD
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <poll.h>
#include <dlfcn.h>
#include <pthread.h>
#include <signal.h>

#define STREAM_THREAD_STACK_SIZE  64 * 1024
#define OUTPUT_THREAD_STACK_SIZE  64 * 1024
#define SLIMPROTO_THREAD_STACK_SIZE  64 * 1024
#define thread_t pthread_t;
#define closesocket(s) close(s)

#define mutex_type pthread_mutex_t
#define mutex_create(m) pthread_mutex_init(&m, NULL)
#define mutex_create_p(m) pthread_mutexattr_t attr; pthread_mutexattr_init(&attr); pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT); pthread_mutex_init(&m, &attr); pthread_mutexattr_destroy(&attr)
#define mutex_lock(m) pthread_mutex_lock(&m)
#define mutex_unlock(m) pthread_mutex_unlock(&m)
#define mutex_trylock(m) pthread_mutex_trylock(&m)
#define mutex_destroy(m) pthread_mutex_destroy(&m)
#define thread_type pthread_t
#define mutex_timedlock(m, t) _mutex_timedlock(&m, t)
int _mutex_timedlock(mutex_type *m, u32_t wait);

#endif

#if WIN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#include <stdbool.h>
#include <sys/timeb.h>
#include "pthread.h"

#define STREAM_THREAD_STACK_SIZE (1024 * 64)
#define DECODE_THREAD_STACK_SIZE (1024 * 128)
#define OUTPUT_THREAD_STACK_SIZE (1024 * 64)
#define SLIMPROTO_THREAD_STACK_SIZE  (1024 * 64)

#define inline __inline

#define mutex_type HANDLE
#define mutex_create(m) m = CreateMutex(NULL, FALSE, NULL)
#define mutex_create_p mutex_create
// in Windows, a mutex never locks if the owner tries to lock it
#define mutex_lock(m) WaitForSingleObject(m, INFINITE)
#define mutex_timedlock(m,t) WaitForSingleObject(m, t)
#define mutex_unlock(m) ReleaseMutex(m)
#define mutex_trylock(m) WaitForSingleObject(m, 0)
#define mutex_destroy(m) CloseHandle(m)
#define thread_type HANDLE

#endif

#endif     // __SQUEEZEDEFS_H
