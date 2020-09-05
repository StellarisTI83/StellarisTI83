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
    NOTIFICATION_ID ID;
    int length;
};

/* entry points ======================================================== */

NotificationList *CreateNotificationList(){
    return (NotificationList*)CreateGenericList();
}

void FreeNotificationList(NotificationList *notificationList) {
    FreeGenericList((GenericList*)notificationList);
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

void DrawNotifications(NotificationList *notificationList, Date *date) {
    Notification *notification = NULL;
    int numberOfNotifications = GenericListArraySize((GenericList*)notificationList);
    int notificationIndex = 1;
    int x = 50;
    int y = 22;
    for(notificationIndex = 1; notificationIndex <= numberOfNotifications; notificationIndex++) {
        notification = GenericCellGet((GenericList*)notificationList, notificationIndex);
        if(notification != NULL) {
            if(notification->type == MED_PRIORITY)
                DrawNotificationMed(x, y);
            else
                DrawNotificationLow(x, y);
            x += 17;
            if(GetTimeClock(date) == 0){
                switch(GetTimeSpeed(date)) {
                    case -2:
                        notification->length--;
                        break;
                    case -1:
                        notification->length -= 2;
                        break;
                    case 1:
                        notification->length -= 3;
                        break;
                    case 2:
                        notification->length -= 4;
                        break;
                    case 3:
                        notification->length -= 6;
                        break;
                    default:
                        break;
                }
            }
            if(notification->length <= 0) {
                FreeGenericCell((GenericList*)notificationList, notificationIndex);
                free(notification);
            }
        }
    }
    if(GetTimeClock(date) == 0)
        AddTimeClock(date);
}

void NewNotification(NotificationList *notificationList, NOTIFICATION_TYPE type, NOTIFICATION_ID ID, int length) {
    Notification *notification = malloc(sizeof(Notification));
    notification->type = type;
    notification->ID = ID;
    notification->length = length;
    GenericCellAdd((GenericList*)notificationList, notification);
    #ifdef DEBUG_VERSION
        dbg_sprintf(dbgout, "New notification\n - type: %d\n - ID: %d\n - length: %d\n", (int)type, (int)ID, length);
    #endif
}