#include <iostream>

#include <deque>
#include <vector>

#include <algorithm>
#include <numeric>

#include "lib.hpp"

auto parse( const std::string& input )
{
    return getNumbers< int >( input, ',' );
}

unsigned long long partOne( const std::string& input )
{
    std::vector< int > sortedPos = parse( input );
    std::sort( sortedPos.begin(), sortedPos.end() );
    
    int median;
    if( sortedPos.size() % 2 == 0 )
        median = ( sortedPos[ sortedPos.size() / 2 ] + sortedPos[ ( sortedPos.size() / 2 ) - 1 ] ) / 2;
    else
        median = sortedPos[ sortedPos.size() / 2 ];
    
    int sum = 0;
    for( int i = 0; i != sortedPos.size(); ++i )
    {
        sum += std::abs( median - sortedPos[ i ] );
    }
    
    return sum;
}

unsigned long long partTwo( const std::string& input )
{
    std::vector< int > positions = parse( input );
    int mean = std::accumulate( positions.begin(), positions.end(), 0 ) / positions.size();
    
    int sum = 0;
    for( int i = 0; i != positions.size(); ++i )
    {
        int n = std::abs( mean - positions[ i ] );
        sum += n * ( n + 1 ) / 2;
    }
    
    return sum;
}