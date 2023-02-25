#ifndef ComponentsModelH
#define ComponentsModelH

#include <Central.h>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

class ComponentsModel : public QStandardItemModel,
                        private FunctionHub
{
   Q_OBJECT
public:
   class Filtered;

public:
   ComponentsModel(MainWindow* mainWindow);

private:
   void patchSelected(QString patchPath, QString key) override;
   void setModified(bool enabled) override;
   void rebuild();
   void update();

private:
   MainWindow* mainWindow;
};

class ComponentsModel::Filtered : public QSortFilterProxyModel
{
   Q_OBJECT

public:
   Filtered(MainWindow* mainWindow);

private:
   ComponentsModel* internal;
};

#endif // NOT ComponentsModelH
