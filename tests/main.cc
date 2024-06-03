#include <iostream>
#include <fstream> // file stream
#include <sstream> // string stream
#include <vector>

namespace rift 
{
    namespace test
    {
        static bool errorOccured = false;

        void error(int line, std::string msg)
        {
            report(line, "", msg);
        }

        void report(int line, std::string where, std::string msg)
        {
            std::cout << "[line " << line << "] Error" << where << ": " + msg << std::endl;
            errorOccured = true;
        }

        void run(std::string lines, int n)
        {
            std::istringstream scanner(lines);
            std::cout << lines << std::endl;
        }

        void runFile(char* path)
        {
            std::ifstream file(std::string(path), std::ios::binary | std::ios::ate);

            if (file) {
                std::streamsize size = file.tellg();
                file.seekg(0, std::ios::beg);

                std::vector<char> buffer(size);
                if (file.read(buffer.data(), size)) {
                    run(buffer.data(), size);                
                    errorOccured = false;
                }
            }   
        }

        void runPrompt()
        {
            while(true)
            {
                std::string input = "";
                std::cout << "> " << std::endl;
                std::cin >> input;
                if (input.empty()) break;
                run(input, input.size());
            }
        }

    }
}

int main(char** argv, int argc)
{
    if (argc > 2) {
        std::cout << "Usage: rift" << std::endl;
    } else if (argc == 2) {
        // file
        rift::test::runFile(argv[1]);
    } else {
        // prompt
    }
}   