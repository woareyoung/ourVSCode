#include "../../stdafx.h"
#include "../../AI1_Header/AI1.h"
///清除无用的预测分支
void AI1::ClearLimb(int usefulPointLine, int usefulPointColumn)
{
	CHAIN *c, *temp;
	for(c = HeadChain; c != NULL;)
	{
		if(c->line != usefulPointLine || c->column != usefulPointColumn)
		{
			temp = c;
			c = c->parallel;
			ClearChild(temp);
		}
		else if(c->line == usefulPointLine && c->column == usefulPointColumn)
		{
			HeadChain = c;
			c = c->parallel;
		}
	}
}
///清除无用的子树
void AI1::ClearChild(CHAIN *child)
{
	CHAIN *temp1, *temp2;
	temp1 = child->next;
	temp2 = child->parallel;
	delete child;
	if(temp1 != NULL) ClearChild(temp1);
	if(temp2 != NULL) ClearChild(temp2);
	
}