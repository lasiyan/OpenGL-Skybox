/* Terrain.h : �����ؽ���, ���ؽ���, �Ȱ��� �������� ����, 4�ܰ� �ؽ���Ÿ�ϸ�	*/
#include <stdio.h>
#include <stdlib.h>
#include <exception>
#include <math.h>
#include <gl\glaux.h>
#include "camera.h"

class Terrain {
private:
	typedef const char* pcStr;
	typedef unsigned int uint;
	typedef unsigned char ubyte;
	uint _texId_ground, _texId_water;//�����ؽ��Ŀ� ���ؽ����� id
	AUX_RGBImageRec *img;//�ؽ����̹��� ��ü�� ����Ǵ� ������
	ubyte* _height; //�������� �迭�� �����ϴ� ������
	GLint _map_width, _map_height; //���������� ���μ���ũ��
	GLuint baseTerrain; //Ÿ���� �ε�����
	GLubyte minHeight; // ������������ �ּҳ���
	static ubyte* readRawData(pcStr filename, uint width, uint height); // �����������о����
public:
	Terrain(pcStr heightmap, pcStr surfaceTex, GLuint width, GLint height);//����������
	~Terrain(void);//�Ҹ���
	void RenderTerrain(GLfloat, GLfloat);//�����׸���
	void TileTerrain(GLint);//LoD�� ���� Ÿ�ϸ�
	void RenderWater(void);// ���׸���
	GLfloat getHeight(GLfloat, GLfloat);//�ش� ��ġ�� ���̱��ϱ�
	GLint getTileX(GLfloat); // �ش���ġ�� xŸ�ϱ��ϱ�
	GLint getTileY(GLfloat);// �ش���ġ�� yŸ�ϱ��ϱ�
	GLfloat waterLevel;// �������
};