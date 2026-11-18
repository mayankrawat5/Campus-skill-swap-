#include "notification.h"

Notification::Notification(std::string msg) : message(msg), isRead(false) {}

std::string Notification::getMessage() const { return message; }
bool Notification::getIsRead() const { return isRead; }
void Notification::markAsRead() { isRead = true; }
