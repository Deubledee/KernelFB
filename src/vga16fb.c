#include "kernelfb.h"

void vga16fb_set_pixel(unsigned int i, unsigned int j, const struct kfb_color* color, struct kfb_handle* handle);
u_int32_t vga16fb_color_to_uint32(const struct kfb_color* color);

struct kfb_ops kfb_ops_vga16fb = {
	.set_pixel = vga16fb_set_pixel,
	.color_to_uint32 = vga16fb_color_to_uint32
};


void vga16fb_set_pixel(unsigned int i, unsigned int j, const struct kfb_color* color, struct kfb_handle* handle){
	u_int32_t mask = 0x00000001;
	unsigned int index, offset;
	u_int32_t icolor, tmp;
	
	if(i > handle->vinfo.xres || j > handle->vinfo.yres)
		return;
	
	index = (j * handle->vinfo.xres + i) / 8;
	offset = (j * handle->vinfo.xres + i) % 8;
	
	if(index > handle->buffer_length)
		return;
		
	icolor = vga16fb_color_to_uint32(color);
	tmp = (handle->buffer[index] & ~(mask << (7 - offset))) | icolor;
	handle->buffer[index] = tmp;
}

u_int32_t vga16fb_color_to_uint32(const struct kfb_color* color){
	if(color->r !=0 || color->b != 0 || color->g != 0)
		return 0x1;
	return 0x0;
}
