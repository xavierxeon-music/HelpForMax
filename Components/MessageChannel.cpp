#include "MessageChannel.h"

#include "MessageBar.h"

Message::Channel::Channel(QObject* parent, PrintFunction printFunction)
   : QIODevice(parent)
   , printFunction(printFunction)
{
   open(QIODevice::WriteOnly);
}

QTextStream Message::Channel::stream()
{
   return QTextStream(this);
}

qint64 Message::Channel::readData(char* data, qint64 maxSize)
{
   Q_UNUSED(data)
   Q_UNUSED(maxSize)

   return 0;
}

qint64 Message::Channel::writeData(const char* data, qint64 maxSize)
{
   if (printFunction)
      printFunction(QString::fromUtf8(data));

   return maxSize;
}
