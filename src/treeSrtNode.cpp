#include <iostream>
#include <string.h>

#include "treeSrtNode.h"
using namespace std;


TreeSortNode::TreeSortNode(int flCll)
{

	call = flCll;
	leftChild = NULL;
	rightChild = NULL;
}

TreeSortNode::~TreeSortNode()
{
	//If subtree exits on either side, delete them
	if(leftChild) delete leftChild;
	if(rightChild) delete rightChild;
}

void TreeSortNode::insertNode(TreeSortNode* inputNode, int upDwn)
{
	if (upDwn == -1){
		//AddTsrtNodeWBTest<<"Call of added Tree node is " << call << endl;
		//AddTsrtNodeWBTest<<"It's being added to the "  << upDwn << "Q" << endl;
	}
	//Inserts the node into the tree
	if (call * upDwn < inputNode->call * upDwn)
	{
		//AddTsrtNodeWBTest<<call * upDwn << " isLessThan " << inputNode->call * upDwn << endl  << endl;
		//If the left child is not NULL, insert the node
		if (inputNode->leftChild)
		{
			//AddTsrtNodeWBTest<<"There is a left child so calling again on the Left Child Node" << endl;
			insertNode(inputNode->leftChild, upDwn);
		}
		//If it is NULL, make this tree the left child
		else
		{
			//AddTsrtNodeWBTest<<"No LeftChild node so adding " << call<< "as the left Child node."  << endl;
			inputNode->leftChild = this;
		}
	}
	if (call * upDwn > inputNode->call * upDwn)
	{
		//AddTsrtNodeWBTest<<call * upDwn << " isGreaterThan " << inputNode->call * upDwn << endl;
		//Same as above but for the opposite node
		if (inputNode->rightChild)
		{
			//AddTsrtNodeWBTest<<"There is a right child so calling again on the right Child Node" << endl;
			insertNode(inputNode->rightChild, upDwn);
		}
		else
		{
			//AddTsrtNodeWBTest<<"No right node so adding " << call<< "as the right Child node. fsda" << endl;
			inputNode->rightChild = this;
		}
	}
}




void	TreeSortNode::printNode()
{
	//Prints the node from the tree
	cout <<  call << " " <<  '\n';
}

void    TreeSortNode::printTree()
{
	// If there is a left subtree, call this function on it
	if (leftChild)
	{
		leftChild->printTree();
	}
	// Print node if no left node or function has returned
	printNode();
	// If there is a left subtree, call this function on it
	if (rightChild)
	{
		rightChild->printTree();
	}
}

bool	TreeSortNode::isGreaterThanThis(TreeSortNode* node)
{
	//(recordAge < node->recordAge || recordAge == node->recordAge && strcmp(recordName, node->recordName) < 0 || recordAge == node->recordAge && strcmp(recordName,node->recordName) == 0 && strcmp(recordCode,node->recordCode) < 0)
	// Compares nodes by age, then name then code
	if (call < node->call)
	{
		return true;
	}


	return false;
}
int		TreeSortNode::getCall(){
	return call;
}


int		TreeSortNode::getNextCall2(int mstRcntFl, int upDwn, TreeSortNode* inputNode){
	int tmp;
	//tsrtWBTest <<   "Get Next Call2. first comparison inputNode->call * upDwn < mstRcntFl * upDwn + upDwn." << endl;
	//tsrtWBTest <<   "Q being accessed is: " << upDwn <<  "    asdf"<<endl;
	if(inputNode->call * upDwn < mstRcntFl * upDwn + 1)
	{
		//tsrtWBTest <<   "InputNodeCall * upDwn: " << inputNode->call * upDwn << " is less than " << "Most Recent Floor * upDwn + upDwn: " << mstRcntFl * upDwn + 1 << endl;
		if(inputNode->rightChild)
		{
			//tsrtWBTest <<   "There is a right child compare the inputNodes call with returned value from the right child " << endl;
			tmp = compareClosest(getNextCall2(mstRcntFl, upDwn, inputNode->rightChild), inputNode->call, upDwn, mstRcntFl + upDwn);
			//tsrtWBTest <<   "The closest match to " << mstRcntFl << " was " << tmp << "This will now be returned." << endl ;
			return tmp;
		}
		else
		{
			//tsrtWBTest <<   "There was no right child so returning -2." << endl ;
			return -2;
		}
	}
	if(inputNode ->call * upDwn > mstRcntFl * upDwn  + 1)
	{
		//tsrtWBTest <<   "InputNodeCall * upDwn: " << inputNode->call * upDwn << " is greater than " << "Most Recent Floor * upDwn + upDwn: " << mstRcntFl * upDwn + 1 << endl;
		if(inputNode->leftChild)
		{
			//tsrtWBTest <<   "There is a left child.  Compare the inputNodes call with returned value from the left child " << endl;
			tmp = compareClosest(getNextCall2(mstRcntFl, upDwn, inputNode->leftChild), inputNode->call, upDwn, mstRcntFl + upDwn);
			//tsrtWBTest <<  "The closest match to " << mstRcntFl << " was " << tmp << "This will now be returned." << endl;
			//tsrtWBTest <<   endl;
			return tmp;
		}

		//tsrtWBTest <<   "Greater than most recent call +-1 and no values which are less so returning " << inputNode->call;
		return inputNode->call;
	}
	//tsrtWBTest <<   "Equal most recent call + "<< upDwn<< " so returning " << inputNode->call;
	return inputNode->call;
}
int		getNextValidCall(int mstRcntFl, int upDwn, TreeSortNode* inNode){
	int tmp;
	tmp = inNode->getNextCall2(2,2,inNode);
	if(tmp >= 0)
	{
		return tmp;
	}
	else
	{
		return 0;
	}
}

int 	TreeSortNode::compareClosest(int comp1, int comp2, int upDwn, int closeToWhat){
	int temp1;
	int temp2;
	if(comp1 * upDwn < closeToWhat * upDwn && comp2 * upDwn < closeToWhat * upDwn)
	{
		return -1;
	}
	temp1 = (comp1 - closeToWhat) * upDwn;
	temp2 = (comp2 - closeToWhat) * upDwn;
	if (temp1 < 0)
	{
		//tsrtWBTest <<   "comparison returning " << comp2 << " not " << comp1 << endl;
		return comp2;
	}
	if (temp2 < 0)
	{
		//tsrtWBTest <<   "comparison returning " << comp1 << " not " << comp2 << endl;
		return comp1;
	}
	if(temp1  < temp2)
	{
		//tsrtWBTest <<   "comparison returning " << comp1 << " not " << comp2 << endl;
		return comp1;
	}
	//tsrtWBTest <<   "comparison returning " << comp2 << " not " << comp1 << endl;
	return comp2;


}
int		TreeSortNode::getNextCall(int mstRcntFl, int upDwn, TreeSortNode* inputNode){
	// upDwn 1 for up -1 for down
	TreeSortNode* cursor = new TreeSortNode(-1);
	int tmpCall = -1;
	if (inputNode) //could be done before function called.
	{
		cursor = inputNode;
		//tSortVerbose "The next call value is: ";
		//tSortVerbose cursor -> call;
	}
	if (cursor->call * upDwn > mstRcntFl * upDwn + 1)
	{
		// check left child to see if there is a closer floor.
		//tSortVerbose "checking the left child" << endl;
		if(cursor->leftChild)
		{
			tmpCall = getNextCall(mstRcntFl, upDwn, cursor->leftChild);
			if(tmpCall < 0)
			{
				//tSortVerbose " return from lftchild is less than 0" << endl;
				return cursor->call;
			}
			else if (tmpCall * upDwn > mstRcntFl * upDwn + 1)
			{
				return tmpCall;
			}
			else
			{
				return cursor->call;
			}
		}
		else
		{
			////tSortVerbose "There is no left child." << endl;
			return cursor->call;
		}
	}
	else if(cursor->call * upDwn == mstRcntFl * upDwn + 1)
	{
		return cursor -> call;
	}
	else
	{
		//check right child to see if greater than. If greater than return it otherwise don't
		////tSortVerbose "Checking the right child."<< endl;
		if(cursor->rightChild)
			{
			////tSortVerbose "There is one" << endl;
				tmpCall = getNextCall(mstRcntFl, upDwn, cursor->rightChild);
				//tSortVerbose "The right side returned call is " << tmpCall << endl;

				if (tmpCall > 0)
				{
					if (tmpCall* upDwn >= mstRcntFl * upDwn + 1)
					{
						return tmpCall;
					}
					else if (cursor -> call * upDwn >= mstRcntFl * upDwn + 1)
					{
						return cursor -> call;
					}
					else{
						return -3;
					}
				}
				else
				{
					return -2;
				}
			}
			else
			{
//				if(cursor ->call >= mstRcntFl +1){
//					return cursor ->call;
//				}
				////tSortVerbose "There is no right child." << endl;
				return - 1;
			}
	}
}
TreeSortNode*	TreeSortNode::joinAtHead(TreeSortNode* qHead){
	cout << "tsrtVerbose :" << "Joining at the head" << endl;
	//this->printNode();
	qHead = this;
	leftChild = NULL;
	rightChild = NULL;
	return qHead;

}
void			TreeSortNode::deleteCallQ(TreeSortNode** node){
	if((*node ) == NULL)
	{
		return;
	}
	deleteCallQ(&(*node)->leftChild);
	deleteCallQ(&(*node)->rightChild);
	if(!((*node)->leftChild)&& !((*node)->rightChild))
	{
		//tSortVerbose "freed" << endl;
		delete((*node));
	}
}





