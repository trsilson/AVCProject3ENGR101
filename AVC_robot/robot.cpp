#include "robot.hpp"
#include <vector>



class ScanLine {
  public:
  ScanLine(std::string lineType, int position);

  double getError();
  bool checkWhite();
  void update(ImagePPM image);
  private:

  double error;
  std::string lineType;
  int position;
  bool containsWhite = false;
  

};

ScanLine::ScanLine(std::string lineType, int position){
  if (lineType == "row"){
    this->lineType = "col";
  }
  if (lineType == "col"){
    this->lineType = "row";
  }
  this->position = position;
}

void ScanLine::update(ImagePPM image){
  //This will hold
  std::vector<int> pixelList;

  int numOfPixels;
  if (lineType == "col"){
    int numOfPixels = image.height;
  }
  else if (lineType == "row"){
    int numOfPixels = image.width;
  }


  int whiteCount = 0;
  this->containsWhite = false;


  for (int iPixel = 0; iPixel < numOfPixels; iPixel++){
    int pixelLum;


    if (lineType == "row"){
      pixelLum = get_pixel(image, this->position, iPixel, 3);
    }
    if (lineType == "col"){
      pixelLum = get_pixel(image, iPixel, this->position, 3);
    }

  

  if (pixelLum > 250){
    pixelList.push_back(1);
    this->containsWhite = true;
    whiteCount++;
  }
  else{
    pixelList.push_back(0);
  }

  }

  this->error = 0;
  
  
  for (int i = 0; i < pixelList.size(); i++){
    if (pixelList.at(i) == 1){
      this-> error = this-> error + (i - (numOfPixels / 2));
      whiteCount ++;
    }
  }
  std::cout<<"Current error is " << this->error <<std::endl;
  std::cout<<"We have white " << this->containsWhite << std::endl;
  std::cout<<"pixelList array size" << pixelList.size() << std::endl;
  std::cout<<"White count" << whiteCount << std::endl;
}

double ScanLine::getError(){
  
}



/* variables */
double vBaseLine = 10; // crusing speed if no errors
double vLeft = vBaseLine, vRight = vBaseLine; // left and right speed initialised to cruising speed

/* function to calculate error */
double calculateError(){ 
  OpenPPMFile("cameraShot.png", cameraView);
  SavePPMFile("currPic.ppm", cameraView);

  int collectPix[cameraView.width]; // create an array with a size of cameraView.width
    
  for(int i = 0; i < cameraView.height; i++) { // scanning rows
    for(int j = 0; j < cameraView.width; j++) { // scanning columns
      /* calculate pixel luminosity */
      int lum = get_pixel(cameraView, i, j, 3); 
      /* if luminosity is > 250 then it's most likely a white pixel is found */
      if(lum > 250) {
      collectPix[j] = 1; // so add 1 to the array
      } else {
      collectPix[j] = 0; // or else add 0 to the array
      }
    }
  }

  double error = 0; 
  for(int i = 0; i < cameraView.width; i++) {
    if(collectPix[i] == 1) {
      error = error + (i - (cameraView.width / 2)); // calculate error
    }
  }
    
  return error; // returning the calculated error
}

/* method to move robot using the calculate error function */
void moveRobot() {
  double marginOfError = calculateError(); // setting the calculate error to the variable marginOfError
  double kP = 0.03; // constant for difference in left and right speed - dv

  /* if white pixels are on the right, increase right speed so robot moves to the left */
  if (marginOfError > 0) { 
    vLeft = vBaseLine;
    vRight = vBaseLine - marginOfError * kP; 
  /* if white pixels are on the left, increase left motor so robot moves to the right */
  } else if (marginOfError < 0) {
    vLeft = vBaseLine  + marginOfError * kP;
    vRight = vBaseLine; 
  }  
  /* else have a steady speed */
  else {
    vLeft = vBaseLine + marginOfError * kP;
    vRight = vBaseLine - marginOfError * kP; 
  }
}

int main(){
	if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}

 

  while(1){
    OpenPPMFile("cameraShot.png", cameraView);
    SavePPMFile("i0.ppm",cameraView);



    moveRobot(); // calling the moveRobot method function in order to move the robot

    setMotors(vLeft,vRight);   
    std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
    usleep(10000);
  } //while

} // main