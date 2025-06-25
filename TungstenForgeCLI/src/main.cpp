#include "TungstenForge.hpp"

int main(int argc, char** argv)
{
    W_LOG_INFO("Argument Count: {}", argc);
    for (uint32_t argumentIndex; argumentIndex < argc; ++argumentIndex) {
        W_LOG_INFO("  Argument Value: {}", argv);
    }
    wForge::TungstenForge tungstenForge;
    tungstenForge.Build();

    return 0;
}