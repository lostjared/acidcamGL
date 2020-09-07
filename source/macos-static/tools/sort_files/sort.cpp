#include<iostream>
#include<unistd.h>
#include<vector>
#include<algorithm>
#include<sstream>

int main(int argc, char **argv) {
    
    if(argc != 2) exit(0);
    
    std::ostringstream stream;
    stream << "/bin/ls " << argv[1];
    FILE *fptr = popen(stream.str().c_str(), "r");
    if(fptr) {
        std::vector<std::string> files;
        while(!feof(fptr)) {
            char buf[2048];
            fgets(buf, 2048, fptr);
            buf[strlen(buf)-1] = 0;
            std::string val = buf;
            if(val.find(".cpp") != std::string::npos || val.find(".hpp") != std::string::npos || val.find(".h") != std::string::npos || val.find(".mm") != std::string::npos)
            files.push_back(val);
        }
        std::sort(files.begin(), files.end());
        for(int i = 0; i < files.size(); ++i)
            std::cout << files[i] << " ";
        std::cout << "\n";
        pclose(fptr);
    }
    
    return 0;
}
