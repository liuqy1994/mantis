/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <eigen3/Eigen/Dense>

inline void calc_Dq(Eigen::MatrixXd& Dq, double IJ0x, double IJ0y, double IJ0z, double IJ1x, double IJ1y, double IJ1z, double IJ2x, double IJ2y, double IJ2z, double l0, double l1, double lc1, double lc2, double m0, double m1, double m2, double r1, double r2) {
	Dq(0,0) = m0+m1+m2;
	Dq(4,0) = l0*m1+l0*m2-l1*m2*cos(r1)-lc1*m1*cos(r1)-lc2*m2*cos(r1+r2);
	Dq(6,0) = l1*m2*cos(r1)+lc1*m1*cos(r1)+lc2*m2*cos(r1+r2);
	Dq(7,0) = lc2*m2*cos(r1+r2);
	Dq(1,1) = m0+m1+m2;
	Dq(3,1) = m2*(-l0+lc2*cos(r1+r2)+l1*cos(r1))-m1*(l0-lc1*cos(r1));
	Dq(5,1) = m2*(lc2*sin(r1+r2)+l1*sin(r1))+lc1*m1*sin(r1);
	Dq(2,2) = m0+m1+m2;
	Dq(4,2) = -l1*m2*sin(r1)-lc1*m1*sin(r1)-lc2*m2*sin(r1+r2);
	Dq(6,2) = l1*m2*sin(r1)+lc1*m1*sin(r1)+lc2*m2*sin(r1+r2);
	Dq(7,2) = lc2*m2*sin(r1+r2);
	Dq(1,3) = m2*(-l0+lc2*cos(r1+r2)+l1*cos(r1))-m1*(l0-lc1*cos(r1));
	Dq(3,3) = IJ0x+m2*pow(-l0+lc2*cos(r1+r2)+l1*cos(r1),2.0)+m1*pow(l0-lc1*cos(r1),2.0)+IJ2y*pow(cos(r1+r2),2.0)+IJ2x*pow(sin(r1+r2),2.0)+IJ1y*pow(cos(r1),2.0)+IJ1x*pow(sin(r1),2.0);
	Dq(5,3) = IJ2x*sin(r1*2.0+r2*2.0)*(-1.0/2.0)+IJ2y*sin(r1*2.0+r2*2.0)*(1.0/2.0)-IJ1x*sin(r1*2.0)*(1.0/2.0)+IJ1y*sin(r1*2.0)*(1.0/2.0)+m2*(lc2*sin(r1+r2)+l1*sin(r1))*(-l0+lc2*cos(r1+r2)+l1*cos(r1))-lc1*m1*sin(r1)*(l0-lc1*cos(r1));
	Dq(0,4) = l0*m1+l0*m2-l1*m2*cos(r1)-lc1*m1*cos(r1)-lc2*m2*cos(r1+r2);
	Dq(2,4) = -l1*m2*sin(r1)-lc1*m1*sin(r1)-lc2*m2*sin(r1+r2);
	Dq(4,4) = IJ0y+IJ1z+IJ2z+(l0*l0)*m1+(l0*l0)*m2+(l1*l1)*m2+(lc1*lc1)*m1+(lc2*lc2)*m2-l0*lc2*m2*cos(r1+r2)*2.0-l0*l1*m2*cos(r1)*2.0-l0*lc1*m1*cos(r1)*2.0+l1*lc2*m2*cos(r2)*2.0;
	Dq(6,4) = -IJ1z-IJ2z-(l1*l1)*m2-(lc1*lc1)*m1-(lc2*lc2)*m2+l0*lc2*m2*cos(r1+r2)+l0*l1*m2*cos(r1)+l0*lc1*m1*cos(r1)-l1*lc2*m2*cos(r2)*2.0;
	Dq(7,4) = -IJ2z-lc2*m2*(lc2-l0*cos(r1+r2)+l1*cos(r2));
	Dq(1,5) = m2*(lc2*sin(r1+r2)+l1*sin(r1))+lc1*m1*sin(r1);
	Dq(3,5) = IJ2x*sin(r1*2.0+r2*2.0)*(-1.0/2.0)+IJ2y*sin(r1*2.0+r2*2.0)*(1.0/2.0)-IJ1x*sin(r1*2.0)*(1.0/2.0)+IJ1y*sin(r1*2.0)*(1.0/2.0)+m2*(lc2*sin(r1+r2)+l1*sin(r1))*(-l0+lc2*cos(r1+r2)+l1*cos(r1))-lc1*m1*sin(r1)*(l0-lc1*cos(r1));
	Dq(5,5) = IJ0z+IJ2x*pow(cos(r1+r2),2.0)+m2*pow(lc2*sin(r1+r2)+l1*sin(r1),2.0)+IJ2y*pow(sin(r1+r2),2.0)+IJ1x*pow(cos(r1),2.0)+IJ1y*pow(sin(r1),2.0)+(lc1*lc1)*m1*pow(sin(r1),2.0);
	Dq(0,6) = l1*m2*cos(r1)+lc1*m1*cos(r1)+lc2*m2*cos(r1+r2);
	Dq(2,6) = l1*m2*sin(r1)+lc1*m1*sin(r1)+lc2*m2*sin(r1+r2);
	Dq(4,6) = -IJ1z-IJ2z-(l1*l1)*m2-(lc1*lc1)*m1-(lc2*lc2)*m2+l0*lc2*m2*cos(r1+r2)+l0*l1*m2*cos(r1)+l0*lc1*m1*cos(r1)-l1*lc2*m2*cos(r2)*2.0;
	Dq(6,6) = IJ1z+IJ2z+(l1*l1)*m2+(lc1*lc1)*m1+(lc2*lc2)*m2+l1*lc2*m2*cos(r2)*2.0;
	Dq(7,6) = IJ2z+lc2*m2*(lc2+l1*cos(r2));
	Dq(0,7) = lc2*m2*cos(r1+r2);
	Dq(2,7) = lc2*m2*sin(r1+r2);
	Dq(4,7) = -IJ2z-lc2*m2*(lc2-l0*cos(r1+r2)+l1*cos(r2));
	Dq(6,7) = IJ2z+lc2*m2*(lc2+l1*cos(r2));
	Dq(7,7) = IJ2z+(lc2*lc2)*m2;
}
