#ifndef ArgumentViewH
#define ArgumentViewH

#include <QTreeView>

#include <QStandardItemModel>

#include <Central.h>
#include <PatchStructure.h>
#include <TypeDelegate.h>

class ArgumentView : public QTreeView,
                     public TypeDelegate::Proxy,
                     private FunctionHub
{
   Q_OBJECT

public:
   ArgumentView(QWidget* parent);

public:
   void allowNameEdit(bool allow);
   void clearMonitors();
   void monitor(PatchStructure::Argument* argument);

private slots:
   void slotItemChanged(QStandardItem* item);

private:
   PatchStructure::Type getType(const int index) override;

private:
   QList<PatchStructure::Argument*> argumentList;
   QStandardItemModel* argumentModel;
   bool nameEditable;
};

#endif // NOT ArgumentViewH
