#include <error/error.hh>

namespace rift
{
    namespace error
    {
        /// @brief Used to report an error.
        void report(int line, std::string where, std::string msg)
        {
            std::cout << "[line " << line << "] Error" << where << ": " + msg << std::endl;
            errorOccured = true;
        }
    }
}