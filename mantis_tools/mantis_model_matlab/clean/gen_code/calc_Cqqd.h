#include <Eigen3/Dense>

inline void calc_Cqqd(Eigen::Vector3d& m, double IJ1x, double IJ1y, double IJ1z, double IJ2x, double IJ2y, double IJ2z, double bvx, double bvy, double bvz, double bw1, double bw2, double bw3, double l1, double l2, double r1, double r1d, double r2, double r2d) {
	Cqqd(1,1) = -r1d*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2)+IJ1x*cos(r1)*sin(r1)-IJ1z*cos(r1)*sin(r1))-r2d*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2));
	Cqqd(3,1) = -r1d*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0)+IJ1x*cos(r1*2.0)*(1.0/2.0)-IJ1z*cos(r1*2.0)*(1.0/2.0))-r2d*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0));
	Cqqd(5,1) = -r2d*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*cos(r2)*(1.0/2.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0)-IJ2z*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))-r1d*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*cos(r1)*(1.0/4.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0));
	Cqqd(7,1) = bvy*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*cos(r1)*(1.0/4.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))+bw1*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2)+IJ1x*cos(r1)*sin(r1)-IJ1z*cos(r1)*sin(r1))+bw3*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0)+IJ1x*cos(r1*2.0)*(1.0/2.0)-IJ1z*cos(r1*2.0)*(1.0/2.0));
	Cqqd(8,1) = bw1*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2))+bvy*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*cos(r2)*(1.0/2.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0)-IJ2z*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))+bw3*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0));
	Cqqd(4,2) = r1d*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(1.0/2.0)+IJ1y*l1*cos(r1)*(1.0/4.0))+IJ2y*l2*r2d*cos(r1+r2)*(1.0/4.0);
	Cqqd(6,2) = -r1d*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))*(1.0/2.0)+IJ1y*l1*sin(r1)*(1.0/4.0))-IJ2y*l2*r2d*sin(r1+r2)*(1.0/4.0);
	Cqqd(7,2) = -bvx*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(1.0/2.0)+IJ1y*l1*cos(r1)*(1.0/4.0))+bvz*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))*(1.0/2.0)+IJ1y*l1*sin(r1)*(1.0/4.0));
	Cqqd(8,2) = IJ2y*bvx*l2*cos(r1+r2)*(-1.0/4.0)+IJ2y*bvz*l2*sin(r1+r2)*(1.0/4.0);
	Cqqd(1,3) = -r1d*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0)+IJ1x*cos(r1*2.0)*(1.0/2.0)-IJ1z*cos(r1*2.0)*(1.0/2.0))-r2d*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0));
	Cqqd(3,3) = r1d*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2)+IJ1x*cos(r1)*sin(r1)-IJ1z*cos(r1)*sin(r1))+r2d*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2));
	Cqqd(5,3) = r2d*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0)-IJ2x*l1*sin(r1+r2)*cos(r2)*(1.0/2.0)+IJ2z*l1*cos(r1+r2)*sin(r2)*(1.0/2.0))+r1d*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*sin(r1)*(1.0/4.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0));
	Cqqd(7,3) = -bvy*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*sin(r1)*(1.0/4.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0))-bw3*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2)+IJ1x*cos(r1)*sin(r1)-IJ1z*cos(r1)*sin(r1))+bw1*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0)+IJ1x*cos(r1*2.0)*(1.0/2.0)-IJ1z*cos(r1*2.0)*(1.0/2.0));
	Cqqd(8,3) = -bw3*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2))-bvy*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0)-IJ2x*l1*sin(r1+r2)*cos(r2)*(1.0/2.0)+IJ2z*l1*cos(r1+r2)*sin(r2)*(1.0/2.0))+bw1*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0));
	Cqqd(2,4) = r1d*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(1.0/2.0)+IJ1y*l1*cos(r1)*(1.0/4.0))+IJ2y*l2*r2d*cos(r1+r2)*(1.0/4.0);
	Cqqd(4,4) = r1d*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))+IJ1y*(l1*l1)*cos(r1)*sin(r1)*(1.0/4.0))+IJ2y*l2*r2d*cos(r1+r2)*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))*(1.0/2.0);
	Cqqd(6,4) = r1d*(IJ2y*((l1*l1)*cos(r1*2.0)+(l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2))*(1.0/2.0)+IJ1y*(l1*l1)*cos(r1*2.0)*(1.0/8.0))+IJ2y*r2d*((l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2)*(1.0/2.0))*(1.0/2.0);
	Cqqd(7,4) = -bvz*(IJ2y*((l1*l1)*cos(r1*2.0)+(l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2))*(1.0/2.0)+IJ1y*(l1*l1)*cos(r1*2.0)*(1.0/8.0))-bw2*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(1.0/2.0)+IJ1y*l1*cos(r1)*(1.0/4.0))-r2d*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))-IJ2y*l2*cos(r1+r2)*(1.0/4.0)+IJ1y*l1*cos(r1)*(1.0/2.0))-bvx*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))+IJ1y*(l1*l1)*cos(r1)*sin(r1)*(1.0/4.0));
	Cqqd(8,4) = r1d*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))-IJ2y*l2*cos(r1+r2)*(1.0/4.0)+IJ1y*l1*cos(r1)*(1.0/2.0))-IJ2y*bvz*((l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2)*(1.0/2.0))*(1.0/2.0)-IJ2y*bw2*l2*cos(r1+r2)*(1.0/4.0)-IJ2y*bvx*l2*cos(r1+r2)*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))*(1.0/2.0);
	Cqqd(1,5) = -r2d*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*cos(r2)*(1.0/2.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0)-IJ2z*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))-r1d*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*cos(r1)*(1.0/4.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0));
	Cqqd(3,5) = r2d*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0)-IJ2x*l1*sin(r1+r2)*cos(r2)*(1.0/2.0)+IJ2z*l1*cos(r1+r2)*sin(r2)*(1.0/2.0))+r1d*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*sin(r1)*(1.0/4.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0));
	Cqqd(5,5) = r2d*(IJ2x*(l1*l1)*cos(r2)*sin(r2)-IJ2z*l1*sin(r2)*(l2+l1*cos(r2)*2.0)*(1.0/2.0));
	Cqqd(7,5) = bw1*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*cos(r1)*(1.0/4.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))-bw3*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*sin(r1)*(1.0/4.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0));
	Cqqd(8,5) = bw1*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*cos(r2)*(1.0/2.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0)-IJ2z*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))-bw3*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0)-IJ2x*l1*sin(r1+r2)*cos(r2)*(1.0/2.0)+IJ2z*l1*cos(r1+r2)*sin(r2)*(1.0/2.0))-bvy*(IJ2x*(l1*l1)*cos(r2)*sin(r2)-IJ2z*l1*sin(r2)*(l2+l1*cos(r2)*2.0)*(1.0/2.0));
	Cqqd(2,6) = -r1d*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))*(1.0/2.0)+IJ1y*l1*sin(r1)*(1.0/4.0))-IJ2y*l2*r2d*sin(r1+r2)*(1.0/4.0);
	Cqqd(4,6) = r1d*(IJ2y*((l1*l1)*cos(r1*2.0)+(l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2))*(1.0/2.0)+IJ1y*(l1*l1)*cos(r1*2.0)*(1.0/8.0))+IJ2y*r2d*((l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2)*(1.0/2.0))*(1.0/2.0);
	Cqqd(6,6) = -r1d*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))+IJ1y*(l1*l1)*cos(r1)*sin(r1)*(1.0/4.0))-IJ2y*l2*r2d*sin(r1+r2)*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(1.0/2.0);
	Cqqd(7,6) = -bvx*(IJ2y*((l1*l1)*cos(r1*2.0)+(l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2))*(1.0/2.0)+IJ1y*(l1*l1)*cos(r1*2.0)*(1.0/8.0))+r2d*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))-IJ2y*l2*sin(r1+r2)*(1.0/4.0)+IJ1y*l1*sin(r1)*(1.0/2.0))+bw2*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))*(1.0/2.0)+IJ1y*l1*sin(r1)*(1.0/4.0))+bvz*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))+IJ1y*(l1*l1)*cos(r1)*sin(r1)*(1.0/4.0));
	Cqqd(8,6) = -r1d*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))-IJ2y*l2*sin(r1+r2)*(1.0/4.0)+IJ1y*l1*sin(r1)*(1.0/2.0))-IJ2y*bvx*((l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2)*(1.0/2.0))*(1.0/2.0)+IJ2y*bw2*l2*sin(r1+r2)*(1.0/4.0)+IJ2y*bvz*l2*sin(r1+r2)*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(1.0/2.0);
	Cqqd(1,7) = -bvy*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*cos(r1)*(1.0/4.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))-bw1*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2)+IJ1x*cos(r1)*sin(r1)-IJ1z*cos(r1)*sin(r1))-bw3*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0)+IJ1x*cos(r1*2.0)*(1.0/2.0)-IJ1z*cos(r1*2.0)*(1.0/2.0));
	Cqqd(2,7) = bvx*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(1.0/2.0)+IJ1y*l1*cos(r1)*(1.0/4.0))-bvz*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))*(1.0/2.0)+IJ1y*l1*sin(r1)*(1.0/4.0));
	Cqqd(3,7) = bvy*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*sin(r1)*(1.0/4.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0))+bw3*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2)+IJ1x*cos(r1)*sin(r1)-IJ1z*cos(r1)*sin(r1))-bw1*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0)+IJ1x*cos(r1*2.0)*(1.0/2.0)-IJ1z*cos(r1*2.0)*(1.0/2.0));
	Cqqd(4,7) = bvz*(IJ2y*((l1*l1)*cos(r1*2.0)+(l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2))*(1.0/2.0)+IJ1y*(l1*l1)*cos(r1*2.0)*(1.0/8.0))+bw2*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(1.0/2.0)+IJ1y*l1*cos(r1)*(1.0/4.0))+r1d*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))+IJ1y*l1*cos(r1)*(1.0/2.0))+r2d*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))+IJ2y*l2*cos(r1+r2)*(1.0/4.0)+IJ1y*l1*cos(r1)*(1.0/2.0))+bvx*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))+IJ1y*(l1*l1)*cos(r1)*sin(r1)*(1.0/4.0));
	Cqqd(5,7) = -bw1*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*cos(r1)*(1.0/4.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))+bw3*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)+IJ1z*l1*sin(r1)*(1.0/4.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0));
	Cqqd(6,7) = bvx*(IJ2y*((l1*l1)*cos(r1*2.0)+(l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2))*(1.0/2.0)+IJ1y*(l1*l1)*cos(r1*2.0)*(1.0/8.0))-r2d*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))+IJ2y*l2*sin(r1+r2)*(1.0/4.0)+IJ1y*l1*sin(r1)*(1.0/2.0))-bw2*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))*(1.0/2.0)+IJ1y*l1*sin(r1)*(1.0/4.0))-r1d*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))+IJ1y*l1*sin(r1)*(1.0/2.0))-bvz*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))+IJ1y*(l1*l1)*cos(r1)*sin(r1)*(1.0/4.0));
	Cqqd(8,7) = -bvz*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))-IJ2y*l2*sin(r1+r2)*(1.0/4.0)+IJ1y*l1*sin(r1)*(1.0/2.0))+bvx*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))-IJ2y*l2*cos(r1+r2)*(1.0/4.0)+IJ1y*l1*cos(r1)*(1.0/2.0));
	Cqqd(1,8) = -bw1*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2))-bvy*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*cos(r2)*(1.0/2.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0)-IJ2z*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))-bw3*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0));
	Cqqd(2,8) = IJ2y*bvx*l2*cos(r1+r2)*(1.0/4.0)-IJ2y*bvz*l2*sin(r1+r2)*(1.0/4.0);
	Cqqd(3,8) = bw3*(IJ2x*cos(r1+r2)*sin(r1+r2)-IJ2z*cos(r1+r2)*sin(r1+r2))+bvy*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0)-IJ2x*l1*sin(r1+r2)*cos(r2)*(1.0/2.0)+IJ2z*l1*cos(r1+r2)*sin(r2)*(1.0/2.0))-bw1*(IJ2x*cos(r1*2.0+r2*2.0)*(1.0/2.0)-IJ2z*cos(r1*2.0+r2*2.0)*(1.0/2.0));
	Cqqd(4,8) = r1d*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))+IJ2y*l2*cos(r1+r2)*(1.0/4.0)+IJ1y*l1*cos(r1)*(1.0/2.0))+IJ2y*bvz*((l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2)*(1.0/2.0))*(1.0/2.0)+IJ2y*bw2*l2*cos(r1+r2)*(1.0/4.0)+IJ2y*l2*r2d*cos(r1+r2)+IJ2y*bvx*l2*cos(r1+r2)*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))*(1.0/2.0);
	Cqqd(5,8) = -bw1*(IJ2z*cos(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*cos(r2)*(1.0/2.0)+IJ2x*l1*sin(r1+r2)*sin(r2)*(1.0/2.0)-IJ2z*l1*sin(r1+r2)*sin(r2)*(1.0/2.0))+bw3*(IJ2z*sin(r1+r2)*(l2*(1.0/2.0)+l1*cos(r2))*(1.0/2.0)-IJ2x*l1*cos(r1+r2)*sin(r2)*(1.0/2.0)-IJ2x*l1*sin(r1+r2)*cos(r2)*(1.0/2.0)+IJ2z*l1*cos(r1+r2)*sin(r2)*(1.0/2.0))+bvy*(IJ2x*(l1*l1)*cos(r2)*sin(r2)-IJ2z*l1*sin(r2)*(l2+l1*cos(r2)*2.0)*(1.0/2.0));
	Cqqd(6,8) = -r1d*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))+IJ2y*l2*sin(r1+r2)*(1.0/4.0)+IJ1y*l1*sin(r1)*(1.0/2.0))+IJ2y*bvx*((l2*l2)*cos(r1*2.0+r2*2.0)*(1.0/4.0)+l1*l2*cos(r1*2.0+r2)*(1.0/2.0))*(1.0/2.0)-IJ2y*bw2*l2*sin(r1+r2)*(1.0/4.0)-IJ2y*l2*r2d*sin(r1+r2)-IJ2y*bvz*l2*sin(r1+r2)*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))*(1.0/2.0);
	Cqqd(7,8) = bvz*(IJ2y*(l2*sin(r1+r2)*(1.0/2.0)+l1*sin(r1))-IJ2y*l2*sin(r1+r2)*(1.0/4.0)+IJ1y*l1*sin(r1)*(1.0/2.0))-bvx*(IJ2y*(l2*cos(r1+r2)*(1.0/2.0)+l1*cos(r1))-IJ2y*l2*cos(r1+r2)*(1.0/4.0)+IJ1y*l1*cos(r1)*(1.0/2.0));
}
