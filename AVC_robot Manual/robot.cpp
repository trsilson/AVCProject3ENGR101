#include "robot.hpp"
#include "SFML/Graphics.hpp"

int main(){
	if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}

  double cruiseSpeed = 50;
  double vLeft = 0;
  double vRight = 0;

  while(1){
    takePicture();
    SavePPMFile("i0.ppm",cameraView);

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