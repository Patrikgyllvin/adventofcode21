#include <sstream>

#include <vector>
#include <unordered_set>

#include <algorithm>

struct Point
{
    int x, y;
    
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
            return point.y * 1000 + point.x;
        }
    };
}

struct Line
{
    Point p1, p2;
};

int run( const std::vector< Line >& lines )
{
    int overlaps = 0;
    std::unordered_set< Point > taken, overlapping;

    for( int i = 0; i != lines.size(); ++i )
    {
        const Line& l = lines[ i ];

        int a = l.p1.x, b = l.p2.x;
        int a1 = l.p1.y, b1 = l.p2.y;

        for( int j = a, k = a1; ( a > b ? j >= b : j <= b ) && ( a1 > b1 ? k >= b1 : k <= b1 ); )
        {
            Point p{ .x = j, .y = k };
            
            if( taken.find( p ) != taken.end() )
            {
                if( overlapping.find( p ) == overlapping.end() )
                {
                    overlaps++;
                    overlapping.emplace( std::move( p ) );
                }
            }
            else
            {
                taken.emplace( std::move( p ) );
            }

            if( l.p1.x == l.p2.x )
            {
                if( a1 > b1 )
                    --k;
                else
                    ++k;
            }
            else if( l.p1.y == l.p2.y )
            {
                if( a > b )
                    --j;
                else
                    ++j;
            }
            else
            {
                if( a > b )
                    --j;
                else
                    ++j;
                
                if( a1 > b1 )
                    --k;
                else
                    ++k;
            }
        }
    }
    
    return overlaps;
}

auto parse( const std::string& input, bool part2 )
{
    std::stringstream stream{ input };
    std::vector< Line > lines;

    std::string line;
    while( std::getline( stream, line ) )
    {
        std::size_t comma1 = line.find( ',' ),
                    space = line.find( ' ', comma1 + 1 ),
                    point2 = line.find_first_not_of( " ->", space + 1 ),
                    comma2 = line.find( ',', point2 + 1 );

        Line l{
            .p1 = Point{ .x = std::stoi( line.substr( 0, comma1 ) ),
                    .y = std::stoi( line.substr( comma1 + 1, space - comma1 - 1 ) ) },
            .p2 = Point{ .x = std::stoi( line.substr( point2, comma2 - point2 ) ),
                    .y = std::stoi( line.substr( comma2 + 1 ) ) } };
        if( part2 || ( l.p1.x == l.p2.x || l.p1.y == l.p2.y ) )
            lines.emplace_back( std::move( l ) );
    }
    
    return lines;
}

unsigned long long partOne( const std::string& input )
{
    const std::vector< Line > lines = parse( input, false );
    
    return run( lines );
}

unsigned long long partTwo( const std::string& input )
{
    std::vector< Line > lines = parse( input, true );
    
    return run( lines );
}