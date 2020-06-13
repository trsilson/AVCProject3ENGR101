#include "robot.hpp"

int main(){
	if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
  /* variables */
  double vBaseLine = 10; // crusing speed if no errors
  double vLeft = vBaseLine, vRight = vBaseLine; // left and right speed initialised to cruising speed
  double kP = 0.03; // constant for difference in left and right speed - dv

  while(1){
    takePicture();
    SavePPMFile("i0.ppm",cameraView);
    setMotors(vLeft,vRight);   
    std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
    usleep(10000);
		
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

    /* if white pixels are on the right, increase right speed so robot moves to the left */
    if (error > 0) { 
      vLeft = vBaseLine;
      vRight = vBaseLine - error * kP; 
    /* if white pixels are on the left, increase left motor so robot moves to the right */
    } else if (error < 0) {
      vLeft = vBaseLine  + error * kP;
      vRight = vBaseLine; 
    }  
    /* else have a steady speed */
    else {
      vLeft = vBaseLine + error * kP;
      vRight = vBaseLine - error * kP; 
    }

  } //while

} // main