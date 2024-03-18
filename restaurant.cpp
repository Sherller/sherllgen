#include "main.h"

int MAXSIZE;
class Restaurant;

class HuffmanNode{
public:
	virtual ~HuffmanNode() = default; // Base destructor
	virtual int getWeight() = 0; // Return frequency
	virtual char getChar() = 0;
	virtual bool isLeaf() = 0;
	virtual HuffmanNode* getLeft()const = 0;
	virtual HuffmanNode* getRight()const = 0;
	virtual void setLeft(HuffmanNode* ) = 0;
	virtual void setRight(HuffmanNode* ) = 0;
};

class  LeafNode : public HuffmanNode{
private:
	char value;
	int weight;
public:
	LeafNode(const char& val, int freq){
		this->value = val;
		this->weight = freq;
	}
	~LeafNode() = default;
	int getWeight(){
		return this->weight;
	}
	char getChar(){
		return this->value;
	}
	bool isLeaf(){
		return true;
	}
	HuffmanNode* getLeft()const{
		return nullptr;
	}
	HuffmanNode* getRight()const{
		return nullptr;
	}
	void setLeft(HuffmanNode* ){};
	void setRight(HuffmanNode* ){};
};
class  InternalNode : public HuffmanNode{
private:
	HuffmanNode* left;
	HuffmanNode* right;
	int weight;
public:
	InternalNode(HuffmanNode* left, HuffmanNode* right){
		this->left = left;
		this->right = right;
		this->weight = left->getWeight() + right->getWeight();
	}
	~InternalNode() override {
        delete this->left;
        delete this->right;
    }
	int getWeight(){
		return this->weight;
	}
	char getChar(){	
		return '0';	
	}
	HuffmanNode* getLeft() const{
		return this->left;
	}
	void setLeft(HuffmanNode* left){
		this->left = left;
	}
	HuffmanNode* getRight() const{
		return this->right;
	}
	void setRight(HuffmanNode* right){
		this->right = right;
	}
	bool isLeaf(){
		return false;
	}
};

class HuffmanTree {
private:
	HuffmanNode* root;
	int priority;
public:
	HuffmanTree(char& val, int freq, int priority){
		this->root = new LeafNode(val, freq);
		this->priority = priority;
	}
	~HuffmanTree(){
		delete this->root;
	}

	HuffmanTree(HuffmanTree* left, HuffmanTree* right, int priority){
		this->root = new InternalNode(left->getRoot(), right->getRoot()); 
		this->priority = priority;
	}

	HuffmanNode* getRoot() { 
		return root; 
	}

	void setRoot(HuffmanNode* node){
		this->root = node;
	}

	int getWeight() {
		return this->root->getWeight(); 
	}

	char getChar(){
		return this->root->getChar();
	}

	int getPriority(){
		return this->priority;
	}

	int getHeight(HuffmanNode* node){
        if (node == NULL)
            return 0;
        int lh = this->getHeight(node->getLeft());
        int rh = this->getHeight(node->getRight());
        return (lh > rh ? lh : rh) + 1;
    }

	void printInOrderHuffTree(HuffmanNode* node){
		if (node != nullptr){
			printInOrderHuffTree(node->getLeft());
			if (node->isLeaf()){
				char c = node->getChar();
				cout<<c<<"\n";
			}
			else {
				int w = node->getWeight();
				cout<<w<<"\n";
			}
			printInOrderHuffTree(node->getRight());
		}
	}

	HuffmanNode* rotateRight(HuffmanNode* node) {
		HuffmanNode *temp1 = node->getLeft();
		HuffmanNode *temp2 = temp1->getRight();
		node->setLeft(temp2);
		temp1->setRight(node);
		return temp1;
	}

	HuffmanNode* rotateLeft(HuffmanNode* node) {
		HuffmanNode *temp1 = node->getRight();
		HuffmanNode *temp2 = temp1->getLeft();
		node->setRight(temp2);
		temp1->setLeft(node);
		return temp1;
	}

	bool checkAvl(HuffmanNode *node){
		if (node == nullptr) 	return true;
		if (abs(getHeight(node->getLeft()) - getHeight(node->getRight())) > 1) return false;
		return checkAvl(node->getLeft()) && checkAvl(node->getRight());
	}

	HuffmanNode* updateTree(HuffmanNode* node, int& rotationTime){	
		while (abs(getHeight(node->getLeft()) - getHeight(node->getRight())) > 1 && rotationTime < 3){
			int leftHigh = getHeight(node->getLeft());
			int rightHigh = getHeight(node->getRight());
				if (leftHigh > rightHigh && rotationTime < 3){
					HuffmanNode* temp = node->getLeft();
					
					if (getHeight(temp->getLeft()) >= getHeight(temp->getRight())){
						//cout<<"Left of Left"<<endl;
						node = rotateRight(node);
						rotationTime++;
					}
					else {
						//cout<<"Right of Left"<<endl;
						node->setLeft(rotateLeft(temp));
						node = rotateRight(node);
						rotationTime++;
					}
				}
				else if (leftHigh < rightHigh && rotationTime < 3){
					HuffmanNode* temp = node->getRight();
					if (getHeight(temp->getRight()) >= getHeight(temp->getLeft())){
						//cout<<"Right of Right"<<endl;
						node = rotateLeft(node);
						rotationTime++;
					}
					else {
						//cout<<"Left of Right"<<endl;
						node->setRight(rotateRight(temp));
						node = rotateLeft(node);
						rotationTime++;
					}
				}
		}
		if (node->getLeft() != NULL && rotationTime < 3) node->setLeft(updateTree(node->getLeft(), rotationTime));
		if (node->getRight() != NULL && rotationTime < 3) node->setRight(updateTree(node->getRight(), rotationTime));
		return node;
	}
};

struct CompareHuffmanTrees {
    bool operator()(HuffmanTree* lhs, HuffmanTree* rhs) const {
        if (lhs->getWeight() == rhs->getWeight()){
			if ((lhs->getRoot()->isLeaf() == true) && (rhs->getRoot()->isLeaf() == true)){
				if(islower(lhs->getChar()) && isupper(rhs->getChar())) {
					return false;
				} 
				else if (isupper(lhs->getChar()) && islower(rhs->getChar())) {
					return true;
				} 
				return lhs->getChar() > rhs->getChar();
			}
			return lhs->getPriority() > rhs->getPriority();
		}
        return lhs->getWeight() > rhs->getWeight();
    }
};

HuffmanTree* buildHuff(vector<HuffmanTree*>& huffNodes) {
	priority_queue<HuffmanTree*, std::vector<HuffmanTree*>, CompareHuffmanTrees> keys(huffNodes.begin(), huffNodes.end());
	int count = 1;
    while (keys.size() > 1) {
		
        HuffmanTree* temp1 = keys.top(); keys.pop();
        HuffmanTree* temp2 = keys.top(); keys.pop();
        HuffmanTree* temp3 = new HuffmanTree(temp1, temp2, count);

		int rotationTime = 0;
		while(!temp3->checkAvl(temp3->getRoot()) && rotationTime < 3){
			temp3->setRoot(temp3->updateTree(temp3->getRoot(), rotationTime));
		}
		if (temp3->getRoot()->isLeaf() == true){
			return nullptr;
		}
        keys.push(temp3);
		count++;
    }
    return keys.top();
}

void assignHuffmanCodes(HuffmanNode* root, string code, unordered_map<char, string>& codes) {
    if (root) {
        if (root->isLeaf()) {
            codes[root->getChar()] = code;
        }
        assignHuffmanCodes(root->getLeft(), code + "0", codes);
        assignHuffmanCodes(root->getRight(), code + "1", codes);
    }
}

int binaryToDecimal(int binaryNum) {
    int decimalNum = 0, temp = 0, remainder;
    while (binaryNum != 0) {
        remainder = binaryNum % 10;
        binaryNum = binaryNum / 10; 
        decimalNum = decimalNum + remainder * pow(2, temp);
        temp++;
    }
    return decimalNum;
}

class customer{
	public:
		string name;
		int result;
	public:
		customer(){}
		customer(string name, int res): name(name), result(res){}
		~customer(){}
};

class BinaryNode{
public:
	int data;// giá trị result từ LAPSE của khách
	BinaryNode* left;
	BinaryNode* right;
public:
	BinaryNode(int data) : data(data), left(NULL), right(NULL) {}
	~BinaryNode() {
		this->left = nullptr;
		this->right = nullptr;
	}
};

class BinarySearchTree{
public: 
	BinaryNode* root;
	queue<int> customerInArea;
	int treeSize;

	BinarySearchTree(){
		this->root = nullptr;
		this->treeSize = 0;
	}
	~BinarySearchTree() {
        clearTree(root);
    }

    void clearTree(BinaryNode* node) {
        if (node != nullptr) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }
	
	void travelAdd(BinaryNode* node, int val){
		if (val >= node->data){
			if (node->right == nullptr){
				node->right = new BinaryNode(val);
				this->customerInArea.push(val);
				this->treeSize++;
			}
			else {
				travelAdd(node->right, val);
			}
		}
		else {
			if (node->left == nullptr){
				node->left = new BinaryNode(val);
				this->customerInArea.push(val);
				this->treeSize++;
			}
			else {
				travelAdd(node->left, val);
			}
		}
	}

	void add(int value){
		if (this->root == nullptr){
			this->root = new BinaryNode(value);
			this->customerInArea.push(value);
			this->treeSize++;
			return;
		}
		travelAdd(this->root, value);
	}

	void travelDelete(BinaryNode* root, BinaryNode* preBinaryNode, int val){
		if (root == nullptr){
			return;
		}
		if (val > root->data){
			travelDelete(root->right, root, val);
		}
		else if (val < root->data){
			travelDelete(root->left, root, val);
		}
		else {
			if (root->left == nullptr && root->right == nullptr) {
				if (preBinaryNode->left == root){
					preBinaryNode->left = nullptr;
				}
				else {
					preBinaryNode->right = nullptr;
				}
				delete root;
				this->treeSize--;
				return;
			}
			else if (root->left == nullptr){
				if (preBinaryNode->left == root){
					preBinaryNode->left = root->right;
				}
				else {
					preBinaryNode->right = root->right;
				}
				delete root;
				this->treeSize--;
				return;
			}
			else if (root->right == nullptr){
				if (preBinaryNode->left == root){
					preBinaryNode->left = root->left;
				}
				else {
					preBinaryNode->right = root->left;
				}
				delete root;
				this->treeSize--;
				return;
			}
			else {
				BinaryNode* temp = root->right;
				while (temp->left != nullptr){
					temp = temp->left;
				}
				root->data = temp->data;
				travelDelete(root->right, root, temp->data);
			}
		}
	}

	void deleteBinaryNode(int value){
		if (value > this->root->data){
			travelDelete(root->right, root, value);
		}
		else if (value < this->root->data){
			travelDelete(root->left, root, value);
		}
		else {
			if (root->right == nullptr){
				BinaryNode* temp = root;
				this->root = root->left;
				this->treeSize--;
				delete temp;
			}
			else if (root->left == nullptr) {
				BinaryNode* temp = root;
				this->root = root->right;
				this->treeSize--;
				delete temp;
			}
			else {
				BinaryNode* temp = root->right;
				while (temp->left != nullptr){
					temp = temp->left;
				}
				root->data = temp->data;
				travelDelete(root->right, root, temp->data);
			}
		}
	}

	void getArray(vector<int>& res){
		queue<BinaryNode*> q;
		q.push(this->root);
    
		while (!q.empty()){
			auto temp = q.front();
			q.pop();
			res.push_back(temp->data);
			if (temp->left != nullptr){
				q.push(temp->left);
			}
			if (temp->right != nullptr){
				q.push(temp->right);
			}
		}
	}

	void printInOrder(BinaryNode* node){
		if(node != nullptr){
			printInOrder(node->left);
			cout<<node->data<<"\n";
			printInOrder(node->right);
		}
	}

	void kokusenHelp(long long eraseTimes){
		eraseTimes = min(eraseTimes, (long long)this->treeSize);
		if (eraseTimes == 0){
			return;
		}
		while (eraseTimes){
			deleteBinaryNode(customerInArea.front());
			customerInArea.pop();
			eraseTimes--;
		}
	} 
};

class Gojo{
public:
	unordered_map<int, BinarySearchTree*> Area;
public:
	void addCustomer(int ID, customer* cus, int result){
		if (Area[ID] == nullptr) {
			Area[ID] = new BinarySearchTree();
		}
        Area[ID]->add(cus->result);
	}

	~Gojo() {
        for (auto& pair : Area) {
            delete pair.second;  
        }
        Area.clear();  
    }

	int mod=1e9+7;

    long long power(long long x,int y, int p){
		long long res = 1; 
		x = x % p; 
		while (y > 0)
		{
			if (y & 1)
				res = (res * x) % p;
			y = y >> 1; 
			x = (x * x) % p;
		}
		return res;
	}

	long long modInverse(unsigned long long n,int p){
		return power(n, p - 2, p);
	}

	long long nCrModPFermat(long long n, int r, int p){
		if (n < r)
			return 0;
		if (r == 0)
			return 1;
		
		vector<long long> fac(n + 1);
		fac[0] = 1;
		for (int i = 1; i <= n; i++){
			fac[i] = (fac[i - 1] * i) % p;
		}
		return (fac[n] * modInverse(fac[r], p) % p * modInverse(fac[n - r], p) % p) % p;
	}
    long long lrbst(vector<int>& nums){
        int n = nums.size();
        if(n <= 2) return 1;
        int root=nums[0];

        vector<int> left; vector<int> right;
        for(int i=1; i<n; i++){
            if(nums[i] < root) {
				left.push_back(nums[i]);
			}
            else {
				right.push_back(nums[i]);
			}
        }
        int l = left.size();

        long long lb = lrbst(left);
        long long rb = lrbst(right);
        return ((lb % mod * rb % mod) % mod * nCrModPFermat(n - 1, l, mod)) % mod;
    }

    int numOfWays(vector<int>& nums) {
        long long ans = lrbst(nums);
        return ans;
    }

};

class RestaurantHeap {
public:
    class HeapNode {
    public:
        int ID;
        int Amount;
		int	Priority;
		queue<customer*> inHeap;
        HeapNode(int ID, int Amount, int Priority, customer* cus){
            this->ID = ID;
			this->Amount = Amount;
			this->Priority = Priority;
			this->inHeap.push(cus);
        }
		~HeapNode(){
			while (!inHeap.empty()){
				inHeap.pop();
			}
		}

		void printQueueLIFO(int num) {
			stack<customer*> s;
			queue<customer*> tempQueue = inHeap;

			while (!tempQueue.empty()) {
				s.push(tempQueue.front());
				tempQueue.pop();
			}
			num = min(num, Amount);
			while (num > 0) {
				cout<<this->ID<<"-"<<s.top()->result<<"\n";
				s.pop();
				num--;
			}
		}

		void eraseCus(int num){
			while (num > 0){
				auto temp = inHeap.front();
				cout<<temp->result<<"-"<<this->ID<<"\n";
				inHeap.pop();
				this->Amount--;
				num--;
			}
		}
    };
public:
    vector<HeapNode> heapRestaurant;
    long long count = 0;

public:

	int findIDIndex(int ID){
		for (int i = 0; i < (int) heapRestaurant.size(); i++){
			if (heapRestaurant[i].ID == ID){
				return i;
			}
		}
		return -1;
	}

   void reheapUp(int index) {
		while (index > 0) {
			int parentIndex = (index - 1) / 2;
			if (heapRestaurant[index].Amount < heapRestaurant[parentIndex].Amount ||
				(heapRestaurant[index].Amount == heapRestaurant[parentIndex].Amount && 
				heapRestaurant[index].Priority < heapRestaurant[parentIndex].Priority)) {
				std::swap(heapRestaurant[index], heapRestaurant[parentIndex]);

				index = parentIndex;
			} 
			else {
				break;
			}
		}
	}

    void reheapDown(int index) {
		int size = heapRestaurant.size();
		int pos1, pos2, amount, amount1, amount2;

		while (index * 2 + 1 < size) {
			pos1 = index * 2 + 1;
			pos2 = index * 2 + 2;

			if (pos1 < size) {
				amount = heapRestaurant[index].Amount;
				int newIndex = index;
				if (pos2 < size) {
					amount1 = heapRestaurant[pos1].Amount;
					amount2 = heapRestaurant[pos2].Amount;

					if (amount1 < amount && amount1 < amount2) {
						newIndex = pos1;
					} 
					else if (amount2 < amount && amount2 < amount1) {
						newIndex = pos2;
					}
					else if(amount1 == amount2 && amount1 < amount){
						newIndex = heapRestaurant[pos1].Priority < heapRestaurant[pos2].Priority ? pos1 : pos2;
					}
					else if (amount1 == amount2 && amount1 == amount){
						newIndex = heapRestaurant[pos1].Priority < heapRestaurant[pos2].Priority ? pos1 : pos2;
						newIndex = heapRestaurant[index].Priority < heapRestaurant[newIndex].Priority ? index : newIndex;
					}
					else if (amount1 == amount) {
						newIndex = heapRestaurant[pos1].Priority < heapRestaurant[newIndex].Priority ? pos1 : newIndex;
					}
					else if (amount2 == amount) {
						newIndex = heapRestaurant[pos2].Priority < heapRestaurant[newIndex].Priority ? pos2 : newIndex;
					}
				}

				else {
					amount1 = heapRestaurant[pos1].Amount;
					if (amount1 < amount) {
						newIndex = pos1;
					} 
					else if (amount1 == amount) {
						if (heapRestaurant[pos1].Priority < heapRestaurant[index].Priority) {
							newIndex = pos1;
						}
					}
				}

				if (newIndex != index) {
					std::swap(heapRestaurant[index], heapRestaurant[newIndex]);
					index = newIndex;
				} 
				else {
					break;
				}
			} 
			else {
				break;
			}
		}
	}

	void updatePriority(int pos){
		count++;
		this->heapRestaurant[pos].Priority = count;
	}

	void addHeap(int ID, int Amount, customer* cus){
		count++;
		HeapNode temp = HeapNode(ID, Amount, count, cus);
		heapRestaurant.push_back(temp);
        reheapUp((int)heapRestaurant.size() - 1);
	}

	void eraseHeapNode(int index){
		if (index == (int) heapRestaurant.size() - 1){
			heapRestaurant.pop_back();
			return;
		}
		else {
			swap(heapRestaurant[index], heapRestaurant[(int)heapRestaurant.size() - 1]);
			heapRestaurant.pop_back();
			reheapDown(index);
    		reheapUp(index);
		}
	}

	void getIdArray(vector<int>& idKeiteiken, int num){
		vector<HeapNode> temp(heapRestaurant.begin(), heapRestaurant.end());
		sort(temp.begin(), temp.end(), [this](const auto& a, const auto& b) {
            if (a.Amount == b.Amount){
				return a.Priority < b.Priority;
			}
			return a.Amount < b.Amount;
        });

		num = min(num, (int)heapRestaurant.size());
		for (int i = 0; i < num; i++){
			idKeiteiken.push_back(temp[i].ID);
		}
	}

	void keiteikenHelp(int id, int num){
		num = min(num, heapRestaurant[findIDIndex(id)].Amount);
		int pos = findIDIndex(id);
		heapRestaurant[pos].eraseCus(num);
		if (heapRestaurant[pos].Amount == 0){
			eraseHeapNode(pos);
			return;
		}

		updatePriority(pos);
		reheapDown(pos);
	}

	void printHeapPreorder(int index, int num) {
		if (index >= (int)heapRestaurant.size()) {
			return;
		}
		heapRestaurant[index].printQueueLIFO(num);
		printHeapPreorder(2 * index + 1, num);
		printHeapPreorder(2 * index + 2, num);
	}	
};

class Sukuna {
public:
	RestaurantHeap Area;
public:
	void addCustomer(int ID, customer* cus, int result){
		int pos = Area.findIDIndex(ID);
		if (pos != -1){
			Area.heapRestaurant[pos].Amount++;
			Area.updatePriority(pos); 
			Area.heapRestaurant[pos].inHeap.push(cus);
			Area.reheapDown(pos);
		}
		else {
			Area.addHeap(ID, 1, cus);
		}
	}
};

class Restaurant{
public:
	Sukuna* sukuna;
	Gojo* gojo;
	HuffmanTree* lastCustomer;
public:
	Restaurant() {
		this->sukuna = new Sukuna();
		this->gojo = new Gojo();
		this->lastCustomer = nullptr;
	}
	~Restaurant() {
		delete sukuna;
	    delete gojo;
	    if (lastCustomer != nullptr) {
       	 	delete lastCustomer;
    	}
	}

	void LAPSE(string name){
		if (name.length() < 3){
			return;
		}
		//Lay tan so
		unordered_map<char, int> characters;
		for (char c : name) {
			characters[c]++;
		}
		if(characters.size() < 3){
			return;
		}
		//ma hoa Ceasar
		vector<pair<char, int>> charVector(characters.begin(), characters.end());
		
		for (auto& c : charVector) {
			if (isalpha(c.first)) {
				char base = isupper(c.first) ? 'A' : 'a';
				c.first = static_cast<char>((c.first - base + c.second) % 26 + base);
			}
		}
		//Gop lai ket qua ma hoa
		unordered_map<char, int> decode;
		for (auto c : charVector) {
			decode[c.first] += c.second;
		}
		//Sort 
		charVector.clear();
		charVector.assign(decode.begin(), decode.end());
		sort(charVector.begin(), charVector.end(), [this](const auto& a, const auto& b) {
            if (a.second == b.second){
				if (islower(a.first) && isupper(b.first)) {
					return true;
				} else if (isupper(a.first) && islower(b.first)) {
					return false;
				} 
				else {
					return a.first < b.first;
				}
			}
			return a.second < b.second;
        });
		
		//Tạo huffTree
		vector<HuffmanTree*> huffNodes; 
		for (auto c : charVector){
			HuffmanTree* temp = new HuffmanTree(c.first, c.second, 0);
			huffNodes.push_back(temp);
		}
		
		HuffmanTree* root = buildHuff(huffNodes);
		if (root == nullptr){
			return;
		}
		this->lastCustomer = root;
		//Lay ma thap phan

		if (root->getRoot()->getLeft() == nullptr && root->getRoot()->getRight() == nullptr){
			customer* cus = new customer(name, 0);
			this->sukuna->addCustomer(1, cus, 0);
			return;
		}

		unordered_map<char, string> codes;
		assignHuffmanCodes(root->getRoot(), "", codes);

		string binaryString = "";
		for (int i = 0; i < (int)name.length(); i++){
			auto it = characters.find(name[i]);
			char base = isupper(it->first) ? 'A' : 'a';
			name[i] = static_cast<char>((it->first - base + it->second) % 26 + base);
			auto temp = codes.find(name[i]);
			if (temp != codes.end()) {
				binaryString += temp->second;
			}
		}

		string temp = "";
		for (int i = binaryString.length() - 1; i >= 0; i--){
			if (temp.length() == 10){
				break;
			}
			temp += binaryString[i];
		}

		int Result = binaryToDecimal(stoi(temp));
		int ID = Result % MAXSIZE + 1;
		customer* cus = new customer(name, Result);
		if (Result & 1){
			this->gojo->addCustomer(ID, cus, Result);
		}
		else {
			this->sukuna->addCustomer(ID, cus, Result);
		}
	}

	void KOKUSEN(){
		long long permutations = 1;
		for (auto area : this->gojo->Area){
			vector<int> temp;
			if (area.second == nullptr){
				continue;
			}
			else if (area.second->treeSize <= 1){
				permutations = 1;
			}
			else {
				area.second->getArray(temp);
				permutations = this->gojo->numOfWays(temp) % MAXSIZE;
			}
			area.second->kokusenHelp(permutations);
		}
	}

	void KEITEIKEN(int num){
		vector<int> idKeiteiken;
		this->sukuna->Area.getIdArray(idKeiteiken, num);
		int size = idKeiteiken.size();

		for (int i = 0; i < size; i++){
			this->sukuna->Area.keiteikenHelp(idKeiteiken[i], num);
		}
	}

	void HAND(){
		if (this->lastCustomer == nullptr){
			return;
		}
		this->lastCustomer->printInOrderHuffTree(this->lastCustomer->getRoot());
	}

	void LIMITLESS(int num){
		if (this->gojo->Area[num] == nullptr){
			return;
		}
		this->gojo->Area[num]->printInOrder(this->gojo->Area[num]->root);
	}

	void CLEAVE(int num){
		this->sukuna->Area.printHeapPreorder(0, num);
	}
		
};

void simulate(string filename)
{
	Restaurant* r = new Restaurant();
	ifstream ss(filename);
	string str, maxsize, name, num;
	while(ss >> str)
	{ 
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);
    	}
        else if(str == "LAPSE") 
        {
            ss >> name;
			r->LAPSE(name);
    	}
    	else if(str == "KOKUSEN") 
    	{
             r->KOKUSEN();  
		}
    	else if(str == "KEITEIKEN") 
    	{
    		ss >> num;
			r->KEITEIKEN(stoi(num));
		}
		else if(str == "HAND") 
		{
    		r->HAND();		
		}
    	else if(str == "LIMITLESS")
     	{   	
    		ss >> num;
			r->LIMITLESS(stoi(num));	
    	}
    	else
    	{
    		ss >> num;
			r->CLEAVE(stoi(num));	
    	}
    }
	return;
}