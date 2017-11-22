#include <stdio.h>
#include <strings.h>
#include <complex.h>
#include <math.h>
#include "matrixlib.h"

void matprint(int m,int n,double A[m][n]){
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            printf("%g ",A[i][j]);
        }
        printf("\n");
    }
}
void vecprint(int m,double X[m]){
    matprint(m,1,(double (*)[1])X);
}
void multAx(int m,int n,
    double A[m][n],double X[n],double B[m]){
    bzero(B,sizeof(double)*m);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            B[i]+=A[i][j]*X[j];
        }
    }
}
void multATx(int m,int n,
    double A[m][n],double X[m],double B[n]){
    bzero(B,sizeof(double)*n);
    for(int j=0;j<m;j++){
        for(int i=0;i<n;i++){
            B[i]+=A[j][i]*X[j];
        }
    }
}
void algob(int m,int n,
    double A[m][n],double X[n],double B[m]){
    bzero(B,8*m);
    for(int j=0;j<n;j++){
        for(int i=0;i<m;i++){
            B[i]+=A[i][j]*X[j];
        }
    }
}
void cmatprint(int m,int n,complex A[m][n]){
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            printf("(%g %g) ",A[i][j]);
        }
        printf("\n");
    }
}
void cvecprint(int m,complex X[m]){
    cmatprint(m,1,(complex (*)[1])X);
}
void cmultAx(int m,int n,
    complex A[m][n],complex X[n],complex B[m]){
    bzero(B,sizeof(complex)*m);
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            B[i]+=A[i][j]*X[j];
        }
    }
}
void calgob(int m,int n,
    complex A[m][n],complex X[n],complex B[m]){
    bzero(B,8*m);
    for(int j=0;j<n;j++){
        for(int i=0;i<m;i++){
            B[i]+=A[i][j]*X[j];
        }
    }
}

void lufact(int n,double A[n][n]){
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            double alpha=A[j][i]/A[i][i];
            for(int k=i+1;k<n;k++){
                A[j][k]-=alpha*A[i][k];
            }
            A[j][i]=alpha;
        }
    }
}
void backsub(int n,double U[n][n],
    double x[n],double y[n]){
    // Rb =c 
    for(int i=n-1;i>=0;i--){  // Ux=y
        x[i]=y[i];
        for(int j=i+1;j<n;j++){
            x[i]-=U[i][j]*x[j];
        }
        x[i]/=U[i][i];
    }
}
void lusolve(int n,double LU[n][n],
    double x[n],double b[n]){
    double y[n];
    for(int i=0;i<n;i++){  // Ly=b
        y[i]=b[i];
        for(int j=0;j<i;j++){
            y[i]-=LU[i][j]*y[j];
        }
    }
    backsub(n,LU,x,y);
}
void plusolve(int n,double LU[n][n],double *P[n],
    double x[n],double b[n]){
    double y[n];
    for(int i=0;i<n;i++){  // Ly=b
        y[i]=b[(P[i]-&LU[0][0])/n];
        for(int j=0;j<i;j++){
            y[i]-=P[i][j]*y[j];
        }
    }
    for(int i=n-1;i>=0;i--){  // Ux=y
        x[i]=y[i];
        for(int j=i+1;j<n;j++){
            x[i]-=P[i][j]*x[j];
        }
        x[i]/=P[i][i];
    }
}
void plufact(int n,double A[n][n],
        double *P[n]){
    for(int i=0;i<n;i++){
        P[i]=&A[i][0];
    }
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            if(fabs(P[i][i])<fabs(P[j][i])){
                double *t=P[i]; P[i]=P[j]; P[j]=t;
            }
        }
        for(int j=i+1;j<n;j++){
            double alpha=P[j][i]/P[i][i];
            for(int k=i+1;k<n;k++){
                P[j][k]-=alpha*P[i][k];
            }
            P[j][i]=alpha;
        }
    }
}

double vecnorm2(int n,double x[n]){
    double r=0;
    for(int i=0;i<n;i++){
        double t=fabs(x[i]);
        r+=t*t;
    }
    return sqrt(r);
}
double matnorm2(int n,double A[n][n]){
    double x[n],y[n],yk[n];
/*    double B[n][n];
    printf("starting to make matrix B.\n");
    bzero(B,sizeof(double)*n*n);
    for(int k=0;k<n;k++){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                B[i][j]+=A[k][i]*A[k][j];
            }
        }
    }
    printf("done with forming matrix B.\n");
*/
    for(int k=0;k<n;k++){
        x[k]=2.0*random()/RAND_MAX-1.0;
    }
 //   multAx(n,n,B,x,y);
    double z[n];
    multAx(n,n,A,x,z);
    multATx(n,n,A,z,y);
    double t,tk,qold=0,qk;
    for(int k=2;k<10000;k++){
 //       multAx(n,n,B,y,yk);  // yk=B^k x
        multAx(n,n,A,y,z);
        multATx(n,n,A,z,yk);
        t=vecnorm2(n,y);
        tk=vecnorm2(n,yk);
        qk=tk/t;
        if(fabs(qk-qold)<=qk*5e-14){
            return sqrt(qk);
        }
        printf("%g,%g tk/t=%.15e\n",t,tk,tk/t);
        for(int i=0;i<n;i++){
            y[i]=yk[i]/tk;
        }
        qold=qk;
//        memcpy(y,yk,sizeof(double)*n);
    }
    printf("Didn't convert before loop ended!\n");
    return(sqrt(tk/t));
}

double vecnorm1(int n,double x[n]){
    double r=0;
    for(int i=0;i<n;i++){
        r+=fabs(x[i]);
    }
    return r;
}
double matnorm1(int n,double A[n][n]){
    double r=0;
    for(int j=0;j<n;j++){
        double s=0;
        for(int i=0;i<n;i++){
            s+=fabs(A[i][j]);
        }
        if(s>r) r=s;
    }
    return r;
}

