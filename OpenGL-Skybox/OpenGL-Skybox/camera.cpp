#include "camera.h"

Camera::Camera(void) {
	setDefaultCamera();//생성자, 초기화함수 별도 호출
}
void Camera::setDefaultCamera(void) {// 초기화함수
	setShape(45.0f, 640 / (float)480, 0.1f, 200.0f);//화면구성값 정의 fov,aspect,near clip,far clip
	Point3 eyePoint = Point3(10.0, 0.0, 0.0); // 카메라위치 지정
	Point3 lookPoint = Point3(0.0, 0.0, 0.0); // 바라보는 지점 지정
	Vector3 upVector = Vector3(0, 1, 0); // 카메라의 업벡터 지정
	set(eyePoint, lookPoint, upVector);
}
void Camera::set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ) {
	Point3 Eye = Point3(eyeX, eyeY, eyeZ); //카메라, 시점, 업벡터 정의
	Point3 look = Point3(lookX, lookY, lookZ);
	Vector3 up = Vector3(upX, upY, upZ);
	eye.set(Eye);//카메라위치정의
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);//시점과 카메라좌표를 빼서 시축(optical axis)구성
	u.set(up.cross(n)); // 카메라 업벡터와의 내적으로 u구성
	v.set(n.cross(u)); // u와 n의 내적으로 시점좌표계의 y축
	u.normalize(); v.normalize(); n.normalize();// 세 시점좌표계 축들을 정규화 이로써 카메라 시공간의 정규직교기저가 완성됨
	setModelViewMatrix();// 현재 정의된 시점좌표계를 모델뷰행렬에 적용
}
void Camera::set(Point3 Eye, Point3 look, Vector3 up) { //위의 set함수의 오버라이딩 같은기능이다.
	eye.set(Eye);
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	u.set(up.cross(n));
	v.set(n.cross(u));
	u.normalize(); v.normalize(); n.normalize();
	setModelViewMatrix();
}
void Camera::setModelViewMatrix(void) {
	float m[16];// 모델뷰행렬에 집어넣을 행렬값 동차좌표계 변환행렬로서 4x4=16개의 원소를 가짐
	Vector3 eVec(eye.x, eye.y, eye.z);//카메라위치
									  // 모델뷰행렬의 회전변환과 이동변환적용
	m[0] = u.x;	m[4] = u.y;	m[8] = u.z;	m[12] = -eVec.dot(u);
	m[1] = v.x;	m[5] = v.y;	m[9] = v.z;	m[13] = -eVec.dot(v);
	m[2] = n.x;	m[6] = n.y;	m[10] = n.z;	m[14] = -eVec.dot(n);
	m[3] = 0;		m[7] = 0;		m[11] = 0;	m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);//행렬m을 모델뷰행렬에 넣어준다
}
void Camera::setShape(float vAngle, float asp, float nr, float fr) {//투상을 정의한다.
	viewAngle = vAngle;//시야각
	aspect = asp;//종횡비
	nearDist = nr;//전방투상면
	farDist = fr;//후방투상면
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();// 투상행렬을 새로정의하기위해 단위행렬
	gluPerspective(viewAngle, aspect, nearDist, farDist);//원근투상정의
	glMatrixMode(GL_MODELVIEW);
}
void Camera::setAspect(float asp) {
	aspect = asp;// 종횡비만새로정의
}
void Camera::getShape(float& vAngle, float& asp, float& nr, float& fr) {
	vAngle = viewAngle;//원근투상의 구성값 반환
	asp = aspect;
	nr = nearDist;
	fr = farDist;
}
void Camera::slide(double du, double dv, double dn) {//이동변환
	eye.x += du*u.x + dv*v.x + dn*n.x;//시점좌표축의 정규직교기저에 이동변환행렬을 곱한다 변환된 결과를 모델뷰행렬에 적용
	eye.y += du*u.y + dv*v.y + dn*n.y;//시점좌표계가 이동한 효과 // Emmanuel Agu자료15페이지 참고
	eye.z += du*u.z + dv*v.z + dn*n.z;
	setModelViewMatrix();
}
void Camera::rotAxes(Vector3& a, Vector3& b, float angle) { //시점좌표계를 이루는 기저를 기울여 회전변환// Emmanuel Agu자료 21페이지 참고
	float ang = 3.14159265 / 180 * angle;//각을 라디안단위로 변환한다. cos, sin 함수를 이용하기 위해서
	float C = cos(ang), S = sin(ang); // 변환하려는 각의 cos, sin 값 구함
	Vector3 t(C*a.x + S*b.x, C*a.y + S*b.y, C*a.z + S*b.z); b.set(-S*a.x + C*b.x, -S*a.y + C*b.y, -S*a.z + C*b.z);// 이를 이용해 두 축을 angle만큼 회전한다
	a.set(t.x, t.y, t.z);
}
void Camera::roll(float angle) {
	rotAxes(u, v, angle);//n축중심의 회전 나머지 u,v축이 angle각도만큼 회전한다
	setModelViewMatrix();//모델뷰행렬에 적용
}
void Camera::pitch(float angle) {
	rotAxes(n, v, angle);////u축중심의 회전 나머지 n,v축이 angle각도만큼 회전한다
	setModelViewMatrix();//모델뷰행렬에 적용
}
void Camera::yaw(float angle) {
	rotAxes(u, n, angle);//v축중심의 회전 나머지 u,n축이 angle각도만큼 회전한다
	setModelViewMatrix();//모델뷰행렬에 적용
}
Vector3 Vector3::cross(Vector3 b) {
	Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	return c;//메소드호출벡터와 파라미터벡터의 외적벡터를 반환하는 함수
}
float Vector3::dot(Vector3 b) { return x*b.x + y*b.y + z*b.z; }//두 벡터의 내적
void Vector3::normalize() {//해당벡터를 정규화하는 함수
	double sizeSq = x*x + y*y + z*z;// 크기를 구하고
	if (sizeSq<0.0000001) {
		//cerr << "\nnormalize() sees vector (0,0,0)!";
		return;// does nothing to zero vectors;
	}
	float scaleFactor = 1.0 / (float)sqrt(sizeSq);// 크기로 나눠준다
	x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;// 나눠준 비율을 기저에 적용하여 정규화
}