#ifndef MessageChannelH
#define MessageChannelH

#include <QIODevice>

namespace Message
{
   class Bar;

   class Channel : public QIODevice
   {
      Q_OBJECT

   public:
      Channel(Bar* bar, bool isWarning);

   private:
      qint64 readData(char* data, qint64 maxSize) override;
      qint64 writeData(const char* data, qint64 maxSize) override;

   private:
      Bar* bar;
      bool isWarning;
   };
} // namespace Message

#endif // NOT MessageChannelH
