#ifndef DRBDTOOL_COMMON_H
#define DRBDTOOL_COMMON_H

#include "drbd_endian.h"
#include <stdarg.h>
#include <linux/major.h>

#define LANANA_DRBD_MAJOR 147	/* we should get this into linux/major.h */
#ifndef DRBD_MAJOR
#define DRBD_MAJOR LANANA_DRBD_MAJOR
#elif (DRBD_MAJOR != LANANA_DRBD_MAJOR)
# error "FIXME unexpected DRBD_MAJOR"
#endif

#ifndef __packed
#define __packed __attribute__((packed))
#endif

#define ARRY_SIZE(A) (sizeof(A)/sizeof(A[0]))

#define COMM_TIMEOUT 120

/* MetaDataIndex for v06 / v07 style meta data blocks */
enum MetaDataIndex {
	Flags,			/* Consistency flag,connected-ind,primary-ind */
	HumanCnt,		/* human-intervention-count */
	TimeoutCnt,		/* timout-count */
	ConnectedCnt,		/* connected-count */
	ArbitraryCnt,		/* arbitrary-count */
	GEN_CNT_SIZE		/* MUST BE LAST! (and Flags must stay first...) */
};

/*
#define PERROR(fmt, args...) \
do { fprintf(stderr,fmt ": " , ##args); perror(0); } while (0)
*/
#define PERROR(fmt, args...) fprintf(stderr, fmt ": %m\n" , ##args);

enum new_strtoll_errs {
	MSE_OK,
	MSE_DEFAULT_UNIT,
	MSE_MISSING_NUMBER,
	MSE_INVALID_NUMBER,
	MSE_INVALID_UNIT,
	MSE_OUT_OF_RANGE,
};

struct option;

extern int only_digits(const char *s);
extern int dt_lock_drbd(const char* device);
extern void dt_unlock_drbd(int lock_fd);
extern void dt_release_lockfile(int drbd_fd);
extern int dt_minor_of_dev(const char *device);
extern int new_strtoll(const char *s, const char def_unit, unsigned long long *rv);
extern unsigned long long m_strtoll(const char* s,const char def_unit);
extern const char* make_optstring(struct option *options, char startc);
extern char* ppsize(char* buf, size_t size);
extern void dt_print_gc(const uint32_t* gen_cnt);
extern void dt_pretty_print_gc(const uint32_t* gen_cnt);
extern void dt_print_uuids(const uint64_t* uuid, unsigned int flags);
extern void dt_pretty_print_uuids(const uint64_t* uuid, unsigned int flags);
extern int fget_token(char *s, int size, FILE* stream);
extern int sget_token(char *s, int size, const char** text);
extern uint64_t bdev_size(int fd);
extern void get_random_bytes(void* buffer, int len);

extern int force; /* global option to force implicit confirmation */
extern int confirmed(const char *text);

extern const char* shell_escape(const char* s);

/* Since glibc 2.8~20080505-0ubuntu7 asprintf() is declared with the
   warn_unused_result attribute.... */
extern int m_asprintf(char **strp, const char *fmt, ...);

extern void fprintf_hex(FILE *fp, off_t file_offset, const void *buf, unsigned len);

/* If the lower level device is resized,
 * and DRBD did not move its "internal" meta data in time,
 * the next time we try to attach, we won't find our meta data.
 *
 * Some helpers for storing and retrieving "last known"
 * information, to be able to find it regardless,
 * without scanning the full device for magic numbers.
 */

/* We may want to store more things later...  if so, we can easily change to
 * some NULL terminated tag-value list format then.
 * For now: store the last known lower level block device size,
 * and its /dev/<name> */
struct bdev_info {
	uint64_t bd_size;
	uint64_t bd_uuid;
	char *bd_name;
};

/* these return 0 on sucess, error code if something goes wrong. */
/* create (update) the last-known-bdev-info file */
extern int lk_bdev_save(const unsigned minor, const struct bdev_info *bd);
/* we may want to remove all stored information */
extern int lk_bdev_delete(const unsigned minor);
/* load info from that file.
 * caller should free(bd->bd_name) once it is no longer needed. */
extern int lk_bdev_load(const unsigned minor, struct bdev_info *bd);

#endif
