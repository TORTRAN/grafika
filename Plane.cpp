class Plane{
public:	
	Point center;
	std::vector<Point> arrPoint;
	std::vector<Point> arrPointOriginal;
	BirdPattern birdPattern;

	Plane(Point x){
		center=x;
		arrPoint.push_back(Point(center.x-50,center.y));
		arrPoint.push_back(Point(center.x+50,center.y-30));
		arrPoint.push_back(Point(center.x+5,center.y));
		arrPoint.push_back(Point(center.x+50,center.y+30));
		arrPointOriginal=arrPoint;
	}
	
	void setColorFloodPlane(Point P){
		Color C = birdPattern.getColor(P.x, P.y);

		setPixelCol(P.x,P.y,C);
		if(getColor(Point(P.x+1,P.y)).isBlack()){
			setColorFloodPlane(Point(P.x+1,P.y));
		}
		if(getColor(Point(P.x,P.y+1)).isBlack()){
			setColorFloodPlane(Point(P.x,P.y+1));
		}
		if(getColor(Point(P.x-1,P.y)).isBlack()){
			setColorFloodPlane(Point(P.x-1,P.y));
		}
		if(getColor(Point(P.x,P.y-1)).isBlack()){
			setColorFloodPlane(Point(P.x,P.y-1));
		}
	}

	void Update(){
		arrPoint[0] = Point(center.x-50,center.y);
		arrPoint[1] = Point(center.x+50,center.y-30);
		arrPoint[2] = Point(center.x+5,center.y);
		arrPoint[3] = Point(center.x+50,center.y+30);
		arrPointOriginal=arrPoint;
	}

	void Draw(){
		Update();
		drawLine(arrPoint[0].x,arrPoint[0].y,arrPoint[1].x,arrPoint[1].y);
		drawLine(arrPoint[0].x,arrPoint[0].y,arrPoint[3].x,arrPoint[3].y);
		drawLine(arrPoint[2].x,arrPoint[2].y,arrPoint[1].x,arrPoint[1].y);
		drawLine(arrPoint[2].x,arrPoint[2].y,arrPoint[3].x,arrPoint[3].y);
		setColorFloodPlane(center);		
	}

	void Swap(){
		if(!(arrPointOriginal[0].x == arrPoint[0].x) && (arrPointOriginal[0].y==arrPoint[0].y)){
			arrPoint[0].x=center.x+50;
			arrPoint[1].x=center.x-50;
			arrPoint[2].x=center.x-5;
			arrPoint[3].x=center.x-50;
		}else{
			arrPoint=arrPointOriginal;
		}
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