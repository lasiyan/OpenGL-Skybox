# OpenGL-Skybox

Making SkyBox using OpenGL(C language) in VisualStudio 2015

본 프로젝트는 openGL을 활용한 SkyBox를 모델링하고 그것을 바탕으로 가상의 비행기 시뮬레이션을 구현한다.

## 개발 환경

* OS : Windows 10 Pro 64bit
* CPU : Intel(R) Core(TM) i5-4590 CPU @ 3.30GHz 3.30 GHz
* RAM : 8.00GB
* IDE : Microsoft Visual Studio Community 2015

## 사용 기술

C++, OpenGL API

## 부가 설명

 먼저 코드를 모듈화하여 실행을 수행할 main.cpp, 시점 뷰포트를 설정할 camera.cpp, 스카이 박스를 구현하여 3D모델링을 수행할 skybox.cpp, 마지막으로 지형 지물을 출력하고 Skybox에 설정을 담당하는 terrain.cpp로 분리하였다

camera에서는 카메라 클래스를 활용하여 카메라의 위치, 바라보는 지점, 카메라의 업벡터 등을 지정하고 전반적인 화면 구성값을 setShape 함수를 통해 설정한다. 이후 현재 정의된 시점 좌표계를 모델 뷰 행렬에 적용하며 시점좌표계의 기저, angle 각도의 회전, 설정한 벡터를 정규화 하는 과정을 통해 시점을 변환시키고 제어한다.

Skybox에서는 debug 파일의 textures파일들을 불러오고 skybox의 깊이 정보 설정, 다른 색과 별도의 영향을 설정할 glTexEnvf의 GL_DECAL을 설정한다. 이 후 각각의 bmp파일(front, back, left, right, up, down)에 대한 Texture을 설정한다.

## 실행 과정

1. https://drive.google.com/file/d/0B3wz4RsRRCj8WVZMMlJRckhxVTA/view
위 주소로 이동해 openGL API download

* Windows Visual 2015를 기준으로 .h 헤더 파일은 
* C:\Program Files (x86)\Windows Kits\8.1\Include\um\gl 에 복사
* lib 파일은
* C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86 에 복사
* .dll 파일은 C:\Windows\System32 에 복사
* 64bit OS의 경우 C:\Windows\SysWOW64 에도 복사해준다

2. 프로젝트 속성 -> 링커 -> 입력 -> 추가종속 에
glu32.lib
glut32.lib
opengl32.lib
glaux.lib 를 입력

3. 파일 실행

* W: 전진 S: 후진 A: 좌향이동 D: 우향이동
* JL: roll
* KI: pitch
* QE: yaw

1,2: solid, wire rendering

## 실행 결과

[![Demo Doccou alpha](http://j.gifs.com/66y8nl.gif)](https://www.youtube.com/watch?v=W_LtUTkPhHc&feature=youtu.be)

(이미지 클릭시 유튜브로 이동합니다)
