#include <specc.h>
#include "strope.h"

specc_main {
  describe ("Strope") {
    Strope *str_abc = NULL;
    before {
      str_abc = Strope_new("abc");
    }

    after {
      Strope_free(str_abc);
    }
  }
}
