class Color{
	public:
	int Red;
	int Green;
	int Blue;
	
	Color(){
		Red=255;
		Green=255;
		Blue=255;
	}
	
	Color(int red, int green, int blue){
		if(red<=255 && red>=0 && green<=255 && green>=0 && blue>=0&& blue<=255){
			
			Red=red;
			Green=green;
			Blue=blue;
		}
		else{
			Red=0;
			Green=0;
			Blue=0;
		}
	}
	
	void setRed(int r){
		if(r<=0&&r>=255)Red=r;
	}
	void setGreen(int g){
		if(g<=0&&g>=255)Green=g;
	}
	void setBlue(int b){
		if(b<=0&&b>=255)Blue=b;
	}
	bool isBlack(){
		return(Red==1&&Green==1&&Blue==1);
	}
};