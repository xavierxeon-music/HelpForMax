#ifndef MessageBarH
#define MessageBarH

#include <QStatusBar>

#include <QLabel>

#include <IOChannel.h>

namespace Message
{
   class Channel;

   class Bar : public QStatusBar
   {
      Q_OBJECT

   public:
      Bar(QWidget* parent);
      ~Bar();

   public:
      static QTextStream message();
      static QTextStream warning();

   private:
      void print(const QString& text, bool isWarning = false);

   private:
      static Bar* me;
      IOChannel* messageChannel;
      IOChannel* warningChannel;
   };
} // namespace Message

#endif // NOT MessageBarH
