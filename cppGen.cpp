#include <iostream>
#include <string>
#include <fstream>  
#include <map>
#include <algorithm>

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while( (start_pos = str.find(from, start_pos)) != std::string::npos ) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void interruptProgram(std::string const& error) {
    std::cerr << error << std::endl;
    exit(1);
}

std::pair<std::string, std::string> splitArg(std::string const& arg) {
    std::pair<std::string, std::string> p;

    size_t pos_name = 0;
    size_t pos_equal = arg.find_first_of('=');

    if (pos_equal != std::string::npos) {
        p.first = arg.substr(pos_name, pos_equal - pos_name);
        p.second = arg.substr(pos_equal + 1, arg.size() - (pos_equal + 1));
    } else {
        p.first = arg.substr(pos_name, arg.size() - (pos_name));
        p.second = "";
    }

    std::cout << p.first << ", " << p.second << std::endl;

    return p;
}

void writeInto (std::string const& path, std::string const& content) {
    std::ofstream out (path.c_str());
    std::cout << "Write into : " << path << std::endl;
    out << content;
    out.close();
}

std::string getFileContent(std::string const& path, std::string const& error = "Cannot open file !") {
    std::ifstream file(path, std::ios::in);
    if (!file)
        interruptProgram(error);

    std::string line;
    std::string content = "";
    
    while(getline(file, line)) {
        content += line + "\n";
    }
    
    file.close();
    return content;
}

void showHelp() {
    std::cout << "File Generator, by Hazurl" << std::endl;
    std::cout << "Replace all selected keyword of a file by a specific value" << std::endl;
    std::cout << "\t-t=... : path to templated file (mandatory)" << std::endl;
    std::cout << "\t-p=... : path to output file (mandatory)" << std::endl;
    std::cout << "\t-b=... : start keyword delimiter (by default \"{{\")" << std::endl;
    std::cout << "\t-e=... : end keyword delimiter (by default \"}}\")" << std::endl << std::endl;

    std::cout << "To specify keyword to be replaced :" << std::endl;
    std::cout << "\tkeyword=value" << std::endl << std::endl;

    std::cout << "The templated file need to write : {{keyword}}" << std::endl;
    std::cout << "There is two modifier available : {{keyword:upper} and {{keyword:lower}}" << std::endl << std::endl;

    std::cout << "Exmaple : " << std::endl;
    std::cout << "templated_file.txt :\n\tThis is a {{file}}, that can be either {{black:upper}} or {{white:lower}}." << std::endl;
    std::cout << "Command : \n\tfgen -p=output.txt -t=templated_file.txt file=JokE black=FUnny white=nOT" << std::endl;
    std::cout << "ouput.txt : \n\tThis is a JokE, that can be either FUNNY or not." << std::endl;  
}

std::string tryGet(std::map<std::string, std::string> & replacer, std::string const& key, std::string const& error) {
    auto it = replacer.find(key);
    if (it == replacer.end())
        interruptProgram(error);

    return it->second;
}

std::string getDefault (std::map<std::string, std::string> & replacer, std::string const& key, std::string const& defaultValue) {
    auto it = replacer.find(key);
    if (it == replacer.end()) {
        return defaultValue;
    }
    return it->second;
}

/////////////////////////////////////////////////////////////////
//                            MAIN                             //
/////////////////////////////////////////////////////////////////

int main (int argc, const char ** argv) {
    std::map<std::string, std::string> replacer {};

    for (int count = argc - 1; count > 0; --count) {
        auto p = splitArg(argv[count]);

        if (p.first == "--help") {
            showHelp();
            return 0;
        }

        replacer.insert(p);
    }

    std::string path = tryGet(replacer, "-p", "No path found ! (select it with \"-p=...\")");
    std::string templated_file = tryGet(replacer, "-t", "No template found ! (select it with \"-t=...\")");
    std::string delimiterBegin = getDefault(replacer, "-b", "{{");
    std::string delimiterEnd = getDefault(replacer, "-e", "}}");

    std::string content = getFileContent(templated_file, "Cannot open template file !");

    for (auto& p : replacer) {
        if (p.first[0] == '-')
            continue;

        std::string upper_case = p.second;
        std::string lower_case = p.second;
        std::transform(p.second.begin(), p.second.end(), upper_case.begin(), toupper);
        std::transform(p.second.begin(), p.second.end(), lower_case.begin(), tolower);
        
        replaceAll(content, delimiterBegin + p.first + delimiterEnd, p.second);
        replaceAll(content, delimiterBegin + p.first + ":upper" + delimiterEnd, upper_case);
        replaceAll(content, delimiterBegin + p.first + ":lower" + delimiterEnd, lower_case);
    }

    writeInto(path, content);

    return 0;
}