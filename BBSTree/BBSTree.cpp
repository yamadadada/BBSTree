// BBSTree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string.h>
#include <malloc.h>

#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0

#pragma warning(disable:4996)

typedef int Status;
typedef int KeyType;

typedef struct RcdType {
	KeyType key;
	int tag;
}RcdType;//数据元素类型

typedef struct BBSTNode {
	RcdType data;
	int bf;//平衡因子
	struct BBSTNode *lchild, *rchild;
}*BBSTree;//平衡二叉树

BBSTree SearchBBSTree(BBSTree T, KeyType key) {
	//查找操作
	while (T != NULL) {
		if (T->data.key == key) {
			return T;
		}
		else if (T->data.key > key) {
			T = T->lchild;
		}
		else {
			T = T->rchild;
		}
	}
	return NULL;
}

void L_Rotate(BBSTree &p) {
	//左旋调整
	BBSTree rc = p->rchild;
	p->rchild = rc->lchild;
	rc->lchild = p;
	p = rc;
}

void R_Rotate(BBSTree &p) {
	//右旋调整
	BBSTree lc = p->lchild;
	p->lchild = lc->rchild;
	lc->rchild = p;
	p = lc;
}

void LeftBalance(BBSTree &T) {
	//左平衡处理
	BBSTree lc, rd;
	lc = T->lchild;
	switch (lc->bf)
	{
	case 1:
		T->bf = lc->bf = 0;
		R_Rotate(T);
		break;
	case -1:
		rd = lc->rchild;
		switch (rd->bf)
		{
		case 1:
			T->bf = -1;
			lc->bf = 0;
			break;
		case 0:
			T->bf = lc->bf = 0;
			break;
		case -1:
			T->bf = 0;
			lc->bf = 1;
			break;
		default:
			break;
		}
		rd->bf = 0;
		L_Rotate(T->lchild);
		R_Rotate(T);
		break;
	default:
		break;
	}
}

void RightBalance(BBSTree &T) {
	//右平衡处理
	BBSTree rc, ld;
	rc = T->rchild;
	switch (rc->bf)
	{
	case -1:
		T->bf = rc->bf = 0;
		L_Rotate(T);
		break;
	case 1:
		ld = rc->lchild;
		switch (ld->bf)
		{
		case 1:
			T->bf = 0;
			rc->bf = -1;
			break;
		case 0:
			T->bf = rc->bf = 0;
			break;
		case -1:
			T->bf = 1;
			rc->bf = 0;
			break;
		default:
			break;
		}
		ld->bf = 0;
		R_Rotate(T->rchild);
		L_Rotate(T);
		break;
	default:
		break;
	}
}

Status InsertBBSTree(BBSTree &T, RcdType e, Status &taller) {
	//插入操作
	if (T == NULL) {
		T = (BBSTree)malloc(sizeof(BBSTNode));
		T->data = e;
		T->bf = 0;
		T->lchild = NULL;
		T->rchild = NULL;
		taller = TRUE;
	}
	else if (e.key == T->data.key) {
		taller = FALSE;
		return FALSE;
	}
	else if (e.key < T->data.key) {
		if (InsertBBSTree(T->lchild, e, taller) == FALSE) {
			return FALSE;
		}
		if (taller == TRUE) {
			switch (T->bf)
			{
			case 1:
				LeftBalance(T);
				taller = FALSE;
				break;
			case 0:
				T->bf = 1;
				taller = TRUE;
				break;
			case -1:
				T->bf = 0;
				taller = FALSE;
				break;
			default:
				break;
			}
		}
	}
	else {
		if (InsertBBSTree(T->rchild, e, taller) == FALSE) {
			return FALSE;
		}
		if (taller == TRUE) {
			switch (T->bf)
			{
			case 1:
				T->bf = 0;
				taller = FALSE;
				break;
			case 0:
				T->bf = -1;
				taller = TRUE;
				break;
			case -1:
				RightBalance(T);
				taller = FALSE;
				break;
			default:
				break;
			}
		}
	}
	return TRUE;
}

int CalBf(BBSTree &T) {
	//计算平衡因子
	int l, r;
	if (T == NULL) {
		return -1;
	}
	l = CalBf(T->lchild) + 1;
	r = CalBf(T->rchild) + 1;
	T->bf = l - r;
	return l > r ? l : r;
}

BBSTree SerachPreNode(BBSTree &T, BBSTree &p) {
	//寻找p结点的父结点
	if (T == p || T == NULL) {
		return NULL;
	}
	if (p->data.key < T->data.key) {
		if (T->lchild == p) {
			return T;
		}
		return SerachPreNode(T->lchild, p);
	}
	else if (p->data.key > T->data.key) {
		if (T->rchild == p) {
			return T;
		}
		return SerachPreNode(T->rchild, p);
	}
	else {
		return NULL;
	}
}

void DeleteNode(BBSTree &p) {
	//删除结点操作
	BBSTNode *q, *s;
	q = p;
	if (p->rchild == NULL) {
		p = p->lchild;
		free(q);
	}
	else if (p->lchild == NULL) {
		p = p->rchild;
		free(q);
	}
	else {
		s = p->lchild;
		while (s->rchild != NULL) {
			q = s;
			s = s->rchild;
		}
		p->data = s->data;
		if (q == p) {
			q->lchild = s->lchild;
		}
		else {
			q->rchild = s->lchild;
		}
		free(s);
	}
}

BBSTree SearchDeleteBBSTNode(BBSTree &T, KeyType key) {
	//递归寻找删除结点
	if (T == NULL) {
		return NULL;
	}
	if (T->data.key == key) {
		DeleteNode(T);
		return T;
	}
	else if (key < T->data.key) {
		return SearchDeleteBBSTNode(T->lchild, key);
	}
	return SearchDeleteBBSTNode(T->rchild, key);
}

Status DeleteBBSTree(BBSTree &T, KeyType key) {
	//平衡二叉树删除操作
	if (T == NULL) {
		return FALSE;
	}
	BBSTree p, q;
	if ((p = SearchDeleteBBSTNode(T, key)) != NULL) {
		q = SerachPreNode(T, p);
		if (p == NULL) {
			p = q;
		}
		CalBf(T);
		if (p->lchild) {
			p = p->lchild;
			while (p && p->rchild) {
				p = p->rchild;
			}
		}
		while (p != NULL) {
			if (p->bf > 1) {
				LeftBalance(p);
				return TRUE;
			}
			else if (p->bf < -1) {
				RightBalance(p);
				return TRUE;
			}
			else {
				p = SerachPreNode(T, p);
			}
		}
		return TRUE;
	}
	else {
		return FALSE;
	}
}

Status displayTree(BBSTree T, int depth) {
	//用凹入表显示二叉树
	if (T == NULL) {
		printf("二叉树不存在！\n");
		return FALSE;
	}
	int i;
	for (i = 0; i < depth - 1; i++) {
		printf("    ");
	}
	printf("%d\n", T->data.key);
	if (T->lchild) {
		displayTree(T->lchild, depth + 1);
	}
	if (T->rchild) {
		displayTree(T->rchild, depth + 1);
	}
	return TRUE;
}

Status CombineTree(BBSTree &T1, BBSTree &T2) {
	//将树2合并到树1
	BBSTree p;
	if (T1 == NULL || T2 == NULL) {
		return ERROR;
	}
	if (T2->lchild) {
		CombineTree(T1, T2->lchild);
	}
	if (T2->rchild) {
		CombineTree(T1, T2->rchild);
	}
	if (T2->lchild == NULL && T2->rchild == NULL) {
		Status taller = FALSE;
		InsertBBSTree(T1, T2->data, taller);
		p = T2;
		T2 = NULL;
		free(p);
	}
	return TRUE;
}

Status SplitTree(BBSTree T, BBSTree &T3, BBSTree &T4, KeyType x) {
	//将树T分裂成T3，T4
	if (T == NULL) {
		return ERROR;
	}
	Status taller;
	if (T->data.key <= x) {
		InsertBBSTree(T3, T->data, taller);
	}
	else {
		InsertBBSTree(T4, T->data, taller);
	}
	if (T->lchild) {
		SplitTree(T->lchild, T3, T4, x);
	}
	if (T->rchild) {
		SplitTree(T->rchild, T3, T4, x);
	}
	return OK;
}

int main()
{
	BBSTree	T1 = NULL;
	BBSTree	T2 = NULL;
	BBSTree	T3 = NULL;
	BBSTree	T4 = NULL;
	Status taller;
	KeyType key;
	RcdType rcd;
	int select;
	int select1;
	do {
		printf("1：操作树A\n");
		printf("2：操作树B\n");
		printf("input your select:");
		scanf("%d", &select1);
		if (select1 == 1) {
			do {
				printf("------你正在操作树A：------\n");
				printf("1：查找\n");
				printf("2：插入\n");
				printf("3：删除\n");
				printf("4：二叉树示例图\n");
				printf("5：将树B合并到树A\n");
				printf("6：将这棵树分裂为两棵树\n");
				printf("7：选择另外一棵树\n");
				printf("input your select:");
				scanf("%d", &select);
				switch (select)
				{
				case 1:
					BBSTree p;
					printf("请输入你要查找的值：");
					scanf("%d", &key);
					p = SearchBBSTree(T1, key);
					if (p == NULL) {
						printf("查找失败！该二叉树无此值\n");
					}
					else {
						printf("该值的二叉树示例图：\n");
						displayTree(p, 1);
					}
					break;
				case 2:
					printf("请输入你要插入的值：");
					scanf("%d", &key);
					rcd.key = key;
					if (InsertBBSTree(T1, rcd, taller)) {
						printf("插入成功！\n");
						printf("二叉树示例图：\n");
						displayTree(T1, 1);
					}
					else {
						printf("插入失败！该值已存在！\n");
					}
					break;
				case 3:
					printf("请输入你要删除的值：");
					scanf("%d", &key);
					rcd.key = key;
					if (SearchBBSTree(T1, key)) {
						DeleteBBSTree(T1, key);
						printf("删除成功！\n");
						printf("二叉树示例图：\n");
						displayTree(T1, 1);
					}
					else {
						printf("删除失败！该值不存在！\n");
					}
					break;
				case 4:
					printf("二叉树示例图：\n");
					displayTree(T1, 1);
					break;
				case 5:
					if (CombineTree(T1, T2)) {
						printf("合并成功！\n");
						displayTree(T1, 1);
					}
					else {
						printf("合并失败！\n");
					}
					break;
				case 6:
					printf("请输入分裂依据的关键字:");
					scanf("%d", &key);
					if (SplitTree(T1, T3, T4, key)) {
						printf("分裂成功！\n");
						printf("分裂的第一棵树：\n");
						displayTree(T3, 1);
						printf("-----------------------------------\n");
						printf("分裂的第二棵树：\n");
						displayTree(T4, 1);
					}
					else {
						printf("分裂失败！");
					}
					break;
				case 7:
					select = -1;
					break;
				default:
					break;
				}
			} while (select > 0 && select < 8);
		}
		else {
			do {
				printf("------你正在操作树B：------\n");
				printf("1：查找\n");
				printf("2：插入\n");
				printf("3：删除\n");
				printf("4：二叉树示例图\n");
				printf("5：将这棵树分裂为两棵树\n");
				printf("6：选择另外一棵树\n");
				printf("input your select:");
				scanf("%d", &select);
				switch (select)
				{
				case 1:
					BBSTree p;
					printf("请输入你要查找的值：");
					scanf("%d", &key);
					p = SearchBBSTree(T2, key);
					if (p == NULL) {
						printf("查找失败！该二叉树无此值\n");
					}
					else {
						printf("该值的二叉树示例图：\n");
						displayTree(p, 1);
					}
					break;
				case 2:
					printf("请输入你要插入的值：");
					scanf("%d", &key);
					rcd.key = key;
					if (InsertBBSTree(T2, rcd, taller)) {
						printf("插入成功！\n");
						printf("二叉树示例图：\n");
						displayTree(T2, 1);
					}
					else {
						printf("插入失败！该值已存在！\n");
					}
					break;
				case 3:
					printf("请输入你要删除的值：");
					scanf("%d", &key);
					rcd.key = key;
					if (SearchBBSTree(T2, key)) {
						DeleteBBSTree(T2, key);
						printf("删除成功！\n");
						printf("二叉树示例图：\n");
						displayTree(T1, 1);
					}
					else {
						printf("删除失败！该值不存在！\n");
					}
					break;
				case 4:
					printf("二叉树示例图：\n");
					displayTree(T2, 1);
					break;
				case 5:
					printf("请输入分裂依据的关键字:");
					scanf("%d", &key);
					if (SplitTree(T2, T3, T4, key)) {
						printf("分裂成功！\n");
						printf("分裂的第一棵树：\n");
						displayTree(T3, 1);
						printf("-----------------------------------\n");
						printf("分裂的第二棵树：\n");
						displayTree(T4, 1);
					}
					else {
						printf("分裂失败！");
					}
					break;
				case 6:
					select = -1;
					break;
				default:
					break;
				}
			} while (select > 0 && select < 7);
		}
	} while (select1 > 0 && select1 < 3);
    return 0;
}

