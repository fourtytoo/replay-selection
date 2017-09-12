/*  replay-selection.c --- Replay the contents of the currently selected text

 Copyright (C) 2017 by Walter C. Pelissero

 Author: Walter C. Pelissero <walter@pelissero.de>
 Project: replay selection

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2, or (at
your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.*/

/* Replay in X events the contents of the clipboard (the selected
    text, if you will).  For this program to work you need xsel and
    xte (in xautomation) installed.

    You are supposed to bind this command to a special key of your
    window manager.
 */

#define _WITH_GETLINE

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


static FILE *
spawn_source ()
{
  int     fd[2];
  pid_t   pid;

  if (pipe(fd) == -1)
    {
      perror("pipe");
      return NULL;
    }
  pid = fork();
  if (pid == 0)
    {
      /* child */
      close(fd[0]);
      dup2(fd[1], 1);
      execlp("xclip", "xclip", "-o", NULL);
      exit(-1);
    }
  else if (pid > 0)
    {
      /* server */
      close(fd[1]);
      return fdopen(fd[0], "r");
    }
  perror("fork");
  return(NULL);
}

static void
replay_line (char *line, size_t len)
{
  char *arg1, *arg2;
  size_t arg1len;
  static char str[] = "str ";
  int pid;

  arg1len = len + sizeof(str);
  arg1 = malloc(arg1len);
  strcpy(arg1, str);
  strcpy(arg1 + sizeof(str) - 1, line);
  arg1[arg1len - 1] = '\0';

  pid = fork();
  if (pid == 0)
    {
      if (arg1[arg1len - 2] == '\n')
	{
	  arg1[arg1len - 2] = '\0';
	  arg2 = "key Return";
	}
      else
	arg2 = NULL;
      execlp("xte", "xte", arg1, arg2, NULL);
    }
  else if (pid > 0)
    {
      int status;
      waitpid(pid, &status, 0);
    }
  free(arg1);
}

static void
replay_clipboard (FILE *input)
{
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  
  while ((linelen = getline(&line, &linecap, input)) > 0)
    replay_line(line, linelen);
  free(line);
}

static void
use_external_programs ()
{
  FILE *fp = spawn_source();
  replay_clipboard(fp);
  fclose(fp);
}

int
main(int argc, char *argv[argc])
{
  use_external_programs();
  return 0;
}
