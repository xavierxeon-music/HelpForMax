#include "FileAbstract.h"

File::Abstract::Abstract(const Package::Info* info, Ref::Structure& structure)
   : info(info)
   , structure(structure)
{
}

void File::Abstract::read(const Patch::Info& patchInfo)
{
   Q_UNUSED(patchInfo)
}

void File::Abstract::write(const Patch::Info& patchInfo)
{
   Q_UNUSED(patchInfo)
}
