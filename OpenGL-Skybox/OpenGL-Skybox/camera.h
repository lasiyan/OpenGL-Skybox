/* camera.h : ���� ����, ī�޶� Ŭ������ �����ϴ� �ҽ� */
#include <windows.h>
#include <strstream>
#include <math.h>
#include <gl/glut.h>

class Point3 { //����Ŭ����
public:
	float x, y, z;//��ǥ
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }//������ǥ����޼ҵ�
	void set(Point3& p) { x = p.x; y = p.y; z = p.z; }//������ǥ����޼ҵ�2 �ٸ����� ����
	Point3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }//������: �����ʱ�ȭ
	Point3() { x = y = z = 0; }//������: �ʱ�ȭ������ ����
};
class Vector3 {
public:
	float x, y, z;//���ͼ���
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }//���ͻ����޼ҵ�
	void set(Vector3& v) { x = v.x; y = v.y; z = v.z; }//���ͻ����޼ҵ�
	void flip() { x = -x; y = -y; z = -z; } //�ݴ���⺤��
	void setDiff(Point3& a, Point3& b)//�κ�������
	{
		x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;
	}
	void normalize();//�������ͷ�
	Vector3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; } //������
	Vector3(Vector3& v) { x = v.x; y = v.y; z = v.z; }
	Vector3() { x = y = z = 0.0; }
	Vector3 cross(Vector3 b);//����
	float dot(Vector3 b);//����
};
class Camera {
public:
	Point3 eye;//������ǥ�����
	Vector3 u, v, n;//������ǥ�踦 �����ϴ� ������������
	float aspect, nearDist, farDist, viewAngle;//gluPerspective�� �Ķ���͵� ������ȯ�� �̿�
	void setModelViewMatrix();// ������ȯ�� ���� �𵨺��������
	Camera(void); // ������

				  // ����ùķ������� ȸ�� ��ȯ�Լ��� roll pitch yaw
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	// �̵���ȯ
	void slide(double du, double dv, double dn);

	//ī�޶���ġ, ����,ī�޶������ ����
	void set(Point3 Eye, Point3 look, Vector3 up); // ���ͷ�
	void set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ); //������

	void setShape(float viewAngle, float aspect, float Near, float Far); //ȭ������
	void setAspect(float asp); // ��Ⱦ������
	void getShape(float& viewAngle, float& aspect, float& Near, float& Far); //ȭ�鱸��������
	void rotAxes(Vector3& a, Vector3& b, float angle);//������ǥ����ȸ��
	void setDefaultCamera();// ī�޶��ʱ⼳���Լ�
};

/////////////////////////////////////////////////////////////
typedef struct tVector3					// expanded 3D vector struct
{
	tVector3() {}	// constructor
	tVector3(float new_x, float new_y, float new_z) // initialize constructor	 
	{
		x = new_x; y = new_y; z = new_z;
	}
	// overload + operator so that we easier can add vectors
	tVector3 operator+(tVector3 vVector) { return tVector3(vVector.x + x, vVector.y + y, vVector.z + z); }
	// overload - operator that we easier can subtract vectors
	tVector3 operator-(tVector3 vVector) { return tVector3(x - vVector.x, y - vVector.y, z - vVector.z); }
	// overload * operator that we easier can multiply by scalars
	tVector3 operator*(float number) { return tVector3(x*number, y*number, z*number); }
	// overload / operator that we easier can divide by a scalar
	tVector3 operator/(float number) { return tVector3(x / number, y / number, z / number); }

	float x, y, z;						// 3D vector coordinates
}tVector3;

class CCamera
{
public:

	tVector3 mPos;
	tVector3 mView;
	tVector3 mUp;

	//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////
	int mWindowWidth;
	int mWindowHeight;


	// This function let you control the camera with the mouse
	void Mouse_Move();

	//NEW//////////////////NEW//////////////////NEW//////////////////NEW////////////////

	void Rotate_View(float x, float y, float z);
	void Move_Camera(float cameraspeed);
	void Position_Camera(float pos_x, float pos_y, float pos_z,
		float view_x, float view_y, float view_z,
		float up_x, float up_y, float up_z);
};