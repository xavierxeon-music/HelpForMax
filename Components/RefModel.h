#ifndef RefModelH
#define RefModelH

#include <CompileTimeString.h>
#include <QStandardItemModel>

#include "RefStructure.h"

class RefModel : public QStandardItemModel
{
   Q_OBJECT

public:
   RefModel(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part);
   virtual ~RefModel();

signals:
   void signalDataEdited();

public:
   virtual void update() = 0;
   virtual void rebuild() = 0;

   const Ref::Structure::PatchPart& getPart() const;

protected:
   void removeContent();

protected:
   Ref::Structure& structure;
   Ref::Structure::PatchPart part;
};

template <CompileTimeString Category>
class ListedRefModel : public RefModel
{
public:
   ListedRefModel(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part);
   virtual ~ListedRefModel() override;

public:
   template <typename ModelClass>
   static void callOnAllInstances(QObject* parent, void (ModelClass::*function)());

private:
   using InstanceList = QList<RefModel*>;
   using InstanceMap = QMap<QObject*, InstanceList>;
   static InstanceMap instanceMap;
};

#ifndef RefModelHPP
#include "RefModel.hpp"
#endif // NOT RefModelHPP

#endif // NOT RefModelH
