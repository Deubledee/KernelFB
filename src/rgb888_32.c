#include "kernelfb.h"

void rgb888_32_set_pixel(unsigned int i, unsigned int j, const struct kfb_color* color, struct kfb_handle* handle);
u_int32_t rgb888_32_color_to_uint32(const struct kfb_color* color);

struct kfb_ops kfb_ops_rgb888_32 = {
	.set_pixel = rgb888_32_set_pixel,
	.color_to_uint32 = rgb888_32_color_to_uint32
};

void rgb888_32_set_pixel(unsigned int i, unsigned int j, const struct kfb_color* color, struct kfb_handle* handle){
	unsigned int index;
	u_int32_t icolor;
	
	if(i >= handle->vinfo.xres || j >= handle->vinfo.yres)
		return;
	
	index = (j * handle->vinfo.xres + i) * 4;
	
	if(index + 4 > handle->buffer_length)
		return;
		
	icolor = rgb888_32_color_to_uint32(color);
	handle->buffer[index] = (unsigned char)((icolor >> 24) & 0x000000FF);
	handle->buffer[index + 1] = (unsigned char)((icolor >> 16) & 0x000000FF);
	handle->buffer[index + 2] = (unsigned char)((icolor >> 8) & 0x000000FF);
	handle->buffer[index + 3] = (unsigned char)(icolor & 0x000000FF);
	//kfb_printe("%X\n", *(u_int32_t*)&handle->buffer[index]);
}

u_int32_t rgb888_32_color_to_uint32(const struct kfb_color* color){
	u_int32_t mask = 0x0000FF00;
	u_int32_t icolor = 0x00000000;
	icolor |= mask & (color->r << 8);
	mask <<= 8;
	icolor |= mask & (color->g << 16);
	mask <<= 8;
	icolor |= mask & (color->b << 24);
	return icolor;
}
