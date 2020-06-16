#include "robot.hpp"
#include "SFML/Graphics.hpp"

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
    this->lineType = "row";
  }
  if (lineType == "col"){
    this->lineType = "col";
  }
  this->position = position;
}

void ScanLine::update(ImagePPM image){
  //This will hold
  std::vector<int> pixelList;

  int numOfPixels;
  if (this->lineType == "col"){
    numOfPixels = image.height;
  }
  else if (this->lineType == "row"){
    std::cout<<"This be a row"<<std::endl;
    numOfPixels = image.width;
    std::cout<<"HELP, I AM STUCK IN A BOX " << numOfPixels << std::endl;
  }


  int whiteCount = 0;
  this->containsWhite = false;
  std::cout<<this->position<<std::endl;
  std::cout<<numOfPixels<<std::endl;

  for (int iPixel = 0; iPixel < numOfPixels; iPixel++){
    int pixelLum;


    if (lineType == "row"){
      pixelLum = get_pixel(image, this->position, iPixel, 3);
      pixelLum = get_pixel()
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



int main(){
	if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}

  double cruiseSpeed = 50;
  double vLeft = 0;
  double vRight = 0;

   ScanLine testLine = ScanLine("row", 5);

  while(1){
    takePicture();
    SavePPMFile("i0.ppm",cameraView);

    testLine.update(cameraView);
    

    //This be bad steering.
    //Was only thrown together to have a look at what the robot was seeing without writing the code for it to get there itself
    //Takes one button at a time. WASD controls. No button pressed = stop

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
      vRight = cruiseSpeed / 2;
      vLeft = -cruiseSpeed / 2;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
      vLeft = cruiseSpeed / 2;
      vRight = -cruiseSpeed / 2;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
      vLeft  = cruiseSpeed;
      vRight = cruiseSpeed;
    }

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
      vLeft = -cruiseSpeed;
      vRight = -cruiseSpeed;
    }
    else{
      vLeft = 0;
      vRight = 0;
    }

    setMotors(vLeft,vRight);   
    std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
    usleep(10000);
  } //while

} // main