

#include <stdio.h>
#include <string.h>

size_t xtoi(const char* hexstr)
{
  size_t r = 0;
  int i;
  int l = strlen(hexstr);

  for (i = 0; i < l; i++)
  {
    switch(hexstr[i])
    {
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        r |= ((size_t)(hexstr[i] - '0')) << ((l - i - 1) * 4);
        break;
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
        r |= ((size_t)(hexstr[i] - 'a' + 10)) << ((l - i - 1) * 4);
        break;
      case 'A':
      case 'B':
      case 'C':
      case 'D':
      case 'E':
      case 'F':
        r |= ((size_t)(hexstr[i] - 'A' + 10)) << ((l - i - 1) * 4);
        break;
      default:
        i = l;  // force loop exit
    }
  }

  return r;
}
