#ifndef H_WIDGET
#define H_WIDGET

#include "graphx.h"

// #define WIDGET_DEBUG

typedef struct WidgetButtonStruct WidgetButton;
typedef struct WidgetImageStruct WidgetImage;
typedef struct WidgetTextStruct WidgetText;
typedef struct WidgetContainerStruct WidgetContainer;
typedef struct WidgetWindowStruct WidgetWindow;

typedef enum {
    FLAG_OUTLINE = 1,
    FLAG_CENTER = 2
} BUTTON_FLAG;

typedef enum {
    WIDGET_BUTTON,
    WIDGET_IMAGE,
    WIDGET_TEXT
} WidgetType;



// Buttons

WidgetButton* widget_ButtonAdd( WidgetContainer *widgetNode, 
                                const char *string,
                                void(*action)(),
                                void* actionData,
                                bool outline,
                                bool justify);

WidgetButton*  widget_ButtonIconAdd(WidgetContainer *widgetNode, 
                                    const char *string,
                                    void(*action)(),
                                    void *actionData,
                                    const bool outline,
                                    const bool justify,
                                    gfx_sprite_t *icon,
                                    const int iconWidth,
                                    const int iconHeight);

void widget_ButtonDestroy(WidgetButton *button);


// Text
void widget_TextNoMalloc(   WidgetText *text,
                            char *string);
                            
void widget_TextByFunction( WidgetText *text,
                            char *(*function)(),
                            void *functionData);

WidgetText* widget_TextAdd( WidgetContainer *widgetNode, 
                            const char *string,
                            const int width,
                            const bool outline,
                            const bool justify);

WidgetText* widget_TextIconAdd( WidgetContainer *widgetNode, 
                                const char *string,
                                const int width,
                                const bool outline,
                                const bool justify,
                                gfx_sprite_t *icon,
                                const int iconWidth,
                                const int iconHeight);

// Images

WidgetImage* widget_ImageAdd(   WidgetContainer *widgetNode, 
                                gfx_sprite_t *image,
                                const int imageX,
                                const int imageY,
                                const char scale);


// Container

WidgetContainer *widget_WindowContainerAdd( WidgetWindow *window, 
                                            const int width, 
                                            const int height);

void widget_WindowContainerBackgroundSet(   WidgetContainer *widgetNode, 
                                            const uint8_t backgroundColor);
void widget_WindowContainerOutlineSet(  WidgetContainer *widgetNode, 
                                        const bool outline);


// Windows

WidgetWindow *widget_WindowCreate(  const char *title, 
                                    const int width, 
                                    const int height);
void widget_WindowNameChange_NoMalloc(  WidgetWindow *widgetWindow,
                                        char *title);
void widget_WindowShow( WidgetWindow *window, 
                        char *key);
char* widget_WindowTitleGet( WidgetWindow *window);

void widget_WindowDestroy(WidgetWindow *window);
#endif