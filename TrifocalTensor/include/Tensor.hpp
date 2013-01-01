#ifndef TENSOR__HPP
#define TENSOR__HPP

#include <iostream>
#include <vector>

class Tensor {

	public:
		std::vector<double> T;
		
	public:
		Tensor() {
			T.reserve(27);
			for(int i = 0; i < 27; ++i) {
				T.push_back(0);
			}
		}
		
		double operator()(int i, int j, int k) const;
};

#endif
