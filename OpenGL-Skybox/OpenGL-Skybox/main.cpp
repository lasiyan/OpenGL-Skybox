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

//��ü ����
Terrain* terrain;	//����
Skybox* skybox;	//��ī�̹ڽ�
Camera cam;	//ī�޶�
CCamera objCamera;

bool keyPressed[256];	//Ű�����Է»�Ȳ�迭
void myKeyboard(unsigned char key, int x, int y) { keyPressed[key] = true; }
						//�ش�迭�� true�� Key down
void myKeyboardUp(unsigned char key, int x, int y) { keyPressed[key] = false; }
						//�ش�迭�� false�� Key Up

void dispose(); // ����� ����Ǵ� �Լ�
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
	terrain->RenderTerrain(cam.eye.x, cam.eye.z);//������ �׸���.��ǥ�� �����ִ� ������ ī�޶� ��ġ�� Ÿ�Ϻ��� ��ǥ�� ����ϱ� ���� ppt����
	glPopMatrix();
	fog(); // ����Ʒ� �Ȱ�ȿ��
	glFlush();
	glutSwapBuffers();//������۸�
}
void Idle() {//�ش�Ű�� �����ִ��� ���������� �˻��� �����Է��� �Ҽ� �ְ� �Ѵ�
	if (keyPressed['1'])glPolygonMode(GL_FRONT, GL_LINE);
	if (keyPressed['2'])glPolygonMode(GL_FRONT, GL_FILL);
	if (keyPressed['d']) { cam.slide(0.2, 0, 0); }
	if (keyPressed['a']) { cam.slide(-0.2, 0, 0); }
	if (keyPressed['s']) { cam.slide(0, 0, 1.0); }
	if (keyPressed['w']) { cam.slide(0, 0, -1.0); }
	if (cam.eye.y<terrain->getHeight(cam.eye.x, cam.eye.z)) { cam.slide(0, 1.0, 0); }// ������ Colision Detection ��ǥ�� �Ʒ��� ī�޶� ���������� ��ǥ�� ���� ������Ŵ
	if (keyPressed['i']) { cam.pitch(-0.5); }
	if (keyPressed['k']) { cam.pitch(0.5); }
	if (keyPressed['q']) { cam.yaw(-0.5); }
	if (keyPressed['e']) { cam.yaw(0.5); }
	if (keyPressed['j']) { cam.roll(0.5); }
	if (keyPressed['l']) { cam.roll(-0.5); }
	else { cam.slide(0, 0, 0); } //�ƹ��͵� ������ �ʾ������� �̵����ٰ� ���� ��� �𵨺� ����� ���������ش�.
	glutPostRedisplay();// �ٽñ׸���
}
void Reshape(GLint w, GLint h) {//�������� �ݹ�
	glViewport(0, 0, w, h);//����Ʈ����
	glMatrixMode(GL_PROJECTION);//���������� ���� �������
	glLoadIdentity();// �������
	gluPerspective(40.0, static_cast<GLdouble>(w / h), 1.0, 3000.0);//����������
}
void dispose() {//���α׷� ����� ��ü��ȯ
	delete terrain;
	delete skybox;
	delete &cam;
}
void fog() {
	//GL_FOG�� Ȱ��ȭ �Ѵ�.
	glEnable(GL_FOG);
	//�Ȱ��� �����̸� �����ϴ� ���� (GL_LINEAR, GL_EXP, GL_EXP2)
	glFogi(GL_FOG_MODE, GL_EXP2);
	//Viewer�� Y�� waterLevel���� ������, �� ���ӿ� �ִٸ�
	GLfloat waterFogColor[4] = { 0.0,0.6,0.6,5.0 };
	GLfloat fogColor[4] = { 0.75,0.75,0.75,0.0 };
	if (cam.eye.y<(terrain->waterLevel - 75)) {
		//�Ȱ� ������ waterFogColor(0.0,0.6,0.6,1.0)���� �����ϰ�
		glFogfv(GL_FOG_COLOR, waterFogColor);
		//�Ȱ� �Ÿ��� waterFogDensity(0.075)�� �����ؼ� �����̶�� ������ �ְ� �Ѵ�.
		glFogf(GL_FOG_DENSITY, 0.075);
	}
	else {//�ƴϸ� 
		  //�Ȱ� ������ fogColor(0.7,0.7.0.7,1) �����ϰ�
		glFogfv(GL_FOG_COLOR, fogColor);
		//�Ȱ� �Ÿ��� fogDensity(0.002) �����ؼ� �����̶�� ������ �ְ� �Ѵ�.
		glFogf(GL_FOG_DENSITY, 0.001);
	}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	// �����켳��
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(400, 30);
	glutCreateWindow("Project_Final");

	// �ݹ��Լ����
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	//objCamera.Mouse_Move();
	glutKeyboardFunc(myKeyboard);
	glutKeyboardUpFunc(myKeyboardUp);
	
	// ���̹���, �ĸ�����Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Terrain, Skybox��ü
	terrain = new Terrain("terrain1.raw", "snow512.bmp", 257, 257);
	skybox = new Skybox();

	// ����Ʈ�� ī�޶� ����
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0);
	cam.setShape(60.0f, 64.0f / 48.0f, 0.5f, 1000.0f);

	// ī�޶��� �ʱ���ġ�� ȸ����
	// ������ ��ģ ī�޶� ó���� ��𿡼� ��� ���������� ����
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