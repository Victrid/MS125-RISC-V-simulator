#include "core_pipeline.hpp"
core_session::core_session(const char* ch) : WB(*this), IF(*this), ID(*this), EX(*this), MEM(*this) {
    memset(reg, 0, 32 * sizeof(taddr));
    memory.memload(ch);
};