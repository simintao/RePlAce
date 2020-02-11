
#ifndef MAKE_REPLACE
#define MAKE_REPLACE

namespace ord {

class OpenRoad;

replace::Replace *
makeReplace();

void
initReplace(OpenRoad *openroad);

void
deleteReplace(replace::Replace *replace);

}

#endif
