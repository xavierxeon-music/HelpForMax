#ifndef MessageBarH
#define MessageBarH

#include <QStatusBar>

#include <QLabel>

#include <IOChannel.h>

class MessageBar : public QStatusBar
{
   Q_OBJECT

public:
   MessageBar(QWidget* parent);
   ~MessageBar();

public:
   static QTextStream message();
   static QTextStream warning();

private:
   void print(const QString& text, bool isWarning = false);

private:
   static MessageBar* me;
   IOChannel* messageChannel;
   IOChannel* warningChannel;
};

#endif // NOT MessageBarH
