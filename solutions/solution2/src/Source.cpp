/*
Detect shape
*/
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include <boost/program_options.hpp>


namespace opt = boost::program_options;


static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}



void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour, std::vector<int> &datafile)
{
	int fontface = CV_FONT_HERSHEY_COMPLEX;
	double scale = 0.5;
	int thickness = 0.9;
	int baseline = 0;

	//set text appearance
	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	//select contour
	cv::Rect r = cv::boundingRect(contour);


	//rectangle
	rectangle(im, cv::boundingRect(contour), cv::Scalar(0, 0, 255), 2, 8, 0);

	//where is the center of traffic sign
	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	//white text background
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255, 255, 255), CV_FILLED);
	//write text
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);

	//top left and down right corner
	datafile.push_back(r.x);
	datafile.push_back(r.y);
	datafile.push_back(r.x + r.width);
	datafile.push_back(r.y + r.height);
}


int main(int argc, char **argv)
{
	std::ios::sync_with_stdio(false);


	std::string filename;
	std::string param, param2="20";


	opt::options_description desc("Program Weroniki Hryniewskiej");
	desc.add_options()
		("help", "wyswietlenie pomocy")
		("param", opt::value<std::string>(&param)->required(), "parametr")
		("solution", opt::value<std::string>(&filename)->required(), "nazwa folderu ze zdjeciami np. ../../../environment/data/test/images")
		("param2", opt::value<std::string>(&param2), "parametr2")
		;

	opt::variables_map vm;
	opt::store(opt::parse_command_line(argc, argv, desc), vm);


	if (argc == 1 /*|| vm.count("help")*/)
	{
		std::cout << desc;
	//filename = "../../../environment/data/test/images";
	return 1;
	}

	try {
		opt::notify(vm);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << '\n';
		std::cerr << "Uzycie: " << argv[0] << " --help\n";
		return 2;
	}


	filename += "/*.jpg";

	

	std::vector<cv::String> fn;
	//std::string filename= "D://Pulpit//OI//Set1Part0//*.jpg";
	glob(filename, fn, false);


	//std::vector<cv::Mat> images;
	//images.reserve(1000);
	//number of jpg files in images folder
	size_t count = fn.size();
	//push images from folder to an image vector
	

	
		std::fstream plik;
		
		plik.open("../solution_result.txt", std::ios::out);
		int photonumber = 0;

		for (size_t i = 0; i < count; i++)
		{
			//images.push_back(imread(fn[i]));
			cv::Mat images = imread(fn[i]);




			std::vector<int> datafile;
			cv::Mat src;
			std::string photoadress;

			while (photonumber < count) {

				photoadress = fn[photonumber];
				src.empty();
				src = cv::imread(photoadress);

				std::string name = "";
				int  k = 0;


				int whereslash = 0;

				for (k; k <= photoadress.size(); ++k) {
					if (photoadress[k] == '/' || photoadress[k] == '\\')
						whereslash = k;
				}

				k = whereslash;
				for (++k; k < photoadress.size(); ++k) {
					name += photoadress[k];
				}



				plik << name << ":";

				++photonumber;
				cv::Mat gray;
				cv::Mat blackwhite;
				cv::Mat dst;
				std::vector<std::vector<cv::Point> > contours;
				std::vector<cv::Point> approx;


				// Convert to grayscale
				cv::cvtColor(src, gray, CV_BGR2GRAY);
				blur(gray, gray, cv::Size(3, 3));
				threshold(gray, gray, 40, 40, 3);

				cv::Canny(gray, blackwhite, std::stoi(param2), std::stoi(param), 3);
				//cv::imshow("dst", blackwhite);
				//cv::waitKey(0);

				// Find contours
				cv::findContours(blackwhite.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

				src.copyTo(dst);

				for (int i = 0; i < contours.size(); i++)
				{
					// Approximate contour with accuracy proportional to the contour perimeter
					cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.01, true);

					// Skip small or non-convex objects
					if (std::fabs(cv::contourArea(contours[i])) < 200 || !cv::isContourConvex(approx))
						continue;

					//triangle
					if (approx.size() == 3)
					{
						setLabel(dst, "ostrzegawczy", contours[i], datafile);
					}
					//rectangle
					else if (approx.size() >= 4 && approx.size() <= 6)
					{
						// number of vertices of polygonal curve
						int vtc = approx.size();

						// get the cosines of corners
						std::vector<double> cos;
						for (int j = 2; j < vtc + 1; j++)
							cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));

						// sort ascending
						std::sort(cos.begin(), cos.end());

						// Get the lowest and the highest cosine
						double mincos = cos.front();
						double maxcos = cos.back();
						cos.clear();

						// use the degrees obtained above and the number of vertices
						if (vtc == 4)
							setLabel(dst, "informacyjny", contours[i], datafile);
					}
					else
					{
						// circle
						double area = cv::contourArea(contours[i]);
						cv::Rect r = cv::boundingRect(contours[i]);
						int radius = r.width / 2;

						if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 && std::abs(1 - (area / (CV_PI * (radius*radius)))) <= 0.2)
							setLabel(dst, "zakazu/nakazu", contours[i], datafile);
					}

					//	if (datafile.begin() != datafile.end()) {

					auto it = datafile.begin();
					while (it != datafile.end()) {
						plik << *(it) << ",";
						++it;
						plik << *(it) << ",";
						++it;
						plik << *(it) << ",";
						++it;
						plik << *(it);
						if ((it) != datafile.end()) {
							plik << ";";
							++it;
						}
					}

					datafile.clear();
					approx.clear();
				

				}
				contours.clear();
				plik << std::endl;
				//cv::imshow("dst", dst);

				//cv::waitKey(0);
				//imwrite(name, dst);
			}
		}
		plik.close();
	return 0;
}
