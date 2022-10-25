#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <iostream>
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

std::string ltrim(std::string str){
    return str.erase(0, str.find_first_not_of(" \t\r\n"));
}
std::string rtrim(std::string str){
    return str.erase(str.find_last_not_of(" \t\r\n") + 1);
}
std::string trim(std::string str){
    return ltrim(rtrim(str));
}
    

namespace config{


bool
updateDefinitions(std::vector<std::string*> *defines, std::string line){
    if(line[0] == '#' && line[1] == '!'){
        std::string alias, value;
        line = trim(line.erase(0, 2));
        std::size_t separator;
        if((separator = line.find_first_of(" \t")) == std::string::npos){
            return false;
        }
        else{
            alias = trim(line.substr(0, separator));
            value = trim(line.substr(separator+1));
        }
        std::string def[2] = {alias, value};
        defines->push_back(def);
    }
    else{
        return false;
    }
    // When a definition refers to another definition, give the referer the value of the definition it refers to
    bool haschanged = true;
    while(haschanged){
        haschanged = false;
        for(int i = 0; i < defines->size(); i++){
            for(int j = 0; j < defines->size(); j++){
                if(i == j)
                    continue;
                if((*defines)[i][1] == (*defines)[j][0]){
                    (*defines)[i][1] = (*defines)[j][1];
                    haschanged = true;
                }
                // handle changed definitions
                if((*defines)[i][0] == (*defines)[j][0]){
                    if(i<j){
                        (*defines)[i][1] = (*defines)[j][1];
                        defines->erase(defines->begin() + j);
                    }
                    else{
                        (*defines)[j][1] = (*defines)[i][1];
                        defines->erase(defines->begin() + i);
                    }
                }
            }
        }
    }
    return true;
}
std::string
applyDefinitions(std::vector<std::string*> definitions, std::string str){
    std::size_t pos = 0;
    for(std::string * &def : definitions){
        while((pos = str.find(def[0])) != std::string::npos){
            str = str.substr(0,pos) + def[1] + str.substr(pos + def[0].length());
        }
    }
    return str;
}
void
parse_key(std::string str, std::vector<Key> keys){
    std::size_t pos = str.find(",");
    if(pos == std::string::npos || pos + 1 > str.size())
        return;
    Key k{};
    //get key section (Modifiers included)
    std::string section = trim(str.substr(0, pos));
    str = str.substr(pos + 1);

    while((pos = section.find_first_of(" \t")) != std::string::npos){
        std::string substr = section.substr(0, pos);
        if( substr ==  "Mod4Mask")
            k.mod |= Mod4Mask;
        else if( substr == "Mod1Mask")
            k.mod |= Mod1Mask;
        else if( substr == "ControlMask")
            k.mod |= ControlMask;
        else{
            if(k.keysym)
                return;
            if(substr.substr(0, 2) == "0x"){

            }
            // numbers
            else if( substr == "0")
                k.keysym = XK_0;
            else if( substr == "1")
                k.keysym = XK_1;
            else if( substr == "2")
                k.keysym = XK_2;
            else if( substr == "3")
                k.keysym = XK_3;
            else if( substr == "4")
                k.keysym = XK_4;
            else if( substr == "5")
                k.keysym = XK_5;
            else if( substr == "6")
                k.keysym = XK_6;
            else if( substr == "7")
                k.keysym = XK_7;
            else if( substr == "8")
                k.keysym = XK_8;
            else if( substr == "9")
                k.keysym = XK_9;
            // letters
            else if( substr == "q")
                k.keysym = XK_q;
            else if( substr == "w")
                k.keysym = XK_w;
            else if( substr == "e")
                k.keysym = XK_e;
            else if( substr == "r")
                k.keysym = XK_r;
            else if( substr == "t")
                k.keysym = XK_t;
            else if( substr == "z")
                k.keysym = XK_z;
            else if( substr == "u")
                k.keysym = XK_u;
            else if( substr == "i")
                k.keysym = XK_i;
            else if( substr == "o")
                k.keysym = XK_o;
            else if( substr == "p")
                k.keysym = XK_p;
            else if( substr == "a")
                k.keysym = XK_a;
            else if( substr == "s")
                k.keysym = XK_s;
            else if( substr == "d")
                k.keysym = XK_d;
            else if( substr == "f")
                k.keysym = XK_f;
            else if( substr == "g")
                k.keysym = XK_g;
            else if( substr == "h")
                k.keysym = XK_h;
            else if( substr == "j")
                k.keysym = XK_j;
            else if( substr == "k")
                k.keysym = XK_k;
            else if( substr == "l")
                k.keysym = XK_l;
            else if( substr == "y")
                k.keysym = XK_y;
            else if( substr == "x")
                k.keysym = XK_x;
            else if( substr == "c")
                k.keysym = XK_c;
            else if( substr == "v")
                k.keysym = XK_v;
            else if( substr == "b")
                k.keysym = XK_b;
            else if( substr == "n")
                k.keysym = XK_n;
            else if( substr == "m")
                k.keysym = XK_m;
            // punctuation
            else if( substr == "space")
                k.keysym = XK_space;
            else if( substr == "comma")
                k.keysym = XK_comma;
            else if( substr == "period")
                k.keysym = XK_period;
            else if( substr == "enter")
                k.keysym = XK_Return;
            else if( substr == "backspace")
                k.keysym = XK_BackSpace;
            else if( substr == "tab")
                k.keysym = XK_Tab;
        }

        pos = section.find_first_not_of(" \t", pos);
        section.erase(0, pos);
    }

}
void
parse_keybinds(){
    std::string conf_paths[2] = {
        (std::string)getenv("HOME") + "/.config/dwm/keybinds.conf",
        "/etc/dwm/keybinds.conf"
    };
    //Open the conf file
    std::ifstream conf_file;
    if(checkFile(conf_paths[0].c_str()))
        conf_file.open(conf_paths[0]);        
    else if(checkFile(conf_paths[1].c_str()))
        conf_file.open(conf_paths[1]);
    else{
        fprintf(stderr, "Couldn't open keybinds.conf at \"%s\" or \"%s\"\n", conf_paths[0].c_str(), conf_paths[1].c_str());
        return;
    }
    if(!conf_file.good()){
        fprintf(stderr, "Couldn't open keybinds.conf at \"%s\" or \"%s\"\n", conf_paths[0].c_str(), conf_paths[1].c_str());
        conf_file.close();
        return;
    }
    std::vector<Key> _keys;
    std::vector<Button> _buttons;
    std::vector<std::string*> defines;
    std::string line;
    while(std::getline(conf_file, line)){
        line = trim(line);
        if(updateDefinitions(&defines, line))
            continue;
        line = applyDefinitions(defines, line);
        if(line.substr(0, 3) == "key"){
            line = ltrim(line.substr(3));
            if(line[0] == '='){
                line = line.substr(1);
                // parse_key(line, )
            }
            continue;
        }
    
    
    
    
    }    
    conf_file.close();
}
void
cleanup(){
    
}
} // End of namespace config