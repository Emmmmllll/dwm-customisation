#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <vector>

#include <X11/X.h>
#include <X11/keysym.h>

#include "util.h"
#include "dwmtypes.h"
#include "dwmaction.h"
#include "config.h"
// #include "config.def.h"

#define LENGTH(x) sizeof x / sizeof x[0]

std::vector<std::string*>
getDefinitions(std::ifstream file){
    std::vector<std::string*> defines;
    int deflen = 0;
    std::string line;
    while(std::getline(file, line)){
        if(line[0] == '#' && line[1] == '!'){
            std::string alias, value;
            size_t seperator = line.find(' ');
            alias = line.substr(2, seperator);
            line = line.substr(seperator, line.length());
            std::string def[2] = {alias, value};
            defines.push_back(def);
        }
    }
    return defines;
}
void
parse_keyBinds(Key * keys, Button * buttons){
    std::string conf_paths[2] = {
        (std::string)getenv("HOME") + "/.config/dwm/keybinds.conf",
        "/etc/dwm/keybinds.conf"
    };
    std::ifstream conf_file;
    if(checkFile(conf_paths[0].c_str()))
        conf_file.open(conf_paths[0]);        
    else if(checkFile(conf_paths[1].c_str())){
        conf_file.open(conf_paths[1]);
    }
    else{
        fprintf(stderr, "Couldn't open keybinds.conf at \"%s\" or \"%s\"\n", conf_paths[0].c_str(), conf_paths[1].c_str());
        return;
    }
    if(!conf_file.good()){
        fprintf(stderr, "Couldn't open keybinds.conf at \"%s\" or \"%s\"\n", conf_paths[0].c_str(), conf_paths[1].c_str());
        conf_file.close();
        return;
    }
    // make defines
    // std::vector<std::string*> defines = getDefinitions(conf_file);

    
    
    
    conf_file.close();
}

