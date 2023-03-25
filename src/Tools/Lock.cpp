#include "Lock.h"

Lock::Lock(bool& variable)
   : variable(variable)
{
   variable = true;
}

Lock::~Lock()
{
   variable = false;
}
