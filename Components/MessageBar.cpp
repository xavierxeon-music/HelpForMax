#include "MessageBar.h"

#include <QApplication>
#include <QTimer>

Message::Bar* Message::Bar::me = nullptr;

Message::Bar::Bar(QWidget* parent)
   : QStatusBar(parent)
   , messageChannel(nullptr)
   , warningChannel(nullptr)
{
   me = this;

   IOChannel::PrintFunction messageFunction = std::bind(&Bar::print, this, std::placeholders::_1, false);
   messageChannel = new IOChannel(this, messageFunction);

   IOChannel::PrintFunction warningFunction = std::bind(&Bar::print, this, std::placeholders::_1, true);
   warningChannel = new IOChannel(this, warningFunction);

   setSizeGripEnabled(false);
}

Message::Bar::~Bar()
{
   me = nullptr;
}

QTextStream Message::Bar::message()
{
   if (!me)
      return QTextStream();

   return me->messageChannel->stream();
}

QTextStream Message::Bar::warning()
{
   if (!me)
      return QTextStream();

   return me->warningChannel->stream();
}

void Message::Bar::print(const QString& text, bool isWarning)
{
   static const QString urgentSymbol = QString::fromUtf8("\u2622");
   if (isWarning)
   {
      setStyleSheet("color: red");
      showMessage(urgentSymbol + " " + text);
   }
   else
   {
      setStyleSheet("");
      showMessage(text, 2000);
   }
}
