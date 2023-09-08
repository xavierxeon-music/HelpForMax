#include "ResultWidget.h"

#include "ResultHighlighter.h"

Result::Widget::Widget(QWidget* parent, Central* central)
   : Abstract::Widget(parent, central, "The reference XML file")
   , textEdit(nullptr)
{
   QAction* editorAction = toolBarAction(QIcon(":/Editor.svg"), "Apply Changes", this, &Widget::slotApplyChanges);
   editorAction->setShortcut(QKeySequence::Print);

   textEdit = new QTextEdit(this);
   textEdit->setLineWrapMode(QTextEdit::NoWrap);
   new Highlighter(textEdit->document());

   setPayload(textEdit);
}

void Result::Widget::slotApplyChanges()
{
   central->blockRef().clear();

   const QString content = textEdit->toPlainText();
   central->blockRef().setRefContent(content);

   callOnOtherHubInstances(&FunctionHub::setModified, true, central->getCurrentKey());
}

void Result::Widget::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   loadPatch();
}

void Result::Widget::setModified(bool enabled, QString key)
{
   // do nothing
   Q_UNUSED(enabled)
   Q_UNUSED(key)

   loadPatch();
}

void Result::Widget::loadPatch()
{
   QString content = central->blockRef().getRefContent();
   textEdit->setText(content);
}
