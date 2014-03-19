% this function will give the energy output of the farm adding up all the
% outputs from all the turbines (integrating for each turbine over all the
% windspeed bins and over all the wind direction bins. 

function [TotalEnergy,TSpE]=AnnualPowerOutput(nTurbines,Plarge,v1large,v2large)
load TurbineSpecResourceAlt
load TurbineSpecResource
load WindResource2
load FarmParameters
prV=wblcdf(v1large,wholecs,wholeks) -wblcdf(v2large,wholecs,wholeks);

pp=wholeoms.*wholethets.*prV.*Plarge;
PRlarge=repmat(PRated,length(thetas(:,1)),nTurbines);
VRlarge=repmat(vRated,length(thetas(:,1)),nTurbines);
wbPrated=1-wblcdf(VRlarge,smallcs,smallks);
prat=wbPrated.*PRlarge.*smalloms.*smallthets;
size(pp)
size(prat)
r1=sum(sum(pp));
r2=sum(sum(prat));
TotalEnergy=r1+r2;

h=1;
for hj=1:21:length(pp(:,1))
temp=pp(hj:hj+20,:);
TSpE(h,:)=sum(temp);
h=h+1;
end
size(TSpE)
TSpE=TSpE+prat;


%save test pp prat
% for hj=1:1:nTurbines
%     ener(hj)=EnergyEstimatePerTurbine(cs{hj},thets{hj},omegs{hj},kss{hj},P);
% end 
%  TotalEnergy=sum(ener);
 
