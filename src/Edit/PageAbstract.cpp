#include "PageAbstract.h"

#include <Central.h>

#include <MainWindow.h>

Page::Abstract::Abstract(MainWindow* mainWindow, const PatchParser::Marker& marker)
   : QWidget(mainWindow)
   , FunctionHub()
   , mainWindow(mainWindow)
   , editMarker(marker)
   , connectionMap()
   , blocked(false)
{
}

Page::Abstract::~Abstract()
{
}

void Page::Abstract::monitor(QLineEdit* lineEdit, QString* variable)
{
   auto update = [&, lineEdit, variable]()
   {
      const QString& newText = lineEdit->text();
      const QString oldText = *variable;
      if (newText == oldText)
         return;

      *variable = newText;
      FunctionHub::callOnAllHubInstances(&FunctionHub::setModified, true);
   };

   if (connectionMap.contains(lineEdit))
      disconnect(connectionMap.value(lineEdit));

   lineEdit->setText(*variable);
   connectionMap[lineEdit] = connect(lineEdit, &QLineEdit::textChanged, update);
}

void Page::Abstract::monitor(QLineEdit* lineEdit, QStringList* variable)
{
   auto update = [&, lineEdit, variable]()
   {
      const QString& newText = lineEdit->text();
      const QString oldText = variable->join(";");
      if (newText == oldText)
         return;

      *variable = newText.split(";");
      FunctionHub::callOnAllHubInstances(&FunctionHub::setModified, true);
   };

   if (connectionMap.contains(lineEdit))
      disconnect(connectionMap.value(lineEdit));

   lineEdit->setText(variable->join(";"));
   connectionMap[lineEdit] = connect(lineEdit, &QLineEdit::textChanged, update);
}

void Page::Abstract::monitor(QPlainTextEdit* textEdit, QString* variable)
{
   auto update = [&, textEdit, variable]()
   {
      const QString& newText = textEdit->toPlainText();
      const QString oldText = *variable;
      if (newText == oldText)
         return;

      *variable = newText;
      FunctionHub::callOnAllHubInstances(&FunctionHub::setModified, true);
   };

   if (connectionMap.contains(textEdit))
      disconnect(connectionMap.value(textEdit));

   textEdit->setPlainText(*variable);
   connectionMap[textEdit] = connect(textEdit, &QPlainTextEdit::textChanged, update);
}

void Page::Abstract::componentSelected(PatchParser::Marker marker, QVariant data)
{
   if (editMarker != marker)
      return;

   blocked = true;
   update(data);
   blocked = false;
}
