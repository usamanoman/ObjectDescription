#include <string>
#include <iostream>
#include <opencv\highgui.h>
#include <cmath>
#include <iostream>  
#include <fstream>  
#include<vector>
#include <sstream>
#include <string>
#include<istream>


using namespace cv;
using namespace std;


void preparing_contours(Mat img,int interation);

Rect box;
Mat src;
int iteration;
string resultant;
int serial=0;
string complete_info;
stringstream ss;
vector<string> extra;
int red_avg=0;
int green_avg=0;
int blue_avg=0;
int result_value=0;

static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}



void setLabel(Mat& im, const std::string label, std::vector<Point>& contour)
{
	ofstream myfile;
	myfile.open ("example.bin", ios::out  || ios::app);

	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	Size text = getTextSize(label, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);
	if((label == "RECT" || label=="CIR" || label=="TRI" ) ){
		Vec3b pix=src.at<Vec3b>(((r.width) / 2)+r.x,(r.y+5));
		int B=pix.val[0];
		int G=pix.val[1];
		int R=pix.val[2];
		ss<< R  << " " << " " <<G  <<" " << B  << " " << r.width << " " << r.height << " "<<r.x << " "<< r.y << endl;
		extra.push_back(label);
		box.width = r.width-5;
		box.height = r.height-5;
		box.x = r.x+5;
		box.y = r.y+5;
		Mat crop(src, box);
		src=crop;
		iteration++;
		preparing_contours(src,iteration);
		
	}

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(im, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
	putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}



void preparing_contours(Mat img,int iteration)
{
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);

	// Use Canny instead of threshold to catch squares with gradient shading
	Mat bw;
	Canny(gray, bw, 0, 50, 5);

	// Find contours
	std::vector<std::vector<Point> > contours;
	findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	std::vector<Point> approx;
	Mat dst = src.clone();



	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects 
		if (std::fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
			continue;

		if (approx.size() == 3)
		{
			setLabel(dst, "TRI", contours[i]);    // Triangles
		}
		else if (approx.size() >= 4 && approx.size() <= 6)
		{
			// Number of vertices of polygonal curve
			int vtc = approx.size();

			// Get the cosines of all corners
			std::vector<double> cos;
			for (int j = 2; j < vtc+1; j++)
				cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

			// Sort ascending the cosine values
			std::sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Use the degrees obtained above and the number of vertices
			// to determine the shape of the contour
			if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
				setLabel(dst, "RECT", contours[i]);
			else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
				setLabel(dst, "PENTA", contours[i]);
			else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
				setLabel(dst, "HEXA", contours[i]);
		}
		else
		{
			// Detect and label circles
			double area = contourArea(contours[i]);
			Rect r = boundingRect(contours[i]);
			int radius = r.width / 2;

			if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
			    std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
				setLabel(dst, "CIR", contours[i]);
		}
	}

	string scr_number= to_string( iteration);
	imshow(scr_number, dst);
	


}

string making_filename(int number)
{
	string name_of_file;
	for(int u=0;u<number;u++)
	{
		name_of_file+=extra.at(u);
	}
	return name_of_file;
}

void color_in_range(vector<int> tar_red,vector<int> tar_green,vector<int> tar_blue,vector<int> source_red,vector<int> source_green,vector<int> source_blue)
{
	vector<int> temporary_red_mean_calculator;
	vector<int> temporary_green_mean_calculator;
	vector<int> temporary_blue_mean_calculator;
	int number;
	for(int i=0;i<tar_red.size();i++)
	{
		number=abs(tar_red.at(i) - source_red.at(i));
		temporary_red_mean_calculator.push_back(number);
		number=abs(tar_green.at(i) - source_green.at(i));
		temporary_green_mean_calculator.push_back(number);
		number=abs(tar_blue.at(i) - source_blue.at(i));
		temporary_blue_mean_calculator.push_back(number);

	}
	red_avg=0;
	green_avg=0;
	blue_avg=0;

	for(int i=0;i<tar_red.size();i++)
	{
		red_avg=red_avg + temporary_red_mean_calculator.at(i);
		green_avg=green_avg + temporary_green_mean_calculator.at(i);
		blue_avg=blue_avg + temporary_blue_mean_calculator.at(i);
			
	}

		red_avg=red_avg / tar_red.size() ;

		green_avg =green_avg  / tar_red.size() ;
		blue_avg=blue_avg / tar_red.size() ;
		result_value= (blue_avg + red_avg + blue_avg ) / 3 ;
}


string inference_model(int size,string name_of_file)
{
		ifstream the_data("intermadiate");
		string line;
		string object_details;
		vector<int> red;
		vector<int> green;
		vector<int> blue;
		vector<int> height;
		vector<int> width;
		vector<int> x_position;
		vector<int> y_position;
		
		vector<int> target_red;
		vector<int> target_green;
		vector<int> target_blue;
		vector<int> target_height;
		vector<int> target_width;
		vector<int> target_x_position;
		vector<int> target_y_position;
		

		int number;
		for(int i=0;i<size;i++)
		{
			the_data>>number;
			red.push_back(number);
			the_data>>number;
			green.push_back(number);
			the_data>>number;
			blue.push_back(number);
			the_data>>number;
			height.push_back(number);
			the_data>>number;
			width.push_back(number);
			the_data>>number;
			x_position.push_back(number);
			the_data>>number;
			y_position.push_back(number);

		}


		for(int i=0;i<size;i++)
		{
			cout<<red.at(i)<<" "<<green.at(i)<<" "<<blue.at(i)<<" "<<height.at(i)<<" "<<width.at(i)<< " "<<x_position.at(i)<<" "<<y_position.at(i)<<endl;

		}

		

		ifstream record(name_of_file.c_str());
		string skipper;
		int  max=255;
		while(!record.eof()){
			for(int i=0;i<size;i++)
			{
				record>>number;
				target_red.push_back(number);
				record>>number;
				target_green.push_back(number);
				record>>number;
				target_blue.push_back(number);
				record>>number;
				target_height.push_back(number);
				record>>number;
				target_width.push_back(number);
				record>>number;
				target_x_position.push_back(number);
				record>>number;
				target_y_position.push_back(number);
			
			}
			
			//getchar();	
			
			getline(record,skipper);
			getline(record,skipper);
			/*for(int i=0;i<size;i++)
			{
			
				cout<<target_red.at(i)<<" "<<target_green.at(i)<<" "<<target_blue.at(i)<<" "<<target_height.at(i)<<" "<<target_width.at(i)<< " "<<target_x_position.at(i)<<" "<<target_y_position.at(i)<<endl;
			}*/
			

			color_in_range(target_red,target_green,target_blue,red,green,blue);
			if(result_value < max){
				max = result_value;
				resultant=skipper;
				//cout<<result_value << " " <<resultant << endl;
			}
			getline(record,skipper);

			//cout<<skipper<<endl;
			
			target_red.clear(); target_green.clear(); target_width.clear(); target_blue.clear(); target_height.clear(); target_y_position.clear(); target_x_position.clear();

		}
		record.close();
		
		
		//resultant="RESULT";
		return resultant;
}

int main()
{
	//Mat src = imread("polygon.png");
	src = imread("CCTest7.jpg");
	if (src.empty())
		return -1;
	char reward;
	
	
    

	preparing_contours(src,iteration);
	
	complete_info= ss.str();
	string name_of_file=making_filename(extra.size());
	
	ofstream the_intermadiate_data("intermadiate");
	the_intermadiate_data<<complete_info;
	the_intermadiate_data<<"+"<<endl;
	the_intermadiate_data.close();

	resultant=inference_model(extra.size(),name_of_file);
	cout<<"Result: "<<resultant<<endl;
	cout<<"Is this result correct? : Yes == y || No == n :"<<endl;
	reward=getchar();
	if(reward=='n')
	{
		cout<<"Please tell what is it?: ";
		cin>>resultant;
	}

/*
	for(int u=0;u<extra.size();u++){
		resultant=inference_model(extra.at(u),);
	}*/

	
	ofstream the_data(name_of_file.c_str(),ios::app);
	the_data<<complete_info;
	the_data<<resultant<<endl;
	the_data<<"+"<<endl;

	
	waitKey(0);
	return 0;
}

