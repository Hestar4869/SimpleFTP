//
// Created by Hestar on 2022/12/15.
//
#include <string.h>
#include "constant.h"

char *cmd_to_str(enum COMMOND cmd) {
    switch (cmd) {
        case CMD_LS:
            return "ls";

        case CMD_CD:
            return "cd";

        case CMD_GET:
            return "get";

        case CMD_PUT:
            return "put";

        case CMD_QUIT:
            return "quit";
    }
}

enum COMMOND str_to_cmd(char *str) {
    if(!strcmp(str,"ls"))
        return CMD_LS;
    else if(!strcmp(str,"cd"))
        return CMD_CD;
    else if(!strcmp(str,"get"))
        return CMD_GET;
    else if(!strcmp(str,"put"))
        return CMD_PUT;
    else if(!strcmp(str,"quit"))
        return CMD_QUIT;
}
