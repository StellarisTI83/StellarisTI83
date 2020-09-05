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
    NONE_ID
};

typedef enum NOTIFICATION_TYPE_STRUCT NOTIFICATION_TYPE;
typedef enum NOTIFICATION_ID_STRUCT NOTIFICATION_ID;

typedef GenericList NotificationList;

typedef struct NotificationStruct Notification;

/* entry points ======================================================== */

NotificationList *CreateNotificationList();
void FreeNotificationList(NotificationList *notificationList);
void DrawNotifications(NotificationList *notificationList, Date *date);
void NewNotification(NotificationList *notificationList, NOTIFICATION_TYPE type, NOTIFICATION_ID ID, int length);

#endif