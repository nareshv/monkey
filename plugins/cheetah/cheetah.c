/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Monkey HTTP Daemon
 *  ------------------
 *  Copyright (C) 2001-2010, Eduardo Silva P.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Monkey Plugin Interface */
#include "MKPlugin.h"

/* Local header files */
#include "cmd.h"
#include "cheetah.h"

MONKEY_PLUGIN("cheetah",              /* shortname */
              "Cheetah! Shell",       /* name */
              "0.12.0",               /* version */
              MK_PLUGIN_CORE_PRCTX);  /* hooks */

void mk_cheetah_loop()
{
    int len;
    char cmd[200];
    char line[200];
    char *rcmd;

    printf("\n%s%s***%s Welcome to %sCheetah!%s, the %sMonkey Shell %s:) %s***%s\n",
           ANSI_BOLD, ANSI_YELLOW,
           ANSI_WHITE, ANSI_GREEN, 
           ANSI_WHITE, ANSI_RED, ANSI_WHITE, ANSI_YELLOW, ANSI_RESET);
    printf("\n      << %sType 'help' or '\\h' for help%s >>\n\n",
           ANSI_BLUE, ANSI_RESET);
    fflush(stdout);

    while (1) {
        printf(MK_CHEETAH_PROMPT, ANSI_BOLD, ANSI_GREEN, ANSI_RESET);
        rcmd = fgets(line, sizeof(line), stdin);

        len = strlen(line);
        
        if (len == 0){
            printf("\n");
            mk_cheetah_cmd_quit();
        }

        strncpy(cmd, line, len - 1);
        cmd[len - 1] = '\0';

        mk_cheetah_cmd(cmd);
        bzero(line, sizeof(line));
    }
}

void *mk_cheetah_init(void *args)
{
    init_time = time(NULL);
    mk_cheetah_loop();
    return 0;
}

/* This function is called when the plugin is loaded, it must
 * return 
 */
int _mkp_init(void **api)
{
    mk_api = *api;
    return 0;
}

void _mkp_exit()
{
}

int _mkp_core_prctx(struct server_config *config)
{
    pthread_t tid;
    pthread_attr_t thread_attr;

    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&tid, &thread_attr, (void *) mk_cheetah_init, config) <
        0) {
        perror("pthread_create");
        exit(1);
    }

    return 0;
}
