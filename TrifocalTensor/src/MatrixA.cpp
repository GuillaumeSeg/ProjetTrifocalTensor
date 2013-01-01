#include <iostream>
#include <vector>

#include "../Eigen/Dense"
#include "../include/MatrixA.hpp"

void fillingA(Eigen::MatrixXd &A, Eigen::MatrixXd &matrixIMG1, Eigen::MatrixXd &matrixIMG2, Eigen::MatrixXd &matrixIMG3) {

	
	if(matrixIMG1.rows() != matrixIMG2.rows()) 
	{
		std::cout << matrixIMG1.rows() <<" "<< matrixIMG2.rows() <<" "<< matrixIMG3.rows()<< std::endl;
		std::cerr << "Erreur nombre de correspondances par image incompatibles" <<std::endl;
		exit(0);
	}
	if(matrixIMG1.rows() != matrixIMG3.rows()) 
	{
		std::cout << matrixIMG1.rows() <<" "<< matrixIMG2.rows() <<" "<< matrixIMG3.rows()<< std::endl;
		std::cerr << "Erreur nombre de correspondances par image incompatibles" <<std::endl;
		exit(0);
	}
	if(matrixIMG2.rows() != matrixIMG3.rows()) 
	{
		std::cout << matrixIMG1.rows() <<" "<< matrixIMG2.rows() <<" "<< matrixIMG3.rows()<< std::endl;
		std::cerr << "Erreur nombre de correspondances par image incompatibles" <<std::endl;
		exit(0);
	}
	
	int nbrCorrespondance = matrixIMG1.rows();
	
	for(int p = 0; p < nbrCorrespondance; ++p) {
		for(int k = 0; k < 3; ++k) {
			A(0 + 4*p, 0 + 9*k) -= matrixIMG1(p, k)*matrixIMG2(p, 2)*matrixIMG3(p, 2);
			A(2 + 4*p, 1 + 9*k) -= matrixIMG1(p, k)*matrixIMG2(p, 2)*matrixIMG3(p, 2);
			A(0 + 4*p, 2 + 9*k) += matrixIMG1(p, k)*matrixIMG2(p, 2)*matrixIMG3(p, 0);
			A(2 + 4*p, 2 + 9*k) += matrixIMG1(p, k)*matrixIMG2(p, 2)*matrixIMG3(p, 1);
			A(1 + 4*p, 3 + 9*k) -= matrixIMG1(p, k)*matrixIMG2(p, 2)*matrixIMG3(p, 2);
			A(3 + 4*p, 4 + 9*k) -= matrixIMG1(p, k)*matrixIMG2(p, 2)*matrixIMG3(p, 2);
			A(1 + 4*p, 5 + 9*k) += matrixIMG1(p, k)*matrixIMG2(p, 2)*matrixIMG3(p, 0);
			A(3 + 4*p, 5 + 9*k) += matrixIMG1(p, k)*matrixIMG2(p, 2)*matrixIMG3(p, 1);
			A(0 + 4*p, 6 + 9*k) += matrixIMG1(p, k)*matrixIMG2(p, 0)*matrixIMG3(p, 2);
			A(1 + 4*p, 6 + 9*k) += matrixIMG1(p, k)*matrixIMG2(p, 1)*matrixIMG3(p, 2);
			A(2 + 4*p, 7 + 9*k) += matrixIMG1(p, k)*matrixIMG2(p, 0)*matrixIMG3(p, 2);
			A(3 + 4*p, 7 + 9*k) += matrixIMG1(p, k)*matrixIMG2(p, 1)*matrixIMG3(p, 2);
			A(0 + 4*p, 8 + 9*k) -= matrixIMG1(p, k)*matrixIMG2(p, 0)*matrixIMG3(p, 0);
			A(1 + 4*p, 8 + 9*k) -= matrixIMG1(p, k)*matrixIMG2(p, 1)*matrixIMG3(p, 0);
			A(2 + 4*p, 8 + 9*k) -= matrixIMG1(p, k)*matrixIMG2(p, 0)*matrixIMG3(p, 1);
			A(3 + 4*p, 8 + 9*k) -= matrixIMG1(p, k)*matrixIMG2(p, 1)*matrixIMG3(p, 1);
		}
	}
	
	//std::cout << A << std::endl;
}

void solvingAt(Eigen::MatrixXd &A, Tensor &t) {
	using namespace Eigen;
	
	VectorXd b = VectorXd::Zero(A.rows());
	
	JacobiSVD<MatrixXd> svd(A, ComputeThinU | ComputeThinV);
	
	/*std::cout << "Les valeurs singulieres sont triées par ordre decroissant:" << std::endl << svd.singularValues() << std::endl;
	std::cout << "Matrix V dernière colonne" << std::endl << svd.matrixV().col(26) << std::endl;*/
	
	VectorXd x = svd.matrixV().col(26);
	
	for(int i = 0; i<x.size(); ++i) {
		t.T[i] = x[i];
	}
}


