#pragma once

#include "ofMain.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

#define FLOWERRAD 5 // ���� ǥ���� ���� �������� ������ ��.

enum MODE { // mode ������ ��
	MODE_TRUNK_START=0, /* ���� ������ �ڶ�� �ϰ� ������ ���Ѵ�. */
	MODE_TRUNK_END, /* ���� ������ �ڶ�� �ϰ� ���� ������ ���Ѵ�. */
};

struct Branch { // �������� ������ �����ϴ� ����ü
	pair<double, double> bottom, top; // �������� �� ���� ��ǥ
	double length; // ���������� ����
	int direction; /* ���������� �ڶ�� ����
				   1 : ��, 2 : �Ʒ�, 3 : ����, 4 : ������ */
	int end = 0; // 1: ������ ��������(���� ���)
	Branch* left = NULL; // ���� �ڽ� ���������� ����Ŵ
	Branch* right = NULL; // ������ �ڽ� ���������� ����Ŵ
};

struct Node { // ���� ����Ʈ�� ��� ����ü
	Branch* tree; // ���� ������ ������ ���� Ʈ���� ��Ʈ ���
	int bloom_check[4]; /* �� ������ ������ �迭 
						[0]: ���� 1�̸� ���� ���� ����
						[1]~[3]: ���� ���� ���� */
	Node* left; // ���� ��带 ����Ŵ
	Node* right; // ������ ��带 ����Ŵ
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

		Node* head; // ���� ���Ḯ��Ʈ�� ��� ��带 ����Ű�� ������
		int tree_count; // ���� ����
		int screenHeight = ofGetHeight(); // ȭ�� ����
		int screenWidth = ofGetWidth(); // ȭ�� �ʺ�
		int mode; // ���� ������ �ڶ�� �ϰ� �ִ��� ���θ� ����
		int trunk_length; // �ڶ�� �ϰ� �ִ� ���� ���� ����
		int mouse_x; // ���� ������ x ��ǥ
		Node* current_node; // ���õ� ���� ��带 ����Ű�� ������
		int watch; /* ���� ������� �ƴ����� ����
				   0: ���õ� ������ �������� ǥ��
				   1: �ƹ� ������ �������� ����(���� ���)
				   */

		double RandomDouble(double a, double b); // a ���� b ���� ������ ������ ��ȯ�Ѵ�.

		void GrowTree(double x, double len); // ������ �����Ѵ�.
		void GrowBranch(Branch** b); // ���������� �����Ѵ�.
		void DeleteBranch(Branch** b); // ���������� �����Ѵ�.
		void DrawBranch(Branch* b); // ���������� �׸���.
		void DrawFlower(Branch* b); // �������� ���� ���� �׸���.

		void InsertNode(Branch** b, int bc[4]); // ���Ḯ��Ʈ�� ��带 �ִ´�.
		void DeleteNode(Node** cur); // ���Ḯ��Ʈ���� ��带 �����Ѵ�.
		void EraseList(); // ���Ḯ��Ʈ ��ü�� �����Ѵ�.

};
