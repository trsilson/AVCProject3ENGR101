
#include "robot.hpp"
#include <iostream>

int main(){
	if (initClientRobot()!=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
    double vLeft = 10;
    double vRight = 10;
    double vBaseLine = 10;
    double kP = 0.03;

    while(1){
	    takePicture();
		SavePPMFile("i0.ppm",cameraView);
		setMotors(vLeft,vRight);   
		std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
		usleep(10000);
		
		int collectPix[cameraView.width];
		
		
		for(int i = 0; i < cameraView.height; i++) {
			for(int j = 0; j < cameraView.width; j++) {
				int lum = get_pixel(cameraView, i, j, 3);

				if(lum > 250) {
					collectPix[j] = 1;
				} else {
					collectPix[j] = 0;
				}
				
			}
		}
		
		double error = 0;
		for(int i = 0; i < cameraView.width; i++) {
			if(collectPix[i] == 1) {
				error = error + (i - (cameraView.width / 2));
				std::cout<<" error = "<<error<<std::endl;
			}
		}

    if ( error > 0 ) { // if the white pixels is on right increase right motor
        vLeft = vBaseLine;
        vRight = vBaseLine - error * kP; 
		    std::cout<<" increasing right motor"<<std::endl;
	  } else if (error < 0 ) { // if the white pixels is on left increase left motor
        vLeft = vBaseLine  + error * kP;
        vRight = vBaseLine; 
        std::cout<<" increasing left motor"<<std::endl;
	  } else { // else have steady speed
        vLeft = vBaseLine;
        vRight = vBaseLine; 
        std::cout<<" steady speed"<<std::endl;
	  }

		
  } //while

} // main
