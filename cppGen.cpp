#include <iostream>
#include <string>
#include <fstream>  

std::string template_h = "\
#ifndef __define__\n\
#define __define__\n\
\n\
class __class__ __base__{\n\
public:\n\
\t__class__();\n\
\t__virtual__~__class__();\n\
\n\
__protected__\
private:\n\
\n\
};\n\
#endif\
";

std::string template_cpp = "\
#include __include__\n\
\n\
__class__::__class__() {\n\
\t\n\
}\n\
\n\
__class__::~__class__() {\n\
\t\n\
}\n\
";


void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while( (start_pos = str.find(from, start_pos)) != std::string::npos ) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}
/*
    --help
    -f=XXX      --filename=XXX
    -c=XXX      --class=XXX
    -v          --virtual
    -b=XXX      --base=XXX
    -d=XXX      --define=XXX
    -i          --include
    --hpp
    
    --oh=XXX
    --oc=XXX
*/

int main (int argc, const char ** argv) {
    std::string __include__ = "";
    std::string __define__ = "";
    std::string __class__ = "";
    std::string __base__ = "";
    std::string __virtual__ = "";
    std::string __protected__ = "";

    bool is_hpp = false;
    bool is_virtual = false;
    bool is_include = false;

    std::string path_h = "";
    std::string path_cpp = "";
    std::string file_name = "";
    std::string file_name_h = "";
    std::string file_name_cpp = "";

    bool error = false;

    for (argc = argc - 1; argc > 0; --argc) {
        std::string arg = argv[argc];

        size_t pos_name = arg.find_first_not_of('-');
        size_t pos_equal = arg.find_first_of('=');

        std::string cmd = "", res = "";

        if (pos_equal != std::string::npos) {
            cmd = arg.substr(pos_name, pos_equal - pos_name);
            res = arg.substr(pos_equal + 1, arg.size() - (pos_equal + 1));
        } else {
            cmd = arg.substr(pos_name, arg.size() - (pos_name));
        }

        if (cmd == "help") {
            std::cout << "No help ATM" << std::endl;
        } else if (cmd == "f" || cmd == "filename") {
            file_name = res;
        } else if (cmd == "c" || cmd == "class") {
            __class__ = res;
        } else if (cmd == "v" || cmd == "virtual") {
            is_virtual = true;
        } else if (cmd == "b" || cmd == "base") {
            __base__ = res;
        } else if (cmd == "d" || cmd == "define") {
            __define__ = res;
        } else if (cmd == "i" || cmd == "include") {
            is_include = true;
        } else if (cmd == "hpp") {
            is_hpp = true;
        } else if (cmd == "oc") {
            path_cpp = res;
        } else if (cmd == "oh") {
            path_h = res;
        } else {
            error = true;
            break;
        }
    }

    if (error) {
        std::cout << "Something is not working, see --help" << std::endl;
        return 1;
    }
    
    if (is_virtual) {
        __virtual__ = "virtual ";
        __protected__ = "protected:\n\n";
    }

    if (__base__ != "") {
        __base__ = ": " + __base__ + " ";
    }

    if (file_name == "") {
        file_name = __class__;
    }

    if (is_hpp) {
        file_name_h = file_name + ".hpp";
    } else {
        file_name_h = file_name + ".h";
    }

    file_name_cpp = file_name + ".cpp";

    if (is_include) {
        if (path_h == "")
            __include__ = "<" + file_name_h + ">";
        else
            __include__ = "<" + path_h + "/" + file_name_h + ">";
    } else {
        if (path_h == "")
            __include__ = "\"" + file_name_h + "\"";
        else
            __include__ = "\"" + path_h + "/" + file_name_h + "\"";
    }

    std::string content_h = template_h;
    replaceAll(content_h, "__define__", __define__);
    replaceAll(content_h, "__class__", __class__);
    replaceAll(content_h, "__virtual__", __virtual__);
    replaceAll(content_h, "__protected__", __protected__);
    replaceAll(content_h, "__base__", __base__);

    if (path_h == "") path_h = ".";
    if (path_cpp == "") path_cpp = ".";

    std::ofstream out_h (std::string(path_h + "/" + file_name_h).c_str());
    std::cout << "Write into : " << path_h + "/" + file_name_h << std::endl;
    out_h << content_h;
    out_h.close();

    std::string content_cpp = template_cpp;
    replaceAll(content_cpp, "__class__", __class__);
    replaceAll(content_cpp, "__include__", __include__);

    std::ofstream out_cpp (std::string(path_cpp + "/" + file_name_cpp).c_str());
    std::cout << "Write into : " << path_cpp + "/" + file_name_cpp << std::endl;
    out_cpp << content_cpp;
    out_cpp.close();


    return 0;
}