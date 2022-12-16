/**
* Ce programme permet de faire la détection de cible d’un drone.
* \authors	Nasereddine HAMITOU
* \date		22 novembre 2022
**/


#pragma region "Inclusions" //{

#include "ModificateurImage.hpp"

#include "GestionImage.hpp"

#include <ciso646>
#include <cstddef>
#include <cstdint>

#include <fstream>
#include <string>

#include "gsl/span"
#include "cppitertools/range.hpp"

using namespace std;
using namespace gsl;
using namespace iter;

#pragma endregion //}




#pragma region "Globaux" //{

#pragma region "Fonctions" //{

EnteteDib
lireEnteteFichier ( fstream& fichier )
{
	EnteteDib enteteDib;
	fichier.seekg(sizeof(EnteteBmp), ios::beg);
	fichier.read((char*)&enteteDib, sizeof(enteteDib));
	return enteteDib;
}


void
lireDonneesImage ( fstream& fichier, Image& image )
{
	unsigned tailleEntete = sizeof(EnteteBmp) + sizeof(EnteteDib);
	fichier.seekg(tailleEntete, ios::beg);

	for (unsigned ligne : range(image.hauteur)) {
		fichier.read((char*)image.pixels[ligne], image.largeur * sizeof(Pixel));
		fichier.seekg(calculerTaillePadding(image), ios::cur);
	}
}


void
ecrireDonneesImage ( fstream& fichier, const Image& image )
{
	unsigned tailleEntete = sizeof(EnteteBmp) + sizeof(EnteteDib);
	fichier.seekg(tailleEntete, ios::beg);
	
	char bytePadding = '0';
	for (unsigned ligne : range(image.hauteur)) {
		fichier.write((char*)image.pixels[ligne], image.largeur * sizeof(Pixel));
		fichier.write((char*)&bytePadding, calculerTaillePadding(image));
	}
}


void
ecrireImage ( const string& nomFichier, const Image& image, bool& ok )
{

	fstream fichier(nomFichier, ios::out|ios::binary);
	ok = !fichier.fail();
	if (ok) {

		EnteteBmp enteteBmp = construireEnteteBmp(image);
		EnteteDib enteteDib = construireEnteteDib(image);

		fichier.write((char*)&enteteBmp, sizeof(enteteBmp));
		fichier.write((char*)&enteteDib, sizeof(enteteDib));

		ecrireDonneesImage(fichier, image);
	}
	fichier.close();
}


Image
allouerImage ( unsigned largeur, unsigned hauteur )
{
	Image image;

	if (largeur != 0 && hauteur != 0) {

		image.largeur = largeur;
		image.hauteur = hauteur;

		image.pixels = new Pixel*[hauteur];
		for (unsigned i : range(hauteur)) {
			image.pixels[i] = new Pixel[largeur];
		}
	}
	return image;
}


void
desallouerImage ( Image& image )
{

	if (image.pixels != NULL) {
		for (unsigned i : range(image.hauteur)) {
			delete[] image.pixels[i];
		}
		delete[] image.pixels;
	}
}


Image
copierImage ( const Image& image )
{
	Image imageCopier = allouerImage(image.largeur, image.hauteur);
	for (unsigned i : range(image.hauteur)) {
		for (unsigned j : range(image.largeur)) {
			imageCopier.pixels[i][j] = image.pixels[i][j];
		}
	}
	return imageCopier;
}


Image
lireImage ( const string& nomFichier, bool& ok )
{
	fstream fichier(nomFichier, ios::binary|ios::in);
	ok = !fichier.fail();
	Image image;

	if (ok) {
		EnteteDib enteteDib = lireEnteteFichier(fichier);
		image = allouerImage(enteteDib.largeurImage, enteteDib.hauteurImage);
		lireDonneesImage(fichier, image);
	}
	fichier.close();
	return image;
}


Image
extraireRectangle ( const Image& image, const Rectangle& zone )
{
	Image nouvelleImage;
	if (estZoneValide(image, zone)) {
		unsigned int largeur = zone.coin2.x - zone.coin1.x;
		unsigned int hauteur = zone.coin2.y - zone.coin1.y;
		nouvelleImage = allouerImage(largeur, hauteur);
		unsigned positionY = zone.coin1.y;
		unsigned positionX = zone.coin1.x;
		for (unsigned i : range(hauteur)) {
			for (unsigned j : range(largeur)) {
				nouvelleImage.pixels[i][j] = image.pixels[positionY][positionX++];
			}
			positionX = zone.coin1.x;
			positionY++;
		}
		
	}
	return nouvelleImage;
}

#pragma endregion //}

#pragma endregion //}

