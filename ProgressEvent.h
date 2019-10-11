#ifndef PROGRESSEVENT_H
#define PROGRESSEVENT_H

#include <QEvent>
#include <QString>

class ProgressEvent : public QEvent {
  public:
    enum {EventId = QEvent::User};
    explicit ProgressEvent(const QString &message_)
        : QEvent(static_cast<Type>(EventId)), message(message_) {}

    const QString message;
};

#endif // PROGRESSEVENT_H
