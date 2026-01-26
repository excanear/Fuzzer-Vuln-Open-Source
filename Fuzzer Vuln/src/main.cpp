// main.cpp
#include "engine/fuzzer_engine.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    try
    {
        if (argc < 3)
        {
            std::cerr << "Usage: " << argv[0] << " <target> <corpus_dir> [--dry-run] [--timeout <ms>] [--gui]" << std::endl;
            return 1;
        }

        bool dry_run = false;
        int timeout = 5000;
        bool use_gui = false;

        for (int i = 3; i < argc; ++i)
        {
            if (std::string(argv[i]) == "--dry-run")
            {
                dry_run = true;
            }
            else if (std::string(argv[i]) == "--timeout" && i + 1 < argc)
            {
                timeout = std::stoi(argv[++i]);
            }
            else if (std::string(argv[i]) == "--gui")
            {
                use_gui = true;
            }
        }

        FuzzerEngine engine(argv[1], argv[2], dry_run, timeout, use_gui);
        engine.run();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception in main" << std::endl;
        return 1;
    }
}