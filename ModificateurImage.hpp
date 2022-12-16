#pragma once


#pragma region "Inclusions" //{

#include <ciso646>
#include <cstddef>
#include <cstdint>


using namespace std;

#pragma endregion //}




#pragma region "Déclarations" //{

#pragma region "Macros" //{

#if defined(__GNUC__) || defined(__clang__)
	#define PACKED_STRUCT(declaration) \
	declaration  __attribute__((__packed__))
#elif defined(_MSC_VER)
	#define PACKED_STRUCT(declaration) \
	__pragma(pack(push, foo, 1)) declaration __pragma(pack(pop, foo))
#else
	#error Compiler not supported.
#endif

#pragma endregion //}


#pragma region "Constantes" //{

static const unsigned ALIGNEMENT_PIXELS = 4;

static const unsigned BMP_ID = 0x4D42; 
static const unsigned COMPRESSION_BI_RGB = 0; 
static const unsigned RESOLUTION_IMPRESSION = 2835; 

#pragma endregion //}


#pragma region "Structures d'enregistrement" //{

struct Point
{
	unsigned x;
	unsigned y;
};

struct Rectangle
{
	Point coin1;
	Point coin2;
};

struct Pixel
{
	uint8_t b;
	uint8_t g;
	uint8_t r;
};

struct Image
{
	unsigned largeur;
	unsigned hauteur;
	Pixel**  pixels;
};

PACKED_STRUCT(
struct EnteteBmp
{
	uint16_t id;
	uint32_t tailleFichier;
	uint16_t inutilise[2];
	uint32_t positionTableau;
});

PACKED_STRUCT(
struct EnteteDib
{
	uint32_t tailleEntete;
	int32_t  largeurImage;
	int32_t  hauteurImage;
	uint16_t nbPlansCouleur;
	uint16_t bpp;
	uint32_t compression;
	uint32_t tailleTableau;
	int32_t  resolutionImpression[2];
	uint32_t nbPalettes;
	uint32_t nbCouleursImportantes;
});

#pragma endregion //}


#pragma region "Globaux" //{

void convertirNoirEtBlanc ( Image& );

bool estRectangleValide ( const Rectangle& );

bool estZoneValide ( const Image&, const Rectangle& );

EnteteBmp construireBmpVide ( );

EnteteDib construireDibVide ( );

unsigned calculerTaillePadding ( const Image& );

unsigned calculerTailleTableau ( const Image& );

EnteteBmp construireEnteteBmp ( const Image& );

EnteteDib construireEnteteDib ( const Image& );

void tracerLigneHorizontale ( Image&, Pixel, const Point[2], unsigned );

void tracerLigneVerticale ( Image&, Pixel, const Point[2], unsigned );

void dessinerCarre ( Image&, Pixel, const Point&, unsigned );

void tracerContourRectangle ( Image&, Pixel, const Rectangle&, unsigned );

#pragma endregion //}

#pragma endregion //}

