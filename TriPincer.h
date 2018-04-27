#ifndef TRIPINCER_H
#define TRIPINCER_H

#include "QPointF"
#include "QVector"
#include "AidFunction.h"
#define PI 3.1415926

void Tri_pincer_attack(QVector<QPointF> m_click_points,QVector<QPointF>& m_BSpline_points)
{
	m_BSpline_points.clear();
	//得有五段B样条曲线：P0-P1的直线，P1-P3的曲线，P3-P7的曲线，P7-P11的曲线，P11-P0的曲线
	//初步想法改成：line0,line1,line2_1,line2_2,line3_1,line3_2,line_4
	QVector<QPointF> line_0,line_1,line_2,line_3,line_4,line_2_1,line_2_2,line_3_1,line_3_2;
	//点击两个点的阶段
	QPointF P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P_mid,P_aid_2,P_aid_3;
	if (m_click_points.size() > 1) 
	{
		P0 = m_click_points[0];
		P1 = m_click_points[1];
		P_mid = (P0 + P1)/2.0;
		QVector<QPointF> clicPots_line_0;
		clicPots_line_0.push_back(P0);
		clicPots_line_0.push_back(P1);
		getGUIBSplinePoint(clicPots_line_0,line_0);
	}
	//点击第三个点形成箭头的阶段
	if (m_click_points.size() > 2)
	{
		//line_1
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

		//line_4
		//用P0,P1,P3计算出P11;
		if (m_click_points.size()>4)
		{
			P11 = m_click_points[4];
		} 
		else
		{
			P11 = (P1 - P0) * (length(P3,P1)/length(P1,P0)) + P0;
			P11 = rotation(P11,P0,angle(P0-P1,P3-P1),direction(P0-P1,P3-P1)==0?1:0);
		}
		//用P0,P11计算出P12;
		P12 = (P11 - P0) * 0.55 + P0;
		P12 = rotation(P12,P0,20.0/180.0*PI,direction(P1-P0,P11-P0));
		QVector<QPointF> clicPots_line_4;
		clicPots_line_4.push_back(P11);
		clicPots_line_4.push_back(P12);
		clicPots_line_4.push_back(P0);
		getGUIBSplinePoint(clicPots_line_4,line_4);

		//用P11和P3计算出P7
		if (m_click_points.size()>3)
		{
			P7 = m_click_points[3];
		} 
		else
		{
			P7 = (P3+P11)/2.0;
		}		
		//用P7和P_mid计算出P_aid_2和P_aid_3
		P_aid_2 = (P7 - P_mid) * 0.55 + P_mid;
		P_aid_2 = rotation(P_aid_2,P_mid,5.0/180.0*PI,direction(P1-P_mid,P7-P_mid));
		P_aid_3 = (P7 - P_mid) * 0.55 + P_mid;
		P_aid_3 = rotation(P_aid_3,P_mid,5.0/180.0*PI,direction(P0-P_mid,P7-P_mid));


		//line_2
		//用P_mid,P1,P3,P7计算出P5
		P5 = crossPoint(P_mid,P1,P3,P7);
		QPointF Pmid_m1 = (P_mid-P1)*0.5+P1;
		P5 = (P5 - Pmid_m1)*0.2 + Pmid_m1;

		//将P5和P9向两翼拉扯
		double rate_5 = length(P1,P3)/length(P0,P1)- 0.2;
		if (rate_5 > 0.4)
		{
			rate_5 = 0.4;
		}
		if (rate_5 < 0)
		{
			rate_5 = 0;
		}

		double rate_9 = length(P0,P11)/length(P0,P1)- 0.2;
		if (rate_9 > 0.4)
		{
			rate_9 = 0.4;
		}
		if (rate_9 < 0)
		{
			rate_9 = 0;
		}
		P5 = (P2 - P5) * rate_5 + P5;

		//用P7和P5计算P6,P3和P5计算P4
		double rate_68 = 0.6;
		rate_68 = length(P0,P1)/length(P7,P_mid);
		if (rate_68 > 0.6)
		{
			rate_68 = 0.6;
		}
		if (rate_68 < 0.4)
		{
			rate_68 = 0.4;
		}

		P4 = (P2-P5)*0.5+P5;
		P4 = (P3-P4)*0.2+P4;
		P6 = (P_aid_2-P5)*rate_68+P5;
		P6 = (P7-P6)*0.2+P6;

// 		QVector<QPointF> clicPots_line_2;
// 		clicPots_line_2.push_back(P3);
// 		clicPots_line_2.push_back(P4);
// 		clicPots_line_2.push_back(P5);
// 		clicPots_line_2.push_back(P6);
// 		clicPots_line_2.push_back(P7);
// 		getGUIBSplinePoint(clicPots_line_2,line_2);

		//Line_3
		//根据P0,P_mid,P7,P11计算出P9
		P9 = crossPoint(P0,P_mid,P7,P11);
		QPointF Pmid_0m = (P_mid-P0)*0.5+P0;
		P9 = (P9 - Pmid_0m)*0.2 + Pmid_0m;
		//将P5和P9向两翼拉扯
		P9 = (P12 - P9) * rate_9+ P9;
		//用P7和P9计算P8,P11和P9计算P10
		P8 = (P_aid_3-P9)*rate_68+P9;
		P8 = (P7-P8)*0.2+P8;
		P10 = (P12-P9)*0.5+P9;
		P10 = (P11-P10)*0.2+P10;

		//swap_points(P6,P8);

		QVector<QPointF> clicPots_line_2;
		clicPots_line_2.push_back(P3);
		clicPots_line_2.push_back(P4);
		clicPots_line_2.push_back(P5);
		clicPots_line_2.push_back(P6);
		clicPots_line_2.push_back(P7);
		getGUIBSplinePoint(clicPots_line_2,line_2);

		QVector<QPointF> clicPots_line_3;
		clicPots_line_3.push_back(P7);
		clicPots_line_3.push_back(P8);
		clicPots_line_3.push_back(P9);
		clicPots_line_3.push_back(P10);
		clicPots_line_3.push_back(P11);
		getGUIBSplinePoint(clicPots_line_3,line_3);
	}
	//添加箭头
	//制作箭头
	QPointF PA,PB,PC,PD,PE,PF,A0,A1,A2,A3,A4,A5;
	int flag0 = 0;
	int flag1 = 0;
	int flag2 = 0;

	if (line_1.size()>20&&line_2.size()>20&&line_3.size()>20&&line_4.size()>10)
	{
		double L0 = length(P2,P4);
		double L1 = L0;
		double L2 = L0;

		if (m_click_points.size()>3)
		{
			L1 = length(P8,P6);
		}
		if (m_click_points.size()>4)
		{
			L2 = length(P10,P12);
		}
		//计算什么地方截取

		while (length(line_1[line_1.size()-flag0-1],line_2[flag0])<L0/4.0)
		{
			flag0++;
		}
		while (length(line_3[flag1],line_2[line_2.size()-flag1-1])<L1/4.0)
		{
			flag1++;
		}
		while (length(line_4[flag2],line_3[line_3.size()-flag2-1])<L2/4.0)
		{
			flag2++;
		}

		PA = line_1[line_1.size()-flag0-1];
		PB = line_2[flag0];
		PC = line_2[line_2.size()-flag1-1];
		PD = line_3[flag1];
		PE = line_3[line_3.size()-flag2-1];
		PF = line_4[flag2];
		A0 = (PA-PB)*1.5+PB;
		A1 = (PB-PA)*1.5+PA;
		A2 = (PC-PD)*1.5+PD;
		A3 = (PD-PC)*1.5+PC;
		A4 = (PE-PF)*1.5+PF;
		A5 = (PF-PE)*1.5+PE;

	}

	//把line_0装进去
	for (int i = 0;i < line_0.size();i++)
	{
		m_BSpline_points.push_back(line_0[i]);
	}
	//把line_1装进去 末尾少装flag0个
	for (int i = 0;i < line_1.size()-flag0;i++)
	{
		m_BSpline_points.push_back(line_1[i]);
	}
	if (line_1.size()>10&&line_2.size()>10&&line_3.size()>10)
	{
		m_BSpline_points.push_back(A0);
		m_BSpline_points.push_back(P3);
		m_BSpline_points.push_back(A1);
	}

	//把line_2装进去 首尾分别少装flag0和flag1个
	for (int i = flag0;i < line_2.size()-flag1;i++)
	{
		m_BSpline_points.push_back(line_2[i]);
	}
	if (line_1.size()>10&&line_2.size()>10&&line_3.size()>10)
	{
		m_BSpline_points.push_back(A2);
		m_BSpline_points.push_back(P7);
		m_BSpline_points.push_back(A3);
	}
	//把line_3装进去 首尾分别少装flag1个和flag2个
	for (int i = flag1;i <line_3.size()-flag2;i++)
	{
		m_BSpline_points.push_back(line_3[i]);
	}
	if (line_1.size()>10&&line_2.size()>10&&line_3.size()>10)
	{
		m_BSpline_points.push_back(A4);
		m_BSpline_points.push_back(P11);
		m_BSpline_points.push_back(A5);
	}
	//把line_4装进去 首部少装flag2个
	for (int i = flag2;i <line_4.size();i++)
	{
		m_BSpline_points.push_back(line_4[i]);
	}
}
#endif



 


