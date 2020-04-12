#pragma once

#include <stdio.h>
#include <stdlib.h>

#define HANDLE_ERROR(msg) \
   do                     \
   {                      \
      perror(msg);        \
      exit(EXIT_FAILURE); \
   } while (0)
