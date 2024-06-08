#include "driver.hh"
#include <iostream>

namespace rift
{
    namespace test 
    {
        void Driver::version()
        {
            std::string versionString = std::string("Rift version ") + "0.0.1" + "\n\n(c) Aman 2024";
            app.set_version_flag("--version,-v", versionString);
            app.footer("(c) Aman 2024");
        }
        int Driver::run(int argc, char **argv)
        {
            try {
                app.parse(argc, argv);
            } catch (const CLI::ParseError &parseError) {
                return app.exit(parseError);
            }
            return 0;
        }
    }
}