#include <sstream>

#include <string>
#include <vector>

#include "lib.hpp"

auto parse( const std::string& input )
{
    return getNumbers< int >( input, '\n' );
}

unsigned long long partOne( const std::string& input )
{
    std::vector< int > nums = parse( input );

    int numInc = 0, last = nums[ 0 ];
    for( int i = 1; i != nums.size(); ++i )
    {
        int num = nums[ i ];

        if( num > last ) numInc++;
        last = num;
    }

    return numInc;
}

unsigned long long partTwo( const std::string& input )
{
    std::vector< int > nums = parse( input );

    int numInc = 0, lastSum = -1;
    for( int i = 0; i != nums.size() - 2; ++i )
    {
        int sum = nums[ i ] + nums[ i + 1 ] + nums[ i + 2 ];

        if( lastSum != -1 && sum > lastSum ) numInc++;
        lastSum = sum;
    }

    return numInc;
}