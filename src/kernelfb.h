#ifndef KFB_KERNEL_FB_H
#define KFB_KERNEL_FB_H

#include <linux/fb.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include "dbg_print.h"

#define KFB_OPS_MAX 4

struct kfb_handle;
struct kfb_color{
	u_int8_t r, g, b;
};

struct kfb_ops{
	void (*set_pixel)(unsigned int i, unsigned int j, const struct kfb_color* color, struct kfb_handle* handle);
	u_int32_t (*color_to_uint32)(const struct kfb_color* color);
};

struct kfb_handle{
	struct file* fp;
	unsigned char* buffer;
	unsigned long buffer_length;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	struct kfb_ops* ops;
};

typedef struct kfb_handle kfb_handle_t;

enum kfb_result{KFB_OK, KFB_FAILED};
typedef enum kfb_result kfb_result_t;

extern struct kfb_ops* kfb_ops_var[KFB_OPS_MAX];

kfb_result_t kfb_create(const char* dev_path, kfb_handle_t ** kfb_handle);
kfb_result_t kfb_delete(kfb_handle_t ** kfb_handle);
kfb_result_t kfb_flush(kfb_handle_t* kfb_handle);

static inline struct kfb_color MAKE_COLOR(u_int8_t r, u_int8_t g, u_int8_t b){
	struct kfb_color k = {r, g, b};
	return k;
}
#endif
