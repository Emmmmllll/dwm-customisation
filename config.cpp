#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#include <X11/X.h>
#include <X11/keysym.h>

#include "util.h"
#include "dwmtypes.h"
#include "dwmaction.h"
#include "config.h"
#include "config.def.h"

#define LENGTH(x) sizeof x / sizeof x[0]

void
parse_keyBinds(Key * &keys, Button * &buttons){
    std::string conf_files[2] = {
        (std::string)getenv("HOME") + "/.config/dwm/keybinds.conf"
        "/etc/dwm/keybinds.conf",
    };
    std::ifstream conf_file;
    if(checkFile(conf_files[0].c_str()))
        conf_file.open(conf_files[0]);        
    else if(checkFile(conf_files[1].c_str())){
        conf_file.open(conf_files[1]);
    }
    else{
        fprintf(stderr, "Couldn't open keybinds.conf at \"%s\" or \"%s\"\n", conf_files[0], conf_files[1]);
        return;
    }
    if(!conf_file.good()){
        fprintf(stderr, "Couldn't open keybinds.conf at \"%s\" or \"%s\"\n", conf_files[0], conf_files[1]);
        return;
    }
    
    
    
    conf_file.close();
}

