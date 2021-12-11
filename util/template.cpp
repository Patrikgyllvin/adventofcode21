#include <iostream>

#include <string>
#include <vector>

#include "lib.hpp"

auto parse( const std::string& input )
{
    return getNumbers< int >( input, ',' );
}

unsigned long long partOne( const std::string& input )
{
    parse( input );

    return 0;
}

unsigned long long partTwo( const std::string& input )
{
    parse( input );

    return 0;
}