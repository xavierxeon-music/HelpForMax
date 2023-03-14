#include "Message.h"

void Message::info(const QString& message)
{
   qInfo() << message;

   if (Widget::me)
      Widget::me->addMessage(Widget::Type::Info, message);
}

void Message::error(const QString& message)
{
   qWarning() << message;

   if (Widget::me)
      Widget::me->addMessage(Widget::Type::Error, message);
}

void Message::undocumented(const QString& patchName, const QString& text)
{
   if (Widget::me)
      Widget::me->addMessage(Widget::Type::Undocumented, text, patchName);
}

// widget

Message::Widget* Message::Widget::me = nullptr;

Message::Widget::Widget(QWidget* parent)
   : QTreeWidget(parent)
{
   setHeaderHidden(true);
   setRootIsDecorated(false);

   setHeaderLabels({"type", "where", "message"});

   me = this;
}

Message::Widget::~Widget()
{
   me = nullptr;
}

void Message::Widget::addMessage(const Type& type, const QString& message, const QString& where)
{
   QTreeWidgetItem* item = new QTreeWidgetItem(this);
   item->setText(1, where);
   item->setText(2, message);

   if (Type::Error == type)
      item->setText(0, "!");
   else if (Type::Undocumented == type)
      item->setText(0, "?");
   else
      item->setText(0, "");

   resizeColumnToContents(0);
   resizeColumnToContents(1);
}
