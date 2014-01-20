#include "kernelfb.h"
#include "general.h"

void vga16fb_set_pixel(unsigned int i, unsigned int j, struct kfb_color color, struct kfb_handle* handle);
u_int32_t vga16fb_color_to_uint32(struct kfb_color color);
struct kfb_point vga16fb_print_char(char c, unsigned int i, unsigned int j, 
									struct kfb_color fore_color, 
									struct kfb_color bg_color, 
									struct kfb_handle* handle);
struct kfb_point vga16fb_print_str(const char* str, unsigned int i, unsigned int j, 
								struct kfb_color fore_color, 
								struct kfb_color bg_color, 
								int wrap_text, unsigned int wrap_start_x, unsigned int wrap_end_x,
								struct kfb_handle* handle);

struct kfb_ops kfb_ops_vga16fb = {
	.set_pixel = vga16fb_set_pixel,
	.color_to_uint32 = vga16fb_color_to_uint32,
	.print_char = vga16fb_print_char,
	.print_str = vga16fb_print_str,
	.draw_rect = general_draw_rect,
	.draw_hline = general_draw_hline,
	.draw_vline = general_draw_vline,
	.fill_rect = general_fill_rect
};


void vga16fb_set_pixel(unsigned int i, unsigned int j, struct kfb_color color, struct kfb_handle* handle){
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

u_int32_t vga16fb_color_to_uint32(struct kfb_color color){
	if(color.r !=0 || color.b != 0 || color.g != 0)
		return 0x1;
	return 0x0;
}

struct kfb_point vga16fb_print_char(char c, unsigned int i, unsigned int j, 
							struct kfb_color fore_color, 
							struct kfb_color bg_color, 
							struct kfb_handle* handle){
	fore_color.r = 0x01;
	bg_color.r = bg_color.g = bg_color.b = 0x00;
	return general_print_char(c, i, i, fore_color, bg_color, handle);
}
struct kfb_point vga16fb_print_str(const char* str, unsigned int i, unsigned int j, 
								struct kfb_color fore_color, 
								struct kfb_color bg_color, 
								int wrap_text, unsigned int wrap_start_x, unsigned int wrap_end_x,
								struct kfb_handle* handle){
	fore_color.r = 0x01;
	bg_color.r = bg_color.g = bg_color.b = 0x00;
	return general_print_str(str, i, i, fore_color, bg_color, wrap_text, wrap_start_x, wrap_end_x, handle);
}
