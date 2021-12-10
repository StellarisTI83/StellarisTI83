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
#include "menus.h"
#include "main.h"

struct WidgetButtonStruct{
    int x;
    int y;
    int width;
    int height;

    char flags;

    int (*action)();
    void* actionData;
    char *text;

    void *next;
};

struct WidgetContainerStruct{
    int x;
    int y;
    int width;
    int height;

    int selection;

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
                        int(*action)(),
                        void* actionData,
                        bool outline,
                        bool justify){
    WidgetButton *button, *tmp;
    int y;

    assert(widgetNode);

    button = calloc(1, sizeof(WidgetButton));
    assert(button);

    if(widgetNode->begin){
        tmp = widgetNode->begin;
        y = tmp->y + tmp->height + MENU_BUTTON_GAP;
        while(tmp->next){
            y += tmp->height + MENU_BUTTON_GAP;
            tmp = tmp->next;
        }
        tmp->next = button;
    } else {
        widgetNode->begin = button;
        y = widgetNode->y + MENU_BUTTON_GAP;
    }

    if(string){
        button->text = calloc(1, sizeof(char) * (strlen(string) + 1));
        assert(button->text);
        strcpy(button->text, string);
    }

    button->width = widgetNode->width - MENU_BUTTON_GAP * 2;
    button->height = TEXT_HEIGHT * 2;
    button->x = widgetNode->x + MENU_BUTTON_GAP;
    button->y = y;

    if(outline)
        button->flags = button->flags | BUTTON_FLAG_OUTLINE;
    if(justify)
        button->flags = button->flags | BUTTON_FLAG_CENTER;

    button->action = action;
    button->actionData = actionData;
}

void widget_ButtonDestroy(WidgetButton *button){
    assert(button);
    assert(button->text);
    if(button->actionData){
        free(button->actionData);
    }
    free(button->text);
    free(button);
}

int widget_ButtonShow(WidgetButton *button, bool status, bool click){
    int x;
    if(button->flags & BUTTON_FLAG_OUTLINE){
        gfx_SetColor(COLOR_HUD_OUTLINES);
        gfx_Rectangle(  button->x, 
                        button->y, 
                        button->width, 
                        button->height);
    }
    
    x = (button->flags & BUTTON_FLAG_CENTER) ? button->x + (button->width - strlen(button->text) * TEXT_HEIGHT)/2 : button->x + MENU_BUTTON_GAP;

    if(status)
        gfx_SetTextFGColor(COLOR_YELLOW);
    else
        gfx_SetTextFGColor(COLOR_WHITE);
    gfx_PrintStringXY(  button->text, 
                        x,
                        button->y + (button->height - TEXT_HEIGHT) / 2);
    if(click && button->action)
        (*button->action)(button->actionData);
    return 0;
}



WidgetContainer *widget_WindowContainerAdd(WidgetWindow *window){
    WidgetContainer *container, *tmp;
    assert(window);
    container = calloc(1, sizeof(WidgetContainer));

    container->x = window->x;
    if(window->titleString)
        container->y = window->y + 11;
    else
        container->y = window->y;
    container->width = window->width;
    container->height = window->height;
    container->selection = 0;

    if(window->begin){
        tmp = window->begin;
        while(tmp->next){
            tmp = tmp->next;
        }
        tmp->next = container;
    } else {
        window->begin = container;
    }

    return container;
}
void widget_WindowContainerDestroy(WidgetContainer *widgetNode){
    WidgetButton *button, *buttonTemp;
    assert(widgetNode);
    button = widgetNode->begin;
    while(button) {
        buttonTemp = button->next;
        widget_ButtonDestroy(button);
        button = buttonTemp;
    }
    free(widgetNode->begin);
}

static void widget_ContainerShow(   WidgetContainer *widgetNode, 
                                    char *key){
    WidgetButton *button;
    int index = 0;
    assert(widgetNode);
    button = widgetNode->begin;
    switch(*key){
        default:
            break;
        case sk_Down:
            widgetNode->selection++;
            break;
        case sk_Up:
            widgetNode->selection--;
            break;
    }
    while(button) {
        widget_ButtonShow(  button, 
                            index == widgetNode->selection, 
                            (index == widgetNode->selection) && *key == sk_Enter);
        button = button->next;
        index++;
    }
    if(widgetNode->selection >= index)
        widgetNode->selection = 0;
    else if(widgetNode->selection < 0)
        widgetNode->selection = index - 1;
}



WidgetWindow *widget_WindowCreate(  const char *title, 
                                    const int width, 
                                    const int height) {
    WidgetWindow *widgetWindow = calloc(1, sizeof(WidgetWindow));
    assert(widgetWindow);
    widgetWindow->x = (gfx_lcdWidth - width) / 2;
    widgetWindow->y = (gfx_lcdHeight - height) / 2;
    widgetWindow->width = width;
    widgetWindow->height = height;
    if(title) {
        widgetWindow->title = true;
        widgetWindow->titleString = calloc(1, strlen(title) * sizeof(char));
        strcpy(widgetWindow->titleString, title);
    }
    return widgetWindow;
}

void widget_WindowShow( WidgetWindow *window, 
                        char *key){
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
        gfx_HorizLine_NoClip(window->x + 191, window->y + 5, 39); //***
        gfx_PrintStringXY(window->titleString, window->x + 5, window->x + 2);
        gfx_SetColor(COLOR_WHITE);
        gfx_SetPixel(270, 45); // Pixel at the end of the line
    }
    
    while(container) {
        widget_ContainerShow(container, key);
        container = container->next;
    }
}
 
void widget_WindowDestroy(WidgetWindow *window) {
    WidgetContainer *container, *containerTemp;
    assert(window);
    container = window->begin;
    while(container) {
        containerTemp = container->next;
        widget_WindowContainerDestroy(container);
        container = containerTemp;
    }
    free(window);
}