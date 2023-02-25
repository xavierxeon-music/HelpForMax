#include "HelpPageAbstract.h"

#include "HelpPersona.h"

Help::Page::Abstract::Abstract(Persona* persona, const PatchParser::Marker& marker)
   : QWidget(persona)
   , Persona::FunctionHub()
   , Central::FunctionHub()
   , persona(persona)
   , editMarker(marker)
   , connectionMap()
   , blocked(false)
{
}

Help::Page::Abstract::~Abstract()
{
}

void Help::Page::Abstract::monitor(QLineEdit* lineEdit, QString* variable)
{
   auto update = [&, lineEdit, variable]()
   {
      const QString& newText = lineEdit->text();
      const QString oldText = *variable;
      if (newText == oldText)
         return;

      *variable = newText;
      Central::FunctionHub::callOnAllHubInstances(&Central::FunctionHub::setModified, true);
   };

   if (connectionMap.contains(lineEdit))
      disconnect(connectionMap.value(lineEdit));

   lineEdit->setText(*variable);
   connectionMap[lineEdit] = connect(lineEdit, &QLineEdit::textChanged, update);
}

void Help::Page::Abstract::monitor(QLineEdit* lineEdit, QStringList* variable)
{
   auto update = [&, lineEdit, variable]()
   {
      const QString& newText = lineEdit->text();
      const QString oldText = variable->join(";");
      if (newText == oldText)
         return;

      *variable = newText.split(";");
      Central::FunctionHub::callOnAllHubInstances(&Central::FunctionHub::setModified, true);
   };

   if (connectionMap.contains(lineEdit))
      disconnect(connectionMap.value(lineEdit));

   lineEdit->setText(variable->join(";"));
   connectionMap[lineEdit] = connect(lineEdit, &QLineEdit::textChanged, update);
}

void Help::Page::Abstract::monitor(QPlainTextEdit* textEdit, QString* variable)
{
   auto update = [&, textEdit, variable]()
   {
      const QString& newText = textEdit->toPlainText();
      const QString oldText = *variable;
      if (newText == oldText)
         return;

      *variable = newText;
      Central::FunctionHub::callOnAllHubInstances(&Central::FunctionHub::setModified, true);
   };

   if (connectionMap.contains(textEdit))
      disconnect(connectionMap.value(textEdit));

   textEdit->setPlainText(*variable);
   connectionMap[textEdit] = connect(textEdit, &QPlainTextEdit::textChanged, update);
}

void Help::Page::Abstract::componentSelected(PatchParser::Marker marker, QVariant data)
{
   if (editMarker != marker)
      return;

   blocked = true;
   update(data);
   blocked = false;
}
