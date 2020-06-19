#include "robot.hpp"
#include <vector>
#include <fstream>



//CALCULATE ERROR: This function scans a row of pixels at rowNum height and calculates the average error of the white from the center of the image
double calculateError(ImagePPM image, int rowNum){
  
  //Create the array that will hold the pixels. Will be size of width
  int pixelList[image.width];


  for (int iPixel = 0; iPixel < image.width; iPixel++){
    int pixelLum;



      pixelLum = get_pixel(image, rowNum, iPixel, 3);

  

  if (pixelLum > 250){
    pixelList[iPixel] = 1;}
  else{
    pixelList[iPixel] = 0;
  }

  }
  
  double error = 0;
  
  for (int i = 0; i < image.width; i++){
    
    
    if (pixelList[i] != 0){
      
      double weightedI = 0;
      if (i > image.width / 2){
        weightedI = i * 1.1;
      }
      if (i < image.width / 2){
        weightedI = i * 0.9;
      }

       error = error + (weightedI - ((image.width / 2)));
    }
  }
  return error;
}



/* variables */
double vBaseLine = 10; // crusing speed if no errors
double vLeft = vBaseLine, vRight = vBaseLine; // left and right speed initialised to cruising speed


void doCore() {
  if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}

  while(1){

    takePicture();
    double marginOfError = calculateError(cameraView, cameraView.height - 5);
    double kP = 0.03;

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

    setMotors(vLeft,vRight);   
    std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;

    usleep(10000);
  } //while
}


void doCompletion() {
  if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}

  


  while (1){

    takePicture();



    double marginOfError = calculateError(cameraView, cameraView.height - 5);
    std::cout<<"Error here is "<<marginOfError<<std::endl;

    double kP = 0.03;

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
    setMotors(vLeft,vRight); 

  std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
  usleep(10000); 
  }
}


struct BoxDims{
	int leftX = 0;
  int bottomY = 0;
  int rightX = 0;
  int topY = 0; 
};

bool boxContainsRed(BoxDims box){
  for (int row = box.topY; row < box.bottomY; row++){
    for (int col = box.leftX; col < box.rightX; col++){
      int pixelRed = get_pixel( cameraView, row, col, 0);
      int pixelGreen = get_pixel( cameraView, row, col, 1);
      int pixelBlue = get_pixel( cameraView, row, col, 2);
      float pixelPercentRed = ((float)pixelRed / (float)(pixelRed + pixelBlue + pixelGreen));
      if (pixelPercentRed > 0.6){
        return true;
      }
      set_pixel(cameraView, row, col, 0, 0, 255);
      
    }
  }
  
  return false;
}

void turn90Left(){
  setMotors(-169, 169);
}
void turn90Right(){
  setMotors(169, -169);
}

void jumpToTurn(){
  setMotors(480, 480);
  setMotors(0, 0);
}

void doChallenge() {
  if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
  //Take a picture for init purposes
  
//STEP ONE: HOW DO WE DETECT A FRONT LINE?
BoxDims topScanBox;
topScanBox.topY = 0;
topScanBox.bottomY = 100;
topScanBox.leftX = 70;
topScanBox.rightX = 80;
double vBaseLine = 40;

BoxDims leftScanBox;
leftScanBox.bottomY = cameraView.height;
leftScanBox.topY = 25;
leftScanBox.rightX = 45;
leftScanBox.leftX = 0;




while (1){
  takePicture();
    
  if (!boxContainsRed(leftScanBox)){
    if (boxContainsRed(topScanBox)){
      std::cout<<"Hold On"<<std::endl;
      jumpToTurn();
      turn90Left();
      jumpToTurn();
      jumpToTurn();
      //jumpToTurn();
      turn90Left();
      jumpToTurn();
      }
  }
    else if (!boxContainsRed(topScanBox)){
      setMotors(vBaseLine, vBaseLine);
      
      
    }
    else{
      jumpToTurn();
      turn90Right();
      jumpToTurn();
      }
  SavePPMFile("i0.ppm",cameraView);
  bool foundFront = boxContainsRed(topScanBox);
    bool foundLeft = boxContainsRed(leftScanBox);
    std::cout<<"We have left: " << foundLeft << "We have forward" << foundFront << std::endl;
}


}



int main(){
  bool choiceMade = false;
  while (!(choiceMade)){
  std::string choice;
  std::cout<<"Which part of the code will you like to run [1 for core | 2 for completion | 3 for challenge]";
  std::cin >> choice;
  if (choice == "1"){
    std::cout << std::endl << "RUNNING CORE" << std::endl;
    choiceMade = true;
    doCore();
  }
  else if (choice == "2"){
    std::cout << std::endl << "RUNNING COMPLETION" << std::endl;
    choiceMade = true;
    doCompletion();
  }
  else if (choice == "3"){
    std::cout << std::endl << "RUNNING CHALLENGE" << std::endl;
    choiceMade = true;
    doChallenge();
  }
  else {
    std::cout << "WRONG! Try again" <<std::endl;
  }
  }

} // main