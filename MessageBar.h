#ifndef MessageBarH
#define MessageBarH

#include <QStatusBar>

#include <QLabel>

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
      friend class Channel;

   private:
      void print(const QString& text, bool isWarning = false);

   private:
      static Bar* me;
      Channel* messageChannel;
      Channel* warningChannel;
   };
} // namespace Message

#endif // NOT MessageBarH
