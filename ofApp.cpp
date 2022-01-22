#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(15); // ������ �ӵ��� 1�ʿ� 15 ���������� �����Ѵ�.
	ofBackground(ofColor::black); // ����� ���������� �����Ѵ�.
	srand(time(NULL)); // ���� ������ ���� �õ带 ���� �ð����� �����Ѵ�.

	// �������� �ʱ�ȭ�Ѵ�.
	head = (Node*)malloc(sizeof(Node)); // ���� ����Ʈ ��� ��忡 �޸𸮸� �Ҵ��Ѵ�.
	head->left = head->right = head; // �������� �����Ѵ�.
	current_node = head;
	watch = 1;
	tree_count = 0;
	mode = MODE_TRUNK_END;
	printf("PROJECT: [FOREST]\t\tMADE BY [20201551 ������]\n"); 
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	/*
		���: ���� ������ ��ġ�� ���̸� ���ϰ� ������ �����Ѵ�. ȭ�鿡 ������ �׸���.
	*/
	ofSetColor(ofColor::white);
	switch (mode) {
	case MODE_TRUNK_START: // ���� ������ �ڶ󳪰� �ϰ� ���� ��
	{
		trunk_length += 10; // �����Ӹ���( draw()�� ȣ��� ������) ���� 10�� �����Ѵ�.
		if (trunk_length >= screenHeight) trunk_length = screenHeight - 1; // ���� ������ ���̴� ��ũ�� ���̸� ���� �ʴ´�.
		ofDrawLine(mouse_x, screenHeight - 1, mouse_x, screenHeight - trunk_length - 1); // ���� ������ ȭ�鿡 �����Ѵ�.
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
			// ���� ���콺 ��ư�� ������ ���� ���ŵ� trunk_length�� ���̷� ���� ���̷� �ϴ� ������ �����Ѵ�.
			printf("A tree has been grown.\n\tX-Positon of Trunk Bottom: %d\n\tTrunk Length: %d\n", mouse_x, trunk_length);
			GrowTree(mouse_x, trunk_length);
			mode = MODE_TRUNK_END; // ���� ������ �ڶ󳪰� �ϴ� ���� �����.
		}
		break;
	}
	case MODE_TRUNK_END: // ���� ������ �ڶ󳪰� �ϰ� ���� ���� ��
		if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
			mode = MODE_TRUNK_START; // ���� ������ �ڶ󳪰� �ϴ� ���� �����Ѵ�.
			mouse_x = ofGetMouseX(); // ���� ���콺 ��ġ �عٴڿ��� ���� ������ �ڶ�� �ϵ��� x ��ġ�� �����Ѵ�.
			trunk_length = 0; // ���� ������ ���̸� 0���� �ʱ�ȭ�Ѵ�.
		}
		break;

	}

	// ���� ������ ������ ���Ḯ��Ʈ�� Ž���ϸ鼭 ������ �׸���.
	Node* cur = head->right;
	for (int i = 0; i < tree_count; i++, cur=cur->right) {
		DrawBranch(cur->tree);
		if (cur->bloom_check[0] == 1) { // �ش� ������ ���� �Ǿ��ٰ� üũ�Ǿ� ������
			// ����� ���� ���� ���� �׸���.
			ofSetColor(cur->bloom_check[1], cur->bloom_check[2], cur->bloom_check[3]);
			DrawFlower(cur->tree);
			ofSetColor(ofColor::white);
		}
	}
	// ������ �� �� �̻��̰�, ���� ��尡 �ƴϸ� ���õ� ������ �������� �׸���.
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
		// ���� ����Ʈ�� �����ϰ� ���α׷��� �����Ѵ�.
		EraseList(); 
		free(head);
		cout << "Dynamically allocated memory has been freed\n";
		_Exit(0); 
	}
	if (key == 'b' || key == 'B') { 
		// ���õ� ������ ���� �ǿ�ų� ���� ���ش�.
		if (tree_count == 0 || watch == 1) {
			// ������ ���ų� ���õ� ������ ������(���� ���) �ƹ��͵� ���� �ʴ´�.
			cout << "Tree has not been selected.\n";
			return;
		}
		if (current_node->bloom_check[0] == 0) {
			// ���õ� ������ ���� �Ǿ����� ������ ���� �Ǿ��ִٰ� ���� �����Ѵ�.
			current_node->bloom_check[0] = 1;
			printf("Flower of following tree has bloomed.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		}
		else {
			// ���õ� ������ ���� �Ǿ������� ���� �Ǿ����� �ʴٴ� ������ �����Ѵ�.
			current_node->bloom_check[0] = 0;
			printf("Flower of following tree has fallen.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		}
		if (current_node->bloom_check[0] == 1) {
			// ���õ� ������ ���� �Ǿ� ������ �������� ���� ���� ���Ѵ�.
			current_node->bloom_check[1] = RandomDouble(0, 255);
			current_node->bloom_check[2] = RandomDouble(0, 255);
			current_node->bloom_check[3] = RandomDouble(0, 255);
		}
	}
	if (key == 'a' || key == 'A') { 
		// ��� ������ �������� ���� ���� ���� �ǿ��.
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
	if (key == 'e' || key == 'E') { // ������ ������ �����.
		if (tree_count == 0 || watch == 1) {
			cout << "Tree has not been selected.\n";
			return;
		}
		printf("Following tree has been erased.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		DeleteNode(&current_node);
	}
	if (key == 'd' || key == 'D') { // ��� ������ ���� ���ش�.
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
	if (key == 's' || key == 'S') { // ȭ�鿡 �׷��� ��� ������ �����.
		cout << "The screen has been cleared.\n";
		EraseList();
		watch = 1;
		mode = MODE_TRUNK_END;
		trunk_length = 0;
	}
	if (key == 'r' || key == 'R') { // ������ ������ �����Ѵ�. ������ x ��ǥ�� ������ ���̸� �����ϰ� ���Ѵ�.
		double x = RandomDouble(20, screenHeight - 20);
		double len = RandomDouble(50, screenHeight - 50);
		GrowTree(x,len );
		printf("A random tree has been grown.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", x, len);
		mode = MODE_TRUNK_END;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (tree_count > 0) { 	// ������ �ϳ� �̻� �־���� �Է¹��� Ű�� �����Ѵ�.
		if (key == OF_KEY_RIGHT) { // ������ ȭ��ǥ Ű�� �Է¹����� ������ ������ �����Ѵ�. 
			watch = 0;
			current_node = current_node->right;
			if (current_node == head) current_node = current_node->right;
			printf("Following tree has been selected.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		}
		if (key == OF_KEY_LEFT) { // ���� ȭ��ǥ Ű�� �Է¹����� ���� ������ �����Ѵ�.
			watch = 0;
			current_node = current_node->left;
			if (current_node == head) current_node = current_node->left;
			printf("Following tree has been selected.\n\tX-Positon of Trunk Bottom: %lf\n\tTrunk Length: %lf\n", current_node->tree->bottom.first, current_node->tree->length);
		}
		if (key == OF_KEY_DOWN) { // �Ʒ��� ȭ��ǥ Ű�� �Է¹����� ������ �������� �ʵ��� �Ѵ�.(������)
			watch = 1;
		}
		if (key == OF_KEY_UP) { // ���� ȭ��ǥ Ű�� �Է¹�����
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
		�Է�:	double a: ���� ����
				double b: ���� ����
		���: [a,b] ������ �ִ� ������ ������ ��ȯ�Ѵ�.
	*/
	int tmp = b + 1 - a;
	return (double)(rand() % tmp) + a;
}

void ofApp::GrowTree(double x, double len) {
	/* 
	* �Է�:
	*	double x: ���� ������ x ��ǥ
	*	double len: ���� ������ ����
	* ���: ������ �������� ������ �����ϴ� ���� Ʈ���� �����Ѵ�.
	*/

	// ������ ������ �����ϴ� ��带 �����Ѵ�.
	Branch* trunk = (Branch*)malloc(sizeof(Branch));
	trunk->bottom = { x,screenHeight - 1 };
	trunk->length = len;
	double n = trunk->bottom.second - trunk->length;
	trunk->top = { trunk->bottom.first, n };
	trunk->direction = 1; // ���� ������ ���� ������ ���Ѵ�.
	trunk->end = 0;
	GrowBranch(&trunk); // ���������� ���´�.
	int bc[4] = { 0,0,0,0 };
	InsertNode(&trunk, bc); // ���Ḯ��Ʈ�� ���� ������ �ִ´�.
}

void ofApp::GrowBranch(Branch** b) {
	/*
		�Է�:	Branch** b: ���� ��������
		���: ���� ���������� ���� ����, ������ �ڽ� ��������(���)�� �����Ѵ�.
	*/
	double len = (*b)->length;
	if (len / 2 <= FLOWERRAD) { // �ڽ� ���������� ���̰� �� ������ �����̸� ���� ����̴�.
		(*b)->end = 1;
		return;
	}
	double bx = (*b)->bottom.first;
	double by = (*b)->bottom.second;
	double tx = (*b)->top.first;
	double ty = (*b)->top.second;

	// ���� �� ������ �ڽ� �������� ��带 �����Ѵ�.
	// �ڽ� ���������� ���̴� ���� �������� ������ ���̴�.
	// �ڽ� �������� �� ��ǥ�� ���� ���������� ǥ���ϴ� ���� �� ���� �κп��� �������� ���Ѵ�.
	double n;
	(*b)->left = (Branch*)malloc(sizeof(Branch));
	(*b)->right = (Branch*)malloc(sizeof(Branch));
	(*b)->left->length = (*b)->right->length = (*b)->length / 2;
	(*b)->left->end = (*b)->right->end = 0;
	switch ((*b)->direction) {
	case 1: /* ���� �������� ����: ��(1)
				������ �ڽ� �������� ����: ����(3)
				���� �ڽ� �������� ����: ������(4)
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
	case 2: /* ���� �������� ����: �Ʒ�(2)
				������ �ڽ� �������� ����: ������(4)
				���� �ڽ� �������� ����: ����(3)
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
	case 3: /* ���� �������� ����: ����(3)
				������ �ڽ� �������� ����: �Ʒ�(2)
				���� �ڽ� �������� ����: ��(1)
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
	case 4: /* ���� �������� ����: ������(4)
				������ �ڽ� �������� ����: ��(1)
				���� �ڽ� �������� ����: �Ʒ�(2)
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
	// ���� �� ������ �ڽ� ���������� ���� �Լ��� ��������� ȣ���Ѵ�.
	GrowBranch(&((*b)->left));
	GrowBranch(&((*b)->right));
}

void ofApp::DrawBranch(Branch* b) {
	/*
		�Է�:	Branch* b: ���� ��������
		���: ���������� ȭ�鿡 �׸���.
	*/
	ofDrawLine(b->bottom.first, b->bottom.second, b->top.first, b->top.second); // �������� ǥ���Ǵ� ���������� ȭ�鿡 �׸���.
	if (b->end == 1) return; // �ڽ� ���������� ������ �Լ��� �����Ѵ�.
	// �ڽ� ���������� ���� �Լ��� ��������� ȣ���Ѵ�.
	DrawBranch(b->left); 
	DrawBranch(b->right);
}

void ofApp::DeleteBranch(Branch** b) {
	/*
		�Է�:	Branch** b: ������ ��������
		���:	�������� ����� �޸𸮸� �����Ѵ�.
	*/
	if ((*b)->end == 0) { // ���� ���(�ڽ� ���������� ���� ��)�� �ƴ� ��
		DeleteBranch(&((*b)->left)); // ���� �ڽ� ���������� �����Ѵ�.
		DeleteBranch(&((*b)->right)); // ������ �ڽ� ���������� �����Ѵ�.
	}
	free(*b); 
}

void ofApp::DrawFlower(Branch* b) {
	/*
		�Է�:	Branch* b: ��������
		���: ���������� ���� �׸���.
	*/
	ofDrawCircle(b->top.first, b->top.second, FLOWERRAD); // �������� �� ���� FLOWERRAD�� ���������� �ϴ� ������ ǥ���Ǵ� ���� ȭ�鿡 �׸���.
	if (b->end == 1) {
		return;
	}
	// �� �ڽ� ���������� ���� �Լ��� ��������� ȣ���Ѵ�.
	DrawFlower(b->left);
	DrawFlower(b->right);
}

void ofApp::InsertNode(Branch** b, int bc[4]) {
	/* 
	* �Է�:	Branch** b: ���� ��������
	*		int bc[4]: ���� ���������� �� ����
	* ���: ���� ���������� �� ������ �����ϴ� ��带 �����ϰ� ���� ����Ʈ�� �ִ´�.
	*/
	Node* newnode = (Node*)malloc(sizeof(Node));
	newnode->tree = *b;
	for (int i = 0; i < 4; i++) newnode->bloom_check[i] = bc[i];
	Node* pre = head; // pre ������ ���ο� ��带 �ִ´�.
	int x = (*b)->top.first;
	int y = (*b)->top.second;
	// ������ �����ϱ� ���� ��ġ�� ã�´�.
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
	* �Է�:	Node** cur: ������ ���
	* ���: ��带 ���� ����Ʈ���� �����Ѵ�.
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
	/* ���: ��ü ���� ����Ʈ�� �����Ѵ�. (��� ��� ����) */ 
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