#ifndef ProgressIndicatorH
#define ProgressIndicatorH

#include <QWidget>

#include <QLabel>
#include <QProgressBar>

class ProgressIndicator : public QWidget
{
   Q_OBJECT

public:
   ProgressIndicator(QWidget* parent);

public:
   void setMaxValue(const int& maxValue);
   void setProgress(const QString& message, const int& value);

private:
   QLabel* messageLabel;
   QProgressBar* progressBar;
};

#endif // NOT ProgressIndicatorH
