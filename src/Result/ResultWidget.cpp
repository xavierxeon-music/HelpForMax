#include "ResultWidget.h"

#include "ResultHighlighter.h"

Result::Widget::Widget(QWidget* parent, Central* central)
   : Abstract::Widget(parent, central)
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
   const QString content = textEdit->toPlainText();
   central->blockRef().setRefContent(content);
}

void Result::Widget::patchSelected(QString patchPath, QString key)
{
   Q_UNUSED(patchPath)
   Q_UNUSED(key)

   QString content = central->blockRef().getRefContent();
   textEdit->setText(content);
}
