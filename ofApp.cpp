#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(15); // 프레임 속도를 1초에 15 프레임으로 설정한다.
	ofBackground(ofColor::black); // 배경을 검은색으로 설정한다.
	srand(time(NULL)); // 난수 생성을 위해 시드를 현재 시각으로 설정한다.

	// 변수들을 초기화한다.
	head = (Node*)malloc(sizeof(Node)); // 연결 리스트 헤드 노드에 메모리를 할당한다.
	head->left = head->right = head; // 원형으로 연결한다.
	current_node = head;
	watch = 1;
	tree_count = 0;
	mode = MODE_TRUNK_END;
	printf("PROJECT: [FOREST]\t\tMADE BY [20201551 권지은]\n"); 
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	/*
		기능: 나무 몸통의 위치와 길이를 정하고 나무를 생성한다. 화면에 나무를 그린다.
	*/
	ofSetColor(ofColor::white);
	switch (mode) {
	case MODE_TRUNK_START: // 나무 몸통을 자라나게 하고 있을 때
	{
		trunk_length += 10; // 프레임마다( draw()가 호출될 때마다) 값이 10씩 증가한다.
		if (trunk_length >= screenHeight) trunk_length = screenHeight - 1; // 나무 몸통의 길이는 스크린 높이를 넘지 않는다.
		ofDrawLine(mouse_x, screenHeight - 1, mouse_x, screenHeight - trunk_length - 1); // 나무 몸통을 화면에 갱신한다.
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
			// 왼쪽 마우스 버튼을 누르면 현재 갱신된 trunk_length를 길이로 몸통 길이로 하는 나무를 생성한다.
			printf("A tree has been grown.\n\tX-Positon of Trunk Bottom: %d\n\tTrunk Length: %d\n", mouse_x, trunk_length);
			GrowTree(mouse_x, trunk_length);
			mode = MODE_TRUNK_END; // 나무 몸통을 자라나게 하는 것을 멈춘다.
		}
		break;
	}
	case MODE_TRUNK_END: // 나무 몸통을 자라나게 하고 있지 않을 때
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
			mode = MODE_TRUNK_START; // 나무 몸통을 자라나게 하는 것을 시작한다.
			mouse_x = ofGetMouseX(); // 현재 마우스 위치 밑바닥에서 나무 몸통이 자라게 하도록 x 위치를 저장한다.
			trunk_length = 0; // 나무 몸통의 길이를 0으로 초기화한다.
		}
		break;

	}

	// 나무 정보를 저장한 연결리스트를 탐색하면서 나무를 그린다.
	Node* cur = head->right;
	for (int i = 0; i < tree_count; i++, cur=cur->right) {
		DrawBranch(cur->tree);
		if (cur->bloom_check[0] == 1) { // 해당 나무에 꽃이 피었다고 체크되어 있으면
			// 저장된 색에 따라 꽃을 그린다.
			ofSetColor(cur->bloom_check[1], cur->bloom_check[2], cur->bloom_check[3]);
			DrawFlower(cur->tree);
			ofSetColor(ofColor::white);
		}
	}
	// 나무가 한 개 이상이고, 감상 모드가 아니면 선택된 나무를 갈색으로 그린다.
	if (tree_count > 0 && watch == 0) {
		ofSetColor(ofColor::brown);
		DrawBranch(current_node->tree);
		if (current_node->bloom_check[0] == 1) {
			DrawFlower(current_node->tree);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'q' || key == 'Q') { 
		// 연결 리스트를 삭제하고 프로그램을 종료한다.
		EraseList(); 
		free(head);
		cout << "Dynamically allocated memory has been freed\n";
		_Exit(0); 
	}
	if (key == 'b' || key == 'B') { 
		// 선택된 나무에 꽃을 피우거나 꽃을 없앤다.
		if (tree_count == 0 || watch == 1) {
			// 나무가 없거나 선택된 나무가 없으면(감상 모드) 아무것도 하지 않는다.
			cout << "Tree has not been selected.\n";
			return;
		}
		if (current_node->bloom_check[0] == 0) {
			// 선택된 나무에 꽃이 피어있지 않으면 꽃이 피어있다고 값을 변경한다.
			current_node->bloom_check[0] = 1;
			printf("Flower of following tree has bloomed.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		}
		else {
			// 선택된 나무에 꽃이 피어있으면 꽃이 피어있지 않다는 값으로 변경한다.
			current_node->bloom_check[0] = 0;
			printf("Flower of following tree has fallen.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		}
		if (current_node->bloom_check[0] == 1) {
			// 선택된 나무에 꽃이 피어 있으면 랜덤으로 꽃의 색을 정한다.
			current_node->bloom_check[1] = RandomDouble(0, 255);
			current_node->bloom_check[2] = RandomDouble(0, 255);
			current_node->bloom_check[3] = RandomDouble(0, 255);
		}
	}
	if (key == 'a' || key == 'A') { 
		// 모든 나무에 랜덤으로 정한 색의 꽃을 피운다.
		if (tree_count == 0) {
			cout << "There are no trees\n";
			return;
		}
		else cout << "Flowers of all trees have bloomed\n";
		Node* cur = head->right;
		for (int i = 0; i < tree_count; i++, cur=cur->right) {
			cur->bloom_check[0] = 1;
			cur->bloom_check[1] = RandomDouble(0, 255);
			cur->bloom_check[2] = RandomDouble(0, 255);
			cur->bloom_check[3] = RandomDouble(0, 255);
		}
	}
	if (key == 'e' || key == 'E') { // 선택한 나무를 지운다.
		if (tree_count == 0 || watch == 1) {
			cout << "Tree has not been selected.\n";
			return;
		}
		printf("Following tree has been erased.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		DeleteNode(&current_node);
	}
	if (key == 'd' || key == 'D') { // 모든 나무의 꽃을 없앤다.
		if (tree_count == 0) {
			cout << "There are no trees.\n";
			return;
		}
		else cout << "Flowers of all trees have fallen.\n";
		Node* cur = head->right;
		for (int i = 0; i < tree_count; i++, cur=cur->right) {
			cur->bloom_check[0] = 0;
		}
	}
	if (key == 's' || key == 'S') { // 화면에 그려진 모든 나무를 지운다.
		cout << "The screen has been cleared.\n";
		EraseList();
		watch = 1;
		mode = MODE_TRUNK_END;
		trunk_length = 0;
	}
	if (key == 'r' || key == 'R') { // 임의의 나무를 생성한다. 나무의 x 좌표와 몸통의 길이를 랜덤하게 정한다.
		double x = RandomDouble(20, screenHeight - 20);
		double len = RandomDouble(50, screenHeight - 50);
		GrowTree(x,len );
		printf("A random tree has been grown.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", x, len);
		mode = MODE_TRUNK_END;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (tree_count > 0) { 	// 나무가 하나 이상 있어야지 입력받은 키에 반응한다.
		if (key == OF_KEY_RIGHT) { // 오른쪽 화살표 키를 입력받으면 오른쪽 나무를 선택한다. 
			watch = 0;
			current_node = current_node->right;
			if (current_node == head) current_node = current_node->right;
			printf("Following tree has been selected.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		}
		if (key == OF_KEY_LEFT) { // 왼쪽 화살표 키를 입력받으면 왼쪽 나무를 선택한다.
			watch = 0;
			current_node = current_node->left;
			if (current_node == head) current_node = current_node->left;
			printf("Following tree has been selected.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		}
		if (key == OF_KEY_DOWN) { // 아래쪽 화살표 키를 입력받으면 나무를 선택하지 않도록 한다.(감상모드)
			watch = 1;
		}
		if (key == OF_KEY_UP) { // 위쪽 화살표 키를 입력받으면
			watch = 0;
			printf("Following tree has been selected.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

double ofApp::RandomDouble(double a, double b) {
	/*
		입력:	double a: 하한 범위
				double b: 상한 범위
		기능: [a,b] 구간에 있는 임의의 정수를 반환한다.
	*/
	int tmp = b + 1 - a;
	return (double)(rand() % tmp) + a;
}

void ofApp::GrowTree(double x, double len) {
	/* 
	* 입력:
	*	double x: 나무 몸통의 x 좌표
	*	double len: 나무 몸통의 길이
	* 기능: 나무의 나뭇가지 정보를 저장하는 이진 트리를 생성한다.
	*/

	// 나무의 몸통을 저장하는 노드를 생성한다.
	Branch* trunk = (Branch*)malloc(sizeof(Branch));
	trunk->bottom = { x,screenHeight - 1 };
	trunk->length = len;
	double n = trunk->bottom.second - trunk->length;
	trunk->top = { trunk->bottom.first, n };
	trunk->direction = 1; // 나무 몸통은 위쪽 방향을 향한다.
	trunk->end = 0;
	GrowBranch(&trunk); // 나뭇가지를 뻗는다.
	int bc[4] = { 0,0,0,0 };
	InsertNode(&trunk, bc); // 연결리스트에 나무 정보를 넣는다.
}

void ofApp::GrowBranch(Branch** b) {
	/*
		입력:	Branch** b: 현재 나뭇가지
		기능: 현재 나뭇가지에 대해 왼쪽, 오른쪽 자식 나뭇가지(노드)를 생성한다.
	*/
	double len = (*b)->length;
	if (len / 2 <= FLOWERRAD) { // 자식 나뭇가지의 길이가 꽃 반지름 이하이면 리프 노드이다.
		(*b)->end = 1;
		return;
	}
	double bx = (*b)->bottom.first;
	double by = (*b)->bottom.second;
	double tx = (*b)->top.first;
	double ty = (*b)->top.second;

	// 왼쪽 및 오른쪽 자식 나뭇가지 노드를 생성한다.
	// 자식 나뭇가지의 길이는 현재 나뭇가지 길이의 반이다.
	// 자식 나뭇가지 밑 좌표는 현재 나뭇가지를 표현하는 선분 위 절반 부분에서 랜덤으로 정한다.
	double n;
	(*b)->left = (Branch*)malloc(sizeof(Branch));
	(*b)->right = (Branch*)malloc(sizeof(Branch));
	(*b)->left->length = (*b)->right->length = (*b)->length / 2;
	(*b)->left->end = (*b)->right->end = 0;
	switch ((*b)->direction) {
	case 1: /* 현재 나뭇가지 방향: 위(1)
				오른쪽 자식 나뭇가지 방향: 왼쪽(3)
				왼쪽 자식 나뭇가지 방향: 오른쪽(4)
			*/
		(*b)->left->direction = 3;
		(*b)->right->direction = 4;
		(*b)->left->bottom = { bx, RandomDouble(ty+FLOWERRAD, (by+ty)/2) };
		(*b)->right->bottom = { bx, RandomDouble(ty + FLOWERRAD, (by + ty) / 2) };

		n = (*b)->left->bottom.first - (*b)->length / 2;
		(*b)->left->top = { n, (*b)->left->bottom.second };

		n = (*b)->right->bottom.first + (*b)->length / 2;
		(*b)->right->top = { n, (*b)->right->bottom.second };
		break;
	case 2: /* 현재 나뭇가지 방향: 아래(2)
				오른쪽 자식 나뭇가지 방향: 오른쪽(4)
				왼쪽 자식 나뭇가지 방향: 왼쪽(3)
			*/
		(*b)->left->direction = 4;
		(*b)->right->direction = 3;
		(*b)->left->bottom = { bx, RandomDouble((ty+by)/2, ty-FLOWERRAD) };
		(*b)->right->bottom = { bx, RandomDouble((ty + by) / 2, ty-FLOWERRAD) };

		n = (*b)->left->bottom.first + (*b)->length / 2;
		(*b)->left->top = { n, (*b)->left->bottom.second };

		n = (*b)->right->bottom.first - (*b)->length / 2;
		(*b)->right->top = { n, (*b)->right->bottom.second };
		break;
	case 3: /* 현재 나뭇가지 방향: 왼쪽(3)
				오른쪽 자식 나뭇가지 방향: 아래(2)
				왼쪽 자식 나뭇가지 방향: 위(1)
			*/
		(*b)->left->direction = 2;
		(*b)->right->direction = 1;
		(*b)->left->bottom = { RandomDouble(tx+FLOWERRAD, (tx+bx)/2),by };
		(*b)->right->bottom = { RandomDouble(tx + FLOWERRAD, (tx + bx) / 2) , by };

		n = (*b)->left->bottom.second + (*b)->length / 2;
		(*b)->left->top = { (*b)->left->bottom.first, n };

		n = (*b)->right->bottom.second - (*b)->length / 2;
		(*b)->right->top = { (*b)->right->bottom.first, n };
		break;
	case 4: /* 현재 나뭇가지 방향: 오른쪽(4)
				오른쪽 자식 나뭇가지 방향: 위(1)
				왼쪽 자식 나뭇가지 방향: 아래(2)
			*/
		(*b)->left->direction = 1;
		(*b)->right->direction = 2;
		(*b)->left->bottom = { RandomDouble((bx+tx)/2, tx-FLOWERRAD),by };
		(*b)->right->bottom = { RandomDouble((bx + tx) / 2, tx - FLOWERRAD) , by };

		n = (*b)->left->bottom.second - (*b)->length / 2;
		(*b)->left->top = { (*b)->left->bottom.first, n };

		n = (*b)->right->bottom.second + (*b)->length / 2;
		(*b)->right->top = { (*b)->right->bottom.first, n };
		break;
	}
	// 왼쪽 및 오른쪽 자식 나뭇가지에 대해 함수를 재귀적으로 호출한다.
	GrowBranch(&((*b)->left));
	GrowBranch(&((*b)->right));
}

void ofApp::DrawBranch(Branch* b) {
	/*
		입력:	Branch* b: 현재 나뭇가지
		기능: 나뭇가지를 화면에 그린다.
	*/
	ofDrawLine(b->bottom.first, b->bottom.second, b->top.first, b->top.second); // 선분으로 표현되는 나뭇가지를 화면에 그린다.
	if (b->end == 1) return; // 자식 나뭇가지가 없으면 함수를 종료한다.
	// 자식 나뭇가지에 대해 함수를 재귀적으로 호출한다.
	DrawBranch(b->left); 
	DrawBranch(b->right);
}

void ofApp::DeleteBranch(Branch** b) {
	/*
		입력:	Branch** b: 삭제할 나뭇가지
		기능:	나뭇가지 노드의 메모리를 해제한다.
	*/
	if ((*b)->end == 0) { // 리프 노드(자식 나뭇가지가 없을 때)가 아닐 때
		DeleteBranch(&((*b)->left)); // 왼쪽 자식 나뭇가지를 삭제한다.
		DeleteBranch(&((*b)->right)); // 오른쪽 자식 나뭇가지를 삭제한다.
	}
	free(*b); 
}

void ofApp::DrawFlower(Branch* b) {
	/*
		입력:	Branch* b: 나뭇가지
		기능: 나뭇가지에 꽃을 그린다.
	*/
	ofDrawCircle(b->top.first, b->top.second, FLOWERRAD); // 나뭇가지 위 끝에 FLOWERRAD를 반지름으로 하는 원으로 표현되는 꽃을 화면에 그린다.
	if (b->end == 1) {
		return;
	}
	// 두 자식 나뭇가지에 대해 함수를 재귀적으로 호출한다.
	DrawFlower(b->left);
	DrawFlower(b->right);
}

void ofApp::InsertNode(Branch** b, int bc[4]) {
	/* 
	* 입력:	Branch** b: 현재 나뭇가지
	*		int bc[4]: 현재 나뭇가지의 꽃 정보
	* 기능: 현재 나뭇가지와 꽃 정보를 저장하는 노드를 생성하고 연결 리스트에 넣는다.
	*/
	Node* newnode = (Node*)malloc(sizeof(Node));
	newnode->tree = *b;
	for (int i = 0; i < 4; i++) newnode->bloom_check[i] = bc[i];
	Node* pre = head; // pre 다음에 새로운 노드를 넣는다.
	int x = (*b)->top.first;
	int y = (*b)->top.second;
	// 정렬을 유지하기 위한 위치를 찾는다.
	while (pre->right != head) {
		if (pre->right->tree->top.first > x) break;
		if (pre->right->tree->top.first == x && pre->right->tree->top.second > y) break;
		pre = pre->right;
	}
	newnode->right = pre->right;
	newnode->left = pre;
	pre->right->left = newnode;
	pre->right = newnode;
	current_node = newnode;
	tree_count++;
}

void ofApp::DeleteNode(Node** cur) { 
	/*
	* 입력:	Node** cur: 삭제할 노드
	* 기능: 노드를 연결 리스트에서 삭제한다.
	*/
	Node* pre = (*cur)->left;
	pre->right = (*cur)->right;
	(*cur)->right->left = pre;
	DeleteBranch(&((*cur)->tree));
	free(*cur);
	current_node = pre;
	if (current_node == head) current_node = current_node->left;
	tree_count--;
}

void ofApp::EraseList() { 
	/* 기능: 전체 연결 리스트를 삭제한다. (헤드 노드 제외) */ 
	Node* del;
	Node* cur = head->right;
	for (int i = 0; i < tree_count; i++) {
		del = cur;
		cur = cur->right;
		DeleteBranch(&(del->tree));
		free(del);
	}
	tree_count = 0;
	head->right = head->left = head;
	current_node = head;
}