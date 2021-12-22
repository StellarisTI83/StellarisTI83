#ifndef H_WIDGET
#define H_WIDGET

#include "graphx.h"

// #define WIDGET_DEBUG

typedef struct WidgetButtonStruct WidgetButton;
typedef struct WidgetImageStruct WidgetImage;
typedef struct WidgetContainerStruct WidgetContainer;
typedef struct WidgetWindowStruct WidgetWindow;

typedef enum {
    BUTTON_FLAG_OUTLINE = 1,
    BUTTON_FLAG_CENTER = 2 } BUTTON_FLAG;

typedef enum {
    WIDGET_BUTTON,
    WIDGET_IMAGE
} WidgetType;

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

WidgetImage* widget_ImageAdd(   WidgetContainer *widgetNode, 
                                gfx_sprite_t *image,
                                const int imageX,
                                const int imageY,
                                const char scale);

WidgetContainer *widget_WindowContainerAdd( WidgetWindow *window, 
                                            const int width, 
                                            const int height);

void widget_WindowContainerBackgroundSet(   WidgetContainer *widgetNode, 
                                            const uint8_t backgroundColor);
void widget_WindowContainerOutlineSet(  WidgetContainer *widgetNode, 
                                        const bool outline);

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