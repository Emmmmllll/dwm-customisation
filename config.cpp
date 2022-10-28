#include <sys/stat.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>

#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>

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

typedef struct ActionFunction{
    void (*func)(Arg * );
    const char * name;
    const unsigned char argtype;
};

static std::vector<ActionFunction> actionFunctions;
std::vector<Key> keys;

void dump_defines();
void dump_keys();
char **
splitstring(std::string str){
    std::size_t pos = 0;
    std::vector<char *>carg;
    bool isInString;
    while(1){
        pos = 0;
        isInString = false;
        while(1){
            pos = str.find_first_of(" \t\"", pos);
            if(pos == std::string::npos){
                pos = str.size();
                break;
            }
            if(str[pos] == '"'){
                if(pos != 0 && str[pos-1] == '\\'){
                    str = str.erase(pos-1, 1);
                    if(isInString)
                        pos = str.find('"', pos);
                    continue;
                }
                str = str.erase(pos, 1);
                if(!isInString){
                    pos = str.find('"', pos);
                    isInString = true;
                }
            }
            else{
                break;
            }
        }
        char * part = new char[pos + 1]();
        std::copy(str.begin(), str.begin()+pos, part);
        carg.push_back(part);
        if(pos == std::string::npos || pos +1 >= str.size())
            break;
        pos = str.find_first_not_of(" \t", pos);
        str = str.erase(0, pos);
    }
    char ** cargarr = new  char*[carg.size()+1]();
    std::copy(carg.begin(), carg.end(), cargarr);
    return cargarr;
}
void
register_actionfunc(void (*func)(Arg *), const char * name, const unsigned char argtype){
    actionFunctions.push_back({func, name, argtype});
}
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
        std::string * def = new std::string[2];
        def[0] = alias;
        def[1] = value;
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
    char c;
    for(std::string * &def : definitions){
        while((pos = str.find(def[0])) != std::string::npos && 
        (str.size() == pos + def[0].size() || (c = str[pos+def[0].size()]) == ' ' ||
         c == '\t' || c == ',' )){
            str = str.substr(0,pos) + def[1] + str.substr(pos + def[0].length());
        }
    }
    return str;
}
void
parse_key(std::string str, std::vector<Key> *keys){
    Key *k = new Key();
    std::size_t pos;
    std::string section;
    //get key section (Modifiers included)
    pos = str.find(",");
    if(pos == std::string::npos || pos + 1 > str.size())
        return;
    section = trim(str.substr(0, pos+1));
    str = str.substr(pos + 1);

    while((pos = section.find_first_of(" \t,")) != std::string::npos){
        std::string substr = section.substr(0, pos);
        if( substr ==  "Mod4Mask")
            k->mod |= Mod4Mask;
        else if( substr == "Mod1Mask")
            k->mod |= Mod1Mask;
        else if( substr == "ControlMask")
            k->mod |= ControlMask;
        else if( substr == "ShiftMask")
            k->mod |= ShiftMask;
        else{
            if(k->keysym)
                return;
            if(substr.substr(0, 2) == "0x"){
                k->keysym = std::stoul(substr, nullptr, 16);
                if(!k->keysym)
                    return;
            }
            else{
                k->keysym = XStringToKeysym(substr.c_str());
                if(!k->keysym)
                    return;
            }
        }
        pos = section.find_first_not_of(" \t,", pos);
        section.erase(0, pos);
    }
    //get actionfunction section
    unsigned char argtype = 0; // 0: no arg, 1: int, 2: uint, 3: float, 4: void
    pos = str.find(",");
    if(pos == std::string::npos || pos + 1 > str.size())
        return;
    section = trim(str.substr(0, pos));
    str = str.substr(pos + 1);

    for(ActionFunction &func : actionFunctions){
        if(!strcmp(section.c_str(),func.name)){
            k->func = func.func;
            argtype = func.argtype;
            break;
        }
    }
    if(k->func == nullptr){
        echo("No matching actionfunction found for: '%s'\n", section.c_str());
        return;
    }
    //get argument
    Arg arg= {0};
    pos = str.find(",");
    if(pos == std::string::npos || pos + 1 > str.size())
        return;

    section = trim(str.substr(0, pos));
    str = str.substr(pos + 1);

    if(!argtype || section.size() <= 0)
        k->arg = arg;
    else{
        if(argtype == 1){ // integer
            int n;
            if(section[0] == '~')
                n = ~std::stoi(section.substr(1));
            else if(section[0] == '<')
                n = 1<<std::stoi(section.substr(1));
            else if(section[0] == '0' && section[1] == 'x')
                n = std::stoi(section, nullptr, 16);
            else
                n = std::stoi(section);
            arg.i = n;
        }
        else if(argtype == 2){ // unsigned int
            unsigned int n;
            if(section[0] == '~')
                n = ~std::stoul(section.substr(1));
            else if(section[0] == '<')
                n = 1<<std::stoul(section.substr(1));
            else if(section[0] == '0' && section[1] == 'x')
                n = std::stoul(section, nullptr, 16);
            else
                n = std::stoul(section);
            arg.ui = n;
        }
        else if(argtype == 3){ // float
            arg.f = std::stof(section);
        }
        else if(argtype == 4){ // char **
            char ** cargarr = splitstring(section);
            // int i = 0;
            // while( cargarr[i] ){
            //     echo("%s\n",cargarr[i]);
            //     i++;
            // }
            // echo("\n");
            arg.v = cargarr;
        }
        else{
            echo("Not implemted Argtype\n");
            return;
        }

        k->arg = arg;
    }
    // mode section (grabbed | repeate | release)
    section = trim(str)+ " ";
    while((pos =section.find_first_of(" \t")) != std::string::npos){
        str = section.substr(0, pos);
        if(str == "Grabbed"){
            k->grabRepRelMask |= 1;
        }
        else if(str == "Repeat"){
            k->grabRepRelMask |= 1<<1;
        }
        else if(str == "Release"){
            k->grabRepRelMask |= 1<<2;
        }
        pos = section.find_first_not_of(" \t", pos);
        section = section.erase(0, pos);
    }
    keys->push_back(*k);
    delete k;
}
void
parse_button(std::string str, std::vector<Button> *buttons){

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
        if(line.substr(0, 3) == "key"){
            line = ltrim(line.substr(3));
            if(line[0] == '='){
                line = line.substr(1);
                // echo("%s\n -> %s\n", line.c_str(), applyDefinitions(defines, line).c_str()); 
                parse_key(applyDefinitions(defines, line), &_keys);
            }
            continue;
        }
        else if(line.substr(0,6) =="button"){
            line = ltrim(line.substr(6));
            if(line[0] == '='){
                line = line.substr(1);
                parse_button(applyDefinitions(defines, line), &_buttons);
            }
        }
    }
    conf_file.close();
    //cleanup defines
    for(std::string* &define : defines){
        delete[] define;
    }
    defines.clear();
    //cleanup keys
    dump_keys();
    keys = _keys;
    _keys.clear();
}
void
cleanup(){
    dump_keys();
    keys.clear();
}
void
dump_keys(){
    bool deleteArgs(false);
    for(Key &key : keys){
        deleteArgs = false;
        for(ActionFunction &func : actionFunctions)
            if(key.func == func.func && (func.name == "spawn"))
                deleteArgs = true;
        if(!deleteArgs)
            break;
        char ** cargarr = (char **)key.arg.v;
        int i = 0;
        while (cargarr[i])
        {
            delete[] cargarr[i];
            i++;
        }
        delete[] cargarr[i];
        delete[] cargarr;        
    }
    keys.clear();
}
} // End of namespace config