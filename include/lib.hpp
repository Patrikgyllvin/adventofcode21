#ifndef LIB_H
#define LIB_H

#include <type_traits>

#include <string>
#include <vector>

unsigned long long partOne( const std::string& input );
unsigned long long partTwo( const std::string& input );

template< typename T >
using EnableIf = typename std::enable_if_t< T::value, int >;

template< typename T, EnableIf< std::is_integral< T > > = 0 >
std::vector< T > getNumbers( const std::string &str, const char& delim )
{
	std::vector< T > res;

	T value = 0;
	bool gotValue = false, neg = false;

	for( int i = 0; i != str.length(); ++i )
	{
		char c = str[ i ];
		if( c == delim || c == '\n')
		{
			if( gotValue ) res.push_back( value );

    		value = 0;
			gotValue = false;
			neg = false;

			continue;
		}
		else if( c == '-' )
		{
			neg = true;

			continue;
		}

		if( !neg )
			value = value * 10 + c -'0';
		else
			value = value * 10 - (c - '0');

		gotValue = true;
	}
    
	if( gotValue ) res.push_back( value );

	return res;
}

template< typename T, EnableIf< std::is_floating_point< T > > = 0 >
std::vector< T > getNumbers( const std::string &str, const char& delim )
{
	static_assert( true, "Not implemented." );
}

#endif