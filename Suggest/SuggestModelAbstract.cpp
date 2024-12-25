#include "SuggestModelAbstract.h"

Suggest::Model::Abstract::Abstract(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest, const Ref::Structure::PatchPart& part)
   : ListedRefModel<"Suggest">(parent, structure, part)
   , suggest(suggest)
   , brushActive(Qt::black)
   , brushInactive(Qt::gray)
{
}
