#ifndef __MAPFILE_H__
#define __MAPFILE_H__

/*
===============================================================================

	Reads or writes the contents of .map files into a standard internal
	format, which can then be moved into private formats for collision
	detection, map processing, or editor use.

	No validation (duplicate planes, null area brushes, etc) is performed.
	There are no limits to the number of any of the elements in maps.
	The order of entities, brushes, and sides is maintained.

===============================================================================
*/

const int OLD_MAP_VERSION					= 1;
const int CURRENT_MAP_VERSION				= 2;
const int DEFAULT_CURVE_SUBDIVISION			= 4;
const float DEFAULT_CURVE_MAX_ERROR			= 4.0f;
const float DEFAULT_CURVE_MAX_ERROR_CD		= 24.0f;
const float DEFAULT_CURVE_MAX_LENGTH		= -1.0f;
const float DEFAULT_CURVE_MAX_LENGTH_CD		= -1.0f;

#include "common/Plane.h"
#include "common/vec3.h"

class idMapPrimitive {
public:
	enum { TYPE_INVALID = -1, TYPE_BRUSH, TYPE_PATCH };

	idDict					epairs;

							idMapPrimitive( void ) { type = TYPE_INVALID; }
	virtual					~idMapPrimitive( void ) { }
	int						GetType( void ) const { return type; }

protected:
	int						type;
};


class idMapBrushSide {
	friend class idMapBrush;

public:
							idMapBrushSide( void );
							~idMapBrushSide( void ) { }
	const char *			GetMaterial( void ) const { return material; }
	void					SetMaterial( const char *p ) { material = p; }
	const Plane &			GetPlane( void ) const { return plane; }
	void					SetPlane( const Plane &p ) { plane = p; }
	void					SetTextureMatrix( const vec3 mat[2] ) { texMat[0] = mat[0]; texMat[1] = mat[1]; }
	void					GetTextureMatrix( vec3 &mat1, vec3 &mat2 ) { mat1 = texMat[0]; mat2 = texMat[1]; }
	void					GetTextureVectors( idVec4 v[2] ) const;

protected:
	idStr					material;
	Plane					plane;
	vec3					texMat[2];
	vec3					origin;
};

ID_INLINE idMapBrushSide::idMapBrushSide( void ) {
	plane.Zero();
	texMat[0].Zero();
	texMat[1].Zero();
	origin.Zero();
}


class idMapBrush : public idMapPrimitive {
public:
							idMapBrush( void ) { type = TYPE_BRUSH; sides.Resize( 8, 4 ); }
							~idMapBrush( void ) { sides.DeleteContents( true ); }
	static idMapBrush *		Parse( idLexer &src, const vec3 &origin, bool newFormat = true, float version = CURRENT_MAP_VERSION );
	static idMapBrush *		ParseQ3( idLexer &src, const vec3 &origin );
	bool					Write( idFile *fp, int primitiveNum, const vec3 &origin ) const;
	int						GetNumSides( void ) const { return sides.Num(); }
	int						AddSide( idMapBrushSide *side ) { return sides.Append( side ); }
	idMapBrushSide *		GetSide( int i ) const { return sides[i]; }
	unsigned int			GetGeometryCRC( void ) const;

protected:
	int						numSides;
	idList<idMapBrushSide*> sides;
};


class idMapPatch : public idMapPrimitive, public idSurface_Patch {
public:
							idMapPatch( void );
							idMapPatch( int maxPatchWidth, int maxPatchHeight );
							~idMapPatch( void ) { }
	static idMapPatch *		Parse( idLexer &src, const vec3 &origin, bool patchDef3 = true, float version = CURRENT_MAP_VERSION );
	bool					Write( idFile *fp, int primitiveNum, const vec3 &origin ) const;
	const char *			GetMaterial( void ) const { return material; }
	void					SetMaterial( const char *p ) { material = p; }
	int						GetHorzSubdivisions( void ) const { return horzSubdivisions; }
	int						GetVertSubdivisions( void ) const { return vertSubdivisions; }
	bool					GetExplicitlySubdivided( void ) const { return explicitSubdivisions; }
	void					SetHorzSubdivisions( int n ) { horzSubdivisions = n; }
	void					SetVertSubdivisions( int n ) { vertSubdivisions = n; }
	void					SetExplicitlySubdivided( bool b ) { explicitSubdivisions = b; }
	unsigned int			GetGeometryCRC( void ) const;

protected:
	idStr					material;
	int						horzSubdivisions;
	int						vertSubdivisions;
	bool					explicitSubdivisions;
};

ID_INLINE idMapPatch::idMapPatch( void ) {
	type = TYPE_PATCH;
	horzSubdivisions = vertSubdivisions = 0;
	explicitSubdivisions = false;
	width = height = 0;
	maxWidth = maxHeight = 0;
	expanded = false;
}

ID_INLINE idMapPatch::idMapPatch( int maxPatchWidth, int maxPatchHeight ) {
	type = TYPE_PATCH;
	horzSubdivisions = vertSubdivisions = 0;
	explicitSubdivisions = false;
	width = height = 0;
	maxWidth = maxPatchWidth;
	maxHeight = maxPatchHeight;
	verts.SetNum( maxWidth * maxHeight );
	expanded = false;
}


class idMapEntity {
	friend class			idMapFile;

public:
	idDict					epairs;

public:
							idMapEntity( void ) { epairs.SetHashSize( 64 ); }
							~idMapEntity( void ) { primitives.DeleteContents( true ); }
	static idMapEntity *	Parse( idLexer &src, bool worldSpawn = false, float version = CURRENT_MAP_VERSION );
	bool					Write( idFile *fp, int entityNum ) const;
	int						GetNumPrimitives( void ) const { return primitives.Num(); }
	idMapPrimitive *		GetPrimitive( int i ) const { return primitives[i]; }
	void					AddPrimitive( idMapPrimitive *p ) { primitives.Append( p ); }
	unsigned int			GetGeometryCRC( void ) const;
	void					RemovePrimitiveData();

protected:
	idList<idMapPrimitive*>	primitives;
};


class idMapFile {
public:
							idMapFile( void );
							~idMapFile( void ) { entities.DeleteContents( true ); }

							// filename does not require an extension
							// normally this will use a .reg file instead of a .map file if it exists,
							// which is what the game and dmap want, but the editor will want to always
							// load a .map file
	bool					Parse( const char *filename, bool ignoreRegion = false, bool osPath = false );
	bool					Write( const char *fileName, const char *ext, bool fromBasePath = true );
							// get the number of entities in the map
	int						GetNumEntities( void ) const { return entities.Num(); }
							// get the specified entity
	idMapEntity *			GetEntity( int i ) const { return entities[i]; }
							// get the name without file extension
	const char *			GetName( void ) const { return name; }
							// get the file time
	ID_TIME_T					GetFileTime( void ) const { return fileTime; }
							// get CRC for the map geometry
							// texture coordinates and entity key/value pairs are not taken into account
	unsigned int			GetGeometryCRC( void ) const { return geometryCRC; }
							// returns true if the file on disk changed
	bool					NeedsReload();

	int						AddEntity( idMapEntity *mapentity );
	idMapEntity *			FindEntity( const char *name );
	void					RemoveEntity( idMapEntity *mapEnt );
	void					RemoveEntities( const char *classname );
	void					RemoveAllEntities();
	void					RemovePrimitiveData();
	bool					HasPrimitiveData() { return hasPrimitiveData; }

protected:
	float					version;
	ID_TIME_T					fileTime;
	unsigned int			geometryCRC;
	idList<idMapEntity *>	entities;
	idStr					name;
	bool					hasPrimitiveData;

private:
	void					SetGeometryCRC( void );
};

ID_INLINE idMapFile::idMapFile( void ) {
	version = CURRENT_MAP_VERSION;
	fileTime = 0;
	geometryCRC = 0;
	entities.Resize( 1024, 256 );
	hasPrimitiveData = false;
}

#endif /* !__MAPFILE_H__ */