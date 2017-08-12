#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream> //split
#include <iomanip> //setw

//#include <boost/program_options.hpp>
//namespace opt = boost::program_options;
using namespace std;
typedef struct Point
{
	int x;
	int y;
};

vector<string> split(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

std::multimap<std::string, std::pair<Point, Point>> fileToMap(std::fstream &test) {
	std::multimap<std::string, std::pair<Point, Point>> testMap;

	std::string line;
	vector<string> imageName;
	while (std::getline(test, line)) {
		imageName = split(line, ':');
		if (imageName.size() > 1) {
			vector<string> semicolon = split(imageName[1], ';');
			for (int i = 0; i < semicolon.size(); ++i) {
				vector<string> comma = split(semicolon[i], ',');
				std::pair<Point, Point> points;

				points.first.x = atoi(comma[0].c_str());
				points.first.y = atoi(comma[1].c_str());
				points.second.x = atoi(comma[2].c_str());
				points.second.y = atoi(comma[3].c_str());

				if (points.first.x > points.second.x)
					swap(points.first.x, points.second.x);
				if (points.first.y > points.second.y)
					swap(points.first.y, points.second.y);

				testMap.insert(make_pair(imageName[0], points));
				comma.clear();
			}
			semicolon.clear();
		}
		else {
			std::pair<Point, Point> points;
			testMap.insert(make_pair(imageName[0], points));
		}
	}
	return testMap;
}


/*a1b1: Top Left coordinate of first rectangle.
a2b2 : Bottom Right coordinate of first rectangle.
c1d1 : Top Left coordinate of second rectangle.
c2d2 : Bottom Right coordinate of second rectangle.*/

bool isOverlap(Point a1b1, Point a2b2, Point c1d1, Point c2d2) {

	if (((c1d1.y < a1b1.y && a1b1.y < c2d2.y) || (c1d1.y < a2b2.y && a2b2.y < c2d2.y)) &&
		((c1d1.x < a1b1.x && a1b1.x < c2d2.x) || (c1d1.x < a2b2.x && a2b2.x < c2d2.x)))
		return true;

	if (((a1b1.y < c1d1.y && c1d1.y < a2b2.y) || (a1b1.y < c2d2.y && c2d2.y < a2b2.y)) &&
		((a1b1.x < c1d1.x && c1d1.x < a2b2.x) || (a1b1.x < c2d2.x && c2d2.x < a2b2.x)))
		return true;

	return false;
}



void compareMaps(std::multimap<std::string, std::pair<Point, Point>> &testMap, std::multimap<std::string, std::pair<Point, Point>> &solutionMap, int num) {
	int totalNumber = 0, overlaped = 0, overlapedZero = 0;
	std::string scorePath = "D:\\Pulpit\\oi\\wyniki_procentowe_" + to_string(num) + ".txt";
	std::fstream wyniki(scorePath, std::ios::out);

	std::string scorePath2 = "D:\\Pulpit\\oi\\wyniki_koncowe.txt";
	std::fstream wyniki2(scorePath2, std::ios::app);
	vector<int> percentCounter;
	for (int c = 0; c <= 5; ++c)
		percentCounter.push_back(0);

	if (!wyniki)
	{
		std::clog << "Nie mo¿na by³o otworzyæ pliku " << scorePath << '\n';
		return;
	}

	for (auto my : solutionMap) {
		for (auto baze : testMap) {

			if (my.first == baze.first) {

				if ((my.second.first.x && my.second.second.x && my.second.first.y && my.second.second.y) == 0 ||
					(baze.second.first.x && baze.second.second.x && baze.second.first.y && baze.second.second.y) == 0) {
					if (my.second.first.x == baze.second.first.x && my.second.second.x == baze.second.second.x &&
						my.second.first.y == baze.second.first.y && my.second.second.y == baze.second.second.y) {
						++overlapedZero;
					}
				}
				else
					if (isOverlap(my.second.first, my.second.second, baze.second.first, baze.second.second)) {
						++overlaped;


						int minX = fmin(my.second.first.x, baze.second.first.x);
						int minY = fmin(my.second.first.y, baze.second.first.y);
						int maxX = fmax(my.second.second.x, baze.second.second.x);
						int maxY = fmax(my.second.second.y, baze.second.second.y);

						Point dim0 = { -1,-1 }, dimMax = { -1,-1 };
						int areaOfOverlap = 0;
						int sizeMy = (my.second.second.x - my.second.first.x) * (my.second.second.y - my.second.first.y);
						int sizeBaze = (baze.second.second.x - baze.second.first.x) * (baze.second.second.y - baze.second.first.y);

						for (int j = minY; j <= maxY; ++j) {
							for (int i = minX; i <= maxX; ++i) {
								if (my.second.first.x <= i && i <= my.second.second.x && baze.second.first.x <= i && i <= baze.second.second.x &&
									my.second.first.y <= j && j <= my.second.second.y && baze.second.first.y <= j && j <= baze.second.second.y) {
									if (dim0.x == -1 && dim0.y == -1)
										dim0 = { i, j };
									else
										dimMax = { i,j };
								}
							}
						}

						if (dimMax.x != -1 && dimMax.y != -1)
							areaOfOverlap = (dimMax.x - dim0.x)*(dimMax.y - dim0.y);


						int areaOfUnion = sizeMy + sizeBaze - areaOfOverlap;


						int percent = 100 * areaOfOverlap / areaOfUnion;
						wyniki << std::setw(8) << areaOfOverlap << std::setw(5) << " / " << std::setw(8) << areaOfUnion << std::setw(5) << " = " << std::setw(8) << percent << "%" << endl;
						if (percent < 50)
							percentCounter[0] += 1;
						else if (percent < 60)
							percentCounter[1] += 1;
						else if (percent < 70)
							percentCounter[2] += 1;
						else if (percent < 80)
							percentCounter[3] += 1;
						else if (percent < 90)
							percentCounter[4] += 1;
						else if (percent <= 100)
							percentCounter[5] += 1;

					}
				++totalNumber;
			}
		}
	}
	wyniki2 << "wartosc parametru " << num << endl;
	wyniki2 << "+ nie bylo znaku " << overlapedZero << endl;
	wyniki2 << "+ rozpoznano znak " << overlaped << endl;
	wyniki2 << "liczba znakow w moim rozwiazaniu " << solutionMap.size() << endl;
	wyniki2 << "liczba znakow do rozpoznania " << testMap.size() << endl;
	wyniki2 << "wszystkich sprawdzen " << totalNumber << endl;

	wyniki2 << "do 50% " << percentCounter[0] << endl <<
		"<50-60) " << percentCounter[1] << endl <<
		"<60-70) " << percentCounter[2] << endl <<
		"<70-80) " << percentCounter[3] << endl <<
		"<80-90) " << percentCounter[4] << endl <<
		"<90-100> " << percentCounter[5] << endl << endl;
	wyniki.close();
	wyniki2.close();
}


int main() {

	std::string testPath = "../data/test/annotations/annotations.txt";


	//opt::options_description desc("Sprawdzanie rozwi¹zañ");
	//desc.add_options()
	//	("test,t", opt::value<std::string>(&testPath)->required(), "dane testowe (poprawne)")
	//	("solution,s", opt::value<std::string>(&solutionPath)->required(), "katalog z danymi")
	//	;


	std::fstream test(testPath, std::ios::in);
	if (!test)
	{
		std::clog << "Nie mo¿na by³o otworzyæ pliku " << testPath << '\n';
		return 1;
	}

	std::multimap<std::string, std::pair<Point, Point>> testMap;
	testMap = fileToMap(test);

	int num = 10;
	//for (int num = 20; num <= 100; num += 10) {
	std::string solutionPath = "../../solutions/solution2/solution_result.txt";

	std::fstream solution(solutionPath, std::ios::in);
	if (!solution)
	{
		std::clog << "Nie mo¿na by³o otworzyæ pliku " << solutionPath << '\n';
		return 1;
	}

	std::multimap<std::string, std::pair<Point, Point>> solutionMap;


	solutionMap = fileToMap(solution);

	test.close();
	solution.close();


	compareMaps(testMap, solutionMap, num);
	solutionMap.clear();
	//}

	testMap.clear();
	return 0;
}