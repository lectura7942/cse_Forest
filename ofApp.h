#pragma once

#include "ofMain.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

#define FLOWERRAD 5 // 꽃을 표현한 원의 반지름을 설정한 값.

enum MODE { // mode 변수의 값
	MODE_TRUNK_START=0, /* 나무 몸통을 자라게 하고 있음을 뜻한다. */
	MODE_TRUNK_END, /* 나무 몸통을 자라게 하고 있지 않음을 뜻한다. */
};

struct Branch { // 나뭇가지 정보를 저장하는 구조체
	pair<double, double> bottom, top; // 나뭇가지 양 끝의 좌표
	double length; // 나뭇가지의 길이
	int direction; /* 나뭇가지가 자라는 방향
				   1 : 위, 2 : 아래, 3 : 왼쪽, 4 : 오른쪽 */
	int end = 0; // 1: 마지막 나뭇가지(리프 노드)
	Branch* left = NULL; // 왼쪽 자식 나뭇가지를 가리킴
	Branch* right = NULL; // 오른쪽 자식 나뭇가지를 가리킴
};

struct Node { // 연결 리스트의 노드 구조체
	Branch* tree; // 나무 정보를 저장할 이진 트리의 루트 노드
	int bloom_check[4]; /* 꽃 정보를 저장할 배열 
						[0]: 값이 1이면 꽃이 핌을 뜻함
						[1]~[3]: 꽃의 색을 저장 */
	Node* left; // 왼쪽 노드를 가리킴
	Node* right; // 오른쪽 노드를 가리킴
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		Node* head; // 나무 연결리스트의 헤드 노드를 가리키는 포인터
		int tree_count; // 나무 개수
		int screenHeight = ofGetHeight(); // 화면 높이
		int screenWidth = ofGetWidth(); // 화면 너비
		int mode; // 나무 몸통을 자라게 하고 있는지 여부를 저장
		int trunk_length; // 자라게 하고 있는 나무 몸통 길이
		int mouse_x; // 나무 몸통의 x 좌표
		Node* current_node; // 선택된 나무 노드를 가리키는 포인터
		int watch; /* 감상 모드인지 아닌지를 저장
				   0: 선택된 나무를 갈색으로 표시
				   1: 아무 나무도 선택하지 않음(감상 모드)
				   */

		double RandomDouble(double a, double b); // a 에서 b 사이 임의의 정수를 반환한다.

		void GrowTree(double x, double len); // 나무를 생성한다.
		void GrowBranch(Branch** b); // 나뭇가지를 생성한다.
		void DeleteBranch(Branch** b); // 나뭇가지를 삭제한다.
		void DrawBranch(Branch* b); // 나뭇가지를 그린다.
		void DrawFlower(Branch* b); // 나뭇가지 끝에 꽃을 그린다.

		void InsertNode(Branch** b, int bc[4]); // 연결리스트에 노드를 넣는다.
		void DeleteNode(Node** cur); // 연결리스트에서 노드를 삭제한다.
		void EraseList(); // 연결리스트 전체를 삭제한다.

};
