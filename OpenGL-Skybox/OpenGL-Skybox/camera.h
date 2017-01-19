/* camera.h : 점과 벡터, 카메라 클래스를 구성하는 소스 */
#include <windows.h>
#include <strstream>
#include <math.h>
#include <gl/glut.h>

class Point3 { //정점클래스
public:
	float x, y, z;//좌표
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }//정점좌표변경메소드
	void set(Point3& p) { x = p.x; y = p.y; z = p.z; }//정점좌표변경메소드2 다른정점 대입
	Point3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }//생성자: 정점초기화
	Point3() { x = y = z = 0; }//생성자: 초기화없을시 원점
};
class Vector3 {
public:
	float x, y, z;//벡터성분
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }//벡터생성메소드
	void set(Vector3& v) { x = v.x; y = v.y; z = v.z; }//벡터생성메소드
	void flip() { x = -x; y = -y; z = -z; } //반대방향벡터
	void setDiff(Point3& a, Point3& b)//두벡터의차
	{
		x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;
	}
	void normalize();//단위벡터로
	Vector3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; } //생성자
	Vector3(Vector3& v) { x = v.x; y = v.y; z = v.z; }
	Vector3() { x = y = z = 0.0; }
	Vector3 cross(Vector3 b);//외적
	float dot(Vector3 b);//내적
};
class Camera {
public:
	Point3 eye;//시점좌표계원점
	Vector3 u, v, n;//시점좌표계를 구성하는 정규직교기저
	float aspect, nearDist, farDist, viewAngle;//gluPerspective의 파라미터들 시점변환후 이용
	void setModelViewMatrix();// 시점변환에 따라 모델뷰행렬조정
	Camera(void); // 생성자

				  // 비행시뮬레이터의 회전 변환함수들 roll pitch yaw
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	// 이동변환
	void slide(double du, double dv, double dn);

	//카메라위치, 시점,카메라업벡터 지정
	void set(Point3 Eye, Point3 look, Vector3 up); // 벡터로
	void set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ); //점으로

	void setShape(float viewAngle, float aspect, float Near, float Far); //화면정의
	void setAspect(float asp); // 종횡비정의
	void getShape(float& viewAngle, float& aspect, float& Near, float& Far); //화면구성값보기
	void rotAxes(Vector3& a, Vector3& b, float angle);//시점좌표계축회전
	void setDefaultCamera();// 카메라초기설정함수
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