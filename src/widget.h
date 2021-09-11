typedef struct WidgetButtonStruct WidgetButton;
typedef struct WidgetContainerStruct WidgetContainer;
typedef struct WidgetWindowStruct WidgetWindow;


void widget_ButtonAdd(  WidgetContainer *widgetNode, 
                        const char *string, 
                        int(*action)(),
                        void* action1,
                        void* action2);
void widget_ButtonDestroy(WidgetButton *button);

WidgetContainer *widget_WindowContainerAdd(WidgetWindow *window);

WidgetWindow *widget_WindowCreate(  const char *title, 
                                    const int width, 
                                    const int height);
void widget_WindowShow( WidgetWindow *window, 
                        char *key);