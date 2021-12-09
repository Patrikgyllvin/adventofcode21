#include <deque>
#include <vector>

#include <numeric>

#include "lib.hpp"

unsigned long long simulate( std::deque< unsigned long long >& fish, int days )
{
    for( int day = 0; day != days; ++day )
    {
        unsigned long long toSpawn = fish.front();
        fish.pop_front();
        
        fish[ 6 ] += toSpawn;
        fish.push_back( toSpawn );
    }

    return std::accumulate( fish.begin(), fish.end(), 0ULL );
}

auto parse( const std::string& input )
{
	std::vector< int > ages = getNumbers< int >( input, ',' );
    std::deque< unsigned long long > fish( 9, 0 );
    
    for( int i = 0; i != ages.size(); ++i )
    {
        fish[ ages[ i ] ]++;
    }
	
	return fish;
}

unsigned long long partOne( const std::string& input )
{
	std::deque< unsigned long long > fish = parse( input );
	return simulate( fish, 80 );
}

unsigned long long partTwo( const std::string& input )
{
	std::deque< unsigned long long > fish = parse( input );
    return simulate( fish, 256 );
}