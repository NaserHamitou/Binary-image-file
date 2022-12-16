/**
* Ce programme permet de faire la détection de cible d’un drone.
* \authors	Nasereddine HAMITOU
* \date		22 novembre 2022
**/


#pragma region "Inclusions" //{

#include "GestionImage.hpp"
#include "ModificateurImage.hpp"

#include <ciso646>
#include <cstddef>
#include <cstdint>

#include <iomanip>
#include <iostream>
#include <string>

#include "cppitertools/range.hpp"

using namespace std;
using namespace iter;

#pragma endregion //}




void traitementBinaire();

void modificationImage();

string construireNomFichierResultat( const string&, const string& );




int main() {
	cout.setf(ios::boolalpha);

	cout << "----------" << "\n";

	traitementBinaire();

	cout << "\n";

	cout << "----------" << "\n";

	modificationImage();

	cout << "\n";

	return 0;
}

void traitementBinaire() {
	const unsigned LARGEUR_GENERATEUR = 100;
	const unsigned HAUTEUR_GENERATEUR = 110;
	const string nomFichierGenerateur = "flight_line_generator.bmp";
	const string nomFichierPartie1 = "resultat_partie_1.bmp";

	Pixel tamponMemoire[HAUTEUR_GENERATEUR][LARGEUR_GENERATEUR];
	Pixel* tamponPixels[HAUTEUR_GENERATEUR];
	for ( unsigned i : range(HAUTEUR_GENERATEUR) )
		tamponPixels[i] = tamponMemoire[i];

	Image imagePartie1 = {LARGEUR_GENERATEUR, HAUTEUR_GENERATEUR,
	                      tamponPixels};

	fstream fichierGenerateur(nomFichierGenerateur, ios::in | ios::binary);
	if ( fichierGenerateur.fail() ) {
		cout << "ERREUR: " << nomFichierGenerateur << " introuvable." << "\n";
		return;
	}

	bool ok;

	EnteteDib dibGenerateur = lireEnteteFichier(fichierGenerateur);

	ok = dibGenerateur.largeurImage == LARGEUR_GENERATEUR and
	     dibGenerateur.hauteurImage == HAUTEUR_GENERATEUR and
	     dibGenerateur.tailleTableau == calculerTailleTableau(imagePartie1);
	cout << "Contenu du DIB OK : " << ok << "\n";

	lireDonneesImage(fichierGenerateur, imagePartie1);

	convertirNoirEtBlanc(imagePartie1);

	ecrireImage(nomFichierPartie1, imagePartie1, ok);
	cout << "Ecriture OK : " << ok << "\n";
}

void modificationImage() {
	const unsigned EPAISSEUR_TRAIT = 3;

	const string commandeFin = "quit";
	const Pixel noir = {0x00, 0x00, 0x00};

	const string nomFichierOriginal = "flight_line.bmp";
	const string nomFichierPartie2 = "resultat_partie_2.bmp";

	bool ok;

	Image image = lireImage(nomFichierOriginal, ok);
	if (ok) cout << "L'ouverture a reussie " << endl;
	else return;
		

	cout << "Entrez '" << commandeFin << "' comme nom d'image pour quitter." << "\n\n";

	string commande;
	while (commande != commandeFin) {
		cout << "Nom de l'image a extraire : ";
		cin >> commande;
		if (commande == "quit") break;

		string nomFichier = construireNomFichierResultat(nomFichierPartie2, commande);

		Rectangle rectangle;
		cout << "Coin inferieur gauche (x y) : ";
		cin >> commande;
		rectangle.coin1.x = stoi(commande);
		cin >> commande;
		rectangle.coin1.y = stoi(commande);
		if (commande == "quit") break;
		cout << "Coin superieur droit (x y) : ";
		cin >> commande;
		rectangle.coin2.x = stoi(commande);
		cin >> commande;
		rectangle.coin2.y = stoi(commande);
		if (commande == "quit") break;

		Image imageRectangle = extraireRectangle(image, rectangle);


		ecrireImage(nomFichier, imageRectangle, ok);
		if (ok) cout << "L'ouverture a reussie." << endl;
		else cout << "L'ouverture a echouee." << endl;

		tracerContourRectangle(image, noir, rectangle, EPAISSEUR_TRAIT);

		ecrireImage(nomFichierPartie2, image, ok);
		desallouerImage(imageRectangle);
	}
	desallouerImage(image);
}


string construireNomFichierResultat(const string& nomFichierPartie2, const string& nomImageExtraite) {
	size_t positionDernierPoint = nomFichierPartie2.rfind('.');
	string extension = nomFichierPartie2.substr(positionDernierPoint);
	size_t longueur = nomFichierPartie2.size() - extension.length();

	return nomFichierPartie2.substr(0, longueur) + "_" + nomImageExtraite + extension;
}
