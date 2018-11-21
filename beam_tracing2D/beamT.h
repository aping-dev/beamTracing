// 改自：http://blog.kaistale.com/?cat=12

#include <vector>
#include <math.h>
using namespace std;

enum isOnline{OnLine1, OnLine2, OnRay1, OnRay2};

class Point
{
public:
	double x, y;

	Point(){}
	Point(double x1, double y1): x(x1), y(y1){}
	Point(const Point &p): x(p.x), y(p.y){}
};

class Line
{
public:
	Point p1, p2;
	
	Line(){}
	Line(const Point &p11): p1(p11){}
	Line(const Point &p11, const Point &p21): p1(p11), p2(p21){}
};

class BSPNode
{
public:
	int id;                  // 墙 id
	Point p1, p2;            // id墙的两个端点，可能其中一点为墙与分割线的交点
	
	BSPNode *front;   
	BSPNode *back;

	BSPNode()
	{
		id = -2;
		p1 = Point(-1, -1);
		p2 = Point(-1, -1);
		front = NULL;
		back = NULL;
	}

	BSPNode(int id1): id(id1)
	{
		p1 = Point(-1, -1);
		p2 = Point(-1, -1);
		front = NULL;
		back = NULL;
	}

	BSPNode(int id1, const Point &p11, Point &p22): id(id1), p1(p11), p2(p22)
	{
		front = NULL;
		back = NULL;
	}
};

class Beam
{
public:
	Point vs;       // 虚拟源
	Point p1, p2;   
	
	Beam(){}
	Beam(const Beam &b): vs(b.vs), p1(b.p1), p2(b.p2){}
	Beam(const Point &vs1, const Point &p11, const Point &p21): vs(vs1), p1(p11), p2(p21){}
};

class BeamNode
{
public:
	int id;
	Point vs;
	BeamNode *parent;
	vector<BeamNode *> children;
	
	BeamNode(){}
	BeamNode(int id1): id(id1){}
	BeamNode(int id1, BeamNode *parent1, Point vs1): id(id1), parent(parent1), vs(vs1){}
};

class BeamTrace
{
public:
	Point listener;
	Point source;
	vector<Line> walls;
	BSPNode *bspNode;
	BeamNode *beamNode;
	
	// 返回两条线的交点。
	// https://blog.csdn.net/wcl0617/article/details/78654944
	Point lineIntersection(double x11, double y11, double x12, double y12, double x21, double y21, double x22, double y22, bool flag[4])
	{
		double denominator = ((y22 - y21) * (x12 - x11)) - ((x22 - x21) * (y12 - y11));
        if (fabs(denominator) < 0.0001) 
		{
            return Point(-1, -1);
        }
        double a = y11 - y21;
        double b = x11 - x21;
        double numerator1 = ((x22 - x21) * a) - ((y22 - y21) * b);
        double numerator2 = ((x12 - x11) * a) - ((y12 - y11) * b);
		
        a = numerator1 / denominator;
        b = numerator2 / denominator;
		
		Point result(-1, -1);
        result.x = x11 + (a * (x12 - x11));
        result.y = y11 + (a * (y12 - y11));
        if (a > 0)
        {
            flag[OnRay1] = true;
            if(a < 1)
			{
                flag[OnLine1] = true;
            }
        }
        if(b > 0)
		{
            flag[OnRay2] = true;
            if(b < 1) 
			{
                flag[OnLine2] = true;
            }
        }
		
		return result;
	}
	
	// 返回true，如果p0 在线段p1--p2的前面
	bool inFrontOf(Point p0, Point p1, Point p2)
	{
		Point n1(-(p2.y - p1.y), p2.x - p1.x);
		return n1.x * (p0.x - p1.x) + n1.y * (p0.y - p1.y) > 0;
	}
	
	// 返回p0关于p1--p2的镜像
	Point pointMirror(Point p0, Point p1, Point p2)
	{
		Point n1 = Point(-(p2.y - p1.y), p2.x - p1.x);
		double n1_len = sqrt(n1.x * n1.x + n1.y * n1.y);
		n1 = Point(n1.x / n1_len, n1.y / n1_len);
		
		double dist = 2 * (n1.x * (p0.x - p1.x) + n1.y * (p0.y - p1.y));
		return Point(p0.x - n1.x * dist, p0.y - n1.y * dist);
	}
	
	// 检查w2中的点是否在w1的前面，不是的话，返回分割w2后的结果
	int BSPDivide(BSPNode *w1, BSPNode *w2, Point &p11, Point &p12, Point &p21, Point &p22)
	{
		bool w2_nx = inFrontOf(w2->p1, w1->p1, w1->p2);
		bool w2_ny = inFrontOf(w2->p2, w1->p1, w1->p2);
		
		if(w2_nx && w2_ny)
		{
			return 1;  // w2 在 w1 的前面
		}
		
		if(!w2_nx && !w2_ny)
		{
			return -1; // w2 在 w1 的后面
		}
		
		bool flag[4];
		memset(flag, false, sizeof(flag));
		Point p3 = lineIntersection(w1->p1.x, w1->p1.y, w1->p2.x, w1->p2.y, w2->p1.x, w2->p1.y, w2->p2.x, w2->p2.y, flag);
		if(w2_nx)
		{
			// w2.p1 在 w1 的前面，w2.p2 在 w2 的后面
			p11 = w2->p1;
			p12 = p3;
			p21 = p3;
			p22 = w2->p2;
		}
		else
		{
			// w2.p1 在 w1 的后面，w2.p2 在 w2 的前面
			p11 = p3;
			p12 = w2->p2;
			p21 = w2->p1;
			p22 = p3;
		}
		return 0;
	}
	
	// 创建BSP树
	BSPNode* BSPBuilder(vector<BSPNode *> &recursiveArray)
	{
		if(recursiveArray.size() == 0)
			return NULL;
		
		if(recursiveArray.size() == 1)
			return recursiveArray[0];

		vector<BSPNode *> frontList;
		vector<BSPNode *> backList;
		while(recursiveArray.size() > 1)
		{
			BSPNode *node = recursiveArray.back();
			recursiveArray.pop_back();
			
			Point p11, p12, p21, p22;
			int retval = BSPDivide(recursiveArray[0], node, p11, p12, p21, p22);
			
			if(retval == 1)
			{
				frontList.push_back(node);
			}
			else if(retval == -1)
			{
				backList.push_back(node);
			}
			else
			{
				frontList.push_back(new BSPNode(node->id, p11, p12));
				backList.push_back(new BSPNode(node->id, p21, p22));
			}
		}
		
		recursiveArray[0]->front = BSPBuilder(frontList);
		recursiveArray[0]->back = BSPBuilder(backList);
		
		return recursiveArray[0];
	}

	// 构建 BSP 树
	BSPNode* BSPTree(vector<Line> &walls)
	{
		vector<BSPNode *> recursiveArray;
		
		for(int i=0; i<walls.size(); i++)
			recursiveArray.push_back(new BSPNode(i, walls[i].p1, walls[i].p2));
		
		bspNode = BSPBuilder(recursiveArray);

		return bspNode;
	}
	
	// 根据给定beam，创建反射beams。
	void beamBuild(Beam *beam, BeamNode *node, const vector<Line> &walls, int order, int MAX_ORDER)
	{
		if(order > MAX_ORDER)
			return;
		
		// 使得虚拟源位于墙的后面
		if(inFrontOf(beam->vs, beam->p1, beam->p2))
		{
			Point temp = beam->p2;
			beam->p2 = beam->p1;
			beam->p1 = temp;
		}
		
		bool flag[4];
		for(int i=0; i<walls.size(); i++)
		{
			if(node->id == i)
				continue;
			
			// A、B(inside)、C 三条线段
			// A\___/C
			//    B
			//  <source>
			
			bool p1_b = !inFrontOf(walls[i].p1, beam->p1, beam->p2);
			bool p2_b = !inFrontOf(walls[i].p2, beam->p1, beam->p2);
			if(p1_b && p2_b) // 墙位于beam的外面，不可能发生反射
				continue;

			bool p1_a = !inFrontOf(walls[i].p1, beam->vs, beam->p2);
			bool p2_a = !inFrontOf(walls[i].p2, beam->vs, beam->p2);
			if(p1_a && p2_a) 
				continue;

			bool p1_c = inFrontOf(walls[i].p1, beam->vs, beam->p1);
			bool p2_c = inFrontOf(walls[i].p2, beam->vs, beam->p1);
			if(p1_c && p2_c)
				continue;

			// 可能发生反射
			bool p1_in = !p1_a && !p1_b && !p1_c;
			bool p2_in = !p2_a && !p2_b && !p2_c;
			
			bool A = false;
			bool B = false;
			bool C = false;
			
			Line new_line(Point(-1, -1), Point(-1, -1));

			if(p1_in && p2_in) // 整个墙都在beam内，可构成反射beam的一部分
			{
				new_line = Line(walls[i].p1, walls[i].p2);
			}
			else if(p1_in)  // p1在beam内，p2在beam外
			{
				new_line.p1 = walls[i].p1;

				if(p2_a && !p2_b) // 与线段A相交
				{
					A = true;
				}
				else if(p2_a && p2_b && p2_c)  // 可能与线段A或B或C相交
				{
					A = true;
					B = true;
					C = true;
				}
				else if(p2_a && p2_b)  // 可能与线段A或B相交
				{
					A = true;
					B = true;
				}
				else if(!p2_a && p2_b && !p2_c)  
				{
					B = true;
				}
				else if(p2_c && p2_b)
				{
					B = true;
					C = true;
				}
				else if(p2_c && !p2_b)
				{
					C = true;
				}
			}
			else if(p2_in)  // p2在beam内，p1在beam外
			{
				new_line.p1 = walls[i].p2;

				if(p1_a && !p1_b)
				{
					A = true;
				}
				else if(p1_a && p1_b && p1_c)
				{
					A = true;
					B = true;
					C = true;
				}
				else if(p1_a && p1_b)
				{
					A = true;
					B = true;
				}
				else if(!p1_a && p1_b && !p1_c)
				{
					B = true;
				}
				else if(p1_c && p1_b)
				{
					B = true;
					C = true;
				}
				else if(p1_c && !p1_b)
				{
					C = true;
				}
			}
			else  // 所有的交点都在边界外，可能有2个交点，也可能没有交点
			{
				if(p1_a && p2_b || p2_a && p1_b)  // 与线段A或B相交
				{
					memset(flag, false, sizeof(flag));
					Point int_a = lineIntersection(beam->p2.x, beam->p2.y, beam->p2.x+(beam->p2.x-beam->vs.x)*2, beam->p2.y+(beam->p2.y-beam->vs.y)*2, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
             
					if(flag[OnRay1])
					{	
						Point int_b = lineIntersection(beam->p1.x, beam->p1.y, beam->p2.x, beam->p2.y, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
						new_line = Line(int_a, int_b);
					}
				}
				else if(p1_b && p2_c || p2_b && p1_c)  // 与线段B或C相交
				{
					memset(flag, false, sizeof(flag));
					Point int_b = lineIntersection(beam->p1.x, beam->p1.y, beam->p2.x, beam->p2.y,walls[i].p1.x, walls[i].p1.y,walls[i].p2.x,walls[i].p2.y, flag);
					if(flag[OnRay1])
					{ 
						Point int_c = lineIntersection(beam->p1.x, beam->p1.y,beam->p1.x+(beam->p1.x-beam->vs.x)*2,beam->p1.y+(beam->p1.y-beam->vs.y)*2, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
						new_line = Line(int_b, int_c); 
					}
				}
				else if((p1_a && p2_c || p2_a && p1_c) && (!p1_b && !p2_b))  // 与线段A和C相交
				{
					Point int_a = lineIntersection(beam->p2.x, beam->p2.y, beam->p2.x+(beam->p2.x-beam->vs.x)*2,beam->p2.y+(beam->p2.y-beam->vs.y)*2,walls[i].p1.x,walls[i].p1.y,walls[i].p2.x,walls[i].p2.y, flag);
					Point int_c = lineIntersection(beam->p1.x, beam->p1.y, beam->p1.x+(beam->p1.x-beam->vs.x)*2,beam->p1.y+(beam->p1.y-beam->vs.y)*2,walls[i].p1.x,walls[i].p1.y,walls[i].p2.x,walls[i].p2.y, flag);
					new_line = Line(int_a, int_c);
				}
			}
			
			// 以下部分为针对只有一个交点的情况求交点
			Point intersection(-1, -1);

			// 与线段A相交
			memset(flag, false, sizeof(flag));
			if(A && !B && !C)     // 与线段A相交
				intersection = lineIntersection(beam->vs.x, beam->vs.y, beam->p2.x, beam->p2.y, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
			else if(A && B && C)  // 与线段A或B或C相交
			{
				memset(flag, false, sizeof(flag));
				intersection = lineIntersection(beam->p2.x, beam->p2.y, beam->p2.x+(beam->p2.x-beam->vs.x),beam->p2.y+(beam->p2.y-beam->vs.y),walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
				if(!flag[OnRay1])
					intersection = lineIntersection(beam->p1.x, beam->p1.y, beam->p2.x, beam->p2.y, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
				memset(flag, false, sizeof(flag));
				if(!flag[OnRay1])
					intersection = lineIntersection(beam->p1.x, beam->p1.y, beam->p1.x+(beam->p1.x-beam->vs.x)*2,beam->p1.y+(beam->p1.y-beam->vs.y)*2,walls[i].p1.x,walls[i].p1.y,walls[i].p2.x,walls[i].p2.y, flag);
			}
			else if(A && B)        // 与线段A或B相交
			{
				memset(flag, false, sizeof(flag));
				intersection = lineIntersection(beam->p2.x, beam->p2.y, beam->p2.x+(beam->p2.x-beam->vs.x), beam->p2.y+(beam->p2.y-beam->vs.y),walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
				if(!flag[OnRay1])
					intersection = lineIntersection(beam->p1.x, beam->p1.y, beam->p2.x, beam->p2.y, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
			}
			else if(!A && B && !C) // 与线段B相交
				intersection = lineIntersection(beam->p1.x, beam->p1.y, beam->p2.x, beam->p2.y, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
			else if(B && C)        // 与线段B或C相交
			{
				memset(flag, false, sizeof(flag));
				intersection = lineIntersection(beam->p1.x, beam->p1.y, beam->p2.x, beam->p2.y, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
				if(!flag[OnRay1])
					intersection = lineIntersection(beam->p1.x, beam->p1.y, beam->p1.x+(beam->p1.x-beam->vs.x), beam->p1.y+(beam->p1.y-beam->vs.y), walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
			}
			else if(!A && !B && C)  // 与线段C相交
				intersection = lineIntersection(beam->vs.x, beam->vs.y, beam->p1.x, beam->p1.y, walls[i].p1.x, walls[i].p1.y, walls[i].p2.x, walls[i].p2.y, flag);
			
			if(intersection.x != -1)
			{
				new_line.p2 = intersection;
			}
			
			// 递归
			if(new_line.p2.x != -1)
			{
				Beam *new_beam = new Beam(pointMirror(beam->vs, walls[i].p1, walls[i].p2), new_line.p1, new_line.p2);
				node->children.push_back(new BeamNode(i, node, new_beam->vs));
				beamBuild(new_beam, node->children[node->children.size() - 1], walls, order + 1, MAX_ORDER);
			}
        }
	}
	
	// 构造beam树
    BeamNode* BeamTree(const Point &source, vector<Line> walls, int MAX_ORDER)
    {
        beamNode = new BeamNode(-1, NULL, source); // 墙的 id 为-1，说明源是真实的源
		
        // beams 位于叶子节点
		vector<Beam*> beams;
        for(int i = 0; i < walls.size(); i++)
        {
            Point vs = pointMirror(source, walls[i].p1, walls[i].p2);
			Beam *newBeam = new Beam(vs, walls[i].p1, walls[i].p2);
            beams.push_back(newBeam);
            beamNode->children.push_back(new BeamNode(i, beamNode, vs)); // 构造第一代 beams (第一批叶子）
			
            // 递归构造子 beam
            int order = 0;
            beamBuild(beams[beams.size() - 1], beamNode->children[beamNode->children.size() - 1], walls, order, MAX_ORDER);
        }

		return beamNode;
    }
	
    /* 射线追踪 */
	
    // 返回与 id 为 valid_id 的墙的交点，射线的方向为 p1 到 p2 
    Point rayTrace(Point p1, Point p2, BSPNode *bsp_node, int ignore_id, int valid_id, int order, int &wallID, bool flag[4]) 
    {
        if(!bsp_node) 
			return Point(-1, -1); // 到达 BSP 树的底部
		
		Point inte(-1, -1);

        // 射线跟踪利用了 bsp 树从前到后的顺序
        if(inFrontOf(p1, bsp_node->p1, bsp_node->p2)) // p1 在 bsp_node 的前面
        {
			memset(flag, false, sizeof(flag));
            inte = rayTrace(p1, p2, bsp_node->front, ignore_id, valid_id, order, wallID, flag);
            if(inte.x == -1 || !flag[OnLine1] || !flag[OnLine2]) 
			{
                order++;

				// 射线跟踪当前 bsp_node
				memset(flag, false, sizeof(flag));
                inte = lineIntersection(p1.x, p1.y, p2.x, p2.y, bsp_node->p1.x, bsp_node->p1.y, bsp_node->p2.x, bsp_node->p2.y, flag); 
                if(bsp_node->id == ignore_id) 
					inte.x = -1;
                if(inte.x != -1) 
					wallID = bsp_node->id;
            }
            if(inte.x == -1 || !flag[OnLine1] || !flag[OnLine2]) // 没有任何交点
			{
				memset(flag, false, sizeof(flag));
                inte = rayTrace(p1, p2, bsp_node->back, ignore_id, valid_id, order, wallID, flag);
			}
        }
        else
        {
			memset(flag, false, sizeof(flag));
            inte = rayTrace(p1, p2, bsp_node->back, ignore_id, valid_id, order, wallID, flag);
            if(inte.x == -1 || !flag[OnLine1] || !flag[OnLine2])
			{
                order++;
				memset(flag, false, sizeof(flag));
                inte = lineIntersection(p1.x, p1.y, p2.x, p2.y, bsp_node->p1.x, bsp_node->p1.y, bsp_node->p2.x, bsp_node->p2.y, flag);
                if(bsp_node->id == ignore_id) 
					inte.x = -1;
                if(inte.x != -1) 
					wallID = bsp_node->id;
            }
            if(inte.x == -1 || !flag[OnLine1] || !flag[OnLine2])
			{
				memset(flag, false, sizeof(flag));
                inte = rayTrace(p1, p2, bsp_node->front, ignore_id, valid_id, order, wallID, flag);
			}
        }
		
        return inte;
    }
	
    // 对给定的 beam 进行递归遍历，以检测交点
	// 第一次调用时，prev_node 与 p_tree 为空
    void traverseBeam(Point p0, const vector<Line> &walls, BSPNode *bsp, BeamNode *beam, BeamNode *prev_beam, vector<Point> &path)
    {
        if(prev_beam == NULL) // 第一次调用
        {
            prev_beam = new BeamNode(-1); // 目前，射线跟踪中没有需要忽略的墙
            path.push_back(p0);         // 将 listener 的位置加入路径
        }
		
        // 发现从当前位置到下一个虚拟源之间的交点
        // - 忽略 prev_beam.id (前一个交点所在的墙)
		bool flag[4];
		memset(flag, false, sizeof(flag));
		int wallID = -2;
        Point inte = rayTrace(p0, beam->vs, bsp, prev_beam->id, beam->id, 0, wallID, flag);
        if(inte.x == -1 || (beam->id != -1 && wallID != beam->id) || !flag[OnLine1] || !flag[OnLine2])
            inte.x = -1; // 没有与光束相交，或交点不在光束内
		
        if(beam->id == -1) // 通往源点的路径，不需要碰撞检测
        {
            if(inte.x == -1) // 没有任何交点，说明可行
            {
                path.push_back(beam->vs); // 最终，将源点加入路径
                return; // 结束
            }
            else
			{
				path.clear();
                return; // 通往源点的路径被阻塞 
            }
        }
        else
        {
            // 没有交点，说明该路径非法
            if(inte.x == -1) 
			{
				path.clear();
				return;
			}
            else
            {
                // 发现了一个交点，继续遍历 beam 的父节点
                path.push_back(inte);
                traverseBeam(inte, walls, bsp, beam->parent, beam, path);
                //return;
            }
        }
    }
	
    // 递归遍历所有 beam, 返回反射路径
    vector<vector<Point> > findPaths(Point listener, const vector<Line> &walls, BSPNode *bsp, BeamNode *beam)
    {
		vector<vector<Point> > path_array;

        for(int i = 0; i < beam->children.size(); i++) // 递归遍历所有 beam 节点
        {
			vector< vector<Point> > tmp = findPaths(listener, walls, bsp, beam->children[i]);

			for(int j=0; j<tmp.size(); j++)
				path_array.push_back(tmp[j]);
        }

        vector<Point> path;
		traverseBeam(listener, walls, bsp, beam, NULL, path); // 寻找到当前 beam 节点的所有合法路径
        if(path.size()) // 如果发现了合法路径
		{
			path_array.push_back(path);  
		}

		return path_array;
    }
	
	void init()
	{
		// 初始化
		walls.push_back( Line(Point(100, 130), Point(120, 220)) );
		walls.push_back( Line(Point(50, 55), Point(220, 60)) );
		walls.push_back( Line(Point(220, 60), Point(250, 220)) );
		walls.push_back( Line(Point(50, 220), Point(200, 220)) );
		walls.push_back( Line(Point(50, 220), Point(50, 55)) );
		walls.push_back( Line(Point(200, 220), Point(40, 230)) );
		walls.push_back( Line(Point(40, 230), Point(30, 290)) );
		walls.push_back( Line(Point(30, 290), Point(60, 270)) );
		walls.push_back( Line(Point(60, 270), Point(290, 270)) );
		walls.push_back( Line(Point(290, 270), Point(250, 220)) );

		listener = Point(80, 100);
		source = Point(200, 80);
	}

	vector<vector<Point> > getPaths()
	{
		bspNode = BSPTree(walls);
		beamNode = BeamTree(source, walls, 4);
	
		vector<vector<Point> > paths = findPaths(listener, walls, bspNode, beamNode);
		return paths;
	}
};
