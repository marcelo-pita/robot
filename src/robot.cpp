//============================================================================
// Name        : robot.cpp
// Author      : 
// Version     :
// Copyright   :
// Description :
//============================================================================

int SIZE_CAR_WIDTH = 23;
int SIZE_CAR_HEIGHT = 36;
int MAX_X = 1200;
int MAX_Y = 600;

#include <allegro.h>
#include<stdio.h>
#include<math.h>

float toAngleAllegro (float teta){
	return (teta*256/360);
}
void printRadius(int x, int y, int radius){
    circle(screen, x,y, radius, 10);
}

int calculateFrontPositionX (int x, int y, float teta, int distance){
	//printf("calculando dentro do x: %d",x +  distance*sin(teta) );
	return (int)(x +  distance*sin(teta*M_PI/180));
}

int calculateFrontPositionY(int x, int y, float teta, int distance){
	return (int)(y -  distance*cos(teta*M_PI/180));
}

float distanceWallToSensor(float x, float y, float teta){
	  int colorDetected = 16777215;
	  float r=1;
	  while (colorDetected != 0 && r<1200){
		  int x1 =  calculateFrontPositionX(x, y,teta, r);
		  int y1 =  calculateFrontPositionY(x,y,teta, r);
		 // printf("x y %d %d", x,y);
		 // putpixel(screen,x1,y1,990);
		  colorDetected = getpixel(screen,x1,y1);
		  putpixel(screen,x1,y1,400);
		  if (colorDetected !=0){
			  r++;
		  }
	  }

	  return r;
}

int main(int argc, char** argv) {
    //Initialize Allegro
    allegro_init();
    set_color_depth(32);
    // Image to draw the scenario
    BITMAP * buf = create_bitmap(MAX_X, MAX_Y);
    BITMAP * back = load_bitmap ("back.bmp", NULL);
    BITMAP * car = load_bitmap("car2.bmp",NULL);
    float xPosition=0, yPosition=0;

    if (back==NULL || car == NULL)
        exit(1);

    //Set the resolution to 640 x 480 with SAFE autodetection.
    set_gfx_mode(GFX_SAFE, MAX_X, MAX_Y, 0, 0);

    //Install the keyboard handler
    install_keyboard();

    //Print your welcome message to the screen
  //  textout(screen, font, "Hello Dream.In.Code! This is my first Allegro Program", 1, 1, 10);
    //textout(screen, font, "Press ESCape to quit.", 1, 12, 11);
    int rotation =0;
    //Loop until escape is pressed
    xPosition = 113.882126;
    yPosition = 466.934723;
    while(! key[KEY_ESC]){
        poll_keyboard();
        clear(buf);
        draw_sprite(buf, back, 0,0);

        		float radianAngle = rotation * M_PI/180;
                if (key[KEY_RIGHT]){
                	rotation++;
                }
                if (key[KEY_LEFT]){
                	rotation--;
                }
                if (key[KEY_UP]){
                	yPosition -= + 2*cos(radianAngle);
                	xPosition += + 2*sin(radianAngle);
                }
                if (key[KEY_DOWN]){
                	yPosition += +2*cos(radianAngle);
                	xPosition -= 2*sin(radianAngle);
                }

                int distanceSensorDetected = 20; // distancia a ser imprimida a partir do sensor
                int distanceSensorDelay = 2; // distancia do carro pro sensor

                int distancePrinterSensor = distanceSensorDetected-distanceSensorDelay;

                // calcular qual é a posicao inicial dos sensores
                int frontX = calculateFrontPositionX(xPosition + SIZE_CAR_WIDTH, yPosition+SIZE_CAR_HEIGHT,rotation, SIZE_CAR_HEIGHT);
                int frontY =calculateFrontPositionY(xPosition + SIZE_CAR_WIDTH, yPosition+SIZE_CAR_HEIGHT,rotation, SIZE_CAR_HEIGHT);

                int rightLateralX =calculateFrontPositionX(xPosition + SIZE_CAR_WIDTH, yPosition+SIZE_CAR_HEIGHT,rotation+30, SIZE_CAR_HEIGHT);
                int rightLateralY =calculateFrontPositionY(xPosition + SIZE_CAR_WIDTH, yPosition+SIZE_CAR_HEIGHT,rotation+30, SIZE_CAR_HEIGHT);

                int leftLateralX = calculateFrontPositionX(xPosition + SIZE_CAR_WIDTH, yPosition+SIZE_CAR_HEIGHT,rotation-30, SIZE_CAR_HEIGHT);
                int leftLateralY =calculateFrontPositionY(xPosition + SIZE_CAR_WIDTH, yPosition+SIZE_CAR_HEIGHT,rotation-30, SIZE_CAR_HEIGHT);

                // calcular a posicao final da linha formada pelo sensor dada a distancia definida em distanceSensorDetected
                int printSensorFrontX = calculateFrontPositionX(frontX, frontY,rotation, distancePrinterSensor);
                int printSensorFrontY = calculateFrontPositionY(frontX, frontY,rotation, distancePrinterSensor);

                int printSensorLeftX =  calculateFrontPositionX(leftLateralX, leftLateralY,rotation-30, distancePrinterSensor);
                int printSensorLeftY =  calculateFrontPositionY(leftLateralX, leftLateralY,rotation-30, distancePrinterSensor);

                int printSensorRightX =  calculateFrontPositionX(rightLateralX, rightLateralY,rotation+30, distancePrinterSensor);
                int printSensorRightY =  calculateFrontPositionY(rightLateralX, rightLateralY,rotation+30, distancePrinterSensor);

                // a posicao de um ponto a partir do sensor dada a distancia distanciaSensorDelay
                int detectedSensorFrontX = calculateFrontPositionX(frontX, frontY,rotation, distanceSensorDelay);
                int detectedSensorFrontY = calculateFrontPositionY(frontX, frontY,rotation, distanceSensorDelay);

                int detectedSensorLeftX =  calculateFrontPositionX(leftLateralX, leftLateralY,rotation-30, distanceSensorDelay);
                int detectedSensorLeftY =  calculateFrontPositionY(leftLateralX, leftLateralY,rotation-30, distanceSensorDelay);

                int detectedSensorRightX =  calculateFrontPositionX(rightLateralX, rightLateralY,rotation+30, distanceSensorDelay);
                int detectedSensorRightY =  calculateFrontPositionY(rightLateralX, rightLateralY,rotation+30, distanceSensorDelay);


                // rotaciona o carro
                rotate_sprite(buf, car, xPosition, yPosition, itofix(toAngleAllegro(rotation)));

                poll_keyboard();

                if (key[KEY_ESC]){
                    break;
                }
                blit(buf, screen, 0,0,0,0,MAX_X, MAX_Y);


                // imprime as linhas que distam distanceSensorDetected do sensor
                line(screen, frontX, frontY, printSensorFrontX, printSensorFrontY, 3000);
                line(screen, leftLateralX, leftLateralY, printSensorLeftX, printSensorLeftY, 3000);
                line(screen, rightLateralX, rightLateralY, printSensorRightX, printSensorRightY, 3000);

                // calcula a distancia da parede a cada um dos sensores
                float distanceSensorFrontToWall = distanceWallToSensor(detectedSensorFrontX, detectedSensorFrontY, rotation);
                float distanceSensorRightToWall = distanceWallToSensor(detectedSensorRightX, detectedSensorRightY, rotation+30);
                float distanceSensorLeftToWall = distanceWallToSensor(detectedSensorLeftX,detectedSensorLeftY, rotation-30);

                printf("distancias: esq %f front %f  dir: %f\n", distanceSensorLeftToWall, distanceSensorFrontToWall, distanceSensorRightToWall);

                circle(screen, xPosition + SIZE_CAR_WIDTH, yPosition+SIZE_CAR_HEIGHT, 40, 100);
                rest(100);


    }


    delete(back);
    delete(car);

    //Exit program
    allegro_exit();
    return 0;
}

END_OF_MAIN();

