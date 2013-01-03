#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "../include/Tensor.hpp"
#include "../include/MatrixA.hpp"
#include "../include/Transfert.hpp"
#include "../include/MathIO.hpp"
#include "../include/draw.hpp"

using namespace std;

int main(int argc, char *argv[]) {

	for(int i=0; i<argc; ++i) {
		if(strcmp(argv[i],"-h")==0) {
			cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
			cout << "$$$$$$$$$$$-                                          -$$$$$$$$$$" << endl;
			cout << "$$$$$$$$$$$-  HOW TO USE THE PROGRAM trifocal tensor  -$$$$$$$$$$" << endl;
			cout << "$$$$$$$$$$$-                                          -$$$$$$$$$$" << endl;
			cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
			cout << "$$$$-                                                       -$$$$" << endl;
			cout << "$$$$-  # if you want to load three lists,                   -$$$$" << endl;
			cout << "$$$$-    you can push \"c\"                                   -$$$$" << endl;
			cout << "$$$$-    then the tensor will be calculated automatically   -$$$$" << endl;
			cout << "$$$$-    with the three lists in the directory              -$$$$" << endl;
			cout << "$$$$-   \"input/listingchargeable/\"                          -$$$$"<< endl;
			cout << "$$$$-                                                       -$$$$" << endl;
			cout << "$$$$-  # if you want to calculate the tensor without        -$$$$" << endl;
			cout << "$$$$-    loading the lists, you can click as many similar   -$$$$" << endl;
			cout << "$$$$-    points that you want on the three images           -$$$$" << endl;
			cout << "$$$$-    (minimum 7 correspondences in order to calculate   -$$$$" << endl;
			cout << "$$$$-    the tensor)                                        -$$$$" << endl;
			cout << "$$$$-    and when you have finished, push \"f\"               -$$$$" << endl;
			cout << "$$$$-                                                       -$$$$" << endl;
			cout << "$$$$- <> When the tensor is calculated, you can access to   -$$$$" << endl;
			cout << "$$$$- the transfer mode. You can click two similar points   -$$$$" << endl;
			cout << "$$$$- on two images (1&2,1&3,2&3), then transfer is made    -$$$$" << endl;
			cout << "$$$$- and the third point appears on the image that you     -$$$$" << endl;
			cout << "$$$$- haven't clicked.                                      -$$$$" << endl;
			cout << "$$$$-                                                       -$$$$" << endl;
			cout << "$$$$-           THANKS FOR USING TRIFOCAL-TENSOR            -$$$$" << endl;
			cout << "$$$$-                                                       -$$$$" << endl;
			cout << "$$$$-                                   by Guillaume SEGADO -$$$$" << endl;
			cout << "$$$$-                                  and Oceane RENNESSON -$$$$" << endl;
			cout << "$$$$-                                                       -$$$$" << endl;
			cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
		}
	}
		
	Uint16 x1,y1,x2,y2,x3,y3;//Points rentrés en cliquant
	int comptimg1 = 0; // compteur de points cliqués sur l'image 1
	int comptimg2 = 0; // compteur de points cliqués sur l'image 2
	int comptimg3 = 0; // compteur de points cliqués sur l'image 3
	bool finiClic = false;
	bool modeTransfertActif = false;
	bool modeChargementActif = false;
	bool pointTransfertChoisiSurImage1 = false;
	bool pointTransfertChoisiSurImage2 = false;
	bool pointTransfertChoisiSurImage3 = false;
	
	//Points du transfert
	Eigen::VectorXd pointimg1 = Eigen::VectorXd::Zero(3);
	Eigen::VectorXd pointimg2 = Eigen::VectorXd::Zero(3);
	Eigen::VectorXd pointimg3 = Eigen::VectorXd::Zero(3);
	
	//Création du Tensor :
	Tensor tensor;
	
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
		cerr << "Impossible d'ouvrir la liste 3 !" << endl;
	}
	
	
	// load the point lists, listes de points pour le chargement
	Eigen::MatrixXd list1;
	Eigen::MatrixXd list2;
	Eigen::MatrixXd list3;
	
	
	// Creation des matrices de points cliqués
	Eigen::MatrixXd listClique1;
	Eigen::MatrixXd listClique2;
	Eigen::MatrixXd listClique3;

	// save a list
	//kn::saveMatrix(list1,"/tmp/myList.mat");

	// some colors
	Uint32 red  = 0xffff0000;
	Uint32 blue = 0xff0000ff;
	Uint32 yellow = 0xffffff00;
	
	
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
					if(e.button.x < image1->w) {
						comptimg1++;
						x1 = e.button.x;
						y1 = e.button.y;
						if(modeTransfertActif) {
							pointimg1[0] = x1;
							pointimg1[1] = y1;
							pointimg1[2] = 1.0;
							std::cout << pointimg1[0] <<"-"<< pointimg1[1] <<" img1"<< std::endl;
							pointTransfertChoisiSurImage1 = true;
						}
						std::cout << e.button.x <<"-"<< e.button.y <<" img1"<< std::endl;
						
						// On ouvre le fichier listing de points 1 pour pouvoir le remplir.
						ofstream listing1("./input/listingcliquable/listing1.list", ios::out | ios::app);
						if(listing1) {
							listing1 << setprecision(4)<< x1 <<" "<< y1 <<" "<<"1"<< endl;
							listing1.close();
						} 	
						else {
						cerr << "Impossible d'ouvrir le listing 1!" << endl;
						}
						fill_circle(screen, x1, y1, 3, red);
					}
					if(e.button.x > (image1->w - 1) && e.button.x < (image1->w+image2->w)) {
						comptimg2++;
						x2 = e.button.x - 400;
						y2 = e.button.y;
						if(modeTransfertActif) {
							pointimg2[0] = x2;
							pointimg2[1] = y2;
							pointimg2[2] = 1.0;
							std::cout << pointimg2[0] <<"-"<< pointimg2[1] <<" img2"<< std::endl;
							pointTransfertChoisiSurImage2 = true;
						}
						std::cout << e.button.x <<"-"<< e.button.y <<" img2"<< std::endl;
						
						// On ouvre le fichier listing de points 3 pour pouvoir le remplir.
						ofstream listing2("./input/listingcliquable/listing2.list", ios::out | ios::app);
						if(listing2) {
							listing2 << setprecision(4)<< x2 <<" "<< y2 <<" "<<"1"<< endl;
							listing2.close();
						} 	
						else {
						cerr << "Impossible d'ouvrir le listing 2 !" << endl;
						}
						fill_circle(screen, x2+image1->w, y2, 3, blue);
					}
					if(e.button.x > (image1->w + image2->w - 1)) {
						comptimg3++;
						x3 = e.button.x - 800;
						y3 = e.button.y;
						if(modeTransfertActif) {
							pointimg3[0] = x3;
							pointimg3[1] = y3;
							pointimg3[2] = 1.0;
							std::cout << pointimg3[0] <<"-"<< pointimg3[1] <<" img3"<< std::endl;
							pointTransfertChoisiSurImage3 = true;
						}
						std::cout << e.button.x <<"-"<< e.button.y <<" img3"<< std::endl;
						
						// On ouvre le fichier listing de points 3 pour pouvoir le remplir.
						ofstream listing3("./input/listingcliquable/listing3.list", ios::out | ios::app);
						if(listing3) {
							listing3 << setprecision (4)<< x3 <<" "<< y3 <<" "<<"1"<< endl;
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
			if(e.type == SDL_KEYDOWN) {
				if(e.key.keysym.sym == SDLK_f) {
					if(comptimg1 < 7 || comptimg2 < 7 || comptimg3 < 7)
					 {
					 	std::cout << "Vous n'avez pas fini de rentrer les correspondances, continuez s'il vous plait " << std::endl;
					 	std::cout << "(mininum 7 correspondances)" << std::endl;
					 }
					else{finiClic = true;}
				}
				if(e.key.keysym.sym == SDLK_c) {
					modeChargementActif = true;   //Pour lancer le chargement
				}
			}


			// Traitement des autres évenements:
		}
		
		if(finiClic) {
			// charge la liste de points dans les matrix
			kn::loadMatrix(listClique1,"input/listingcliquable/listing1.list");
			kn::loadMatrix(listClique2,"input/listingcliquable/listing2.list");
			kn::loadMatrix(listClique3,"input/listingcliquable/listing3.list");
			
			
			//Sauvegarde des listes
			ofstream listing1("./input/listingcliquable/listing1.list", ios::out | ios::trunc);
			if(listing1) {
				listing1 <<"row "<<comptimg1<<std::endl;
				listing1 <<"col 3"<<std::endl;
				listing1 <<" "<<std::endl;
				listing1 << listClique1 << std::endl;
				listing1.close();
			} 	
			else {
				cerr << "Impossible d'ouvrir la liste 1 !" << endl;
			}
			ofstream listing2("./input/listingcliquable/listing2.list", ios::out | ios::trunc);
			if(listing2) {
				listing2 <<"row "<<comptimg2<<std::endl;
				listing2 <<"col 3"<<std::endl;
				listing2 <<" "<<std::endl;
				listing2 << listClique2 << std::endl;
				listing2.close();
			} 	
			else {
				cerr << "Impossible d'ouvrir la liste 2 !" << endl;
			}
			ofstream listing3("./input/listingcliquable/listing3.list", ios::out | ios::trunc);
			if(listing3) {
				listing3 <<"row "<<comptimg3<<std::endl;
				listing3 <<"col 3"<<std::endl;
				listing3 <<" "<<std::endl;
				listing3 << listClique3 << std::endl;
				listing3.close();
			} 	
			else {
				cerr << "Impossible d'ouvrir la liste 3 !" << endl;
			}
			
			imageOffset.x = 0;
			imageOffset.y = 0;
			SDL_BlitSurface(image1, NULL, screen, &imageOffset);
			imageOffset.x = image1->w;
			SDL_BlitSurface(image2, NULL, screen, &imageOffset);
			imageOffset.x = image1->w + image2->w;
			SDL_BlitSurface(image3, NULL, screen, &imageOffset);
			
			std::cout <<"Points de l'image 1 " << std::endl;
			std::cout << listClique1 << std::endl;
			std::cout <<"Points de l'image 2 " << std::endl;
			std::cout << listClique2 << std::endl;
			std::cout <<"Points de l'image 3 " << std::endl;
			std::cout << listClique3 << std::endl;
			
			
			MatrixXd A = MatrixXd::Zero(listClique1.rows()*4, 27);
			
			fillingA(A, listClique1, listClique2, listClique3);
			
			solvingAt(A, tensor);
			modeTransfertActif = true;
			
			
			finiClic = false;
		}
		
		if(modeChargementActif) {
			kn::loadMatrix(list1,"input/listingchargeable/list1.list");
			kn::loadMatrix(list2,"input/listingchargeable/list2.list");
			kn::loadMatrix(list3,"input/listingchargeable/list3.list");
			
			// draw points on image1
			for(int i=0; i<list1.rows(); ++i)
				fill_circle(screen, list1(i,0), list1(i,1), 3, red);

			// draw points on image2
			for(int i=0; i<list2.rows(); ++i)
				fill_circle(screen, list2(i,0)+image1->w, list2(i,1), 3, blue);

			// draw points on image3
			for(int i=0; i<list3.rows(); ++i)
				fill_circle(screen, list3(i,0)+image1->w+image2->w, list3(i,1), 3, yellow);
			
			std::cout << "........Chargement des points effectué" <<std::endl;
			SDL_Flip(screen);
			
			std::cout <<"Points de l'image 1 " << std::endl;
			std::cout << list1 << std::endl;
			std::cout <<"Points de l'image 2 " << std::endl;
			std::cout << list2 << std::endl;
			std::cout <<"Points de l'image 3 " << std::endl;
			std::cout << list3 << std::endl;
			
			
			MatrixXd A = MatrixXd::Zero(list1.rows()*4, 27);
			
			fillingA(A, list1, list2, list3);
			
			solvingAt(A, tensor);
			
			SDL_Delay(4000); // Une pause pour laisser afficher les points chargés avant la phase de transfert
			
			imageOffset.x = 0;
			imageOffset.y = 0;
			SDL_BlitSurface(image1, NULL, screen, &imageOffset);
			imageOffset.x = image1->w;
			SDL_BlitSurface(image2, NULL, screen, &imageOffset);
			imageOffset.x = image1->w + image2->w;
			SDL_BlitSurface(image3, NULL, screen, &imageOffset);
			
			modeChargementActif = false;
			modeTransfertActif = true;
		}
		
		if(modeTransfertActif) {
		
				if(pointTransfertChoisiSurImage1 && pointTransfertChoisiSurImage2) {
					std::cout << "Mode de Transfert choisi : Retrouver la position sur l'image 3" << std::endl;
					pointimg3 = transfert(tensor, pointimg1, pointimg2, 3);
					
					draw_circle(screen, pointimg3[0]+image1->w+image2->w, pointimg3[1], 3, yellow);
					pointTransfertChoisiSurImage1 = false;
					pointTransfertChoisiSurImage2 = false;
				}
				if(pointTransfertChoisiSurImage1 && pointTransfertChoisiSurImage3) {
					std::cout << "Mode de Transfert choisi : Retrouver la position sur l'image 2" << std::endl;
					pointimg2 = transfert(tensor, pointimg1, pointimg3, 2);

					draw_circle(screen, pointimg2[0]+image1->w, pointimg2[1], 3, blue);
					pointTransfertChoisiSurImage1 = false;
					pointTransfertChoisiSurImage3 = false;
				}
				if(pointTransfertChoisiSurImage2 && pointTransfertChoisiSurImage3) {
					std::cout << "Mode de Transfert choisi : Retrouver la position sur l'image 1" << std::endl;
					pointimg1 = transfert(tensor, pointimg2, pointimg3, 1);
					
					draw_circle(screen, pointimg1[0], pointimg1[1], 3, red);
					pointTransfertChoisiSurImage2 = false;
					pointTransfertChoisiSurImage3 = false;
				}
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



