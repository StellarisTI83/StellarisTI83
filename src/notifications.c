#include <stdlib.h>

#include <debug.h>

#include <graphx.h>
#include <fontlibc.h>

#include "gfx/gfx.h"

#include "main.h"

#include "notifications.h"

/* structures ========================================================== */

struct NotificationStruct {
    NOTIFICATION_TYPE type;
    NotificationId ID;
    int length;
};

/* entry points ======================================================== */

NotificationList *CreateNotificationList(){
    return (NotificationList*)GenericList_Create();
}

void FreeNotificationList(NotificationList *notificationList) {
    Notification *notification = NULL;
    int i = 0;
    #ifdef DEBUG_VERSION
    dbg_sprintf(dbgout, "Free notification list\n");
    #endif
    if(!notificationList)
        return;
    notification = GenericCell_Get((GenericList*)notification, i);
    while(notification != NULL) {
        free(notification);
        i++;
        notification = GenericCell_Get((GenericList*)notification, i);
    }
    GenericList_Free((GenericList*)notificationList);
}

static void DrawNotificationLow(int x, int y) {
    gfx_SetColor(6);
    gfx_FillCircle_NoClip(x + 7, y + 14, 7);
    gfx_SetColor(7);
    gfx_VertLine_NoClip(x + 7, y + 1, 5);
    gfx_Circle_NoClip(x + 7, y + 14, 7);
    gfx_SetColor(1);
    gfx_SetPixel(x + 7, y);
    gfx_SetPixel(x + 7, y + 6);
}

static void DrawNotificationMed(int x, int y) {
    gfx_SetColor(31);
    gfx_FillRectangle_NoClip(x, y + 7, 15, 15);
    gfx_SetColor(30);
    gfx_VertLine_NoClip(x + 7, y + 1, 5);
    gfx_Rectangle_NoClip(x, y + 7, 15, 15);
    gfx_SetColor(1);
    gfx_SetPixel(x + 7, y);
    gfx_SetPixel(x + 7, y + 6);
}

static void DrawNotificationLogo(int x, int y, NotificationId ID) {
    switch(ID){
        default:
            gfx_Sprite_NoClip(icon_low_ressources, x + 1, y + 7);
            break;
    }
}

void DrawNotifications(NotificationList *notificationList, Time *date) {
    Notification *notification = NULL;
    int numberOfNotifications = GenericList_ArraySize((GenericList*)notificationList);
    int notificationIndex;
    int x = 50;
    int y = 22;
    for(notificationIndex = 0; notificationIndex <= numberOfNotifications; notificationIndex++) {
        notification = GenericCell_Get((GenericList*)notificationList, notificationIndex);
        if(notification != NULL) {
            if(notification->type == MED_PRIORITY)
                DrawNotificationMed(x, y);
            else
                DrawNotificationLow(x, y);

            DrawNotificationLogo(x, y, notification->ID);
            x += 17;
            if(time_TickGet(date) == 0){
                notification->length--;
            }
            if(notification->length <= 0) {
                GenericCell_Free((GenericList*)notificationList, notificationIndex);
                free(notification);
            }
        }
    }
    if(time_TickGet(date) == 0)
        time_TickIncrement(date);
}

void NewNotification(NotificationList *notificationList, NOTIFICATION_TYPE type, NotificationId ID, int length) {
    Notification *notification = malloc(sizeof(Notification));	
    if(!notification){
        #ifdef DEBUG_VERSION
        dbg_sprintf(dbgerr, "Malloc returned NULL when adding notification");
        #endif
        exit(EXIT_FAILURE);
    }
    notification->type = type;
    notification->ID = ID;
    notification->length = length;
    GenericCell_Add((GenericList*)notificationList, notification);
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "New notification\n - type: %d\n - ID: %d\n - length: %d\n", (int)type, (int)ID, length);
    #endif
}