#ifndef LINE_H
#define LINE_H
#include "QPointF"
#include "QVector"
#include "AidFunction.h"

 void finsertAidPoints(QVector<QPointF> m_click_points,QVector<QPointF>& m_BSpline_points)
 {
 	QVector<QPointF> m_dikaer_click_points;
 	QVector<QPointF> m_clicPForAidPoints;
 	QVector<QPointF> m_dikaerclicPForAidPonts;
 	QVector<QPointF> m_clickPointsForComputeSpline;
 	QVector<QPointF> m_points_for_sending;//模拟传输的点集
 	//把原始的点击的点复制一份给m_points_for_sending
 	m_points_for_sending = m_click_points;
 
 	//想办法模拟之前的点击过程
 	while(!m_points_for_sending.isEmpty())
 	{
 		//点击一个
 		QPointF temp = m_points_for_sending[0];
 		m_points_for_sending.pop_front();//删除掉第一个点
 		//同时加入到m_click_points_for_compute和m_click_pointsForAid;
 		m_clicPForAidPoints.append(temp);
 		m_clickPointsForComputeSpline.append(temp);
 
 
 
 		if (m_clicPForAidPoints.size() < 3)
 		{
 			getGUIBSplinePoint(m_clickPointsForComputeSpline,m_BSpline_points);
 		}
 		if (m_clicPForAidPoints.size() >= 3) 
 		{
 			int n = m_clickPointsForComputeSpline.size();
 			int m = m_clicPForAidPoints.size();
 
 			m_dikaer_click_points = m_clickPointsForComputeSpline;
 			m_dikaerclicPForAidPonts = m_clicPForAidPoints;
 
 
 			QPointF A0 = m_dikaerclicPForAidPonts[m-3];
 			QPointF A1 = m_dikaerclicPForAidPonts[m-2];
 			QPointF A2 = m_dikaerclicPForAidPonts[m-1];
 
 			QVector<QPointF> m_two_aidPoint;
 
 			QPointF T1 = A0 - A1;
 			QPointF T2 = A2 - A1;
 
 			double moudle_T1 = sqrt(T1.x()*T1.x() + T1.y() * T1.y());
 			double moudle_T2 = sqrt(T2.x()*T2.x() + T2.y() * T2.y());
 			//计算两个向量之间的夹角
 			double theta;
 			double m_costheta;
 
 			m_costheta = (T1.x()*T2.x() + T1.y()*T2.y())/(moudle_T1*moudle_T2);
 			theta = acos(m_costheta);
 			//如果角度大于30°就不处理,并且将insert_flag = 0；
 			if(theta > 4.0/8.0 * PI)
 			{
 
 			} 
 			else
 			{
 				//判断T1是顺时针还是逆时针转到T2的
 				int m_direction = direction(T1,T2);
 				//计算三个点之间插入辅助点
 				m_two_aidPoint = compute_two_aid_point(A0,A1,A2);
 				//插入两个辅助点
 				if (m_direction == 0)
 				{
 					m_dikaer_click_points.insert(n - 2,m_two_aidPoint[1]);//插入顺时针的点；
 					m_dikaer_click_points.insert(n,m_two_aidPoint[0]);//插入逆时针的点；
 				}
 
 				if (m_direction == 1)
 				{
 					m_dikaer_click_points.insert(n - 2,m_two_aidPoint[0]);//插入逆时针的点；
 					m_dikaer_click_points.insert(n,m_two_aidPoint[1]);//插入顺时针的点；
 				}
 				m_clickPointsForComputeSpline = m_dikaer_click_points;
 			}
 			getGUIBSplinePoint(m_clickPointsForComputeSpline,m_BSpline_points);
 		}	
 
 	}	
 }


#endif
