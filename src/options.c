/*************************************************************************
  > File Name: options.c
  > Author:perrynzhou
  > Mail:perrynzhou@gmail.com
  > Created Time: Mon 21 Feb 2022 05:41:11 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "options.h"
#define _GNU_SOURCE

int options_init(options_t *opts, int argc, char *argv[])
{
  memset(opts, 0, sizeof(*opts));
  int c = 0;

  int long_index = 0;
  char *arg_ptr = NULL;
  char *tmp_ptr = NULL;
  static struct option long_options[] = {
      {"start", required_argument, 0, 's'},
      {"id", required_argument, 0, 'i'},
      {"join", required_argument, 0, 'j'},
      {"leave", required_argument, 0, 'l'},
      {0, 0, 0, 0}};
  while ((c = getopt_long(argc, argv, "s:i:j:l",
                          long_options, &long_index)) != -1)
  {
    if (c == 's' || c == 'l' || c == 'j')
    {
      arg_ptr = strdup(optarg);
      tmp_ptr = strchr(arg_ptr, ':');
      if (tmp_ptr == NULL)
      {
        return -1;
      }
      *tmp_ptr++ = '\0';
      opts->host = strdup(arg_ptr);
      opts->port = strdup(tmp_ptr + 1);
      free(arg_ptr);
      arg_ptr = NULL;
    }
    else
    {
      opts->id = strdup(optarg);
    }
    switch (c)
    {
    case 's':
      opts->type_info.name = strdup("start");
      opts->type_info.type = OPTION_START;

      break;
    case 'l':
      opts->type_info.name = strdup("leave");
      opts->type_info.type = OPTION_LEAVE;
      break;
    case 'j':
      opts->type_info.name = strdup("join");
      opts->type_info.type = OPTION_JOIN;
      break;
    }
  }
  if (opts->host == NULL || (opts->type_info.type < OPTION_START || opts->type_info.type > OPTION_LEAVE))
  {
    return -1;
  }
  return 0;
}
void options_dump(options_t *opt)
{
  if (opt != NULL)
  {
    fprintf(stdout, "host:%s\n", opt->host);
    fprintf(stdout, "port:%s\n", opt->host);
    fprintf(stdout, "id:%s\n", opt->id);
    fprintf(stdout, "type:%s-%d\n", opt->type_info.name, opt->type_info.type);
  }
}
#ifdef TEST
#include "options.h"
int main(int argc, char *argv[])
{
  options_t opt;
  int ret = options_init(&opt, argc, argv);
  options_dump(&opt);
  return 0;
}
#endif