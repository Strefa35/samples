#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
  printf("-----------------------\n");
  printf("  Version - %s\n", OPT_VERSION);
  printf("-----------------------\n");

  {
    char* env_sn = getenv("ENV_SN");

    if (env_sn != NULL)
    {
      unsigned long sn_strtoul = strtoul(env_sn, NULL, 0);
      long int sn_strtol = strtol(env_sn, NULL, 0);
      long sn_atol = atol(env_sn);

      printf("ENV_SN=%s\n", env_sn);

      printf("        strtoul=%ld [0x%08lx]\n", sn_strtoul, sn_strtoul);
      printf("         strtol=%ld [0x%08lx]\n", sn_strtol, sn_strtol);
      printf("           atol=%ld [0x%08lx]\n", sn_atol, sn_atol);

    }
  }
  printf("     -> %s(), line: %d\n", __FUNCTION__, __LINE__);
  return 0;
}
