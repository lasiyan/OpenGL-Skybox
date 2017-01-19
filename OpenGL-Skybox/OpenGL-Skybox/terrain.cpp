#include "terrain.h"
Terrain::Terrain(pcStr heightFile, pcStr surfaceTexFile, GLuint width, GLint height) {
	_map_width = width;// �� ���μ��� ũ�� ����
	_map_height = height;

	img = NULL;// ���� ������ ��Ʈ���̹��� ������ �ʱ�ȭ
	_texId_ground = 0; _texId_water = 0;//�ؽ��� id��
	_height = 0;
	
	/* �����ؽ��� ���� �� ���� */
	wchar_t text[50];
	mbstowcs(text, surfaceTexFile, strlen(surfaceTexFile) + 1);
	LPCWSTR LPsurfaceTexFile = text;

	img = auxDIBImageLoad(LPsurfaceTexFile);//�����ؽ����̹��� �ε�
	glGenTextures(1, &_texId_ground);// �ؽ��� �����
	glBindTexture(GL_TEXTURE_2D, _texId_ground); //�ؽ���id�� ����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);//2d�ؽ����̹�������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// ���εɹ�ü�� ������� �ؽ��� �������� ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// ���ι�ü�� Ŭ��� �ؽ����� Ȯ�� ����� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	/////////////////////////////////////////////////

	/////////////���ؽ��� ���� �� ����/////////////
	const char *waterTexFile = "water.bmp";
	wchar_t text2[30];
	mbstowcs(text2, waterTexFile, strlen(waterTexFile) + 1);
	LPCWSTR LPwaterTexFile = text2;

	img = auxDIBImageLoad(LPwaterTexFile); // ���̹��� �ε�
	glGenTextures(1, &_texId_water);//�ؽ���id�� ����
	glBindTexture(GL_TEXTURE_2D, _texId_water);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->sizeX, img->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	////////////////////////////////////////////////

	_height = readRawData(heightFile, width, height);// ���Ϸκ��� ���μ��θ�ŭ�� ���� �����͸� �о��

													 // ������ �ּҰ� ����
	int i;
	for (i = 0, minHeight = 2 ^ 10; i<_map_width*height; i++) {
		if (minHeight>_height[i]) { minHeight = _height[i]; }
	}
	// �����̴� �ּ������� 60��ġ
	waterLevel = (GLfloat)minHeight + 100;
	// ������ �ּҳ��̸� �������� �߶󳻸�
	for (int i = 0; i<_map_width*_map_height + 1; i++) _height[i] -= minHeight;
	// ���� Ÿ�ϸ��ϱ�
	baseTerrain = glGenLists(5);// 4�ܰ��� ���� Ÿ��, �� 5���� ���÷��̸���Ʈ
	glNewList(baseTerrain, GL_COMPILE);
	TileTerrain(1);//detail level1 ����
	glEndList();
	glNewList(baseTerrain + 1, GL_COMPILE);
	TileTerrain(4);//detail level2 ����
	glEndList();
	glNewList(baseTerrain + 2, GL_COMPILE);
	TileTerrain(8);//detail level3 ����
	glEndList();
	glNewList(baseTerrain + 3, GL_COMPILE);
	TileTerrain(16);//detail level4 ����
	glEndList();
	glNewList(baseTerrain + 4, GL_COMPILE);
	RenderWater();//�� �׸���
	glEndList();
}
Terrain::~Terrain() {
	// Terrain��ü�� �Ҹ��� �޸𸮸� ��ȯ�Ѵ�
	if (_height != 0) { delete[] _height; }
	if (_texId_ground != 0) { glDeleteTextures(1, &_texId_ground); }
	if (_texId_water != 0) { glDeleteTextures(1, &_texId_water); }
	if (img != 0) { free(img->data); free(img); }
}

void Terrain::RenderTerrain(GLfloat x, GLfloat y) {
	int i, j;
	// Ÿ�ϸ��� ���� ���÷��̸���Ʈ���� �ҷ����� �Լ�
	//detail level1 ����
	//ī�޶� ��ġ�� ���� �� ���� �׸���.
	glPushMatrix();
	glTranslatef(getTileX(x)*(_map_width - 1), 0.0, getTileY(y)*(_map_height - 1));
	glCallList(baseTerrain);
	glPopMatrix();
	//detail level2 ����
	//level1���� �����ϰ� 3x3������ �׸���.
	for (i = -1; i<2; i++) {
		for (j = -1; j<2; j++) {
			if (!(i == 0 && j == 0)) {
				glPushMatrix();
				glTranslatef(getTileX(x)*(_map_width - 1) + (i*(_map_width - 1)), 0.0, getTileY(y)*(_map_height - 1) + (j*(_map_height - 1)));
				glCallList(baseTerrain + 1);
				glPopMatrix();
			}
		}
	}
	//detail level3 ����
	//level2���� �����ϰ� 5x5������ �׸���.
	for (i = -2; i<3; i++) {
		for (j = -2; j<3; j++) {
			if ((i<-1 || i>1) || (j<-1 || j>1)) {
				glPushMatrix();
				glTranslatef(getTileX(x)*(_map_width - 1) + (i*(_map_width - 1)), 0.0, getTileY(y)*(_map_height - 1) + (j*(_map_height - 1)));
				glCallList(baseTerrain + 2);
				glPopMatrix();
			}
		}
	}
	//detail level4 ����
	//level2���� �����ϰ� 7x7������ �׸���.
	for (i = -3; i<4; i++) {
		for (j = -3; j<4; j++) {
			if ((i<-2 || i>2) || (j<-2 || j>2)) {
				glPushMatrix();
				glTranslatef(getTileX(x)*(_map_width - 1) + (i*(_map_width - 1)), 0.0, getTileY(y)*(_map_height - 1) + (j*(_map_height - 1)));
				glCallList(baseTerrain + 3);
				glPopMatrix();
			}
		}
	}
	//�� ���÷��� ����Ʈ
	//3x3������ �׸���
	for (i = -2; i<3; i++) {
		for (j = -2; j<3; j++) {
			glPushMatrix();
			glTranslatef(getTileX(x)*(_map_width - 1) + (i*(_map_width)-1), 0.0, getTileY(y)*(_map_height - 1) + (j*(_map_height - 1)));
			glCallList(baseTerrain + 4);
			glPopMatrix();
		}
	}
}
void Terrain::TileTerrain(GLint levelOfDetail) { //levelOfDetail�Ķ���͸� �޾� �� Ÿ���� �׸��� �Լ�
	glEnable(GL_TEXTURE_2D); //�ؽ���Ȱ��ȭ
	glMatrixMode(GL_MODELVIEW);//�𵨺����

	glPushMatrix();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);//�ؽ���ȯ�漳��
	glBindTexture(GL_TEXTURE_2D, _texId_ground); //�����ؽ��� id�� ����Ű�� �ؽ��ķ� �Ʒ� ������ ����
	float coordX = (float)1.0 / _map_width; // �����̵�����
	float coordZ = (float)1.0 / _map_height;// �����̵�����
	for (int z = 0; z<_map_height; z += levelOfDetail) { // x,z���� �������� �׸� ������ �ٴڿ� �����ϱ� ����
		glBegin(GL_TRIANGLE_STRIP);// ���ӻﰢ�� �׸���� �ؽ��ĸ���
		for (int x = 0; x<_map_width; x += levelOfDetail) {
			glTexCoord2f(coordX*x, coordZ*z); // �ؽ���������
			glVertex3f(x, _height[z*_map_height + x], z);// ��ü���� ����
			glTexCoord2f(coordX*x, coordZ*(z + levelOfDetail));// �ؽ���������
			glVertex3f(x, _height[(z + levelOfDetail)*_map_height + x], z + levelOfDetail);// ��ü���� ����
		}
		glEnd();
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void Terrain::RenderWater() {//���׸���
	glEnable(GL_TEXTURE_2D);// �ؽ���Ȱ��ȭ
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//�ؽ���ȯ�漳��
																//glPushAttrib(GL_ENABLE_BIT);

	glEnable(GL_BLEND);//����Ȱ��ȭ
	glDisable(GL_CULL_FACE);//�ĸ����ź�Ȱ��ȭ
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//�����������
	glColor4f(0.0, 0.4, 0.5, 0.7);

	glBindTexture(GL_TEXTURE_2D, _texId_water);// �ؽ��ĸ���
	glBegin(GL_QUADS);// �簢�������� �׸���
	glTexCoord2f(0.0, 0.0);	glVertex3f(0.0, waterLevel, 0.0);// ���� ������ �����ϰ� ��������
	glTexCoord2f(15.0, 0.0);	glVertex3f(0.0, waterLevel, _map_height);
	glTexCoord2f(15.0, 15.0); glVertex3f(_map_height, waterLevel, _map_height);
	glTexCoord2f(0.0, 15.0);	glVertex3f(_map_height, waterLevel, 0.0);
	glEnd();
	glEnable(GL_CULL_FACE);// �ĸ�����Ȱ��ȭ
	glDisable(GL_BLEND);// ���� ��Ȱ��ȭ
						//glPopAttrib();
	glDisable(GL_TEXTURE_2D);
}
// raw ������ �д� �Լ�
// filename: ���̸� ������ ���
// w : ���̸��� ���� ũ��
// h : ���̸��� ���� ũ��
Terrain::ubyte* Terrain::readRawData(pcStr filename, uint w, uint h) {
	FILE *f;
	fopen_s(&f, filename, "rb"); // �������̵����� raw���Ͽ���
	if (!f) {//������ �������
		printf("file not found : %s\n", filename);
		throw std::exception();
	}
	// ����� ���� raw ������ ���,"������ ũ��=�̹����� �ȼ���"�̱�
	// ������ width*height ũ�� ��ŭ�� ������ �Ҵ��ϸ� ��� �����͸� �о�� �� ����
	ubyte* raw = new ubyte[w*h];// raw�� ũ�⸦ ���
	fread(raw, sizeof(ubyte), w*h, f);// ���̵������о����
	fclose(f);
	return raw;
}
GLfloat Terrain::getHeight(GLfloat _x, GLfloat _y) {
	//�ؽ����̹����� �ش� _x,_y���� ���� �����͸� ������ ���� �Լ�
	int x, y;
	if (_x<0) x = _map_width + ((int)_x%_map_width);//�ؽ����̹��������� �Ѿ�� �ݴ�����ǥ��
	else x = ((int)_x % _map_width);
	if (_y<0) y = _map_height + ((int)_y % _map_height);
	else y = ((int)_y % _map_height);
	// ��ǥ���� �ؽ���ũ��ȿ� ������Ŵ
	// ���� �����ʹ� 1�����迭�� ����Ǿ� �����Ƿ� ���ó��
	return (_height[y*_map_height + x] / 5 + 10);// �ش���ġ�� ���̰� ��ȯ
}
GLint Terrain::getTileX(GLfloat x) {// ����ī�޶� ��ġ�ϰ� �ִ� �ؽ��� Ÿ���� ����ǥ�� ��ȯ : x��ǥ
	if (x>0) return ((int)x) / (int)_map_width;
	else return ((int)x) / (int)_map_width - 1;
}
GLint Terrain::getTileY(GLfloat y) {//// ����ī�޶� ��ġ�ϰ� �ִ� �ؽ��� Ÿ���� ����ǥ�� ��ȯ : y��ǥ
	if (y>0) return ((int)y) / (int)_map_height;
	else return ((int)y) / (int)_map_height - 1;
}