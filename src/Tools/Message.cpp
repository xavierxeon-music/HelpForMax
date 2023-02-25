#include "Message.h"

void Message::info(const QString& message)
{
   qInfo() << message;
}

void Message::warning(const QString& message)
{
   qWarning() << message;
}

void Message::error(const QString& message)
{
   qWarning() << message;
}

// widget

Message::Widget::Widget(QWidget* parent)
   : QTreeWidget(parent)
{
}
