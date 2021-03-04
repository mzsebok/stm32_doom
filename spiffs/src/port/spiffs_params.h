/*
 * spiffs_params.h
 *
 *  Created on: Feb 13, 2021
 *      Author: Marci
 */

#ifndef SRC_PORT_SPIFFS_PARAMS_H_
#define SRC_PORT_SPIFFS_PARAMS_H_

/////////// SPIFFS BUILD CONFIG  ////////////
#define SPIFFS_SINGLETON            0                   //

#define SPIFFS_USE_MAGIC            1
#define SPIFFS_USE_MAGIC_LENGTH     1                   // using filesystem magic length

#define SPIFFS_FILEHDL_OFFSET       1                   // using filehandle offset

//#define USE_MUTEX_LOCK
#ifdef USE_MUTEX_LOCK
extern void fs_lock(struct spiffs_t *fs);
extern void fs_unlock(struct spiffs_t *fs);
#define SPIFFS_LOCK(fs)   fs_lock(fs)
#define SPIFFS_UNLOCK(fs) fs_unlock(fs)
#else
#define SPIFFS_LOCK(fs)
#define SPIFFS_UNLOCK(fs)
#endif

// dbg output
#define SPIFFS_DBG(_f, ...) //printf("\x1b[32m" _f "\x1b[0m", ## __VA_ARGS__)
#define SPIFFS_API_DBG(_f, ...) //printf("\n\x1b[1m\x1b[7m" _f "\x1b[0m", ## __VA_ARGS__)
#define SPIFFS_GC_DBG(_f, ...) //printf("\x1b[36m" _f "\x1b[0m", ## __VA_ARGS__)
#define SPIFFS_CACHE_DBG(_f, ...) //printf("\x1b[33m" _f "\x1b[0m", ## __VA_ARGS__)
#define SPIFFS_CHECK_DBG(_f, ...) //printf("\x1b[31m" _f "\x1b[0m", ## __VA_ARGS__)

// needed types
typedef signed int s32_t;
typedef unsigned int u32_t;
typedef signed short s16_t;
typedef unsigned short u16_t;
typedef signed char s8_t;
typedef unsigned char u8_t;

#endif /* SRC_PORT_SPIFFS_PARAMS_H_ */
