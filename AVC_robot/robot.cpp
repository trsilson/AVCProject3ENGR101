#include "robot.hpp"
#include <vector>
#include <fstream>

/*This class represents a line in the camera view which we scan for white pixels on
 *This class contructor can be used to create multiple scanline objects, and we can easily check these objects to see if they
 * Have detected a white line and what the error is from the center of that scan line
 * 
*/ 
class ScanLine {
  
  //PUBLIC INTERFACE: Represents the methods and the attritubes that anybody using the class has access too
  public:
  //CONTRUCTOR: To create an instance of ScanLine, you need to specify if this is scanning a whole row or column in the image
  //You also need to specify what the row nuber / column number you are scanning
  //So far NO form of type checking. Ideally the postion would actually exist in the image
  ScanLine(std::string lineType, int position);
  
  //Returns the calculated error of the white line from the center of the scanline
  double getError();
  //Returns whether or not white pixels have been detected in this scanline
  bool checkWhite();
  //This method must be called every time in the main image processing loop. This makes sure that the checkWhite and getError actually return the latest and relevant data
  //Must pass the image that you wish the ScanLine to operate on. For our purposes, this will be the cameraView image that is defined in the included robot.hpp file.
  void update(ImagePPM image);

  void setBias(std::string direction);

  std::string getBias();
  
  //PRIVATE: Between here and the end of the class, these are internal variables which are important for the proper function of instance of this class, and have
  //been protected from public use.
  private:

  double error;
  std::string lineType;
  int position;
  bool containsWhite = false;
  std::string directionBias = "CENTER";
  

};

//Contructor method implementation:
ScanLine::ScanLine(std::string lineType, int position){
  //Check to see if the user has provided an appropriate keyword as our lineType (Must be either "row" or "col")
  //Again, no type checking is happening here. if you don't put anything except exactly "row" or "col", the code will proceed to fail in
  //Spectacular fashion. I am yet to educate myself on what the best practices regarding this are.
  //If so, set our attribute ccordingly
  if (lineType == "row"){
    this->lineType = "row";
  }
  if (lineType == "col"){
    this->lineType = "col";
  }

  //Set our position attribute
  this->position = position;
}

//
void ScanLine::update(ImagePPM image){
  
  std::vector<int> pixelList;

  int numOfPixels;
  if (this->lineType == "col"){
    numOfPixels = image.height;
  }
  else if (this->lineType == "row"){
    numOfPixels = image.width;
  }


  int whiteCount = 0;
  this->containsWhite = false;

  // std::cout<<this->position<<std::endl;
  // std::cout<<numOfPixels<<std::endl;

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
    
    
    if (pixelList.at(i) != 0){
      
      double weightedI = 0;
      if (i > pixelList.size() / 2){
        weightedI = i * 1.1;
      }
      if (i < pixelList.size() / 2){
        weightedI = i * 0.9;
      }

      this-> error = this-> error + (weightedI - ((numOfPixels / 2)));
      whiteCount ++;
    }
  }
}

double ScanLine::getError(){
  return this->error;
}

bool ScanLine::checkWhite(){
  return this->containsWhite;
}

void ScanLine::setBias(std::string bias){
  this->directionBias = bias;
}

std::string ScanLine::getBias(){
  return this->directionBias;
}



/* variables */
double vBaseLine = 10; // crusing speed if no errors
double vLeft = vBaseLine, vRight = vBaseLine; // left and right speed initialised to cruising speed


void doCore() {
  if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}

  ScanLine bottomLine = ScanLine("row", (cameraView.height - 5));

  while(1){

    takePicture();
    bottomLine.update(cameraView);
    double marginOfError = bottomLine.getError();
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

  // std::ofstream logFile; // Create an output stream for a log file
  // logFile.open("completion log.txt", std::ios::out);
  // logFile << "HAHAHAHA RONAHLDINO SOCCER!!!";
  // logFile.close();
  ScanLine centerLine = ScanLine("row", (cameraView.height - 5));
  ScanLine leftLine = ScanLine("col", 5);
  ScanLine rightLine = ScanLine("col", (cameraView.width - 5));
  //ScanLine topLine = ScanLine ("row", 5);

  


  while (1){

    takePicture();
    //topLine.update(cameraView);
    leftLine.update(cameraView);
    rightLine.update(cameraView);
    centerLine.update(cameraView);




    double marginOfError = centerLine.getError();
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
  std::cout<<"Left white detected: " << leftLine.checkWhite() << " Center white detected: " << centerLine.checkWhite() << " Right white detected: " << rightLine.checkWhite() << std::endl;
  std::cout<<"Left error: " << leftLine.getError() << " Center error: " << centerLine.getError() << " right error: " << rightLine.getError() << std::endl;
  std::cout<<"Center line is biasing: " << centerLine.getBias() << std::endl;
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

void doChallenge() {
  if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
  //Take a picture for init purposes
  
//STEP ONE: HOW DO WE DETECT A FRONT LINE?
BoxDims topScanBox;
topScanBox.topY = 0;
topScanBox.bottomY = 50;
topScanBox.leftX = 50;
topScanBox.rightX = 100;
double vBaseLine = 40;

BoxDims leftScanBox;
leftScanBox.bottomY = cameraView.height;
leftScanBox.topY = 25;
leftScanBox.rightX = 45;
leftScanBox.leftX = 0;

BoxDims rightScanBox;
rightScanBox.bottomY = 100;
rightScanBox.topY = 0;
rightScanBox.rightX = cameraView.width;
rightScanBox.leftX = cameraView.width - 45;


while (1){
  takePicture();
    
  if (!boxContainsRed(leftScanBox)){
    if (boxContainsRed(topScanBox)){
      std::cout<<"Hold On"<<std::endl;
      //usleep(5000000);
      setMotors(20, 40);}
  }
    else if (!boxContainsRed(topScanBox)){
      setMotors(vBaseLine, vBaseLine);
      
      
    }
    else{setMotors(40,20);}
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