#ifndef food_include_file
#define food_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define food_width 9
#define food_height 11
#define food_size 101
#define food ((gfx_sprite_t*)food_data)
extern unsigned char food_data[101];

#ifdef __cplusplus
}
#endif

#endif
