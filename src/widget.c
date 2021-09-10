/**
 * @file widget.c
 * @author Cocheril Dimitri (cochgit.dimitri@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-09-09
 * 
 * @copyright GNU General Public License v3.0
 * 
 */
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <graphx.h>
#include <debug.h>

#include "widget.h"
#include "colors.h"

struct WidgetButtonStruct{
    int (*action)();
    char *text;
};

struct WidgetContainerStruct{
    int x;
    int y;
    int width;
    int height;

    void *begin;

    WidgetContainer *next;
};
                
struct WidgetWindowStruct{
    int x;
    int y;
    int width;
    int height;
    bool title;
    char *titleString;

    WidgetContainer *begin;
};

void widget_ButtonAdd(  WidgetContainer *widgetNode, 
                        const char *string, 
                        int(*action)()){
    WidgetButton *button = calloc(1, sizeof(WidgetButton));
    assert(button);
    assert(widgetNode);

    if(string){
        button->text = calloc(1, sizeof(char) * (strlen(string) + 1));
        assert(button->text);
        strcpy(button->text, string);
    }

    assert(action);
    button->action = action;
}

void widget_ButtonDestroy(WidgetButton *button){
    assert(button);
    free(button->text);
    free(button);
}




WidgetContainer *widget_WindowContainerAdd(WidgetWindow *window){
    WidgetContainer *container, *tmp;
    assert(window);
    container = calloc(1, sizeof(WidgetContainer));
    container->width = 100;
    container->height = 100;

    tmp = window->begin;
    while(tmp->next){
        tmp = tmp->next;
    }
    tmp->next = container;

    return container;
}
void widget_WindowContainerDestroy(WidgetWindow *window){
    free(window->begin);
}
void widget_ContainerShow(WidgetContainer *widgetNode){
    gfx_SetColor(COLOR_HUD_OUTLINES);
    gfx_Rectangle(  widgetNode->x, 
                    widgetNode->y, 
                    widgetNode->width, 
                    widgetNode->height);
}



WidgetWindow *widget_WindowCreate(const char *title) {
    WidgetWindow *widgetWindow = calloc(1, sizeof(WidgetWindow));
    assert(widgetWindow);
    widgetWindow->x = 40;
    widgetWindow->y = 40;
    widgetWindow->width = 240;
    widgetWindow->height = 160;
    if(title) {
        widgetWindow->title = true;
        widgetWindow->titleString = calloc(1, strlen(title) * sizeof(char));
        strcpy(widgetWindow->titleString, title);
    }
    return widgetWindow;
}

void widget_WindowShow(WidgetWindow *window){
    WidgetContainer *container = window->begin;
    gfx_SetColor(COLOR_HUD_BACKGROUND);
    gfx_FillRectangle_NoClip(   window->x, 
                                window->y, 
                                window->width, 
                                window->height); //Background

    gfx_SetColor(COLOR_HUD_OUTLINES);
    gfx_Rectangle_NoClip(   window->x, 
                            window->y, 
                            window->width, 
                            window->height); //Outlines

    if(window->titleString){
        gfx_HorizLine_NoClip(window->x + 5, window->y + 11, 180); //***
        gfx_Line_NoClip(window->x + 185, window->y + 11, window->x + 190, window->y + 6); //* Horizontal line under the title
        gfx_HorizLine_NoClip(window->x + 191, window->y + 6, 39); //***
        gfx_PrintStringXY(window->titleString, window->x + 5, window->x + 2);
    }

    gfx_SetColor(COLOR_WHITE);
    gfx_SetPixel(270, 45); // Pixel at the end of the line
    while(container) {
        widget_ContainerShow(container);
        container = container->next;
    }
}

void widget_WindowDestroy(WidgetWindow *window) {
    free(window);
}