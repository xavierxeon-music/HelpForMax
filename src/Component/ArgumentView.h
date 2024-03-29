#ifndef ArgumentViewH
#define ArgumentViewH

#include <QTreeView>

#include <QStandardItemModel>

#include "Tools/Central.h"
#include "Tools/TypeDelegate.h"

class ArgumentView : public QTreeView,
                     public TypeDelegate::Proxy,
                     private FunctionHub
{
   Q_OBJECT

public:
   ArgumentView(QWidget* parent);

public:
   void allowNameEdit(bool allow);
   void clearMonitors(const QString& patchKey);
   void monitor(Structure::Argument* argument);

private slots:
   void slotItemChanged(QStandardItem* item);

private:
   Structure::Type getType(const int index) override;

private:
   QString key;
   QList<Structure::Argument*> argumentList;
   QStandardItemModel* argumentModel;
   bool nameEditable;
};

#endif // NOT ArgumentViewH
