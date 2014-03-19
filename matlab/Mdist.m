function [D]=Mdist(fpbestv)
% This function will calculate multiple distances and put them in a matrix 
% Author: Kalyan Veermachaneni 
% Created: 1/12/2011 
X=fpbestv(:,1);
lx=length(X);
Y=fpbestv(:,2);
ly=length(Y);

xx1=repmat(X,1,lx);
xx2=repmat(X',lx,1);
xx=(xx1-xx2).^2;

yy1=repmat(Y,1,ly);
yy2=repmat(Y',ly,1);
yy=(yy1-yy2).^2;

D=sqrt(xx+yy);
