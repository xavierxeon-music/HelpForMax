#ifndef ProgressIndicatorH
#define ProgressIndicatorH

#include <QWidget>

#include <QLabel>
#include <QProgressBar>

#include <IOChannel.h>

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
   IOChannel* messageChannel;
};

#endif // NOT ProgressIndicatorH
