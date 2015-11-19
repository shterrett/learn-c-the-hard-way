#ifndef _duffs_macro_h
#define _duffs_macro_h
#define eight_case \
    int n = (count + 7) / 8; \
    switch (count % 8) { \
      case 0: \
        do { \
          *to++ = *from++; \
          case 7: \
          *to++ = *from++; \
          case 6: \
          *to++ = *from++; \
          case 5: \
          *to++ = *from++; \
          case 4: \
          *to++ = *from++; \
          case 3: \
          *to++ = *from++; \
          case 2: \
          *to++ = *from++; \
          case 1: \
          *to++ = *from++; \
        } while (--n > 0); \
    }
#endif
