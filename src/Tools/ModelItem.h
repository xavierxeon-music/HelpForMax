#ifndef ModelItemH
#define ModelItemH

#include <QSortFilterProxyModel>
#include <QStandardItem>

class ModelItem : public QStandardItem
{
public:
   ModelItem();
   ModelItem(const QString& text);
   ModelItem(const QIcon& icon, const QString& text);

public:
   void setVisible(bool enabled);
   bool isVisible();
};

class FilteredModel : public QSortFilterProxyModel
{
   Q_OBJECT

public:
   FilteredModel(QObject* parent);

private:
   bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
};

#endif // NOT ModelItemH
