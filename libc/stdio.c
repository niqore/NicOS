/*
 * ENSICAEN
 * 6 Boulevard Marechal Juin 
 * F-14050 Caen Cedex 
 *
 * This file is owned by ENSICAEN students.
 * No portion of this document may be reproduced, copied
 * or revised without written permission of the authors.
 */ 

/**
 * @author Nicolas Virard <nicolas.virard@ecole.ensicaen.fr> 
 * @version     0.0.1 - 23-08-2020
 */

#include "stdio.h"
#include "../drivers/screen.h"
#include "string.h"

void printf(const char *format, ...) {
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;
  
  while ((c = *format++) != 0)
    {
      if (c != '%')
        print_char(c);
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;
          
          c = *format++;
          if (c == '0')
            {
              pad0 = 1;
              c = *format++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *format++;
            }

          switch (c)
            {
            case 'c':
              print_char(*((char *) arg++));
              break;
            case 'd':
            case 'u':
            case 'x':
              itoa(*((int *) arg++), buf, c == 'x' ? 16 : 10);
              p = buf;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (!p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
                print_char(pad0 ? '0' : ' ');
              while (*p)
                print_char(*p++);
              break;

            default:
              print_char(*((int *) arg++));
              break;
            }
        }
    }
}