#include <stdio.h>
#include <math.h>
#include <gl\GLAUX.H>
#include <gl/glut.h>
#include "Terrain.h"
#include "Skybox.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glut32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glaux.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")

//객체 선언
Terrain* terrain;	//지형
Skybox* skybox;	//스카이박스
Camera cam;	//카메라
CCamera objCamera;

bool keyPressed[256];	//키보드입력상황배열
void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }
						//해당배열이 true면 Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }
						//해당배열이 false면 Key Up

void dispose(); // 종료시 수행되는 함수
void Display(); // display callback
void Idle(); // idle callback
void Reshape(GLint w, GLint h); // reshape callback
void fog();



void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(cam.eye.x, cam.eye.y, cam.eye.z);
	skybox->draw();
	glPopMatrix();

	glScalef(1.0f, 0.2f, 1.0f);
	glPushMatrix();
	terrain->RenderTerrain(cam.eye.x, cam.eye.z);//지형을 그린다.좌표를 보내주는 이유는 카메라가 위치한 타일블럭의 좌표를 계산하기 위해 ppt참조
	glPopMatrix();
	fog(); // 수면아래 안개효과
	glFlush();
	glutSwapBuffers();//더블버퍼링
}
void Idle() {//해당키가 눌려있는지 지속적으로 검사해 다중입력을 할수 있게 한다
	if (keyPressed['1'])glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])glPolygonMode(GL_FRONT, GL_FILL);
	if (keyPressed['d']) { cam.slide(0.2, 0, 0); }
	if (keyPressed['a']) { cam.slide(-0.2, 0, 0); }
	if (keyPressed['s']) { cam.slide(0, 0, 1.0); }
	if (keyPressed['w']) { cam.slide(0, 0, -1.0); }
	if (cam.eye.y<terrain->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }// 간단한 Colision Detection 지표면 아래로 카메라가 내려갈때는 지표면 위로 유지시킴
	if (keyPressed['i']) { cam.pitch(-0.5); }
	if (keyPressed['k']) { cam.pitch(0.5); }
	if (keyPressed['q']) { cam.yaw(-0.5); }
	if (keyPressed['e']) { cam.yaw(0.5); }
	if (keyPressed['j']) { cam.roll(0.5); }
	if (keyPressed['l']) { cam.roll(-0.5); }
	else { cam.slide(0, 0, 0); } //아무것도 눌리지 않았을때는 이동없다고 보고 계속 모델뷰 행렬을 유지시켜준다.
	glutPostRedisplay();// 다시그리기
}
void Reshape(GLint w, GLint h) {//리셰이프 콜백
	glViewport(0, 0, w, h);//뷰포트설정
	glMatrixMode(GL_PROJECTION);//원근투상을 위한 투상행렬
	glLoadIdentity();// 단위행렬
	gluPerspective(40.0, static_cast<GLdouble>(w / h), 1.0, 3000.0);//원근투상설정
}
void dispose() {//프로그램 종료시 객체반환
	delete terrain;
	delete skybox;
	delete &cam;
}
void fog() {
	//GL_FOG를 활성화 한다.
	glEnable(GL_FOG);
	//안개의 농도차이를 결정하는 인자 (GL_LINEAR, GL_EXP, GL_EXP2)
	glFogi(GL_FOG_MODE, GL_EXP2);
	//Viewer의 Y가 waterLevel보다 작으면, 즉 물속에 있다면
	GLfloat waterFogColor[4] = { 0.0,0.6,0.6,5.0 };
	GLfloat fogColor[4] = { 0.75,0.75,0.75,0.0 };
	if (cam.eye.y<(terrain->waterLevel - 75)) {
		//안개 색상을 waterFogColor(0.0,0.6,0.6,1.0)으로 변경하고
		glFogfv(GL_FOG_COLOR, waterFogColor);
		//안개 거리를 waterFogDensity(0.075)로 변경해서 물속이라는 느낌을 주게 한다.
		glFogf(GL_FOG_DENSITY, 0.075);
	}
	else {//아니면 
		  //안개 색상을 fogColor(0.7,0.7.0.7,1) 변경하고
		glFogfv(GL_FOG_COLOR, fogColor);
		//안개 거리를 fogDensity(0.002) 변경해서 물밖이라는 느낌을 주게 한다.
		glFogf(GL_FOG_DENSITY, 0.001);
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	// 윈도우설정
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(400, 30);
	glutCreateWindow("Project_Final");

	// 콜백함수등록
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	//objCamera.Mouse_Move();
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp);
	
	// 깊이버퍼, 후면제거활성화
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Terrain, Skybox객체
	terrain = new Terrain("terrain1.raw", "snow512.bmp", 257, 257);
	skybox = new Skybox();

	// 뷰포트와 카메라 설정
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0);
	cam.setShape(60.0f, 64.0f / 48.0f, 0.5f, 1000.0f);

	// 카메라의 초기위치와 회전각
	// 설정을 마친 카메라가 처음에 어디에서 어디를 보고있을지 결정
	cam.slide(0, 100, 0);
	cam.roll(0);
	cam.yaw(180);
	cam.pitch(45);

	atexit(dispose);
	printf("W: foward S: backwad A: left D: right\n");
	printf("JL: roll\n");
	printf("KI: pitch\n");
	printf("QE: yaw\n\n");
	printf("1,2: solid, wire rendering\n");
	glutMainLoop();
	return 0;
}