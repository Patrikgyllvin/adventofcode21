#include <iostream>

#include <string>
#include <vector>

#include "lib.hpp"

auto parse( const std::string& input )
{
    return getNumbers< int >( input, ',' );
}

int partOne( const std::string& input )
{
    parse( input );

    return 0;
}

int partTwo( const std::string& input )
{
    parse( input );

    return 0;
}