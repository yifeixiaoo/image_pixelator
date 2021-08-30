
/**
 *
 * toqutree (pa3)
 * significant modification of a quadtree .
 * toqutree.cpp
 * This file will be used for grading.
 *
 */

#include "toqutree.h"

toqutree::Node::Node(pair<int,int> ctr, int dim, HSLAPixel a)
	:center(ctr),dimension(dim),avg(a),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
	{}

toqutree::~toqutree(){
	clear(root);
}

toqutree::toqutree(const toqutree & other) {
	root = copy(other.root);
}


toqutree & toqutree::operator=(const toqutree & rhs){
	if (this != &rhs) {
		clear(root);
		root = copy(rhs.root);
	}
	return *this;
}

toqutree::toqutree(PNG & imIn, int k){ 


	int width = imIn.width();
	int height = imIn.height();
	unsigned int dim = pow(2,k);
	

	pair <int,int> ul;


	ul.first = (width - dim)/2;
	ul.second = (height - dim)/2;

	PNG * square = new PNG(dim,dim);


	
	for (unsigned int x = 0; x < dim; x++){
		unsigned int imgX = (unsigned int) ul.first;
		imgX = imgX + x;
		for (unsigned int y = 0; y < dim; y++){
			unsigned int imgY = (unsigned int) ul.second;
			imgY = imgY + y;
			HSLAPixel * curr = square->getPixel((unsigned int) x, (unsigned int) y);
			*curr = *imIn.getPixel(imgX,imgY);		
		}
	}
	root = buildTree(square, k);
	// cout<<"after builtTree"<<endl;

	// Node curr = Node( ,k-1,s.getAvg(ul,lr));


/* This constructor grabs the 2^k x 2^k sub-image centered */
/* in imIn and uses it to build a quadtree. It may assume  */
/* that imIn is large enough to contain an image of that size. */

/* your code here */
}

int toqutree::size() {
	int count = 0;
	Node * curr = root;
	int level = 0;
	if (curr == NULL){
		return count;
	}else{
		count = 1;
	}
	curr = curr -> SE;
	level++;
	while(curr != NULL){
		count += pow(4,level);
		curr = curr ->SE;
		level++;

	}
	return count;
	
/* your code here */
}


toqutree::Node * toqutree::buildTree(PNG * im, int k) {
int dim = pow(2,k);
int sdim = pow(2,k-1);
stats s = stats(*im);

// build 2 helper: finding optimal split, making the 4 sub-images(dimension of image, top left corner)

/* your code here */

// Note that you will want to practice careful memory use
// In this function. We pass the dynamically allocated image
// via pointer so that it may be released after it is used .
// similarly, at each level of the tree you will want to 
// declare a dynamically allocated stats object, and free it
// once you've used it to choose a split point, and calculate
// an average.

	if (k == 0){
		toqutree::Node * u = new toqutree::Node (pair<int,int> (0,0), k, *im->getPixel(0,0));

		return u;
		

	}
	if (k == 1){
		toqutree::Node * n = new toqutree::Node(pair<int,int> (1,1), k, s.getAvg(pair<int,int> (0,0), pair<int,int> (1,1)));
		
	PNG* se = SEcreateIMG(im, pair<int,int> (1,1), k-1);
	PNG* sw = SWcreateIMG(im, pair<int,int> (1,1), k-1);
	PNG* nw = NWcreateIMG(im, pair<int,int> (1,1), k-1);
	PNG* ne = NEcreateIMG(im, pair<int,int> (1,1), k-1);

	n->SE = buildTree(se, k-1);
	n->SW = buildTree(sw, k-1);
	n->NW = buildTree(nw, k-1);
	n->NE = buildTree(ne, k-1);
		// implement base case
		// return;
	}
	int width = im->width();
	int height = im->height();

	pair<int,int> ul;
	pair<int,int> lr;

	ul.first = 0;
	ul.second = 0;
	lr.first = width-1;
	lr.second = height-1;


	

	pair<int,int> point;
	point.first = dim - (dim/2);
	point.second = dim - (dim/2);

	pair <int,int> sul;
	pair <int,int> slr;

	sul.first = (width - sdim)/2;
	sul.second = (height -sdim)/2;
	slr.first = sul.first + sdim -1;
	slr.second = sul.second + sdim -1;

	int area = (int) s.rectArea(sul,slr);

	int dist = slr.first - sul.first;
	pair<int,int> lowestEntropyPoint;
	double lowestEntropy = 0;


	for (int i = sul.first; i<=slr.first; i++){
		for (int j = sul.second; j <= slr.second; j++){
			pair<int,int> currEntropyPoint (i,j);
			double totalEntropy;

			if(i<point.first && j<point.second){
				cout<<"case 1"<<endl;
				pair<int,int> TopLeftul (ul.first,ul.second);
				pair<int,int> TopLeftlr (i-1,j-1);
				vector<int> TopLeft = s.buildHist(TopLeftul, TopLeftlr);

				pair<int,int> TopRightul (i + dist + 1, ul.second);
				pair<int,int> TopRightlr (lr.first, j-1);
				vector<int> TopRight = s.buildHist(TopRightul, TopRightlr);
				
				pair<int,int> BottomRightul (i + dist + 1, j + dist + 1);
				pair<int,int> BottomRightlr (lr.first,lr.second); 
				vector<int> BottomRight = s.buildHist(BottomRightul, BottomRightlr);

				pair<int,int> BottomLeftul (ul.first, j + dist + 1);
				pair<int,int> BottomLeftlr (i-1,lr.second);
				vector<int> BottomLeft = s.buildHist(BottomLeftul, BottomLeftlr);

				vector<int> totalUL = sumFourVectors(TopLeft, TopRight, BottomRight, BottomLeft);
				double entropyUL = s.entropy(totalUL, area);

				pair<int,int> MiddleTopul (i, ul.second);
				pair<int,int> MiddleToplr (i + dist,j-1);
				vector<int> MiddleTop = s.buildHist(MiddleTopul, MiddleToplr);

				pair<int,int> MiddleBottomul (i, j + dist + 1);
				pair<int,int> MiddleBottomlr (i + dist, lr.second);
				vector<int> MiddleBottom = s.buildHist(MiddleBottomul, MiddleBottomlr);

				vector<int> totalUR = sumTwoVectors(MiddleTop, MiddleBottom);
				double entropyUR = s.entropy(totalUR, area);

				pair<int,int> Middleul(i,j);
				pair<int,int> Middlelr(i + dist, j + dist);
				vector<int> Middle = s.buildHist(Middleul, Middlelr);

				vector<int> totalLR = Middle;
				double entropyLR = s.entropy(totalLR, area);
				
				pair <int, int> MiddleLeftul (ul.first, j);
				pair <int, int> MiddleLeftlr (i-1, j + dist);
				vector<int> MiddleLeft = s.buildHist(MiddleLeftul, MiddleLeftlr);

				pair <int, int> MiddleRightul (i + dist + 1, j);
				pair <int, int> MiddleRightlr (lr.first, j + dist);
				vector<int> MiddleRight = s.buildHist(MiddleRightul, MiddleRightlr);

				vector<int> totalLL = sumTwoVectors(MiddleLeft, MiddleRight);
				double entropyLL = s.entropy(totalLL, area);

				totalEntropy = entropyUL + entropyUR + entropyLR + entropyLL;
				
			}else if(i>=point.first && j<point.second){
				cout<<"case 2"<<endl;
				pair<int,int> TopLeftul (ul.first,ul.second);
				pair<int,int> TopLeftlr (i - dist - 2, j - 1);
				vector<int> TopLeft = s.buildHist(TopLeftul, TopLeftlr);

				pair<int,int> TopRightul (i, ul.second);
				pair<int,int> TopRightlr (lr.first, j-1);
				vector<int> TopRight = s.buildHist(TopRightul, TopRightlr);
				
				pair<int,int> BottomRightul (i, j + dist + 1);
				pair<int,int> BottomRightlr (lr.first,lr.second); 
				vector<int> BottomRight = s.buildHist(BottomRightul, BottomRightlr);

				pair<int,int> BottomLeftul (ul.first, j + dist + 1);
				pair<int,int> BottomLeftlr (i- dist - 2,lr.second);
				vector<int> BottomLeft = s.buildHist(BottomLeftul, BottomLeftlr);

				vector<int> TotalUR = sumFourVectors(TopLeft, TopRight, BottomRight, BottomLeft);
				if (i == point.first){
					TotalUR = sumTwoVectors(TopRight, BottomRight);
				}
				double entropyUR = s.entropy(TotalUR, area);

				pair<int,int> MiddleTopul (i-dist-1, ul.second);
				pair<int,int> MiddleToplr (i-1,j-1);
				vector<int> MiddleTop = s.buildHist(MiddleTopul, MiddleToplr);

				pair<int,int> MiddleBottomul (i-dist-1, j + dist + 1);
				pair<int,int> MiddleBottomlr (i-1, lr.second);
				vector<int> MiddleBottom = s.buildHist(MiddleBottomul, MiddleBottomlr);

				vector<int> totalUL = sumTwoVectors(MiddleTop, MiddleBottom);
				double entropyUL = s.entropy(totalUL, area);

				pair<int,int> Middleul(i-dist-1,j);
				pair<int,int> Middlelr(i-1, j+dist);
				vector<int> Middle = s.buildHist(Middleul, Middlelr);

				vector<int> totalLL = Middle;
				double entropyLL = s.entropy(totalLL, area);
				
				pair <int, int> MiddleLeftul (ul.first, j);
				pair <int, int> MiddleLeftlr (i-dist-2, j + dist);
				vector<int> MiddleLeft = s.buildHist(MiddleLeftul, MiddleLeftlr);

				pair <int, int> MiddleRightul (i, j);
				pair <int, int> MiddleRightlr (lr.first, j + dist);
				vector<int> MiddleRight = s.buildHist(MiddleRightul, MiddleRightlr);

				vector<int> totalLR = sumTwoVectors(MiddleLeft, MiddleRight);
				if (i == point.first){
					totalLR = MiddleRight;
				}
				double entropyLR = s.entropy(totalLR, area);

				totalEntropy = entropyUL + entropyUR + entropyLR + entropyLL;


			}else if(i<point.first && j>=point.second){
				cout<<"case 3"<<endl;
				pair<int,int> TopLeftul (ul.first,ul.second);
				pair<int,int> TopLeftlr (i-1,j-dist-2);
				vector<int> TopLeft = s.buildHist(TopLeftul, TopLeftlr);
	
				pair<int,int> TopRightul (i+dist+1, ul.second);
				pair<int,int> TopRightlr (lr.first, j-dist-2);
				vector<int> TopRight = s.buildHist(TopRightul, TopRightlr);				
				
				pair<int,int> BottomRightul (i+dist+1, j);
				pair<int,int> BottomRightlr (lr.first,lr.second); 
				vector<int> BottomRight = s.buildHist(BottomRightul, BottomRightlr);

				pair<int,int> BottomLeftul (ul.first, j);
				pair<int,int> BottomLeftlr (i-1,lr.second);
				vector<int> BottomLeft = s.buildHist(BottomLeftul, BottomLeftlr);

				vector<int> totalLL = sumFourVectors(TopLeft, TopRight, BottomRight, BottomLeft);
				if (j == point.second){
					totalLL = sumTwoVectors(BottomLeft, BottomRight);
				}
				double entropyLL = s.entropy(totalLL, area);

				pair<int,int> MiddleTopul (i, ul.second);
				pair<int,int> MiddleToplr (i + dist,j-dist-2);
				vector<int> MiddleTop = s.buildHist(MiddleTopul, MiddleToplr);
				
				pair<int,int> MiddleBottomul (i, j);
				pair<int,int> MiddleBottomlr (i + dist, lr.second);
				vector<int> MiddleBottom = s.buildHist(MiddleBottomul, MiddleBottomlr);

				vector<int> totalLR = sumTwoVectors(MiddleTop, MiddleBottom);
				if (j == point.second){
					totalLR = MiddleBottom;
				}
				double entropyLR = s.entropy(totalLR, area);

				pair<int,int> Middleul(i,j-dist-1);
				pair<int,int> Middlelr(i + dist, j-1);
				vector<int> Middle = s.buildHist(Middleul, Middlelr);

				vector<int> totalUR = Middle;
				double entropyUR = s.entropy(totalUR, area);
				
				pair <int, int> MiddleLeftul (ul.first, j-dist-1);
				pair <int, int> MiddleLeftlr (i-1, j-1);
				vector<int> MiddleLeft = s.buildHist(MiddleLeftul, MiddleLeftlr);

				pair <int, int> MiddleRightul (i + dist + 1, j-dist-1);
				pair <int, int> MiddleRightlr (lr.first, j-1);
				vector<int> MiddleRight = s.buildHist(MiddleRightul, MiddleRightlr);

				vector<int> totalUL = sumTwoVectors(MiddleLeft, MiddleRight);
				double entropyUL = s.entropy(totalUL, area);

				totalEntropy = entropyUL + entropyUR + entropyLR + entropyLL;



			}else if(i>=point.first && j>=point.second){
				cout<<"case 4"<<endl;
				pair<int,int> TopLeftul (ul.first,ul.second);
				pair<int,int> TopLeftlr (i-dist-2,j-dist-2);
				vector<int> TopLeft = s.buildHist(TopLeftul, TopLeftlr);

				pair<int,int> TopRightul (i, ul.second);
				pair<int,int> TopRightlr (lr.first, j-dist-2);
				vector<int> TopRight = s.buildHist(TopRightul, TopRightlr);
				
				pair<int,int> BottomRightul (i, j);
				pair<int,int> BottomRightlr (lr.first,lr.second); 
				vector<int> BottomRight = s.buildHist(BottomRightul, BottomRightlr);

				pair<int,int> BottomLeftul (ul.first, j);
				pair<int,int> BottomLeftlr (i-dist-2,lr.second);
				vector<int> BottomLeft = s.buildHist(BottomLeftul, BottomLeftlr);

				vector<int> totalLR = sumFourVectors(TopLeft, TopRight, BottomRight, BottomLeft);
				if (i ==point.first && j==point.second){
					totalLR = BottomRight;
				}else if (i ==point.first && j!=point.second){
					totalLR = sumTwoVectors(BottomRight,TopRight);

				}else if (i !=point.first && j==point.second){
					totalLR = sumTwoVectors(BottomRight, BottomLeft);

				}
				
				double entropyLR = s.entropy(totalLR, area);

				pair<int,int> MiddleTopul (i-dist-1, ul.second);
				pair<int,int> MiddleToplr (i-1,j-dist-2);
				vector<int> MiddleTop = s.buildHist(MiddleTopul, MiddleToplr);

				pair<int,int> MiddleBottomul (i-dist-1, j);
				pair<int,int> MiddleBottomlr (i-1, lr.second);
				vector<int> MiddleBottom = s.buildHist(MiddleBottomul, MiddleBottomlr);

				vector<int> totalLL = sumTwoVectors(MiddleTop, MiddleBottom);
				if (i ==point.first && j==point.second){
					totalLL = MiddleBottom;
				}else if (i ==point.first && j!=point.second){
					totalLL = sumTwoVectors(MiddleBottom,MiddleTop);

				}else if (i !=point.first && j==point.second){
					totalLL = MiddleBottom;

				}
				double entropyLL = s.entropy(totalLL, area);

				pair<int,int> Middleul(i-dist-1,j-dist-1);
				pair<int,int> Middlelr(i-1, j-1);
				vector<int> Middle = s.buildHist(Middleul, Middlelr);

				vector<int> totalUL = Middle;
				double entropyUL = s.entropy(totalUL, area);
				
				pair <int, int> MiddleLeftul (ul.first, j-dist-1);
				pair <int, int> MiddleLeftlr (i-dist-2, j-1);
				vector<int> MiddleLeft = s.buildHist(MiddleLeftul, MiddleLeftlr);

				pair <int, int> MiddleRightul (i, j-dist-1);
				pair <int, int> MiddleRightlr (lr.first, j-1);
				vector<int> MiddleRight = s.buildHist(MiddleRightul, MiddleRightlr);

				vector<int> totalUR = sumTwoVectors(MiddleLeft, MiddleRight);
				if (i ==point.first && j==point.second){
					totalUR = MiddleRight;
				}else if (i ==point.first && j!=point.second){
					totalUR = MiddleRight;

				}else if (i !=point.first && j==point.second){
					totalUR = sumTwoVectors(MiddleLeft,MiddleRight);

				}
				double entropyUR = s.entropy(totalUR, area);

				totalEntropy = entropyUL + entropyUR + entropyLR + entropyLL;
				

			}

			if (lowestEntropy == 0 || totalEntropy < lowestEntropy){
				lowestEntropy = totalEntropy;
				lowestEntropyPoint = currEntropyPoint;

			}
			// if currEntropy is less than lowestEntropy, lowestEntropy = currEntropy 

		

		}
	}
	PNG* se = SEcreateIMG(im, lowestEntropyPoint, k-1);
	PNG* sw = SWcreateIMG(im, lowestEntropyPoint, k-1);
	PNG* nw = NWcreateIMG(im, lowestEntropyPoint, k-1);
	PNG* ne = NEcreateIMG(im, lowestEntropyPoint, k-1);

	toqutree::Node * node = new toqutree::Node(point, k, s.getAvg(ul,lr));
	node->SE = buildTree(se,k-1);
	node->SW = buildTree(sw,k-1);
	node->NW = buildTree(nw,k-1);
	node->NE = buildTree(ne,k-1);
	
	// cout<<"end buildtree"<<endl;
	return node;



}

HSLAPixel toqutree::FindPixel(pair<int,int> find, Node * curr){
	pair<int,int> split = curr->center;
	int dimen = curr->dimension;
	int dim = pow(2,dimen);
	int sdim = pow(2,dimen-1);
	int Xdist = sdim - split.first;
	int Ydist = sdim - split.second;
	HSLAPixel pixel;

	int X;
	int Y;
	
	
	if (curr -> SE == NULL){
       cout<<"FIndPixel"<<endl;
		return curr->avg;
	}


	if (find.first < split.first && find.second < split.second){

		if (find.first<(split.first-sdim) && find.second >=(split.second-sdim)){
			X =  dim- (split.first+find.first);
			Y = Ydist + find.second;
			pixel = FindPixel(pair<int,int> (X,Y), curr -> NE);
		}else if(find.first>=(split.first-sdim) && find.second <(split.second-sdim)){
			X = Xdist + find.first;
			Y = dim- (split.second+find.second);
			pixel = FindPixel(pair<int,int> (X,Y), curr -> SW);
		}else if(find.first<(split.first-sdim) && find.second <(split.second-sdim)){
			X = dim- (split.first+find.first);
			Y = dim- (split.second+find.second);
			pixel = FindPixel(pair<int,int> (X,Y), curr -> SE);
		}else{
			X = Xdist + find.first;
			Y = Ydist + find.second;
			pixel = FindPixel(pair<int,int> (X,Y), curr -> NW);
		}
		
	}else if (find.first >= split.first && find.second < split.second){
		
		if (find.first >= (split.first+sdim) && find.second >= (split.second-sdim)){
			X = find.first - (split.first + sdim);
			Y = Ydist + find.second;
			pixel = FindPixel(pair<int,int> (X,Y), curr -> NW);

		}else if (find.first < (split.first+sdim) && find.second < (split.second-sdim)){
			X = find.first - split.first;
			Y = find.second + (dim - split.second);
			pixel = FindPixel(pair<int,int> (X,Y), curr -> SE);

		}else if (find.first >= (split.first+sdim) && find.second < (split.second-sdim)){
			X = find.first - (split.first + sdim);
			Y = find.second + (dim - split.second);
			pixel = FindPixel(pair<int,int> (X,Y), curr -> SW);

		}else{
			X = find.first - split.first;
			Y = Ydist + find.second;
			pixel = FindPixel(pair<int,int> (X,Y), curr -> NE);
		}

	}else if (find.first >= split.first && find.second >= split.second){
		
		if (find.first >= (split.first+sdim) && find.second < (split.second+sdim)){
			X = find.first - (split.first + sdim);
			Y = find.second - split.second;
			pixel = FindPixel(pair<int,int> (X,Y), curr -> SW);

		}else if(find.first < (split.first+sdim) && find.second >= (split.second+sdim)){
			X = find.first -split.first;
			Y = find.second - (split.second + sdim);
			pixel = FindPixel(pair<int,int> (X,Y), curr -> NE);

		}else if(find.first >= (split.first+sdim) && find.second >= (split.second+sdim)){
			X = find.first - (split.first + sdim);
			Y = find.second - (split.second + sdim);
			pixel = FindPixel(pair<int,int> (X,Y), curr -> NW);

		}else{
			X = find.first - split.first;
			Y = find.second - split.second;
			pixel = FindPixel(pair<int,int> (X,Y), curr -> SE);
		}

	}else if (find.first < split.first && find.second >= split.second){

		if (find.first < (split.first-sdim) && find.second < (split.second+sdim)){
			X = find.first + (dim-split.first);
			Y = find.second - split.second;
			pixel = FindPixel(pair<int,int> (X,Y), curr -> SE);

		}else if (find.first >= (split.first-sdim) && find.second >= (split.second+sdim)){
			X = Xdist + find.first;
			Y = find.second - (split.second + sdim);
			pixel = FindPixel(pair<int,int> (X,Y), curr -> NW);

		}else if (find.first < (split.first-sdim) && find.second >= (split.second+sdim)){
			X = find.first + (dim-split.first);
			Y = find.second - (split.second + sdim);
			pixel = FindPixel(pair<int,int> (X,Y), curr -> NE);

		}else {
			X = Xdist + find.first;
			Y = find.second - split.second;
			pixel = FindPixel(pair<int,int> (X,Y), curr -> SW);
		}

	}
	return pixel;
	
}
PNG toqutree::renderHelper(PNG * png , Node * node, int k){
   
    //node is a leaf, PNG should be a single pixel--> recolor png pixel (0,0) to node->av color
    if(k == 0){
        //printf("line 227 renderhelp%i\n", k);
        *png->getPixel(0,0) = node->avg;
       
        return *png;
    }else {
        //printf("line 231 renderhelp\n");
        int dim = pow(2, k-1);
        int kpow = pow(2, k);
        //initialize 4 subPNGs of 2^k-1 * 2^k-1
        PNG SEpng =  PNG(dim, dim);
        PNG SWpng =  PNG(dim, dim);
        PNG NWpng =  PNG(dim, dim);
        PNG NEpng =  PNG(dim, dim);
 
 
        //intialize center pair
        pair<int, int> ctr = node->center;
 
        //each subpng represents one of 4 subareas
        //recusive calls to populate subPNGs
        SEpng = renderHelper(&SEpng, node->SE, k-1);
        SWpng = renderHelper(&SWpng, node->SW, k-1);
        NEpng = renderHelper(&NEpng, node->NE, k-1);
        NWpng = renderHelper(&NWpng, node->NW, k-1);
 
        //subarea SEpng, gets images from the populated subpngs and
        //paints them onto the appropriate places of png (the png that this function returns)
        for(unsigned int x = 0; x<SEpng.width(); x++){
            for(unsigned int y = 0; y<SEpng.height(); y++){
                //printf("line 246 innerfor renderhelp%i\n", k);
                int a = (ctr.first + x)%kpow;
                //printf("line 248 innerfor renderhelp%i\n", k);
                int b= (ctr.second + y)%kpow;
                //printf("line 250 innerfor renderhelp%i\n", k);
                *png->getPixel(a,b) = *SEpng.getPixel(x,y);
                //printf("line 252 innerfor renderhelp%i\n", k);
            }
           
        }
       
        for(unsigned int x = 0; x<SWpng.width(); x++){
            for(unsigned int y = 0; y<SWpng.height(); y++){
                int a = (node->center.first + x + dim)%kpow;
                int b = (node->center.second + y)%kpow;
                *png->getPixel(a,b) = *SWpng.getPixel(x,y);
 
            }
        }
        //printf("line 259 renderhelp%i\n", k);
        for(unsigned int x = 0; x<NEpng.width(); x++){
            for(unsigned int y = 0; y<NEpng.height(); y++){
                int a = (node->center.first + x)%kpow;
                int b = (node->center.second + y + dim)%kpow;
                *png->getPixel(a,b) = *NEpng.getPixel(x,y);
            }
        }
        //printf("line 267 renderhelp %i\n", k);
        for( unsigned int x = 0; x<NWpng.width(); x++){
            for(unsigned int y = 0; y<NWpng.height(); y++){
                int a= (node->center.first + x + dim)%kpow;
                int b= (node->center.second + y + dim)%kpow;
                *png->getPixel(a,b) = *NWpng.getPixel(x,y) ;
            }
        }
        return *png;
     }
}
//  }
//  //printf("rendered\n");
// }

PNG toqutree::render(){
	// PNG rendered = PNG(pow(2,root->dimension),pow(2,root->dimension));
	// pair<int,int> point;
	// int width = (int)rendered.width();
	// int height = (int)rendered.height();

	// point.first = (width/2);
	// point.second = (height/2);

	// for (int i = 0; i<width; i++){
	// 	for (int j = 0; j<height; j++){

	// 		*rendered.getPixel(i,j) = FindPixel(pair<int,int> (i,j), root);
	// 		cout<<i<<endl;
	// 		cout<<j<<endl;

	// 	}
	// }
	int k = root->dimension;
int dim = pow(2,k);
PNG result = PNG(dim,dim);
 
for(int x =0 ; x<result.width(); x++){
 for(int y = 0; y < result.height(); y++){
     HSLAPixel pixel = HSLAPixel(100, 80, 10);
     *result.getPixel(x,y) = pixel;
 }
}
 
//printf("line 300 render\n");
renderHelper(&result, root, k);
//printf("line 302 render\n");
return result;

// My algorithm for this problem included a helper function
// that was analogous to Find in a BST, but it navigated the 
// quadtree, instead.

// return rendered;

}

/* oops, i left the implementation of this one in the file! */
void toqutree::prune(double tol){

	// prune(root,tol);

}

/* called by destructor and assignment operator*/
void toqutree::clear(Node * & curr){
	if(curr == NULL){
		return; 
	}
	if (curr->NW != NULL){
		clear(curr->NW);
	}if(curr->NE !=NULL){
		clear(curr->NE);
	}if(curr->SE != NULL){
		clear(curr->SE);
	}if(curr->SW != NULL){
		clear(curr->SW); 
	}
	delete curr; 
	curr = NULL;

/* your code here */
}

/* done */
/* called by assignment operator and copy constructor */
toqutree::Node * toqutree::copy(const Node * other) {
		if(other != NULL){
		toqutree::Node * curr =  new toqutree::Node(other->center, other->dimension, other->avg);
		curr->SE = copy(other->SE);
		curr->SW = copy(other->SW);
		curr->NE = copy(other->NE);
		curr->NW = copy(other->NW);
		return curr; 
	}else 
	return NULL;

/* your code here */
}

vector<int> toqutree::sumFourVectors(vector<int> one, vector<int> two, vector<int> three, vector<int> four){
	vector<int> total;
	total.resize(one.size());
	for (unsigned long i = 0; i<one.size(); i++){
		total[i] = one[i] + two[i] + three [i] + four[i];
	}
	return total;
}

vector<int> toqutree::sumTwoVectors(vector<int> one, vector<int> two){
	vector<int> total;
	total.resize(one.size());
	for (unsigned long i = 0; i<one.size(); i++){
		total[i] = one[i] + two[i];
	}
	return total;

}

PNG * toqutree::SEcreateIMG(PNG * im, pair<int,int> point, int k){
	int dim = pow(2,k);
	int width = (int) im->width();
	int height = (int) im->height();
	
	PNG * square = new PNG((unsigned int) dim, (unsigned int) dim);

	for (int i = 0; i < dim; i++){
		int imgX = (unsigned int) point.first;
		imgX = imgX + i;
		for (int j = 0; j < dim; j++){
		
			int imgY = (unsigned int) point.second;
			imgY = imgY + j;
			HSLAPixel * curr = square->getPixel((unsigned int) i, (unsigned int) j);
			unsigned int x = imgX;
			unsigned int y = imgY;
			if (imgX > width-1){
				x = imgX - width;

			}
			if (imgY > height-1){
				y = imgY - height;

			}
			*curr = *im->getPixel(x, y);
			
		}
	}

return square;
}
PNG * toqutree::SWcreateIMG(PNG * im, pair<int,int> point, int k){
	int dim = pow(2,k);
	int width = (int) im->width();
	int height = (int) im->height();
	
	PNG * square = new PNG((unsigned int) dim, (unsigned int) dim);

	for (int i = 0; i < dim; i++){
		int imgX =(point.first - dim);
		imgX = imgX + i;
		for (int j = 0; j < dim; j++){
		
			int imgY = point.second;
			imgY = imgY + j;
			HSLAPixel * curr = square->getPixel((unsigned int) i, (unsigned int) j);
			unsigned int x = imgX;
			unsigned int y = imgY;
			if (imgX < 0){
				x = imgX + width;

			}
			if (imgY > height-1){
				y = imgY - height;

			}
			*curr = *im->getPixel(x, y);
			
		}
	}

return square;
}

PNG * toqutree::NWcreateIMG(PNG * im, pair<int,int> point, int k){
	int dim = pow(2,k);
	int width = (int) im->width();
	int height = (int) im->height();
	
	PNG * square = new PNG((unsigned int) dim, (unsigned int) dim);

	for (int i = 0; i < dim; i++){
		int imgX =(point.first - dim);
		imgX = imgX + i;
		for (int j = 0; j < dim; j++){
		
			int imgY = point.second - dim;
			imgY = imgY + j;
			HSLAPixel * curr = square->getPixel((unsigned int) i, (unsigned int) j);
			unsigned int x = imgX;
			unsigned int y = imgY;
			if (imgX < 0){
				x = imgX + width;

			}
			if (imgY < 0){
				y = imgY + height;

			}
			*curr = *im->getPixel(x, y);
			
		}
	}

return square;
}

PNG * toqutree::NEcreateIMG(PNG * im, pair<int,int> point, int k){
	int dim = pow(2,k);
	int width = (int) im->width();
	int height = (int) im->height();
	
	PNG * square = new PNG((unsigned int) dim, (unsigned int) dim);

	for (int i = 0; i < dim; i++){
		int imgX = point.first;
		imgX = imgX + i;
		for (int j = 0; j < dim; j++){
			int imgY = point.second - dim;
			imgY = imgY + j;
			HSLAPixel * curr = square->getPixel((unsigned int) i, (unsigned int) j);
			unsigned int x = imgX;
			unsigned int y = imgY;
			if (imgX > width - 1){
				x = imgX - width;

			}
			if (imgY < 0){
				y = imgY + height;

			}
			*curr = *im->getPixel(x, y);
			
		}
	}

return square;
}

