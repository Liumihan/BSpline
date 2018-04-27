#ifndef STAGING_AREA_H
#define STAGING_AREA_H

#include "QPointF"
#include "QVector"
#include "AidFunction.h"

void staging_area(QVector<QPointF> m_click_points,QVector<QPointF>& m_BSpline_points)
{
	//////////////////////////////////////////////////////////////////////////
	//һ�����ݸ������� �����������������߻�����ʵ�֡�
	//////////////////////////////////////////////////////////////////////////

	if (m_click_points.size() > 1)
	{

		QVector<QPointF> m_result;
		QPointF P0,P1,P2,P3,P4,P5;
		QPointF T54,T53;
		QPointF temp_point;

		P5 = m_click_points[0];
		P3 = m_click_points[1];
		//P4 = m_click_points[2];

		//����P0��P1��P2
		double k0 = 0.25;//��Ϊ���P0��P2��ģ��������߶Σ�P3��P5��ģ����ϵ��
		double k1 = 1.0/(2.0*cos(16.0/180.0*PI));
		double k4 = 1.0/(2.0*cos(25.0/180.0*PI));//
// 		if (m_click_points.size() > 2)
// 		{
// 			P4 = m_click_points[2];
// 		}
// 		if (m_click_points.size() < 3)
		{
			//��P5������P4;
			temp_point = (P3 - P5)*k4 + P5;
			P4 = rotation(temp_point,P5,25.0/180.0*PI,0);
		}

		T54 = P4 - P5;//���������ж���ԽǶȣ�������P0��P1;
		T53 = P3 - P5;

		int m_direction = direction(T53,T54);
		//��P5������P1
		temp_point = (P3 - P5)*k1 + P5;
		P1 = rotation(temp_point,P5,16.0/180.0*PI,m_direction == 0 ? 1 : 0);

		//��P5������P0
		temp_point = (P3 - P5)*k0 + P5;
		P0 = rotation(temp_point,P5,50.0/180.0*PI,m_direction == 0 ? 1 : 0);//temp_point��ʱ����ת45�㵽��P0��
		//��P3������P2						
		temp_point = (P5 - P3)*k0 + P3;
		P2 = rotation(temp_point,P3,50.0/180.0*PI,m_direction);

		m_result.push_back(P0);
		m_result.push_back(P1);
		m_result.push_back(P2);
		m_result.push_back(P3);
		m_result.push_back(P4);
		m_result.push_back(P5);

		QVector<QPointF> m_staging_ctrl_points = compute_close_ctrlPoints(m_result);

		m_BSpline_points = getSplinePoints(m_staging_ctrl_points);
	}

}



#endif
