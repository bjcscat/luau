// This file is part of the Luau programming language and is licensed under MIT License; see LICENSE.txt for details
#include "luacode.h"

#include "Luau/Compiler.h"

#include <string.h>

char* luau_compile(const char* source, size_t size, lua_CompileOptions* options, size_t* outsize)
{
    LUAU_ASSERT(outsize);

    Luau::CompileOptions opts;

    if (options)
    {
        static_assert(sizeof(lua_CompileOptions) == sizeof(Luau::CompileOptions), "C and C++ interface must match");
        memcpy(static_cast<void*>(&opts), options, sizeof(opts));
    }

    Luau::ParseOptions parseOpts;

    if (options && options->knownAttributes)
    {
        std::vector<const char*> knownAttributes;

        for (const char* const * p = options->knownAttributes;*p;p++)
        {
            knownAttributes.push_back(*p);
        }

        parseOpts.knownAttributes = std::move(knownAttributes);
    }

    std::string result = compile(std::string(source, size), opts, parseOpts);

    char* copy = static_cast<char*>(malloc(result.size()));
    if (!copy)
        return nullptr;

    memcpy(copy, result.data(), result.size());
    *outsize = result.size();
    return copy;
}
