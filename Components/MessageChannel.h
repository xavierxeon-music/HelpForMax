#ifndef MessageChannelH
#define MessageChannelH

#include <QIODevice>

namespace Message
{
   class Channel : public QIODevice
   {
      Q_OBJECT

   public:
      using PrintFunction = std::function<void(const QString& message)>;

   public:
      Channel(QObject* parent, PrintFunction printFunction);

   public:
      QTextStream stream();

   private:
      qint64 readData(char* data, qint64 maxSize) override;
      qint64 writeData(const char* data, qint64 maxSize) override;

   private:
      PrintFunction printFunction;
   };
} // namespace Message

#endif // NOT MessageChannelH
