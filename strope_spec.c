#include <specc.h>
#include "strope.h"
#include <stdlib.h>
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
      const char *cstr = NULL;

      after {
        if (cstr != NULL) {
          free((char *)cstr);
          cstr = NULL;
        }
      }

      describe ("with \"The qui\"") {
        it ("returns \"The qui\" in C") {
          cstr = Strope_cstring(leaf);
          expect_that(strcmp(cstr, "The qui") == 0);
        }
      }

      describe ("with \"The quick brown fox\"") {
        it ("returns \"The quick brown fox\" in C") {
          cstr = Strope_cstring(node);
          expect_that(strcmp(cstr, "The quick brown fox") == 0);
        }
      }
    }

    describe ("Strope_length()") {
      describe ("with \"The qui\"") {
        it ("returns 7") {
          expect_that(Strope_length(leaf) == 7);
        }
      }

      describe ("with \"The quick brown fox\"") {
        it ("returns 19") {
          expect_that(Strope_length(node) == 19);
        }
      }
    }

    describe ("Strope_at()") {
      describe ("with \"The qui\" and 4") {
        it ("returns 'q'") {
          expect_that(Strope_at(leaf, 4) == 'q');
        }
      }

      describe ("with \"The quick brown fox\"") {
        describe ("and 4") {
          it ("returns 'q'") {
            expect_that(Strope_at(node, 4) == 'q');
          }
        }

        describe ("and 10") {
          it ("returns 'b'") {
            expect_that(Strope_at(node, 10) == 'b');
          }
        }

        describe ("and 16") {
          it ("returns 'f'") {
            expect_that(Strope_at(node, 16) == 'f');
          }
        }
      }
    }

    describe ("Strope_substring()") {
      describe ("with \"The quick brown fox\"") {
        Strope *sub = NULL;
        const char *sub_cstr = NULL;

        after {
          if (sub != NULL) {
            Strope_free(sub);
            sub = NULL;
          }

          if (sub_cstr != NULL) {
            free((char *)sub_cstr);
            sub_cstr = NULL;
          }
        }

        describe ("and 8, 4") {
          it ("returns \"k br\"") {
            sub = Strope_substring(node, 8, 4);
            sub_cstr = Strope_cstring(sub);
            expect_that(strcmp(sub_cstr, "k br") == 0);
          }
        }

        describe ("and 2, 15") {
          it ("returns \"e quick brown f\"") {
            sub = Strope_substring(node, 2, 15);
            sub_cstr = Strope_cstring(sub);
            expect_that(strcmp(sub_cstr, "e quick brown f") == 0);
          }
        }
      }
    }
  }
}
