#include "OverviewWidget.h"

#include <QDesktopServices>
#include <QUrl>

#include "OverviewGraph.h"

Overview::Widget::Widget(QWidget* parent, Central* central)
   : Abstract::Widget(parent, central, "Patch overview")
{
   toolBarAction(QIcon(":/OpenPatch.svg"), "Open In Max", this, &Widget::slotOpenInMax);

   Graph* graph = new Graph(this, central);
   setPayload(graph);
}

void Overview::Widget::slotOpenInMax()
{
   if (central->getCurrentKey().isEmpty())
      return;

   const Block block = central->block();
   const QString patchPath = block.getPatchPath();

   QDesktopServices::openUrl(QUrl::fromLocalFile(patchPath));
}
