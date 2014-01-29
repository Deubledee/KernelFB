
#include "kernelfb.h"
#include "dbg_print.h"
#include <asm/uaccess.h>

static struct kfb_ops* get_proper_ops(kfb_handle_t *handle){
	switch(handle->vinfo.bits_per_pixel){
	case 32:
		return kfb_ops_var[1];
		break;
	case 4:
		if(handle->vinfo.xres == handle->finfo.line_length * 8){ //1bit per pixel
			handle->vinfo.bits_per_pixel = 1;
			return kfb_ops_var[0];
		}
	default:
		return NULL;
	}
}

kfb_result_t kfb_create(const char* dev_path, kfb_handle_t ** kfb_handle){
	kfb_handle_t *handle;
	mm_segment_t old_fs;
	
	if(!kfb_handle)
		return KFB_FAILED;
	
	handle = kmalloc(sizeof(kfb_handle_t), GFP_KERNEL);
	memset(handle, 0, sizeof(kfb_handle_t));

	handle->fp = filp_open(dev_path, O_RDWR, 0);

    if (IS_ERR(handle->fp)){  
		kfb_printe("Failed to open device: %s\n", dev_path);
		goto FAILED;
	}

	old_fs = get_fs();
	set_fs(get_ds());

	if (handle->fp->f_op->unlocked_ioctl(handle->fp, FBIOGET_FSCREENINFO, (unsigned long)&handle->finfo)){  
		kfb_printe("Failed to get fix_screeninfo\n");
	    goto FAILED2;
	}  
          
	if (handle->fp->f_op->unlocked_ioctl(handle->fp, FBIOGET_VSCREENINFO, (unsigned long)&handle->vinfo)){  
		kfb_printe("Failed to get car_screeninfo\n");  
		goto FAILED2;
	}

	if(NULL == (handle->ops = get_proper_ops(handle))){
		kfb_printe("Failed to find a proper handler\n");
		goto FAILED2;
	}
	
	handle->buffer_length = handle->vinfo.xres * handle->vinfo.yres / 8 * handle->vinfo.bits_per_pixel;
	handle->buffer_length = (handle->buffer_length > handle->finfo.smem_len) ? handle->finfo.smem_len : handle->buffer_length;
	handle->buffer = kmalloc(handle->buffer_length, GFP_KERNEL);
	
	set_fs(old_fs);
	*kfb_handle = handle;
	return KFB_OK;
	
FAILED2:
	set_fs(old_fs);
	filp_close(handle->fp, NULL);
FAILED:
	kfree(handle);
	*kfb_handle = NULL;
	return KFB_FAILED;
	return KFB_OK;
}

kfb_result_t kfb_delete(kfb_handle_t ** kfb_handle){
	if(!kfb_handle || !*kfb_handle)
		return KFB_FAILED;
		
	if((*kfb_handle)->fp){
		filp_close((*kfb_handle)->fp, NULL);
	}
	
	if((*kfb_handle)->buffer){
		kfree((*kfb_handle)->buffer);
	}
	
	kfree(*kfb_handle);
	*kfb_handle = NULL;
	return KFB_OK;
}

kfb_result_t kfb_flush(kfb_handle_t* kfb_handle){
	mm_segment_t old_fs;
	unsigned int i;
	unsigned int l;
	
	if(!kfb_handle)
		return KFB_FAILED;
	
	l = kfb_handle->vinfo.xres * kfb_handle->vinfo.bits_per_pixel / 8;
	old_fs = get_fs();
	set_fs(get_ds());
	for(i = 0; i < kfb_handle->vinfo.yres; i++){
		kfb_handle->fp->f_pos = i * kfb_handle->finfo.line_length;
		kfb_handle->fp->f_op->write(kfb_handle->fp, kfb_handle->buffer + i * l, l, &kfb_handle->fp->f_pos);
	}
	set_fs(old_fs);
	return KFB_OK;
}
