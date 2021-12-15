#include <sstream>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

#include "lib.hpp"

struct Point
{
    const int x, y;
    
    inline bool operator==( const Point& rhs ) const
    {
        return this->x == rhs.x && this->y == rhs.y;
    }
};

namespace std
{
    template<>
    struct hash< Point >
    {
        inline size_t operator()( const Point& point ) const
        {
            return point.y * 2000 + point.x;
        }
    };
}

auto parse( const std::string& input, std::unordered_set< Point >& dots, std::vector< std::pair< char, int > >& instructions )
{
    std::pair< int, int > finalSize = std::make_pair( 0, 0 );

    std::size_t lineBreak = input.find( "\n\n" );
    std::stringstream sstream{ input.substr( 0, lineBreak ) };
    std::string line;
    
    while( std::getline( sstream, line ) )
    {
        std::vector< int > nums = getNumbers< int >( line, ',' );
        dots.emplace( nums[ 0 ], nums[ 1 ] );
    }
    
    sstream = std::stringstream{ input.substr( lineBreak + 2 ) };
    while( std::getline( sstream, line ) )
    {
        std::size_t pos = line.find_last_of( ' ' );
        int fold = std::stoi( line.substr( pos + 3 ) );

        instructions.emplace_back( line[ pos + 1 ], fold );
        if( line[ pos + 1 ] == 'x' ) finalSize = std::make_pair( fold * 2, std::get< 1 >( finalSize ) );
        else if( line[ pos + 1 ] == 'y' ) finalSize = std::make_pair( std::get< 0 >( finalSize ), fold * 2 );
    }

    return finalSize;
}

void fold( std::unordered_set< Point >& dots, const std::pair< char, int >& instruction )
{
    for( auto it = dots.begin(); it != dots.end(); )
    {
        int instrVal;
        if( std::get< 0 >( instruction ) == 'x' && it->x > ( instrVal = std::get< 1 >( instruction ) ) )
        {
            int newX = instrVal - ( it->x - instrVal ), newY = it->y;
            it = dots.erase( it );
            dots.emplace( newX, newY );
        }
        else if( std::get< 0 >( instruction ) == 'y' && it->y > ( instrVal = std::get< 1 >( instruction ) ) )
        {
            int newX = it->x, newY = instrVal - ( it->y - instrVal );
            it = dots.erase( it );
            dots.emplace( newX, newY );
        }
        else
        {
            ++it;
        }
    }
}

void printDots( const std::unordered_set< Point >& dots, const std::pair< int, int >& size )
{
    for( int i = 0; i != std::get< 1 >( size ); ++i )
    {
        for( int j = 0; j != std::get< 0 >( size ); ++j )
        {
            if( dots.count( Point{ .x = j, .y = i } ) )
                std::cout << '#';
            else
                std::cout << '.';
        }
        
        std::cout << '\n';
    }
}

unsigned long long partOne( const std::string& input )
{
    std::vector< std::pair< char, int > > instructions;
    std::unordered_set< Point > dots;
    parse( input, dots, instructions );

    fold( dots, instructions[ 0 ] );
    
    return dots.size();
}

unsigned long long partTwo( const std::string& input )
{
    std::vector< std::pair< char, int > > instructions;
    std::unordered_set< Point > dots; 
    std::pair< int, int > finalSize = parse( input, dots, instructions );
    
    for( const auto& instruction : instructions )
    {
        fold( dots, instruction );
    }
    
    printDots( dots, finalSize );
    
    return dots.size();
}