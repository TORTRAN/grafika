class Ship{
public:
	Point center;
	std::vector<Point> arrPoint;
	std::vector<Point> arrPointOriginal;
	
	Ship(Point x){
		center=x;
		arrPoint.push_back(Point(center.x-100,center.y));
		arrPoint.push_back(Point(center.x+100,center.y));
		arrPoint.push_back(Point(center.x-50,center.y+50));
		arrPoint.push_back(Point(center.x+50,center.y+50));
		arrPoint.push_back(Point(center.x,center.y-50));
		arrPoint.push_back(Point(center.x+30,center.y-30));
		arrPoint.push_back(Point(center.x,center.y-10));
		arrPoint.push_back(Point(center.x,center.y));
		arrPointOriginal=arrPoint;	
	}
	
	void Draw(){
		drawLine(arrPoint[0].x,arrPoint[0].y, arrPoint[1].x,arrPoint[1].y);
		drawLine(arrPoint[0].x,arrPoint[0].y, arrPoint[2].x,arrPoint[2].y);
		drawLine(arrPoint[1].x,arrPoint[1].y, arrPoint[3].x,arrPoint[3].y);
		drawLine(arrPoint[2].x,arrPoint[2].y, arrPoint[3].x,arrPoint[3].y);	
		drawLine(arrPoint[7].x,arrPoint[7].y, arrPoint[4].x,arrPoint[4].y);
		drawLine(arrPoint[4].x,arrPoint[4].y, arrPoint[5].x,arrPoint[5].y);
		drawLine(arrPoint[5].x,arrPoint[5].y, arrPoint[6].x,arrPoint[6].y);	
	}

	void Update(){
		arrPoint[0] = Point(center.x-100,center.y);
		arrPoint[1] = Point(center.x+100,center.y);
		arrPoint[2] = Point(center.x-50,center.y+50);
		arrPoint[3] = Point(center.x+50,center.y+50);
		arrPoint[4] = Point(center.x,center.y-50);
		arrPoint[5] = Point(center.x+30,center.y-30);
		arrPoint[6] = Point(center.x,center.y-10);
		arrPoint[7] = Point(center.x,center.y);
		arrPointOriginal=arrPoint;
	}

	void MoveDown(){
		center.y += 10;
		Update(); 
		Draw();
	}

	void MoveUp(){
		center.y -= 10;
		Update(); 
		Draw();
	}

	void MoveRight(){
		center.x += 10;
		Update();
		Draw();
	}

	void MoveLeft(){
		center.x -= 10;
		Update(); 
		Draw();
	}
};