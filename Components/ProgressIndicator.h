#ifndef ProgressIndicatorH
#define ProgressIndicatorH

#include <QWidget>

#include <QLabel>
#include <QProgressBar>

namespace Message
{
   class Channel;
}

class ProgressIndicator : public QWidget
{
   Q_OBJECT

public:
   ProgressIndicator(QWidget* parent);

public:
   QTextStream message();

private:
   void print(const QString& text);

private:
   QLabel* messageLabel;
   QProgressBar* progressBar;
   Message::Channel* messageChannel;
};

#endif // NOT ProgressIndicatorH
