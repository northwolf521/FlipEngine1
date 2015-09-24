#ifndef __DRAWVERT_H__
#define __DRAWVERT_H__

#include "vec2.h"
#include "vec3.h"

class DrawVert {
public:
	vec3			xyz;
	vec2			st;
	vec3			normal;
	vec3			tangents[2];
	unsigned char color[4];

	float		operator[]( const int index ) const;
	float &		operator[]( const int index );

	void			Clear( void );

	void			Lerp( const idDrawVert &a, const idDrawVert &b, const float f );
	void			LerpAll( const idDrawVert &a, const idDrawVert &b, const float f );

	void			Normalize( void );

	//void		SetColor(  color );
	//dword		GetColor( void ) const;
};




#endif



