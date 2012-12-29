#include <iostream>
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "../include/MathIO.hpp"
#include "../include/draw.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	
	Uint16 x1,y1,x2,y2,x3,y3;//Points rentrés en cliquant
	int comptimg1 = 0; // compteur de points cliqués sur l'image 1
	int comptimg2 = 0; // compteur de points cliqués sur l'image 2
	int comptimg3 = 0; // compteur de points cliqués sur l'image 3
	
	// init SDL image
	if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == -1){
		std::cerr << "error IMG_Init" << std::endl;
		return EXIT_FAILURE;
	}

	// load some images
	SDL_Surface *image1 = IMG_Load("input/image1.jpg");
	SDL_Surface *image2 = IMG_Load("input/image2.jpg");
	SDL_Surface *image3 = IMG_Load("input/image3.jpg");
	if(image1 == 0 || image2 == 0 || image3 == 0){
		std::cerr << "error loading images" << std::endl;
		return 0;
	}

	// init screen surface
	if(SDL_Init(SDL_INIT_VIDEO) == -1){
		std::cerr << "error SDL_Init" << std::endl;
		return EXIT_FAILURE;
	}

	// create a screen surface that will include the 3 images
	SDL_Surface *screen = SDL_SetVideoMode(image1->w + image2->w + image3->w, image1->h, 32, SDL_HWSURFACE);
	SDL_WM_SetCaption("Trifocal Tensor", NULL);

	// bibd the images on the surface  
	SDL_Rect imageOffset;
	imageOffset.x = 0;
	imageOffset.y = 0;
	SDL_BlitSurface(image1, NULL, screen, &imageOffset);
	imageOffset.x = image1->w;
	SDL_BlitSurface(image2, NULL, screen, &imageOffset);
	imageOffset.x = image1->w + image2->w;
	SDL_BlitSurface(image3, NULL, screen, &imageOffset);
	
	// On ouvre les fichiers list de points pour effacer le contenu déja présent.
	ofstream listing1("./input/listingcliquable/listing1.list", ios::out | ios::trunc);
	if(listing1) {
		listing1.close();
	} 	
	else {
		cerr << "Impossible d'ouvrir la liste 1 !" << endl;
	}
	ofstream listing2("./input/listingcliquable/listing2.list", ios::out | ios::trunc);
	if(listing2) {
		listing2.close();
	} 	
	else {
		cerr << "Impossible d'ouvrir la liste 2 !" << endl;
	}
	ofstream listing3("./input/listingcliquable/listing3.list", ios::out | ios::trunc);
	if(listing3) {
		listing3.close();
	} 	
	else {
		cerr << "Impossible d'ouvrir la liste 2 !" << endl;
	}
	
	
	// load the point lists
	Eigen::MatrixXd list1;
	Eigen::MatrixXd list2;
	Eigen::MatrixXd list3;
	kn::loadMatrix(list1,"input/list1.list");
	kn::loadMatrix(list2,"input/list2.list");
	kn::loadMatrix(list3,"input/list3.list");
	
	// Creation des matrices de points cliqués
	/*Eigen::MatrixXd listClique1;
	Eigen::MatrixXd listClique2;
	Eigen::MatrixXd listClique3;*/

	// save a list
	kn::saveMatrix(list1,"/tmp/myList.mat");

	// some colors
	Uint32 red  = 0xffff0000;
	Uint32 blue = 0xff0000ff;
	Uint32 yellow = 0xffffff00;
	
	// draw points on image1
	for(int i=0; i<list1.rows(); ++i)
		fill_circle(screen, list1(i,0), list1(i,1), 3, red);

	// draw points on image2
	for(int i=0; i<list2.rows(); ++i)
		fill_circle(screen, list2(i,0)+image1->w, list2(i,1), 3, blue);

	// draw points on image3
	for(int i=0; i<list3.rows(); ++i)
		fill_circle(screen, list3(i,0)+image1->w+image2->w, list3(i,1), 3, yellow);
	
	
	//pause();
	bool done = false;
	while(!done) {
   // display everything
	SDL_Flip(screen);
  
	SDL_Event e;
		while(SDL_PollEvent(&e)) {
			// Traitement de l'évenement fermeture de fenêtre
			if(e.type == SDL_QUIT) {
				done = true;
				break;
			}
			if(e.type == SDL_MOUSEBUTTONDOWN) {
				if(e.button.button == SDL_BUTTON_LEFT) {
					if(e.button.x < 400) {
						comptimg1++;
						x1 = e.button.x;
						y1 = e.button.y;
						std::cout << e.button.x <<"-"<< e.button.y <<" img1"<< std::endl;
						
						// On ouvre le fichier listing de points 1 pour pouvoir le remplir.
						ofstream listing1("./input/listingcliquable/listing1.list", ios::out | ios::app);
						if(listing1) {
							listing1 << x1 <<" "<< y1 <<" "<<"1"<< endl;
							listing1.close();
						} 	
						else {
						cerr << "Impossible d'ouvrir le listing 1!" << endl;
						}
						fill_circle(screen, x1, y1, 3, red);
					}
					if(e.button.x > 399 && e.button.x < 800) {
						comptimg2++;
						x2 = e.button.x - 400;
						y2 = e.button.y;
						std::cout << e.button.x <<"-"<< e.button.y <<" img2"<< std::endl;
						
						// On ouvre le fichier listing de points 3 pour pouvoir le remplir.
						ofstream listing2("./input/listingcliquable/listing2.list", ios::out | ios::app);
						if(listing2) {
							listing2 << x2 <<" "<< y2 <<" "<<"1"<< endl;
							listing2.close();
						} 	
						else {
						cerr << "Impossible d'ouvrir le listing 2 !" << endl;
						}
						fill_circle(screen, x2+image1->w, y2, 3, blue);
					}
					if(e.button.x > 799) {
						comptimg3++;
						x3 = e.button.x - 800;
						y3 = e.button.y;
						std::cout << e.button.x <<"-"<< e.button.y <<" img3"<< std::endl;
						
						// On ouvre le fichier listing de points 3 pour pouvoir le remplir.
						ofstream listing3("./input/listingcliquable/listing3.list", ios::out | ios::app);
						if(listing3) {
							listing3 << x3 <<" "<< y3 <<" "<<"1"<< endl;
							listing3.close();
						} 	
						else {
						cerr << "Impossible d'ouvrir le listing 3 !" << endl;
						}
						fill_circle(screen, x3+image1->w+image2->w, y3, 3, yellow);
					}
					
					break;
				}
				break;
			}


			// Traitement des autres évenements:
		}
	}

	// quit sdl
	SDL_FreeSurface(image1); 
	SDL_FreeSurface(image2); 
	SDL_FreeSurface(image3); 
	IMG_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}



