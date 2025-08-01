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

private:
   QLabel* messageLabel;
   QProgressBar* progressBar;
};

#endif // NOT ProgressIndicatorH
