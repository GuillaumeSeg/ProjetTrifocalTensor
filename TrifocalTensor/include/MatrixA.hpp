#ifndef MATRIXA__HPP
#define MATRIXA__HPP

#include "../Eigen/Dense"
#include "../include/Tensor.hpp"

void fillingA(Eigen::MatrixXd &A, Eigen::MatrixXd &matrixIMG1, Eigen::MatrixXd &matrixIMG2, Eigen::MatrixXd &matrixIMG3);

void solvingAt(Eigen::MatrixXd &A, Tensor &t);

#endif


