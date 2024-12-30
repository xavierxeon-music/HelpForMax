#include "ProgressIndicator.h"

#include <QHBoxLayout>

#include "MessageChannel.h"

ProgressIndicator::ProgressIndicator(QWidget* parent)
   : QWidget(parent)
   , messageLabel(nullptr)
   , progressBar(nullptr)
   , messageChannel(nullptr)
{
   messageLabel = new QLabel(this);
   messageLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
   messageLabel->setText("Hello");

   progressBar = new QProgressBar(this);
   progressBar->setFixedWidth(100);
   progressBar->setRange(0, 100);
   progressBar->setTextVisible(false);

   progressBar->setValue(100);

   QHBoxLayout* masterLayout = new QHBoxLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->addWidget(messageLabel);
   masterLayout->addWidget(progressBar);

   Message::Channel::PrintFunction printFunction = std::bind(&ProgressIndicator::print, this, std::placeholders::_1);
   messageChannel = new Message::Channel(this, printFunction);
}

QTextStream ProgressIndicator::message()
{
   return messageChannel->stream();
}

void ProgressIndicator::print(const QString& text)
{
   messageLabel->setText(text);
}
