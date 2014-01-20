#include "kernelfb.h"
#include "general.h"
#include "pixel_font.h"

struct kfb_point general_print_char(char c, unsigned int i, unsigned int j, 
							struct kfb_color fore_color, 
							struct kfb_color bg_color, 
							struct kfb_handle* handle){
	kfb_point_t point = {i + DEFAULT_FONT_WIDTH, j};
	int k, l;
	unsigned char mask;
	for(k = 0; k < 16; k++){
		mask = 0x01;
		for(l = 7; l >= 0; l--){
			if(mask & default_font[(unsigned char)c][k]){
				handle->ops->set_pixel(i + l, j + k, fore_color, handle);
			}else{
				handle->ops->set_pixel(i + l, j + k, bg_color, handle);
			}
			mask <<= 1;
		}
	}
	return point;
}
struct kfb_point general_print_str(const char* str, unsigned int i, unsigned int j, 
								struct kfb_color fore_color, 
								struct kfb_color bg_color, 
								int wrap_text, unsigned int wrap_start_x, unsigned int wrap_end_x,
								struct kfb_handle* handle){
	kfb_point_t point = {i, j};

	if(!wrap_text)
		wrap_start_x = i;
	else if(wrap_start_x > handle->vinfo.xres - 1)
		wrap_start_x = 0;
		
	if(!wrap_text || wrap_end_x > handle->vinfo.xres - 1)
		wrap_end_x = handle->vinfo.xres - 1;
	
	while(*str != '\0'){
		if(j > handle->vinfo.yres)
			break;
			
		if(i > wrap_end_x || *str == '\n'){
			if(!wrap_text)
				break;
			i = wrap_start_x;
			j += DEFAULT_FONT_HEIGHT;
		}else if (*str == '\r'){
			if(wrap_text)
				i = wrap_start_x;
		}else{
			i += DEFAULT_FONT_WIDTH;
			handle->ops->print_char(*str, i, j, fore_color, bg_color, handle);
		}
		str++;
	}
	
	point.x = i, point.y = j;
	return point;
}
