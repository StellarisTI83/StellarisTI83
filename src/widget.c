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

    void (*action)();
    void *actionData;
    char *text;

    gfx_sprite_t *icon;
    int iconWidth;
    int iconY;

    void *next;
};

struct WidgetTextStruct{
    int x;
    int y;
    int width;
    int height;

    char flags;

    char *text;
    char *(*function)();
    void *functionData;

    gfx_sprite_t *icon;
    int iconWidth;
    int iconY;

    void *next;
};

struct WidgetImageStruct{
    int x;
    int y;
    char scale;

    gfx_sprite_t *image;
    gfx_sprite_t *overlay;

    void *next;
};

struct WidgetContainerStruct{
    int x;
    int y;
    int width;
    int height;

    uint8_t backgroundColor;
    bool outline;

    int selection;

    WidgetType type;

    void *begin;

    WidgetContainer *next;
};
                
struct WidgetWindowStruct{
    int x;
    int y;
    int width;
    int height;

    char titleMallocType;
    char *titleString;

    WidgetContainer *begin;
};

WidgetButton*  widget_ButtonIconAdd(  WidgetContainer *widgetNode, 
                        const char *string,
                        void(*action)(),
                        void *actionData,
                        const bool outline,
                        const bool justify,
                        gfx_sprite_t *icon,
                        const int iconWidth,
                        const int iconHeight){
    WidgetButton* button = widget_ButtonAdd(widgetNode, 
                                            string,
                                            action,
                                            actionData,
                                            outline,
                                            justify);

    button->icon = icon;
    button->iconWidth = iconWidth;
    button->iconY = button->y + (button->height - iconHeight) / 2;
    return button;
}

WidgetButton* widget_ButtonAdd(  WidgetContainer *widgetNode, 
                        const char *string,
                        void(*action)(),
                        void *actionData,
                        const bool outline,
                        const bool justify){
    WidgetButton *button, *tmp;
    int y;

    assert(widgetNode);

    button = calloc(1, sizeof(WidgetButton));
    assert(button);
    
    if(widgetNode->begin){
        if(widgetNode->type != WIDGET_BUTTON)
            exit(EXIT_FAILURE);
        tmp = widgetNode->begin;
        y = tmp->y + tmp->height + MENU_BUTTON_GAP;
        while(tmp->next){
            y += tmp->height + MENU_BUTTON_GAP;
            tmp = tmp->next;
        }
        tmp->next = button;
    } else {
        widgetNode->begin = button;
        widgetNode->type = WIDGET_BUTTON;
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
        button->flags = button->flags | FLAG_OUTLINE;
    if(justify)
        button->flags = button->flags | FLAG_CENTER;
    
    button->action = action;
    button->actionData = actionData;
    return button;
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
    if(button->flags & FLAG_OUTLINE){
        gfx_SetColor(COLOR_HUD_OUTLINES);
        gfx_Rectangle(  button->x, 
                        button->y, 
                        button->width, 
                        button->height);
    }
    
    x = (button->flags & FLAG_CENTER) ? button->x + (button->width - strlen(button->text) * TEXT_HEIGHT)/2 : button->x + MENU_BUTTON_GAP;

    if(status)
        gfx_SetTextFGColor(COLOR_YELLOW);
    else
        gfx_SetTextFGColor(COLOR_WHITE);

    
    if(button->icon){
        gfx_TransparentSprite_NoClip(button->icon, button->x + MENU_BUTTON_GAP, button->iconY);
        x += button->iconWidth + MENU_BUTTON_GAP;
    }
    gfx_PrintStringXY(  button->text, 
                        x,
                        button->y + (button->height - TEXT_HEIGHT) / 2);

    if(click && button->action)
        (*button->action)(button->actionData);
    return 0;
}






WidgetImage* widget_ImageAdd(   WidgetContainer *widgetNode, 
                                gfx_sprite_t *image,
                                const int imageX,
                                const int imageY,
                                const char scale){
    WidgetImage* widgetImage = calloc(1, sizeof(WidgetImage)), *tmp;

    assert(widgetNode);

    assert(widgetNode);
    assert(widgetImage);

    if(widgetNode->begin){
        if(widgetNode->type != WIDGET_IMAGE)
            exit(EXIT_FAILURE);
        tmp = widgetNode->begin;
        while(tmp->next){
            tmp = tmp->next;
        }
        tmp->next = widgetImage;
    } else {
        widgetNode->begin = widgetImage;
        widgetNode->type = WIDGET_IMAGE;
    }

    widgetImage->image = image;
    widgetImage->scale = scale;
    widgetImage->x = imageX;
    widgetImage->y = imageY;
    
    return widgetImage;
}

void widget_ImageShow(WidgetImage* widgetImage){
    gfx_ScaledTransparentSprite_NoClip(widgetImage->image, widgetImage->x, widgetImage->y, widgetImage->scale, widgetImage->scale);
}




void widget_TextNoMalloc(   WidgetText *text,
                            char *string){
    assert(text);
    assert(string);
    text->text = string;
}

void widget_TextByFunction( WidgetText *text,
                            char *(*function)(),
                            void *functionData){
    assert(text);
    assert(function);
    text->function = function;
    text->functionData = functionData;
}

WidgetText* widget_TextIconAdd( WidgetContainer *widgetNode, 
                                const char *string,
                                const int width,
                                const bool outline,
                                const bool justify,
                                gfx_sprite_t *icon,
                                const int iconWidth,
                                const int iconHeight){

    WidgetText* text = widget_TextAdd(  widgetNode, 
                                        string,
                                        width,
                                        outline,
                                        justify);   
    text->icon = icon;
    text->iconWidth = iconWidth;
    text->iconY = text->y + (text->height - iconHeight) / 2;
    return text;
}


WidgetText* widget_TextAdd( WidgetContainer *widgetNode, 
                            const char *string,
                            const int width,
                            const bool outline,
                            const bool justify){
    WidgetText *text, *tmp;
    int x = widgetNode->x + MENU_BUTTON_GAP, y = widgetNode->y + MENU_BUTTON_GAP;

    assert(widgetNode);

    text = calloc(1, sizeof(WidgetText));
    assert(text);
    
    text->width = ((widgetNode->width - MENU_BUTTON_GAP * 2) * width) / 100;
    text->height = TEXT_HEIGHT * 2;

    if(widgetNode->begin){
        if(widgetNode->type != WIDGET_TEXT)
            exit(EXIT_FAILURE);

        tmp = widgetNode->begin;

        while(tmp->next){
            tmp = tmp->next;
        }

        tmp->next = text;
        x = tmp->x + tmp->width + MENU_BUTTON_GAP;
        y = tmp->y;
        if(x + text->width > widgetNode->x + widgetNode->width) {
            x = widgetNode->x + MENU_BUTTON_GAP;
            y += tmp->height + MENU_BUTTON_GAP;
        }
    } else {
        widgetNode->begin = text;
        widgetNode->type = WIDGET_TEXT;
        y = widgetNode->y + MENU_BUTTON_GAP;
    }

    text->x = x;
    text->y = y;

    if(string){
        text->text = calloc(1, sizeof(char) * (strlen(string) + 1));
        assert(text->text);
        strcpy(text->text, string);
    }

    if(outline)
        text->flags = text->flags | FLAG_OUTLINE;
    if(justify)
        text->flags = text->flags | FLAG_CENTER;
    
    return text;
}

void widget_TextShow(WidgetText* text){
    int x;
    if(text->flags & FLAG_OUTLINE){
        gfx_SetColor(COLOR_HUD_OUTLINES);
        gfx_Rectangle(  text->x, 
                        text->y, 
                        text->width, 
                        text->height);
    }
    
    x = (text->flags & FLAG_CENTER) ? text->x + (text->width - strlen(text->text) * TEXT_HEIGHT)/2 : text->x + MENU_BUTTON_GAP;
    
    if(text->icon){
        gfx_TransparentSprite_NoClip(text->icon, text->x + MENU_BUTTON_GAP, text->iconY);
        x += text->iconWidth + MENU_BUTTON_GAP;
    }
    
    if(text->function)
        text->text = (*text->function)(text->functionData);

    gfx_SetTextFGColor(COLOR_WHITE);

    gfx_PrintStringXY(  text->text, 
                        x,
                        text->y + (text->height - TEXT_HEIGHT) / 2);
}







WidgetContainer *widget_WindowContainerAdd( WidgetWindow *window, 
                                            const int width, 
                                            const int height){
    WidgetContainer *container, *tmp;
    int x = 0, y = 0, windowY = window->y, windowHeight = window->height;
    assert(window);
    container = calloc(1, sizeof(WidgetContainer));


    if(window->titleMallocType){ // If there is a title
        windowY += 11;
        windowHeight -= 11;
    }

    x = window->x;
    y = windowY;
    
    container->width = (window->width*width)/100;
    container->height = (windowHeight*height)/100;
    
    container->backgroundColor = COLOR_TRANSPARENT;

    if(window->begin){
        tmp = window->begin;
        x += tmp->width;
        if(x + container->width > window->x + window->width) {
            x = window->x;
            y += tmp->height;
        }
        while(tmp->next){
            x += tmp->width;
            if(x + container->width > window->x + window->width) {
                x = window->x;
                y += tmp->height;
            }
            tmp = tmp->next;
        }
        tmp->next = container;
    } else {
        window->begin = container;
    }
    
    container->x = x;
    container->y = y;

    return container;
}

void widget_WindowContainerDestroy(WidgetContainer *widgetNode){
    WidgetButton *button, *buttonTemp;
    WidgetImage *image, *imageTemp;
    WidgetText *text, *textTemp;
    assert(widgetNode);
    button = widgetNode->begin;
    image = widgetNode->begin;
    text = widgetNode->begin;
    if(widgetNode->type == WIDGET_BUTTON){
        while(button) {
            buttonTemp = button->next;
            widget_ButtonDestroy(button);
            button = buttonTemp;
        }
    } else if(widgetNode->type == WIDGET_IMAGE){
        while(image) {
            imageTemp = image->next;
            free(image);
            image = imageTemp;
        }
    } else if(widgetNode->type == WIDGET_TEXT){
        while(text) {
            textTemp = text->next;
            free(text);
            text = textTemp;
        }
    }
}

void widget_WindowContainerBackgroundSet(   WidgetContainer *widgetNode, 
                                            const uint8_t backgroundColor){
    widgetNode->backgroundColor = backgroundColor;
}
void widget_WindowContainerOutlineSet(  WidgetContainer *widgetNode, 
                                        const bool outline){
    assert(widgetNode);
    widgetNode->outline = outline;
}

static void widget_ContainerShow(   WidgetContainer *widgetNode, 
                                    char *key){
    WidgetButton *button;
    WidgetImage *image;
    WidgetText *text;
    int index = 0;
    assert(widgetNode);
    button = widgetNode->begin;
    image = widgetNode->begin;
    text = widgetNode->begin;
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

    #ifdef WIDGET_DEBUG
    gfx_SetColor(COLOR_ORANGE);
    gfx_Rectangle(widgetNode->x, widgetNode->y, widgetNode->width,widgetNode->height);
    #endif

    if(widgetNode->backgroundColor != COLOR_TRANSPARENT){
        gfx_SetColor(widgetNode->backgroundColor);
        gfx_FillRectangle_NoClip(widgetNode->x, widgetNode->y, widgetNode->width, widgetNode->height);
    }
    if(widgetNode->outline){
        gfx_SetColor(COLOR_HUD_OUTLINES);
        gfx_Rectangle_NoClip(widgetNode->x, widgetNode->y, widgetNode->width, widgetNode->height);
    }

    if(widgetNode->type == WIDGET_BUTTON){
        while(button) {
            widget_ButtonShow(  button, 
                                index == widgetNode->selection, 
                                (index == widgetNode->selection) && *key == sk_Enter);
            button = button->next;
            index++;
        }
    } else if(widgetNode->type == WIDGET_IMAGE){
        while(image) {
            widget_ImageShow(image);
            image = image->next;
        }
    } else if(widgetNode->type == WIDGET_TEXT){
        while(text) {
            widget_TextShow(text);
            text = text->next;
        }
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
    if(title > (char*)1) {
        widgetWindow->titleMallocType = true;
        widgetWindow->titleString = calloc(1, strlen(title) * sizeof(char));
        strcpy(widgetWindow->titleString, title);
    } else if(title == (char*)1) {
        widgetWindow->titleMallocType = true;
    }
    return widgetWindow;
}

void widget_WindowNameChange_NoMalloc(  WidgetWindow *widgetWindow,
                                        char *title){
    widgetWindow->titleString = title;
    widgetWindow->titleMallocType = 2;
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

char* widget_WindowTitleGet( WidgetWindow *window){
    return window->titleString;
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
    if(window->titleMallocType == true)
        free(window->titleString);
    free(window);
}