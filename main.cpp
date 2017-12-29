#include <stdlib.h>  
#include <windows.h>  
#include <stdio.h>  
#include "glut.h"  


void drawFloor(){
     int line,column;
     glPushMatrix();
     for(line=0;line<10;line++){
         glPushMatrix();
         
         for(column=0;column<10;column++){
             
             glColor3f(0.0,0.0,0.0);
         
             glBegin(GL_QUADS);
             glVertex3f(0.0,0.0,0.0);
             glVertex3f(1.0,0.0,0.0);
             glVertex3f(1.0,0.0,1.0);
             glVertex3f(0.0,0.0,1.0);
             glEnd();
             
             glTranslatef(1.0,0.0,0.0);
             
             glColor3f(1.0,1.0,1.0);
             
             glBegin(GL_QUADS); 
             glVertex3f(0.0,0.0,0.0);
             glVertex3f(1.0,0.0,0.0);
             glVertex3f(1.0,0.0,1.0);
             glVertex3f(0.0,0.0,1.0);
             glEnd();
             
             glTranslatef(1.0,0.0,0.0);
             }
         
         glPopMatrix();
         glTranslatef(0.0,0.0,2.0);
         }
     
     glPopMatrix();
     glTranslatef(0.0,0.0,1.0);
     
     for(line=0;line<10;line++){
         glPushMatrix();
         
         for(column=0;column<10;column++){
             
             glColor3f(1.0,1.0,1.0);
         
             glBegin(GL_QUADS);
             glVertex3f(0.0,0.0,0.0);
             glVertex3f(1.0,0.0,0.0);
             glVertex3f(1.0,0.0,1.0);
             glVertex3f(0.0,0.0,1.0);
             glEnd();
             
             glTranslatef(1.0,0.0,0.0);
             
             glColor3f(0.0,0.0,0.0);
             
             glBegin(GL_QUADS); 
             glVertex3f(0.0,0.0,0.0);
             glVertex3f(1.0,0.0,0.0);
             glVertex3f(1.0,0.0,1.0);
             glVertex3f(0.0,0.0,1.0);
             glEnd();
             
             glTranslatef(1.0,0.0,0.0);
             }
         
         glPopMatrix();
         glTranslatef(0.0,0.0,2.0);
         }    
     }
	 
void  main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Final Project");
	init();
	glutReshapeFunc(reshape);
	glutSpecialFunc(keys);
	glutDisplayFunc(draw);
    glutMainLoop();
 
}