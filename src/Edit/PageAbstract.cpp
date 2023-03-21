#include "PageAbstract.h"

#include "Tools/Central.h"

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

void Page::Abstract::monitor(QCheckBox* checkBox, bool* variable, const QString& patchKey)
{
   auto update = [&, variable, patchKey](bool checked)
   {
      *variable = checked;
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, patchKey);
   };

   if (connectionMap.contains(checkBox))
      disconnect(connectionMap.value(checkBox));

   checkBox->setCheckable(true);
   checkBox->setChecked(*variable);
   connectionMap[checkBox] = connect(checkBox, &QCheckBox::clicked, update);
}

void Page::Abstract::monitor(QLineEdit* lineEdit, QString* variable, const QString& patchKey)
{
   auto update = [&, lineEdit, variable, patchKey]()
   {
      const QString& newText = lineEdit->text();
      const QString oldText = *variable;
      if (newText == oldText)
         return;

      *variable = newText;
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, patchKey);
   };

   if (connectionMap.contains(lineEdit))
      disconnect(connectionMap.value(lineEdit));

   lineEdit->setText(*variable);
   connectionMap[lineEdit] = connect(lineEdit, &QLineEdit::textChanged, update);
}

void Page::Abstract::monitor(QLineEdit* lineEdit, QStringList* variable, const QString& patchKey)
{
   auto update = [&, lineEdit, variable, patchKey]()
   {
      const QString& newText = lineEdit->text();
      const QString oldText = variable->join(";");
      if (newText == oldText)
         return;

      *variable = newText.split(";");
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, patchKey);
   };

   if (connectionMap.contains(lineEdit))
      disconnect(connectionMap.value(lineEdit));

   lineEdit->setText(variable->join(";"));
   connectionMap[lineEdit] = connect(lineEdit, &QLineEdit::textChanged, update);
}

void Page::Abstract::monitor(QPlainTextEdit* textEdit, QString* variable, const QString& patchKey)
{
   auto update = [&, textEdit, variable, patchKey]()
   {
      const QString& newText = textEdit->toPlainText();
      const QString oldText = *variable;
      if (newText == oldText)
         return;

      *variable = newText;
      FunctionHub::callOnOtherHubInstances(&FunctionHub::setModified, true, patchKey);
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
