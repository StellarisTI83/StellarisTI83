#ifndef H_WIDGET
#define H_WIDGET

typedef struct WidgetButtonStruct WidgetButton;
typedef struct WidgetContainerStruct WidgetContainer;
typedef struct WidgetWindowStruct WidgetWindow;

typedef enum {
    BUTTON_FLAG_OUTLINE = 1,
    BUTTON_FLAG_CENTER = 2 } BUTTON_FLAG;

void widget_ButtonAdd(  WidgetContainer *widgetNode, 
                        const char *string,
                        int(*action)(),
                        void* actionData,
                        bool outline,
                        bool justify);
void widget_ButtonDestroy(WidgetButton *button);

WidgetContainer *widget_WindowContainerAdd(WidgetWindow *window);

WidgetWindow *widget_WindowCreate(  const char *title, 
                                    const int width, 
                                    const int height);
void widget_WindowShow( WidgetWindow *window, 
                        char *key);

void widget_WindowDestroy(WidgetWindow *window);
#endif