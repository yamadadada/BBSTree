// BBSTree.cpp : �������̨Ӧ�ó������ڵ㡣
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
}RcdType;//����Ԫ������

typedef struct BBSTNode {
	RcdType data;
	int bf;//ƽ������
	struct BBSTNode *lchild, *rchild;
}*BBSTree;//ƽ�������

BBSTree SearchBBSTree(BBSTree T, KeyType key) {
	//���Ҳ���
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
	//��������
	BBSTree rc = p->rchild;
	p->rchild = rc->lchild;
	rc->lchild = p;
	p = rc;
}

void R_Rotate(BBSTree &p) {
	//��������
	BBSTree lc = p->lchild;
	p->lchild = lc->rchild;
	lc->rchild = p;
	p = lc;
}

void LeftBalance(BBSTree &T) {
	//��ƽ�⴦��
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
	//��ƽ�⴦��
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
	//�������
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
	//����ƽ������
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
	//Ѱ��p���ĸ����
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
	//ɾ��������
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
	//�ݹ�Ѱ��ɾ�����
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
	//ƽ�������ɾ������
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
	//�ð������ʾ������
	if (T == NULL) {
		printf("�����������ڣ�\n");
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
	//����2�ϲ�����1
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
	//����T���ѳ�T3��T4
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
		printf("1��������A\n");
		printf("2��������B\n");
		printf("input your select:");
		scanf("%d", &select1);
		if (select1 == 1) {
			do {
				printf("------�����ڲ�����A��------\n");
				printf("1������\n");
				printf("2������\n");
				printf("3��ɾ��\n");
				printf("4��������ʾ��ͼ\n");
				printf("5������B�ϲ�����A\n");
				printf("6�������������Ϊ������\n");
				printf("7��ѡ������һ����\n");
				printf("input your select:");
				scanf("%d", &select);
				switch (select)
				{
				case 1:
					BBSTree p;
					printf("��������Ҫ���ҵ�ֵ��");
					scanf("%d", &key);
					p = SearchBBSTree(T1, key);
					if (p == NULL) {
						printf("����ʧ�ܣ��ö������޴�ֵ\n");
					}
					else {
						printf("��ֵ�Ķ�����ʾ��ͼ��\n");
						displayTree(p, 1);
					}
					break;
				case 2:
					printf("��������Ҫ�����ֵ��");
					scanf("%d", &key);
					rcd.key = key;
					if (InsertBBSTree(T1, rcd, taller)) {
						printf("����ɹ���\n");
						printf("������ʾ��ͼ��\n");
						displayTree(T1, 1);
					}
					else {
						printf("����ʧ�ܣ���ֵ�Ѵ��ڣ�\n");
					}
					break;
				case 3:
					printf("��������Ҫɾ����ֵ��");
					scanf("%d", &key);
					rcd.key = key;
					if (SearchBBSTree(T1, key)) {
						DeleteBBSTree(T1, key);
						printf("ɾ���ɹ���\n");
						printf("������ʾ��ͼ��\n");
						displayTree(T1, 1);
					}
					else {
						printf("ɾ��ʧ�ܣ���ֵ�����ڣ�\n");
					}
					break;
				case 4:
					printf("������ʾ��ͼ��\n");
					displayTree(T1, 1);
					break;
				case 5:
					if (CombineTree(T1, T2)) {
						printf("�ϲ��ɹ���\n");
						displayTree(T1, 1);
					}
					else {
						printf("�ϲ�ʧ�ܣ�\n");
					}
					break;
				case 6:
					printf("������������ݵĹؼ���:");
					scanf("%d", &key);
					if (SplitTree(T1, T3, T4, key)) {
						printf("���ѳɹ���\n");
						printf("���ѵĵ�һ������\n");
						displayTree(T3, 1);
						printf("-----------------------------------\n");
						printf("���ѵĵڶ�������\n");
						displayTree(T4, 1);
					}
					else {
						printf("����ʧ�ܣ�");
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
				printf("------�����ڲ�����B��------\n");
				printf("1������\n");
				printf("2������\n");
				printf("3��ɾ��\n");
				printf("4��������ʾ��ͼ\n");
				printf("5�������������Ϊ������\n");
				printf("6��ѡ������һ����\n");
				printf("input your select:");
				scanf("%d", &select);
				switch (select)
				{
				case 1:
					BBSTree p;
					printf("��������Ҫ���ҵ�ֵ��");
					scanf("%d", &key);
					p = SearchBBSTree(T2, key);
					if (p == NULL) {
						printf("����ʧ�ܣ��ö������޴�ֵ\n");
					}
					else {
						printf("��ֵ�Ķ�����ʾ��ͼ��\n");
						displayTree(p, 1);
					}
					break;
				case 2:
					printf("��������Ҫ�����ֵ��");
					scanf("%d", &key);
					rcd.key = key;
					if (InsertBBSTree(T2, rcd, taller)) {
						printf("����ɹ���\n");
						printf("������ʾ��ͼ��\n");
						displayTree(T2, 1);
					}
					else {
						printf("����ʧ�ܣ���ֵ�Ѵ��ڣ�\n");
					}
					break;
				case 3:
					printf("��������Ҫɾ����ֵ��");
					scanf("%d", &key);
					rcd.key = key;
					if (SearchBBSTree(T2, key)) {
						DeleteBBSTree(T2, key);
						printf("ɾ���ɹ���\n");
						printf("������ʾ��ͼ��\n");
						displayTree(T1, 1);
					}
					else {
						printf("ɾ��ʧ�ܣ���ֵ�����ڣ�\n");
					}
					break;
				case 4:
					printf("������ʾ��ͼ��\n");
					displayTree(T2, 1);
					break;
				case 5:
					printf("������������ݵĹؼ���:");
					scanf("%d", &key);
					if (SplitTree(T2, T3, T4, key)) {
						printf("���ѳɹ���\n");
						printf("���ѵĵ�һ������\n");
						displayTree(T3, 1);
						printf("-----------------------------------\n");
						printf("���ѵĵڶ�������\n");
						displayTree(T4, 1);
					}
					else {
						printf("����ʧ�ܣ�");
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

