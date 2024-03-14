#include "walkdir.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <string.h>
#include <stdlib.h>
#include <StringBuffer.h>

int walkdir (const char *root, StringBuffer &sb)
{
    char * const path_argv[] = { strdup(root), NULL };
    FTS *ftsp = fts_open (path_argv, FTS_PHYSICAL, NULL);
    if (ftsp == NULL)
    {
        free (path_argv[0]);
        return -1;
    }

    FTSENT *ent;
    while ((ent = fts_read (ftsp))) {
        if (ent->fts_info == FTS_F) {
            (void) sb.append(ent->fts_path);
        }
    }

    fts_close (ftsp);
    free (path_argv[0]);
    return 0;
}
