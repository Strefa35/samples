#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define PATH_SIZE     100
static char patchStr[PATH_SIZE];

static int getParams(int argc, char *argv[])
{
  int itr = 0;
  int result = 0;

  while (itr < argc)
  {
    if (strcmp(argv[itr],"--path")==0)
    {
      itr++;
      if (itr < argc)
      {
        int size = strlen(argv[itr]);

        if (size >= PATH_SIZE)
        {
          size = PATH_SIZE - 1;
        }

        if (size > 0)
        {
          memset(patchStr, 0, PATH_SIZE);
          memcpy(patchStr, argv[itr], size);

          result = 1;
        }
      }
      else
      {
        break;
      }
    }
    itr++;
  }
  return result;
}


int main(int argc, char *argv[])
{
  DIR *dir;
  struct dirent *dp;
  char * file_name;

  if (getParams(argc, argv))
  {
    printf("DIR open('%s')\n", patchStr);

    dir = opendir(patchStr);
    //while ((dp=readdir(dir)) != NULL)
    while(1)
    {
      dp = readdir(dir);
      //printf("    readdir(dir: %p) - dp: %p\n", dir, dp);

      if (dp == NULL)
      {
        printf("DIR end\n");
        break;
      }
      if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
      {
        // do nothing (straight logic)
        printf("    '%s'\n", dp->d_name);
      }
      else
      {
        file_name = dp->d_name; // use it
        printf("    '%s'\n", dp->d_name);
      }
    }
    closedir(dir);
  }
  return 0;
}
