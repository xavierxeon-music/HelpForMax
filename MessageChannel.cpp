#include "MessageChannel.h"

#include "MessageBar.h"

Message::Channel::Channel(Bar* bar, bool isWarning)
   : QIODevice(bar)
   , bar(bar)
   , isWarning(isWarning)
{
   open(QIODevice::WriteOnly);
}

qint64 Message::Channel::readData(char* data, qint64 maxSize)
{
   Q_UNUSED(data)
   Q_UNUSED(maxSize)

   return 0;
}

qint64 Message::Channel::writeData(const char* data, qint64 maxSize)
{
   bar->print(data, isWarning);
   return maxSize;
}
