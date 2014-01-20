#ifndef KFB_GENERAL_H
#define KFB_GENERAL_H
struct kfb_point general_print_char(char c, unsigned int i, unsigned int j, 
							struct kfb_color fore_color, 
							struct kfb_color bg_color, 
							struct kfb_handle* handle);
struct kfb_point general_print_str(const char* str, unsigned int i, unsigned int j, 
							struct kfb_color fore_color, 
							struct kfb_color bg_color, 
							int wrap_text, unsigned int wrap_start_x, unsigned int wrap_end_x,
							struct kfb_handle* handle);
#endif
