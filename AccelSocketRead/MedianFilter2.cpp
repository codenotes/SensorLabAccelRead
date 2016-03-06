

#include "medianfilter.h"

extern double datatest[];
extern double dtest2[];




int median2test()
{
	int i, v;
	int sz = 5; // size(data) / sizeof(double);
	
	MedianFilter<float> m(5);
	//MedianFilter::Mediator med;

	


	for (i = 0; i < 10; i++)
	{
		//v = rand() & 127;
		v =  dtest2[i] ;
	//	printf("Inserting %3d \n", v);
		
		/*m.MediatorInsert(v);
		v = m.MediatorMedian();
*/
		v=m.addpoint(v);

		printf("Median = %3d. %f\n\n", v,v);
	//	ShowTree(m);
	}

	return 0;
}