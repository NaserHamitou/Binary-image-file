#pragma region "Inclusions" //{

#include "ModificateurImage.hpp"

#include <ciso646>
#include <cstddef>
#include <cstdint>

#include <algorithm>

#include "cppitertools/range.hpp"

using namespace std;
using namespace iter;

#pragma endregion //}




#pragma region "Globaux" //{

#pragma region "Fonctions" //{

void
convertirNoirEtBlanc ( Image& image )
{
	for ( unsigned y : range(image.hauteur) ) {
		for ( unsigned x : range(image.largeur) ) {
			Pixel& pixel = image.pixels[y][x];
			uint8_t nuanceGris = uint8_t((int(pixel.r) + pixel.g + pixel.b) / 3);
			pixel = { nuanceGris, nuanceGris, nuanceGris };
		}
	}
}


bool
estRectangleValide ( const Rectangle& rectangle )
{
	return rectangle.coin1.x <= rectangle.coin2.x and
	       rectangle.coin1.y <= rectangle.coin2.y;
}

bool
estZoneValide ( const Image& image, const Rectangle& rectangle )
{
	return estRectangleValide(rectangle) and
	       rectangle.coin2.x <= image.largeur and
	       rectangle.coin2.y <= image.hauteur;
}


EnteteBmp
construireBmpVide ( )
{
	EnteteBmp resultat = {};
	resultat.id = BMP_ID;
	resultat.positionTableau = sizeof(EnteteBmp) + sizeof(EnteteDib);
	
	return resultat;
}


EnteteDib
construireDibVide ( )
{
	EnteteDib resultat = {};
	
	resultat.tailleEntete = sizeof(EnteteDib);
	resultat.nbPlansCouleur = 1;
	resultat.bpp = sizeof(Pixel) * 8;
	resultat.compression = COMPRESSION_BI_RGB;
	resultat.resolutionImpression[0] = resultat.resolutionImpression[1] = RESOLUTION_IMPRESSION;
	
	return resultat;
}


unsigned
calculerTaillePadding ( const Image& image )
{
	unsigned tailleBruteLigne = image.largeur * sizeof(Pixel);
	
	return (ALIGNEMENT_PIXELS - (tailleBruteLigne % ALIGNEMENT_PIXELS)) %
	       ALIGNEMENT_PIXELS;
}


unsigned
calculerTailleTableau ( const Image& image )
{
	unsigned padding = calculerTaillePadding(image);
	unsigned tailleLigne = image.largeur * sizeof(Pixel) + padding;
	
	return tailleLigne * image.hauteur;
}

EnteteBmp
construireEnteteBmp ( const Image& image )
{
	EnteteBmp resultat = construireBmpVide();
	
	resultat.tailleFichier = sizeof(EnteteBmp) + sizeof(EnteteDib) +
	                         calculerTailleTableau(image);
	
	return resultat;
}

EnteteDib
construireEnteteDib ( const Image& image )
{
	EnteteDib resultat = construireDibVide();
	
	resultat.largeurImage = image.largeur;
	resultat.hauteurImage = image.hauteur;
	resultat.tailleTableau = calculerTailleTableau(image);
	
	return resultat;
}

void
tracerLigneHorizontale ( Image& image, Pixel couleur, const Point ligne[2], unsigned epaisseur )
{
	unsigned debutX = min(ligne[0].x, ligne[1].x);
	unsigned finX   = max(ligne[0].x, ligne[1].x) + 1;
	
	unsigned debutY = ligne[0].y - min(epaisseur / 2, ligne[0].y);
	unsigned finY   = min(debutY + epaisseur, image.hauteur - 1);
	
	for ( unsigned y : range(debutY, finY) )
		for ( unsigned x : range(debutX, finX) )
			image.pixels[y][x] = couleur;
}

void
tracerLigneVerticale ( Image& image, Pixel couleur, const Point ligne[2], unsigned epaisseur )
{
	unsigned debutY = min(ligne[0].y, ligne[1].y);
	unsigned finY   = max(ligne[0].y, ligne[1].y) + 1;
	
	unsigned debutX = ligne[0].x - min(epaisseur / 2, ligne[0].x);
	unsigned finX   = min(debutX + epaisseur, image.largeur - 1);
	
	for ( unsigned y : range(debutY, finY) )
		for ( unsigned x : range(debutX, finX) )
			image.pixels[y][x] = couleur;
}

void
dessinerCarre ( Image& image, Pixel couleur, const Point& centre, unsigned dimension )
{
	unsigned debutX = centre.x - min(dimension / 2, centre.x);
	Point ligne[2] = {
		{debutX                                        , centre.y},
		{min(debutX + dimension - 1, image.largeur - 1), centre.y}
	};
	
	tracerLigneHorizontale(image, couleur, ligne, dimension);
}

void
tracerContourRectangle ( Image& image, Pixel couleur, const Rectangle& rectangle, unsigned epaisseur )
{
	Point sommets[4];
	
	sommets[0] = rectangle.coin1;
	sommets[1] = {rectangle.coin2.x, rectangle.coin1.y};
	sommets[2] = rectangle.coin2;
	sommets[3] = {rectangle.coin1.x, rectangle.coin2.y};
	
	if ( estZoneValide(image, rectangle) )
		for ( int i : range(4) ) {
			Point ligne[2] = { sommets[i], sommets[(i + 1) % 4] };
			
			if ( i % 2 == 0 )
				tracerLigneHorizontale(image, couleur, ligne, epaisseur);
			else
				tracerLigneVerticale(image, couleur, ligne, epaisseur);
			
			dessinerCarre(image, couleur, sommets[i], epaisseur);
		}
}

#pragma endregion //}

#pragma endregion //}

