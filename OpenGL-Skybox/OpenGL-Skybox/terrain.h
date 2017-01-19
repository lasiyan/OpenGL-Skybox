/* Terrain.h : 지면텍스쳐, 물텍스쳐, 안개를 지형으로 구성, 4단계 텍스쳐타일링	*/
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
	uint _texId_ground, _texId_water;//지형텍스쳐와 물텍스쳐의 id
	AUX_RGBImageRec *img;//텍스쳐이미지 객체와 연결되는 포인터
	ubyte* _height; //지형높이 배열과 연결하는 포인터
	GLint _map_width, _map_height; //단위지형의 가로세로크기
	GLuint baseTerrain; //타일의 인덱스값
	GLubyte minHeight; // 지형데이터의 최소높이
	static ubyte* readRawData(pcStr filename, uint width, uint height); // 지형데이터읽어오기
public:
	Terrain(pcStr heightmap, pcStr surfaceTex, GLuint width, GLint height);//지형생성자
	~Terrain(void);//소멸자
	void RenderTerrain(GLfloat, GLfloat);//지형그리기
	void TileTerrain(GLint);//LoD를 위한 타일링
	void RenderWater(void);// 물그리기
	GLfloat getHeight(GLfloat, GLfloat);//해당 위치의 높이구하기
	GLint getTileX(GLfloat); // 해당위치의 x타일구하기
	GLint getTileY(GLfloat);// 해당위치의 y타일구하기
	GLfloat waterLevel;// 수면높이
};