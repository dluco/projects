#ifndef _NODE_HPP_
#define _NODE_HPP_

class Node
{
	public:
		int data;
		Area *next;

		Node(int data, Node *next)
		{
			this->data = data;
			this->next = next;
		}
};

#endif /* _NODE_HPP_ */
