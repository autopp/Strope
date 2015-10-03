#include <specc.h>
#include "strope.h"
#include <string.h>

#define pending_for_not_impemented() pending("not implemented")

specc_main {
  describe ("Strope") {
    Strope *leaf = NULL;
    Strope *node = NULL;

    before {
      leaf = Strope_new("The qui");
      node = Strope_concat(Strope_concat(leaf, Strope_new("ck brown")), Strope_new(" fox"));
    }

    after {
      if (leaf != NULL) {
        Strope_free(leaf);
        leaf = NULL;
      }

      if (node != NULL) {
        Strope_free(node);
        node = NULL;
      }
    }

    describe ("Strope_cstring()") {
      describe ("with \"The qui\"") {
        it ("returns \"The qui\" in C") {
          pending_for_not_impemented();
          expect_that(strcmp(Strope_cstring(leaf), "The qui") == 0);
        }
      }

      describe ("with \"The quick brown fox\"") {
        it ("returns \"The quick brown fox\" in C") {
          pending("not implemented");
          expect_that(strcmp(Strope_cstring(leaf), "The quick brown fox"));
        }
      }
    }

    describe ("Strope_length()") {
      describe ("with \"The qui\"") {
        it ("returns 7") {
          pending_for_not_impemented();
          expect_that(Strope_length(leaf) == 7);
        }
      }

      describe ("with \"The quick brown fox\"") {
        it ("returns 7") {
          pending_for_not_impemented();
          expect_that(Strope_length(node) == 19);
        }
      }
    }

    describe ("Strope_at()") {
      describe ("with \"The qui\" and 4") {
        it ("returns 'q'") {
          pending_for_not_impemented();
          expect_that(Strope_at(leaf, 4) == 'q');
        }
      }

      describe ("with \"The quick brown fox\"") {
        describe ("and 4") {
          it ("returns 'q'") {
            pending_for_not_impemented();
            expect_that(Strope_at(node, 4) == 'q');
          }
        }

        describe ("and 10") {
          it ("returns 'b'") {
            pending_for_not_impemented();
            expect_that(Strope_at(node, 10) == 'b');
          }
        }

        describe ("and 16") {
          it ("returns 'f'") {
            pending_for_not_impemented();
            expect_that(Strope_at(node, 16) == 'f');
          }
        }
      }
    }

    describe ("Strope_substring()") {
      describe ("with \"The quick brown fox\"") {
        describe ("and 8, 4") {
          it ("returns \"k br\"") {
            pending_for_not_impemented();
            Strope *sub = Strope_substring(node, 8, 4);
            expect_that(strcmp(Strope_cstring(sub), "k br") == 0);
          }
        }

        describe ("and 2, 15") {
          it ("returns \"e quick brown f\"") {
            pending_for_not_impemented();
            Strope *sub = Strope_substring(node, 2, 15);
            expect_that(strcmp(Strope_cstring(sub), "e quick brown f") == 0);
          }
        }
      }
    }
  }
}
