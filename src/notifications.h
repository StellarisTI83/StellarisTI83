#ifndef H_NOTIFICATIONS
#define H_NOTIFICATIONS

#include "generic_lists.h"
#include "time.h"

/* types =============================================================== */

enum NOTIFICATION_TYPE_STRUCT {
    NONE_TYPE,
    LOW_PRIORITY,
    MED_PRIORITY,
    HIG_PRIORITY
};

enum NOTIFICATION_ID_STRUCT {
    NONE_ID,
    LOW_RESSOURCES
};

typedef enum NOTIFICATION_TYPE_STRUCT NOTIFICATION_TYPE;
typedef enum NOTIFICATION_ID_STRUCT NotificationId;

typedef GenericList NotificationList;

typedef struct NotificationStruct Notification;

/* entry points ======================================================== */

NotificationList *CreateNotificationList();
void FreeNotificationList(NotificationList *notificationList);
void DrawNotifications(NotificationList *notificationList, Time *date);
void NewNotification(NotificationList *notificationList, NOTIFICATION_TYPE type, NotificationId ID, int length);

#endif