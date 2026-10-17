#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>

class Notification {
private:
    std::string message;
    bool isRead;

public:
    Notification(std::string msg);

    std::string getMessage() const;
    bool getIsRead() const;
    void markAsRead();
};

#endif // NOTIFICATION_H
