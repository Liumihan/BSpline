#ifndef PINCER_H
#define PINCER_H

#include "QPointF"
#include "QVector"
#include "AidFunction.h"

void pincer_attack(QVector<QPointF> m_click_points,QVector<QPointF>& m_BSpline_points)
{
	m_BSpline_points.clear();
	//得有四段B样条曲线：①P0-P1的直线，P0-P5的曲线，P1-P3的曲线，P3-P5的曲线
	QVector<QPointF> line_0,line_1,line_2,line_3;
	//点击两个点的阶段
	QPointF P0,P1,P2,P3,P4,P5,P6,P7,P8;
	if (m_click_points.size() > 1) 
	{
		P0 = m_click_points[0];
		P1 = m_click_points[1];
		QVector<QPointF> clicPots_line_0;
		clicPots_line_0.push_back(P0);
		clicPots_line_0.push_back(P1);
		getGUIBSplinePoint(clicPots_line_0,line_0);
	}
	//点击第三个点形成箭头的阶段
	if (m_click_points.size() > 2)
	{
		//line_0
		P3 = m_click_points[2];
		if (P3 == P1)
		{
			P3.setX(P3.x() + 1.0);
			P3.setY(P3.y() + 1.0);
		}
		//用P1和P3计算出P2
		P2 = (P3 - P1) * 0.55 + P1;
		P2 = rotation(P2,P1,20.0/180.0*PI,direction(P0-P1,P3-P1));
		QVector<QPointF> clicPots_line_1;
		clicPots_line_1.push_back(P1);
		clicPots_line_1.push_back(P2);
		clicPots_line_1.push_back(P3);
		getGUIBSplinePoint(clicPots_line_1,line_1);

		//line_3
		//用P0,P1,P3计算出P7;
		if (m_click_points.size()>3)
		{
			P7 = m_click_points[3];
		}
		else
		{
			P7 = (P1 - P0) * (length(P3,P1)/length(P1,P0)) + P0;
			P7 = rotation(P7,P0,angle(P0-P1,P3-P1),direction(P0-P1,P3-P1)==0?1:0);
		}
		//用P0,P7计算出P8;
		P8 = (P7 - P0) * 0.55 + P0;
		P8 = rotation(P8,P0,20.0/180.0*PI,direction(P1-P0,P7-P0));
		QVector<QPointF> clicPots_line_3;
		clicPots_line_3.push_back(P7);
		clicPots_line_3.push_back(P8);
		clicPots_line_3.push_back(P0);
		getGUIBSplinePoint(clicPots_line_3,line_3);
		//line_2
		//求P5
		P5 = crossPoint(P0,P1,P3,P7);
		QPointF Pmid_01 = (P0+P1)/2.0;
		P5 = (P5 - Pmid_01)*0.35 + Pmid_01;

		//用P7和P5计算P6,P3和P5计算P4
		P4 = (P2-P5)*0.5+P5;
		P4 = (P3-P4)*0.2+P4;
		P6 = (P8-P5)*0.5+P5;
		P6 = (P7-P6)*0.2+P6;

		QVector<QPointF> clicPots_line_2;
		clicPots_line_2.push_back(P3);
		clicPots_line_2.push_back(P4);
		clicPots_line_2.push_back(P5);
		clicPots_line_2.push_back(P6);
		clicPots_line_2.push_back(P7);
		getGUIBSplinePoint(clicPots_line_2,line_2);
	}
	//添加箭头
	//制作箭头
	QPointF PA,PB,PC,PD,A0,A1,A2,A3;
	int flag = 0;
	int flag2 = 0;
	if (line_1.size()>20&&line_2.size()>20&&line_3.size()>20)
	{
		double L = length(P2,P4);
		double L2 = L;
		if (m_click_points.size()>3)
		{
			L2 = length(P8,P6);
		}
		//计算什么地方截取

		while (length(line_1[line_1.size()-flag-1],line_2[flag])<L/4.0)
		{
			flag++;
		}
		while (length(line_2[line_2.size()-flag2-1],line_3[flag2])<L2/4.0)
		{
			flag2++;
		}

		PA = line_1[line_1.size()-flag-1];
		PB = line_2[flag];
		PC = line_2[line_2.size()-flag2-1];
		PD = line_3[flag2];
		A0 = (PA-PB)*(1+0.5)+PB;
		A1 = (PB-PA)*(1+0.5)+PA;
		A2 = (PC-PD)*(1+0.5)+PD;
		A3 = (PD-PC)*(1+0.5)+PC;

	}
	//把line_0装进去
	for (int i = 0;i < line_0.size();i++)
	{
		m_BSpline_points.push_back(line_0[i]);
	}
	//把line_1装进去 末尾少装flag个
	for (int i = 0;i < line_1.size()-flag;i++)
	{
		m_BSpline_points.push_back(line_1[i]);
	}
	if (line_1.size()>10&&line_2.size()>10&&line_3.size()>10)
	{
		m_BSpline_points.push_back(A0);
		m_BSpline_points.push_back(P3);
		m_BSpline_points.push_back(A1);
	}

	//把line_2装进去 头尾分别少装flag和flag2个
	for (int i = flag;i < line_2.size()-flag2;i++)
	{
		m_BSpline_points.push_back(line_2[i]);
	}
	if (line_1.size()>10&&line_2.size()>10&&line_3.size()>10)
	{
		m_BSpline_points.push_back(A2);
		m_BSpline_points.push_back(P7);
		m_BSpline_points.push_back(A3);
	}
	//把line_3装进去 头少装flag2个
	for (int i = flag2;i <line_3.size();i++)
	{
		m_BSpline_points.push_back(line_3[i]);
	}
}

#endif


