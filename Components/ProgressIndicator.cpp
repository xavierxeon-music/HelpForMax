#include "ProgressIndicator.h"

#include <QHBoxLayout>

ProgressIndicator::ProgressIndicator(QWidget* parent)
   : QWidget(parent)
   , messageLabel(nullptr)
   , progressBar(nullptr)
{
   setMinimumHeight(30);

   messageLabel = new QLabel(this);
   messageLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
   messageLabel->setText("Progress Indicator");

   progressBar = new QProgressBar(this);
   progressBar->setFixedWidth(100);
   progressBar->setRange(0, 100);
   progressBar->setTextVisible(false);
   progressBar->setValue(0);

   QHBoxLayout* masterLayout = new QHBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(messageLabel);
   masterLayout->addWidget(progressBar);
}

void ProgressIndicator::setMaxValue(const int& maxValue)
{
   progressBar->setRange(0, maxValue);
}

void ProgressIndicator::setProgress(const QString& message, const int& value)
{
   messageLabel->setText(message);
   progressBar->setValue(value);
}
