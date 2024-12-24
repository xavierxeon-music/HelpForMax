#include "SuggestModelAbstract.h"

Suggest::Model::Abstract::Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest, const Ref::Structure::PatchPart& part)
   : ListedRefModel<"PatchRef">(parent, structure, part)
   , suggest(suggest)
{
}
