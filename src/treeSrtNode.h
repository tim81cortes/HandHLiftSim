#ifndef CALLSFROMEVENTS_SRC_TREESRTNODE_H_
#define CALLSFROMEVENTS_SRC_TREESRTNODE_H_

class TreeSortNode
{
private:

	int call;
	int numFloors;
	TreeSortNode* leftChild;
	TreeSortNode* rightChild;
public:
	TreeSortNode(int rcAge);
	~TreeSortNode();
	void	insertNode(TreeSortNode* inputNode, int upDwn);
	void	printNode();
	void    printTree();
	bool	isGreaterThanThis(TreeSortNode *node);
	int 	getCall();

	int		getNextCall2(int mstRcntFl, int upDwn, TreeSortNode* inputNode);
	int		getNextValidCall(int mstRcntFl, int upDwn, TreeSortNode* inNode);
	int		compareClosest(int comp1, int comp2, int upDwn, int closeToWhat);
	int		getNextCall(int mstRcntFl, int upDwn, TreeSortNode* inputNode);
	TreeSortNode* 	joinAtHead(TreeSortNode* qHead);
	void 	deleteCallQ(TreeSortNode** node);
	//sort child tree
};


#endif /* CALLSFROMEVENTS_SRC_TREESRTNODE_H */
