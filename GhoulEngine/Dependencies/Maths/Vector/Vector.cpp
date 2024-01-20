#include "Vector2D.h"
#include <iostream>


int main() {

	//Construction testing...
	Vector2 myVec(0, 0);
	std::cout << "MyVec at first: " << myVec << std::endl;
	myVec += Vector2(2, 3);
	std::cout << "MyVec after change: " << myVec << std::endl;
	Vector2 secVec = myVec;
	std::cout << "SecVec at first: " << secVec << ", " << secVec.Length() << std::endl;
	

	//Construction and operator + testing...
	Vector2 thiVec = myVec + secVec;
	std::cout << "ThiVec at first: " << thiVec << ", " << thiVec.Length() << std::endl;
	std::cout << "MyVec after addition: " << myVec << ", and SecVec after addition: " << secVec << std::endl;
	myVec += secVec;
	std::cout << "MyVec after += SecVec: " << myVec << ", and SecVec after: " << secVec << std::endl;
	Vector2 fourthVec(myVec + secVec);
	std::cout << "FourthVec at first: " << fourthVec << std::endl;
	std::cout << "MyVec after FourthVec: " << myVec << ", and SecVec after ForthVec: " << secVec << std::endl;

	//Scale and Normalize testing...
	myVec.ScaleDiv(2);
	std::cout << "MyVec after Scale: " << myVec << std::endl;
	auto myFifth = myVec.Normalized();
	std::cout << "MyVec as a normalized vec: " << myFifth << ", " << myFifth.Magnitude() << std::endl;
	myVec.Reset();
	std::cout << "MyVec after Reset: " << myVec << ", Mag: " << myVec.Magnitude() << std::endl;

	//Dot and Projection testing...
	thiVec.Set(-25, 50);
	thiVec.Normalize();
	float dir = thiVec.Dot(myFifth);
	std::cout << "The dot product between vec3 and vec5 (both norms): " << dir << std::endl;
	std::cout << std::endl;
	std::cout << "SecVec: " << secVec << std::endl;
	std::cout << "FourthVec: " << fourthVec << std::endl;
	float project = secVec.ScProjectThisOnto(fourthVec);
	std::cout << "The scalar projection of vec2 onto vec4: " << project << std::endl;
	std::cout << std::endl;
	std::cout << "SecVec: " << secVec << std::endl;
	std::cout << "FourthVec: " << fourthVec << std::endl;
	project = secVec.ScProjectOntoThis(fourthVec);
	std::cout << "The scalar projection of vec4 onto vec2: " << project << std::endl;

	std::cout << "\nPress any key to continue... ";
	std::cin.get();
}