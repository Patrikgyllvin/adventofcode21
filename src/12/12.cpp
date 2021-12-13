#include <iostream>
#include <sstream>

#include <cctype>

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <stack>

#include <vector>

#include "lib.hpp"

auto parse( const std::string& input )
{
    std::unordered_map< std::string, std::unordered_set< std::string > > graph;

    std::stringstream sstream{ input };
    std::string line;
    
    while( std::getline( sstream, line ) )
    {
        std::size_t pos = line.find( '-' );
        
        std::string node1 = line.substr( 0, pos ),
                    node2 = line.substr( pos + 1 );
        
        graph[ node1 ].emplace( node2 );
        graph[ node2 ].emplace( node1 );
    }
    
    return graph;
}

bool isLower( const std::string& str )
{
    for( const char& c : str )
    {
        if( !islower( c ) )
            return false;
    }
    
    return true;
}

void findPaths(
    std::unordered_map< std::string, std::unordered_set< std::string > >& graph,
    std::set< std::deque< std::string > >& paths,
    const std::string& start,
    const std::string& mayRepeat )
{
    std::stack<
        std::tuple< 
            std::string,
            std::unordered_multiset< std::string >,
            std::unordered_map< std::string, std::deque< std::string > > >
    > stack;
    stack.push(
        std::make_tuple(
            start,
            std::unordered_multiset< std::string >{},
            std::unordered_map< std::string, std::deque< std::string > >{}
        )
    );

    while( !stack.empty() )
    {
        std::tuple<
            std::string,
            std::unordered_multiset< std::string >,
            std::unordered_map< std::string, std::deque< std::string > >
        > top = stack.top();
        stack.pop();
        
        const std::string& current = std::get< 0 >( top );
        std::unordered_multiset< std::string >& visited = std::get< 1 >( top );
        std::unordered_map< std::string, std::deque< std::string > >& prev = std::get< 2 >( top );

        if( current == "end" )
        {
            std::deque< std::string > path;
            path.emplace_front( current );

            for( std::string node = prev[ current ].back(); node != start; )
            {
                path.emplace_front( node );

                std::string tmp = prev[ node ].back();
                prev[ node ].pop_back();
                node = tmp;
            }

            path.emplace_front( start );
            paths.emplace( std::move( path ) );

            continue;
        }
        
        if( !isLower( current ) || !visited.count( current ) || ( current == mayRepeat && visited.count( current ) < 2 ) )
        {
            if( isLower( current ) )
                visited.emplace( current );
            
            for( const auto& node : graph[ current ] )
            {
                std::unordered_map< std::string, std::deque< std::string > > prevCopy{ prev };
                prevCopy[ node ].emplace_back( current );

                stack.push( std::make_tuple( node, std::unordered_multiset< std::string >{ visited }, prevCopy ) );
            }
        }
    }
}

unsigned long long partOne( const std::string& input )
{
    std::unordered_map< std::string, std::unordered_set< std::string > > graph = parse( input );

    std::set< std::deque< std::string > > paths;
    findPaths( graph, paths, "start", "" );

    return paths.size();
}

unsigned long long partTwo( const std::string& input )
{
    std::unordered_map< std::string, std::unordered_set< std::string > > graph = parse( input );

    std::set< std::deque< std::string > > paths;
    for( auto it = graph.begin(); it != graph.end(); ++it )
    {
        const std::string& key = it->first;
        std::cout << key << '\n';
        if( key != "start" && key != "end" && isLower( key ) )
            findPaths( graph, paths, "start", key );
    }

    return paths.size();
}