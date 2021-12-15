#include <iostream>

#include <algorithm>

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "lib.hpp"

auto parse( std::string& input )
{
    std::size_t w = input.find( '\n' );
    input.erase( std::remove( input.begin(), input.end(), '\n' ), input.end() );
    
    std::size_t h = input.size() / w;

    return std::make_pair( w, h );
}

bool checkLowPoint( const std::string& board, int w, int h, int point )
{
    int height = board[ point ] - '0', x = point % w, y = point / w;

    int left, right, up, down;
    if( x == 0 )
        left = 9;
    else
        left = board[ point - 1 ] - '0';
    
    if( x == w - 1 )
        right = 9;
    else
        right = board[ point + 1 ] - '0';
    
    if( y == 0 )
        up = 9;
    else
        up = board[ point - w ] - '0';
    
    if( y == h - 1 )
        down = 9;
    else
        down = board[ point + w ] - '0';
    
    return height < left && height < right && height < up && height < down;
}

int findFlowNext( const std::string& board, int w, int h, int point )
{
    int x = point % w, y = point / w,
        left = point - 1, right = point + 1, up = point - w, down = point + w;

    int height = board[ point ] - '0';
    if( x != 0 && board[ left ] - '0' < height )
        return left;
    else if( x != w - 1 && board[ right ] - '0' < height )
        return right;
    else if( y != 0 && board[ up ] - '0' < height )
        return up;
    else if( y != h - 1 && board[ down ] - '0' < height )
        return down;
    
    return -1;
}

void findLowPoint(
    const std::string& board,
    std::vector< std::pair< bool, int > >& visited,
    std::unordered_set< int >& path,
    std::unordered_map< int, std::unordered_set< int > >& basins,
    int w, int h,
    int curr )
{
    if( checkLowPoint( board, w, h, curr ) )
    {
        basins[ curr ].insert( path.begin(), path.end() );

        for( auto& i : path )
        {
            visited[ i ].first = true;
            visited[ i ].second = curr;
        }
        
        return;
    }

    path.insert( curr );
    
    int next = findFlowNext( board, w, h, curr );
    
    if( visited[ next ].first )
    {
        basins[ visited[ next ].second ].insert( path.begin(), path.end() );
        
        for( auto& i : path )
        {
            visited[ i ].first = true;
            visited[ i ].second = visited[ next ].second;
        }
    }
    else
    {
        findLowPoint( board, visited, path, basins, w, h, next );
    }
}

unsigned long long partOne( const std::string& input )
{
    std::string board{ input };
    std::pair< std::size_t, std::size_t > size = parse( board );
    
    int sumRisk = 0;
    for( int i = 0; i != size.first * size.second; ++i )
    {
        if( checkLowPoint( board, size.first, size.second, i ) )
        {
            sumRisk += board[ i ] - '0' + 1;
        }
    }

    return sumRisk;
}

unsigned long long partTwo( const std::string& input )
{
    std::string board{ input };
    std::pair< std::size_t, std::size_t > size = parse( board );
    
    std::unordered_map< int, std::unordered_set< int > > basins;

    std::vector< std::pair< bool, int > > visited( size.first * size.second );
    for( int i = 0; i != size.first * size.second; ++i )
    {
        if( board[ i ] - '0' == 9 || visited[ i ].first ) continue;
        
        std::unordered_set< int > path;
        findLowPoint( board, visited, path, basins, size.first, size.second, i );
    }
    
    std::vector< std::pair< int, std::unordered_set< int > > > basinVec( basins.begin(), basins.end() );
    std::sort( basinVec.begin(), basinVec.end(), []( auto& p1, auto& p2 ) { return p1.second.size() > p2.second.size(); } );
    
    int mul = 1;
    for( int i = 0; i != 3; ++i )
    {
        mul *= basinVec[ i ].second.size() + 1;
    }

    return mul;
}